#ifndef PHYS_H
#define PHYS_H

#define MAX_PLANETS 2
#define G 100000

typedef struct {
	double x, y;
} Vector2D;

typedef struct {
	Vector2D position, velocity;
	float radius, mass;
} Planet;

void resolveForce(Planet *p1, Planet *p2, double dt);

#endif
