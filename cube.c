#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>

float A, B, C;
float x, y, z;
float ooz;
float cubeWidth = 20;
int width, height;
float* zBuffer = NULL;
char* buffer = NULL;
int background = ' ';
float increment = 0.6;
int distance = 120;
int xp, yp;
float K1 = 40;
int idx;
int fps = 24;
float accel = 0.000001;
int light = 0;
int stats = 1;
int lmode = 0;


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


void calculate(float cubeX, float cubeY, float cubeZ, int ch, float L) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distance;

    ooz = 1 / z;

    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;

    //potentially to make the light better
    // if (yp <= height / 3){
    //     L +=1;
    // } else if (yp < (height / 3) + 5){
    //     L +=0.5;
    // }

    //for dark mode
    if (lmode){
        L = L * -1;
    }
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

int is_input_available() {
    fd_set read_fds;
    struct timeval timeout = {0}; // No timeout, return immediately

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    return select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
}

int main(int argc, char *argv[]) {
    if (argc == 1){
        lmode = 1; //basically go to light mode
    }
    //this lets us input without curses
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    //initialize
    float XVelocity = 0;
    float Xaccel = 0;

    float YVelocity = 0;
    float Yaccel = 0;

    float ZVelocity = 0;
    float Zaccel = 0;

    int XTime = 0;
    int YTime = 0;
    int ZTime = 0;

    int speed = 1000000 / fps;
    int framecount = 0;

    int key;
    int mode = 0;

    float L[6];

    struct winsize w;

    zBuffer = (float*)malloc(width * height * sizeof(float));
    buffer = (char*)malloc(width * height * sizeof(char));

    printf("\x1b[2j");

    //python stuff for mousemove
    FILE *fp;
    FILE *fptr;
    char path[32];
    int mousex, mousey;

    fp = popen("python3 mouse.py", "r");
    if (fp == NULL) {
        printf("Failed to run mouse.py\n");
        exit(1);
    }

    while (1) {
        //setup for resize term
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        height = w.ws_row;
        width = w.ws_col;

        //setup for mouse control
        //set for 1440 x 900 screen
        int halfw = 1440 / 2;
        int halfh = 900 / 2;
        float rad = halfw;

        zBuffer = (float*)realloc(zBuffer, width * height * sizeof(float));
        buffer = (char*)realloc(buffer, width * height * sizeof(char));

        framecount++;

        if (is_input_available()) {
            key = getchar();
            switch (key) {
                case 'w':
                    Yaccel += accel;
                    YTime = 0;
                    break;
                case 's':
                    Yaccel -= accel;
                    YTime = 0;
                    break;
                case 'a':
                    Xaccel += accel;
                    XTime = 0;
                    break;
                case 'd':
                    Xaccel -= accel;
                    XTime = 0;
                    break;
                case 'q':
                    Zaccel += accel;
                    ZTime = 0;
                    break;
                case 'e':
                    Zaccel -= accel;
                    ZTime = 0;
                    break;
                case ' ': // pause button
                    while (1) {
                        if (getchar() == ' ') {
                            break;
                        }
                    }
                    break;
                case 'r': // reset button
                    XVelocity = 0;
                    Xaccel = 0;

                    YVelocity = 0;
                    Yaccel = 0;

                    ZVelocity = 0;
                    Zaccel = 0;

                    XTime = 0;
                    YTime = 0;
                    ZTime = 0;

                    A = 0;
                    B = 0;
                    C = 0;
                    break;
                case '=': // zoom in (plus button)
                    distance -= 5;
                    break;
                case '-': // zoom out (minus button)
                    distance += 5;
                    break;
                case 'l': //lighting toggle
                    light = !light;
                    break;
                case 'p':
                    exit(0);
                    break;
                case 'm':
                    mode++;
                    mode = mode % 3;
                    break;
                case 'i':
                    stats = !stats;
                    break;
            }
        }

        if (mode == 0){ //auto spin
            A += 0.05;
            B += 0.05;
            C += 0.05;
        } else if (mode == 1){ //acceleration control
            // v = v0 + at
            // d = d0 + vt
            // t = number of frames since changed direction

            XTime++;
            YTime++;
            ZTime++;

            XVelocity += (Xaccel * XTime);
            YVelocity += (Yaccel * YTime);
            ZVelocity += (Zaccel * ZTime);

            A += YVelocity;
            B += XVelocity;
            C += ZVelocity;
        } else{ //mouse pos
            // Open the file where mouse position is written
            fptr = fopen("mouse.txt", "r");
            if (fptr == NULL) {
                printf("Failed to open file\n");
                pclose(fp);
                exit(1);
            }

            // Read the mouse position from the file
            if (fgets(path, sizeof(path) - 1, fptr) != NULL) {
                sscanf(path, "%d,%d", &mousex, &mousey);
                mousex = mousex - halfw;
                mousey = mousey - halfh;
                B = -acos(mousex / rad)+ (3.14 / 2);
                A = acos(mousey / rad);  
            }
            // Close the files
            fclose(fptr);
        }

        //if the light is on, we'll precompute the normals at this angle
        if (light){
            //float normx = calculateX(faceX, faceY, faceZ);
            //float normy = calculateY(faceX, faceY, faceZ);
            //float normz = calculateZ(faceX, faceY, faceZ);
            //not using this ^

            //light is at (0, 1, 0), so we're only using the y norm
            L[0] = calculateY(0, 0, 1) * 2;
            L[1] = calculateY(-1, 0, 0) * 2;
            L[2] = calculateY(1, 0, 0) * 2;
            L[3] = calculateY(0, 0, -1) * 2;
            L[4] = calculateY(0, 1, 0) * 2;
            L[5] = calculateY(0, -1, 0) * 2;
        }

        memset(buffer, background, width * height);
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

        //clears the screen
        printf("\x1b[H");

        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : 10);
        }

        //debug info
        if (stats){
            printf(" Frame: %d (%d fps)\n", framecount, fps);
            printf("Stats: 'i', Change Mode: 'm', Light: 'l', Zoom: '-/=', Pause: 'space', Reset: 'r', Quit: 'p'\n");
            switch (mode){
                case 0:
                    printf("Mode: Auto");
                    break;
                case 1:
                    printf("Mode: Acceleration Control (wasdqe)\n");
                    printf("X Velocity = %g, X Acceleration = %g\n", XVelocity, Xaccel);
                    printf("Y Velocity = %g, Y Acceleration = %g\n", YVelocity, Yaccel);
                    printf("Z Velocity = %g, Z Acceleration = %g", ZVelocity, Zaccel);
                    break;
                case 2:
                    printf("Mode: Mouse Control\n");
                    printf("Mouse Position: %d, %d\n", mousex, mousey);
                    printf("Angle: %g, %g", A, B);
                    break;
            }    
            fflush(stdout);
        }
        usleep(speed);
    }

    //reset terminal and free :)
    pclose(fp);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    free(buffer);
    free(zBuffer);

    return 0;
}
