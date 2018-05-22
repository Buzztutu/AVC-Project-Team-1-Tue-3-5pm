#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include "E101.h"

#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
#define MOTOR_SPEED 150
unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

int main(){
	init();
	while(true){
		int pos = p_data;
		int dv = dev(pos);
	}
	followLine(pos,dv);
}

int followLine(int position, int dv){
	if (position<0){
		v_left = MOTOR_SPEED+dv;
		v_right = MOTOR_SPEED-dv;
        set_motor(1, );
        set_motor(2, MOTOR_SPEED+motorOffset);
	}
	else if(error>0){
		set_motor(1,  MOTOR_SPEED+motorOffset);
        set_motor(2, 0);
	}
}

int p_data(){
	take_picture;
		int error = 0;
		int max = 0;
		int min = 0;
		int threshold = (max-min)/2;
		int white = 0;
		int position = error/white;
		for(int i=0; i<320;i++){
			unsigned char pix = get_pixel(160,i,3);
			if(i<120){
				pd = 120-i;
				error = error+pix*pd;
				if(pix>max){
					max = pix;
				}
				if(pix<min){
					min = pix;
				}
				if(pix>threshold){
					white = white + 1;
					}
			}
			if(i>120){
				pd = i-120;
				error = error+pix*pd;
				if(pix>max){
					max = pix;
				}
				if(pix<min){
					min = pix;
				}
				if(pix>threshold){
					white = white + 1;
				}
			}
		}
	return position;
}
	
int dev(){
	int error = 0;
	int previousError = 0;
	int np = 0;
	for (int i = 0; i < 320; i++){
		previousError = previousError + (i - 160)*whitePixels[i];
		if (whitePixels[i] == 1){
			np = np + 1;
		}
	}

	struct timeval t1;
	struct timeval t2; //structure with time
	gettimeofday(&t1, 0); // mark time

	sleep1(0, 0); // sleep 10 milliseconds

	gettimeofday(&t2, 0); // mark time now
	long elapsed = (t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec - t1.tv_usec);
	printf("Time = %d\n", elapsed);					

	for (int i = 0; i < 320; i++){
		error = error + (i - 160)*whitePixels[i];
		if (whitePixels[i] == 1){
			np = np + 1;
		}
	}
	error = error/np;
			
	if(error==0){
		error= -10000;
	}
	double kp = 0.01;
	double kd = 1;
	double derivative = (error - previousError)/(elapsed);
	int dv = error*kp + derivative*kd;
