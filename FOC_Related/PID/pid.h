#ifndef __PID_H_
#define __PID_H_

typedef struct
{
	float Set;
	float Actual;
	float err;
	float err_last;
	float Kp, Ki, Kd;
	float voltage;
	float integral;
} pid;

void PID_init(void);
extern pid pid_angle;
extern pid pid_speed;
extern pid pid_current;

#endif
