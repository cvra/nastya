#ifndef OBSTACLE_AVOIDANCE_H_
#define OBSTACLE_AVOIDANCE_H_

#include <lwip/ip.h>

typedef struct {
    int x,y; /* mm */
    int vx, vy; /* mm / s */
    int r; /* mm */
} obstacle_avoidance_obstacle_t;

typedef struct {
    int vx, vy;
    int x, y;
    int timestamp;
} obstacle_avoidance_point_t;

typedef struct {
    obstacle_avoidance_point_t *points;
    int len;
} obstacle_avoidance_path_t;

typedef struct {
    int desired_datapoints;
    int desired_samplerate; /* ms */

    obstacle_avoidance_point_t start;
    obstacle_avoidance_point_t end;

    obstacle_avoidance_obstacle_t *obstacles;
    int obstacle_count;
} obstacle_avoidance_request_t;


void obstacle_avoidance_request_create(obstacle_avoidance_request_t *r, int obstacle_count);

void obstacle_avoidance_request_delete(obstacle_avoidance_request_t *r);

char *obstacle_avoidance_request_encode(obstacle_avoidance_request_t *r);

int obstacle_avoidance_decode_path(obstacle_avoidance_path_t *path,const char *json);

void obstacle_avoidance_delete_path(obstacle_avoidance_path_t *path);

int obstacle_avoidance_send_request(obstacle_avoidance_request_t *request, struct ip_addr remote_ip, int port, obstacle_avoidance_path_t *path);
#endif
