#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <control_system_manager/control_system_manager.h>
#include <pid/pid.h>

struct holonomic_base_speed_cs {
    bool vx_control_enable;
    bool vy_control_enable;
    bool omega_control_enable;
    struct cs vx_cs;
    struct cs vy_cs;
    struct cs omega_cs;
    struct pid_filter vx_pid;
    struct pid_filter vy_pid;
    struct pid_filter omega_pid;
    float vx;
    float vy;
    float omega;
    int32_t out_x;
    int32_t out_y;
    int32_t out_rotation;
};

extern struct holonomic_base_speed_cs nastya_cs;

void control_update_setpoint_vx(float x);
void control_update_setpoint_vy(float y);
void control_update_setpoint_omega(float r);
void control_init(void);

#endif // CONTROL_H
