#include "cvra_cs.h"
#include "strat.h"
#include <string.h>
#include <holonomic/trajectory_manager.h>
#include <holonomic/position_manager.h>
#include <scheduler.h>
#include <aversive/error.h>
#include <cvra_servo.h>
#include <uptime.h>
#include "adresses.h"

struct strat_info strat;

void strat_long_arm_up(void){
        cvra_servo_set((void*)SERVOS_BASE, 1, 15000); 
}

void strat_long_arm_down(void){
        cvra_servo_set((void*)SERVOS_BASE, 1, 7000); 
}

void strat_short_arm_up(void){
        cvra_servo_set((void*)SERVOS_BASE, 0, 17000); 
}

void strat_short_arm_down(void){
        cvra_servo_set((void*)SERVOS_BASE, 0, 8000); 
}


/** Increments the match timer, called every second. */
static void increment_timer(__attribute__((unused))void *data) {
    strat.time++;
}


void strat_set_objects(void) {
    memset(&strat.glasses, 0, sizeof(glass_t)*12);
    memset(&strat.gifts, 0, sizeof(gift_t)*4);

    /* Init gifts position. */ 
    strat.gifts[0].x = 525; /* middle of the gift. */
    strat.gifts[1].x = 1125;
    strat.gifts[2].x = 1725;
    strat.gifts[3].x = 2325;

    /* Init glasses positions. */
    strat.glasses[0].pos.x = 900; strat.glasses[0].pos.y = (1550);
    strat.glasses[1].pos.x = 900; strat.glasses[1].pos.y = (1050);
    strat.glasses[2].pos.x = 1050; strat.glasses[2].pos.y = (1200);

    /*XXX Not sure about coordinates of 3 and 4. */
    strat.glasses[3].pos.x = 1200; strat.glasses[3].pos.y = (1550);
    strat.glasses[4].pos.x = 1200; strat.glasses[4].pos.y = (1050);
    strat.glasses[5].pos.x = 1350; strat.glasses[5].pos.y = (1200);
    strat.glasses[6].pos.x = 1650; strat.glasses[6].pos.y = (1300);
    strat.glasses[7].pos.x = 1800; strat.glasses[7].pos.y = (1550);
    strat.glasses[8].pos.x = 1800; strat.glasses[8].pos.y = (1050);
    strat.glasses[9].pos.x = 1950; strat.glasses[9].pos.y = (1300);
    strat.glasses[10].pos.x = 2100; strat.glasses[10].pos.y = (1550);
    strat.glasses[11].pos.x = 2100; strat.glasses[11].pos.y = (1050);
}


/** @todo : passe to double */
void strat_start_position(void) {
    //distance centre/ coté : 88.5 mm
    //distance centre /calibre : 112.87 mm
    //épaisseur bord blanc : 100 mm
    holonomic_position_set_x_s16(&robot.pos, 88.5);
    holonomic_position_set_y_s16(&robot.pos,COLOR_Y(2000 - 213));
    holonomic_position_set_a_s16(&robot.pos, COLOR_A(90));

}

void strat_begin(strat_color_t color) {
    int i = 0;

    /* Starts the game timer. */
    strat.time = 0;
    strat.color = color;
    scheduler_add_periodical_event(increment_timer, NULL, 1000000/SCHEDULER_UNIT);
    
    strat_set_objects();
    strat_start_position();

    strat_long_arm_down();
    strat_short_arm_down();

    holonomic_trajectory_moving_straight_goto_xy_abs(&robot.traj, 200, COLOR_Y(1800));

    while((IORD(PIO_BASE, 0) & 0x1000) == 0);

    holonomic_trajectory_moving_straight_goto_xy_abs(&robot.traj, 500, COLOR_Y(1500));

    while(!holonomic_end_of_traj(&robot.traj));

    for (i = 0; i < 4; i++) {
        strat_do_gift(i);
    }
    strat_short_arm_down();
}

/** 
 * @brief Do the gift
 */
void strat_do_gift(int number) {
    strat_short_arm_down();
    holonomic_trajectory_moving_straight_goto_xy_abs(&robot.traj,
                                                     strat.gifts[number].x 
                                                     + COLOR_C,
                                                     COLOR_Y(2000-150));
    while(!holonomic_end_of_traj(&robot.traj));
    holonomic_trajectory_turning_cap(&robot.traj, COLOR_A(TO_RAD(-90)));
    while(!holonomic_end_of_traj(&robot.traj));
    holonomic_trajectory_moving_straight_goto_xy_abs(&robot.traj,
                                                     strat.gifts[number].x 
                                                     + COLOR_C,
                                                     COLOR_Y(2000-150));
    while(!holonomic_end_of_traj(&robot.traj));
    strat_short_arm_up();

    int32_t time = uptime_get();
    while(time + 500000 > uptime_get());

}
