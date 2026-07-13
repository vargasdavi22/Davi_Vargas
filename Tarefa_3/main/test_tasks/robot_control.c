
#include "robot_control.h"

volatile int line_error = 0;

volatile float left_speed_ref = 0.0f;
volatile float right_speed_ref = 0.0f;

volatile float encoder_left_speed = 0.0f;
volatile float encoder_right_speed = 0.0f;

volatile int obstacle_detected = 0;