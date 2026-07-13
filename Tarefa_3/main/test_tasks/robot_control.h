
#ifndef ROBOT_CONTROL_H
#define ROBOT_CONTROL_H

#include <stdint.h>

extern volatile int line_error;

extern volatile float left_speed_ref;
extern volatile float right_speed_ref;

extern volatile float encoder_left_speed;
extern volatile float encoder_right_speed;

extern volatile int obstacle_detected;

extern volatile int fault_mode;

extern volatile uint32_t wheel_heartbeat;
extern volatile uint32_t threeeyes_heartbeat;
extern volatile uint32_t ir_heartbeat;

#endif