#include <raylib.h>
#include <stdio.h>
#include "physics.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define FPS 60


double YwToYs(double Yw){
    return SCREEN_HEIGHT - Yw;
}

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

void generatePlanets(Planet* planetsList, int size){
    srand(time(NULL));
    int randx;
    int randy;
    int randvx;
    int randvy;
    for(int i = 0; i < size; i++){
       randx = rand() % SCREEN_WIDTH;
       randy = rand() % SCREEN_HEIGHT;
       randvx = rand() % 40;
       randvy = rand() % 40;
       printf("randvx %d, randvy %d\n", randvx, randvy); 
       planetsList[i] = (Planet) {(Vector2D){randx, randy}, (Vector2D){randvx, randvy}, 30.0, 30.0}; 
    }
}

void updatePlanets(Planet* planetsList, int size, double dt){
    for(int i = 0; i < size; i++){
        Planet* p1 = &planetsList[i];
        for(int j = i + 1; j < size; j++){
            resolveForce(p1, &planetsList[j], dt); 
        }
    }
}

void DrawPlanets(Planet* planetsList, int size){
    Vector2 pos;
    double x, y;
    for(int i = 0; i < size; i++){
        x = planetsList[i].position.x;
        y = planetsList[i].position.y;
        DrawCircle(round(x), YwToYs(round(y)), planetsList[i].radius, RED);
    }
}

int main(){
    Planet* planets = malloc(MAX_PLANETS * sizeof(Planet));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit sim");
    SetTargetFPS(FPS);
    generatePlanets(planets, MAX_PLANETS);
    long previousTime = timeInMilliseconds();
    while(!WindowShouldClose()){
       long startTime = timeInMilliseconds();
       long dt = startTime - previousTime;
       previousTime = startTime;
       updatePlanets(planets, MAX_PLANETS, (double)dt/1000); 

       BeginDrawing();
        ClearBackground(WHITE);
        DrawPlanets(planets, MAX_PLANETS);
       EndDrawing(); 
    }

    printf("Orbit sim inshallah\n");
}
