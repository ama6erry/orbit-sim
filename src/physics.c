#include "physics.h"
#include <math.h>
#include <stdio.h>

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
	Vector2D r = sub(p2->position, p1->position);
	double distance = magnitude(r);
	if(distance < 1e-6) return;
	double force = (G * p1->mass * p2->mass) / (distance * distance + 10000);
	Vector2D direction = {r.x / distance, r.y / distance};
	Vector2D forceDir = scale(direction, force);
	Vector2D accelerationP1 = scale(forceDir, 1/p1->mass);
	Vector2D accelerationP2 = scale(forceDir, -1 / p2->mass);
	Vector2D newVelocity = add(p1->velocity, scale(accelerationP1, dt));

	p1->velocity = newVelocity;
	p2->velocity = add(p2->velocity, scale(accelerationP2, dt));

	Vector2D initialPos = p1->position;
	p1->position = add(p1->position, scale(p1->velocity, dt));
	p2->position = add(p2->position, scale(p2->velocity, dt));
	return;	
}
