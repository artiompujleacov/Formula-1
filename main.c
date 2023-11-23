#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPS 9

void print_sensor_data(sensor *sensor)
{
	if (sensor->sensor_type == TIRE) {
		tire_sensor *tire = (tire_sensor *) sensor->sensor_data;
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", tire->pressure);
		printf("Temperature: %.2f\n", tire->temperature);
		printf("Wear Level: %d%%\n", tire->wear_level);
		if (tire->performace_score == 0) {
			printf("Performance Score: Not Calculated\n");
		} else {
			printf("Performance Score: %d\n", tire->performace_score);
		}
	} else {
power_management_unit *pmu = (power_management_unit *) sensor->sensor_data;
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", pmu->voltage);
		printf("Current: %.2f\n", pmu->current);
		printf("Power Consumption: %.2f\n", pmu->power_consumption);
		printf("Energy Regen: %d%%\n", pmu->energy_regen);
		printf("Energy Storage: %d%%\n", pmu->energy_storage);
	}
}

void remove_sensor_from_array(sensor *sensors, int num_sensors)
{
	for (int i = 0; i < num_sensors; i++) {
		if (sensors[i].sensor_type == TIRE) {
			tire_sensor *tire = (tire_sensor *) sensors[i].sensor_data;
			if ((tire->pressure >= 19 && tire->pressure <= 28 &&
					tire->temperature >= 0 && tire->temperature <= 120 &&
					tire->wear_level >= 0 && tire->wear_level <= 100) == 0) {
      free(sensors[i].sensor_data);
      free(sensors[i].operations_idxs);
				for (int j = i; j < num_sensors - 1; j++) {
					sensors[j] = sensors[j + 1];
				}
				num_sensors--;
				i--;
			}
		} else if (sensors[i].sensor_type == PMU) {
			power_management_unit *pmu =
				(power_management_unit *) sensors[i].sensor_data;
      if ((pmu->voltage >= 10 && pmu->voltage <= 20 && pmu->current >= -100 &&
					pmu->current <= 100 && pmu->power_consumption >= 0 &&
					pmu->power_consumption <= 1000 && pmu->energy_regen >= 0 &&
					pmu->energy_regen <= 100 && pmu->energy_storage >= 0 &&
					pmu->energy_storage <= 100) == 0) {
      free(sensors[i].sensor_data);
      free(sensors[i].operations_idxs);
				for (int j = i; j < num_sensors - 1; j++) {
					sensors[j] = sensors[j + 1];
				}
				num_sensors--;
				i--;
			}
		}
	}
}

int comparator(const void *a, const void *b)
{
	sensor *s1 = (sensor *) a;
	sensor *s2 = (sensor *) b;
	return s2->sensor_type - s1->sensor_type;
}
void get_operations(void **operations);
int main(int argc, char const *argv[])
{
	int num_sensors, sensor_type, num_operations, *operations_idxs;
	void *sensor_data;
	sensor *sensors;
	char a[OPS];

	FILE *f = fopen(argv[1], "rb");
	fread(&num_sensors, sizeof(int), 1, f);
	sensors = malloc(num_sensors* sizeof(sensor));
	for (int i = 0; i < num_sensors; i++) {
		fread(&sensor_type, sizeof(int), 1, f);
		if (sensor_type == PMU) {
			sensor_data = malloc(sizeof(power_management_unit));
			fread(sensor_data, sizeof(power_management_unit), 1, f);
		} else if (sensor_type == TIRE) {
			sensor_data = malloc(sizeof(tire_sensor));
			fread(sensor_data, sizeof(tire_sensor), 1, f);
		}
		fread(&num_operations, sizeof(int), 1, f);
		operations_idxs = malloc(num_operations* sizeof(int));
		fread(operations_idxs, sizeof(int), num_operations, f);
		sensors[i].sensor_type = sensor_type;
		sensors[i].sensor_data = sensor_data;
		sensors[i].nr_operations = num_operations;
		sensors[i].operations_idxs = operations_idxs;
	}
	fclose(f);

	void *operations[OPS];
	get_operations(operations);

	qsort(sensors, num_sensors, sizeof(sensor), comparator);

	while (scanf("%s", a)) {
		int param = 0;
		if (strcmp(a, "exit") == 0) {
			for (int i = 0; i < num_sensors; i++) {
				free(sensors[i].sensor_data);
				free(sensors[i].operations_idxs);
			}
			free(sensors);
			break;
		} else if (strcmp(a, "print") == 0) {
			scanf("%d", &param);
			if ((param < 0) || (param > num_sensors)) {
				printf("Index not in range!\n");
			} else {
				print_sensor_data(&sensors[param]);
			}
		} else if (strcmp(a, "analyze") == 0) {
			scanf("%d", &param);
			for (int i = 0; i < sensors[param].nr_operations; i++) {
      ((void(*)()) operations[sensors[param].operations_idxs[i]])
      (sensors[param].sensor_data);
			}
		} else if (strcmp(a, "clear") == 0) {
			remove_sensor_from_array(sensors, num_sensors);
		}
	}
	return 0;
}
