#ifndef VIBRATION_H
#define VIBRATION_H

#define MAX_POINTS 1000

extern double time_data[MAX_POINTS], displacement[MAX_POINTS];
extern int data_count;

void simulate_vibration(double frequency, double amplitude, double damping, double max_time, double time_step);
void export_to_csv(const char *filename);

#endif // VIBRATION_H
