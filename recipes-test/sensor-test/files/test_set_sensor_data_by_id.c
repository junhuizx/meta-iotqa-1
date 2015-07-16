#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sensor_internal.h>
#include <stdbool.h>
#include <sf_sensor.h>
#include <sf_sensor_data.h>
#include <sf_sensor_type.h>
#include <sensor_common.h>
#include <unistd.h>
#include <string.h>
#define TIMESTAMP 708970
#define VALUECOUNT 10
int
main(int argc, char* argv[])
{
	sf_sensor_id_t sensor_id;
	sf_sensor_t sensor;
	unsigned int count;
	int result;
	int i;
	sf_sensor_data_t data, set_data;
	sensor_id = atoi(argv[1]);
	//initialize sensor environment
	int sensor_type_result = sf_get_sensor_type_count(&count);
	if (sensor_type_result < 0) {
		fprintf(stdout, "error: %d, failed to get sensor type count\n", sensor_type_result);
		return false;
	}
	result = sf_connect_sensor(sensor_id);
	if(result < 0)
	{
		fprintf(stdout, "eroor: %d, failed to connect sensor with id %d, pls check sensor id. \n", result, sensor_id);
		return false;
	}
	//make fake data and write it to target sensor	
	set_data.time_stamp = TIMESTAMP;
	set_data.value_count = VALUECOUNT;
	for (i = 0; i < set_data.value_count; i++) {
		set_data.values[i] = i*i;
	}
	result = sf_set_sensor_data(sensor_id, set_data);
	if (result < 0) {
		sf_disconnect_sensor(sensor_id);
		fprintf(stdout, "Sensor[%d], failed at sf_set_sensor_data ret=%d\n", sensor_id, result);
		return false;
	}
	fprintf(stdout, "sf_set_sensor_data successfully value_count=%d\n", set_data.value_count);

	result = sf_get_sensor_data(sensor_id, &data);
	if (result < 0) {
		sf_disconnect_sensor(sensor_id);
		fprintf(stdout, "Sensor - %d, failed at sf_get_sensor_data after set data.\n",sensor_id);
		return false;
	}
	fprintf(stdout, "sf_get_sensor_data count=%d timestamp=%llu\n", data.value_count, data.time_stamp);
	for (i = 0; i < data.value_count; i++) {
		fprintf(stdout, "value %d: %f\n", i, data.values[i]);
	}
	sf_disconnect_sensor(sensor_id);
	return true;
}
