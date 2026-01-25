#ifndef PHYS_H
#define PHYS_H

#define MAX_PLANETS 30
#define G 1000

typedef struct {
	float x, y, radius, vx, vy, fx, fy, mass;
} Planet;

Planet planets[MAX_PLANETS];

void resolveForce(Planet p1, Planet p2);

#endif
