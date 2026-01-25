#include "physics.h"
#include <math.h>

#define PI 3.141592653

int current_capacity = 0;

double magnitude(Vector2D v){
	return sqrt(v.x * v.x + v.y * v.y);
}

Vector2D sub(Vector2D a, Vector2D b){
	return (Vector2D){a.x - b.x, a.y - b.y};
}

Vector2D add(Vector2D a, Vector2D b){
	return (Vector2D){a.x + b.x, a.y + b.y};
}

Vector2D scale(Vector2D v, double f){
	return (Vector2D){v.x * f, v.y * f};
}

void resolveForce(Planet *p1, Planet *p2, double dt){
	Vector2D r = sub(p1->position, p2->position);
	double distance = magnitude(r);
	double force = (G * p1->mass * p2->mass) / (distance * distance);
	Vector2D direction = {r.x / distance, r.y / distance};
	Vector2D forceDir = scale(direction, force);
	Vector2D acceleration = scale(forceDir, 1/p1->mass);
	Vector2D newVelocity = add(p1->velocity, scale(acceleration, dt));

	p1->velocity = newVelocity;

	return;	
}
