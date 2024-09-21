#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>

float A, B, C;
float cubeWidth = 20;
int width, height;
float* zBuffer = NULL;
char* buffer = NULL;
int background = ' ';
float increment = 0.6;
int distance;
float K1 = 40;
int light = 1;
float L[6];
int lmode = 1;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculate(float cubeX, float cubeY, float cubeZ, char ch, float L) {
    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + distance;

    float ooz = 1 / z;

    int xp = (int)(width / 2 + K1 * ooz * x * 2);
    int yp = (int)(height / 2 + K1 * ooz * y);

    if (lmode){
        L = L * -1;
    }

    int idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            if (light){
                if (L < -1.6){
                buffer[idx] = '@';
                } else if (L < -1){
                    buffer[idx] = '#';
                } else if (L < -0.5){
                    buffer[idx] = '$';
                } else if (L < 0){
                    buffer[idx] = '+';
                } else if (L < 0.5){
                    buffer[idx] = '!';
                } else if (L < 1){
                    buffer[idx] = ';';
                } else if (L < 1.6){
                    buffer[idx] = '~';
                } else{
                    buffer[idx] = '.';
                }
            } else{
                buffer[idx] = ch;
            }
        }
    }
}

EMSCRIPTEN_KEEPALIVE
void init() {
    zBuffer = (float*)malloc(width * height * sizeof(float));
    buffer = (char*)malloc(width * height * sizeof(char));
}

EMSCRIPTEN_KEEPALIVE
void updateRotation(float newA, float newB, float newC) {
    A = newA;
    B = newB;
    C = newC;
}

EMSCRIPTEN_KEEPALIVE
void updateDistance(int newDistance) {
    distance = newDistance;
}

EMSCRIPTEN_KEEPALIVE
void updateLight(int newLight) {
    light = newLight;
}

EMSCRIPTEN_KEEPALIVE
void darkMode(int newMode) {
    lmode = newMode;
}


EMSCRIPTEN_KEEPALIVE
char* renderFrame(int newHeight, int newWidth) {
    height = newHeight;
    width = newWidth;
    zBuffer = (float*)realloc(zBuffer, width * height * sizeof(float));
    buffer = (char*)realloc(buffer, width * height * sizeof(char));

    //if the light is on, we'll precompute the normals at this angle
     if (light){
            //light is at (0, 1, 0), so we're only using the y norm
            L[0] = calculateY(0, 0, 1) * 2;
            L[1] = calculateY(-1, 0, 0) * 2;
            L[2] = calculateY(1, 0, 0) * 2;
            L[3] = calculateY(0, 0, -1) * 2;
            L[4] = calculateY(0, 1, 0) * 2;
            L[5] = calculateY(0, -1, 0) * 2;
        }

    memset(buffer, background, width * height * sizeof(char));
    memset(zBuffer, 0, width * height * sizeof(float));

    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += increment) {
        for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += increment) {
            calculate(cubeX, cubeY, -cubeWidth, '~', L[0]);
                calculate(cubeWidth, cubeY, cubeX, '$', L[1]);
                calculate(-cubeWidth, cubeY, -cubeX, ';', L[2]);
                calculate(-cubeX, cubeY, cubeWidth, '+', L[3]);
                calculate(cubeX, -cubeWidth, -cubeY, '@', L[4]);
                calculate(cubeX, cubeWidth, cubeY, '#', L[5]);
        }
    }

    return buffer;
}

EMSCRIPTEN_KEEPALIVE
void cleanup() {
    free(zBuffer);
    free(buffer);
}