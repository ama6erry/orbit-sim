#include <raylib.h>
#include <stdio.h>
#include "physics.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform vec2 planetPos;\n"
"uniform float planetRadius;\n"
"uniform vec2 screenSize;\n"
"out vec2 localPos;\n"
"void main(){\n"
"localPos = aPos;\n"
"vec2 pos = planetPos + aPos * planetRadius;\n"
"vec2 ndc = (pos / screenSize) * 2.0 - 1.0;\n"
"gl_Position = vec4(ndc, 0.0, 1.0);\n"
"}\n";   

const char* fragShaderSource =
"#version 330 core\n"
"in vec2 localPos;\n"
"out vec4 FragColor;\n"
"void main(){\n"
"float dist = length(localPos);\n"
"if(dist > 1.0)\n"
"   discard;\n"
"FragColor = vec4(1.0, 0.5, 1.0, 1.0);\n"
"}\n";    
    
    

GLFWwindow* initGLFW(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbital Sim", NULL, NULL);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);	
    glViewport(0, 0, fbWidth, fbHeight);
    return window;

}

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
       planetsList[i] = (Planet) {(Vector2D){randx, randy}, (Vector2D){randvx, randvy}, 30.0, 300.0}; 
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

//void DrawPlanets(Planet* planetsList, int size){
//    Vector2 pos;
//    double x, y;
//    for(int i = 0; i < size; i++){
//        x = planetsList[i].position.x;
//        y = planetsList[i].position.y;
//        DrawCircle(round(x), YwToYs(round(y)), planetsList[i].radius, RED);
//    }
//}

int main(){
    Planet* planets = malloc(MAX_PLANETS * sizeof(Planet));
    //InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit sim");
    //SetTargetFPS(FPS);
    generatePlanets(planets, MAX_PLANETS);
    GLFWwindow* window = initGLFW();
    float quadVertices[] = {
    -1.f, -1.f,
     1.f, -1.f,
     1.f,  1.f,
    -1.f,  1.f
    };

    unsigned int indices[] = {
    0,1,2,
    2,3,0
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);	
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    	printf("Vertex shader error:\n%s\n", infoLog);
	return -1;	
    } 

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    	printf("Vertex shader error:\n%s\n", infoLog);
	return -1;	
    } 

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);		
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    GLint posLoc = glGetUniformLocation(shaderProgram, "planetPos");
    GLint radiusLoc = glGetUniformLocation(shaderProgram, "planetRadius");
    GLint screenLoc = glGetUniformLocation(shaderProgram, "screenSize");


    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);	
    long previousTime = timeInMilliseconds();
    while(!glfwWindowShouldClose(window)){
       long startTime = timeInMilliseconds();
       long dt = startTime - previousTime;
       previousTime = startTime;
       updatePlanets(planets, MAX_PLANETS, (double)dt/1000); 
 
       glClear(GL_COLOR_BUFFER_BIT);
       glBindVertexArray(VAO);

       glUseProgram(shaderProgram);
       glUniform2f(screenLoc, fbWidth, fbHeight);		
       for(int i=0; i<MAX_PLANETS; i++){
		Planet* p = &planets[i];
		glUniform2f(posLoc, p->position.x, p->position.y);
		glUniform1f(radiusLoc, p->radius);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	
	}	 	
	glfwSwapBuffers(window);
	glfwPollEvents();	
       //BeginDrawing();
        //ClearBackground(WHITE);
        //DrawPlanets(planets, MAX_PLANETS);
       //EndDrawing(); 
    }

    printf("Orbit sim inshallah\n");
}
