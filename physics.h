#ifndef PHYS_H
#define PHYS_H

#define MAX_PLANETS 30
#define G 1000

typedef struct {
	double x, y;
} Vector2D;

typedef struct {
	Vector2D position, velocity;
	float radius, mass;
} Planet;

Planet planets[MAX_PLANETS];

#endif
