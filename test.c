#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//never going to rotate the z axis
//need the x and y axis angles (A and B)
//lets start by projecting the x and y values into a fake 3d value

float rad = 400; //half the height in this case


//when x and y are 0, z is 40
//when x is all the way to the left, y and z are 0
//z = sqrt(radius2 - xpos2 - ypos2)

//1440 x 900 screen

int halfw = 400;
int halfh = 400;


int findZ(int x, int y){
    int z;
    if ((x * x) + (y * y)> (rad * rad)){
        z = 0;
    } else{
        z = sqrt(fabs((rad * rad) - (x * x) - (y * y)));
    }

    return z;
}


int main() {
    FILE *fp;
    FILE *fptr;
    char path[32];
    int x, y;

     // Run the Python script
    fp = popen("python3 mouse.py", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    while(1){
       
        // Open the file where mouse position is written
        fptr = fopen("mouse.txt", "r");
        if (fptr == NULL) {
            printf("Failed to open file\n");
            pclose(fp);
            exit(1);
        }

        // Read the mouse position from the file
        if (fgets(path, sizeof(path) - 1, fptr) != NULL) {
            sscanf(path, "%d,%d", &x, &y);
            x = x - halfw;
            y = y - halfh;
            int z = findZ(x, y);
            float A = acos(x / rad);
            float B = acos(y / rad);
            printf("Mouse Position: %d, %d, %d\n", x, y, z);
            printf("angle: %g, %g\n", A, B);
        } else {
            printf("Failed to read file\n");
        }

        // Close the files
        fclose(fptr);
    }
    pclose(fp);

    return 0;
}
