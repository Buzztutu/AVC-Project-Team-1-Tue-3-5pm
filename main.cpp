/** ~ Group Code Full ~ **/
/** Team 1 ~ Tuesday ~ 3-5pm **/

//Include Libraries
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include <sys/time.h>
	#include "E101.h"
 
//Camera Setup
	#define CAMERA_WIDTH 320 //Control Resolution from Camera
	#define CAMERA_HEIGHT 240 //Control Resolution from Camera
	#define MOTOR_SPEED 150 //Set motor speed
	
//Quadrant Setup
	int Quad = 1;
	
//Variable Setup for Q2 & Q3
	int error = 0;
	int max = 255;
	int min = 0;
	int threshold = 0;
	int pos = 0;
	int pixelHor[320];
	int Kp = -0.005;
	int Kd = -0.05;
	int prevError = 0;
	int de = 0;
	int dt = 0;
	
 unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];
int main() {
/** ~ Quadrant 1 - Network Gate ~ **/
	while (Quad = 1) {
	//Server and port specification
		char serverIP [15]= "130.195.6.196"; 
		int Port = 1024;
	//Initialise message conversation
		char message [24]= "Please";
		connect_to_server(serverIP,Port); //Connect to server
		send_to_server(message); //Send password request
		receive_from_server(message); //Recieve password
		send_to_server(message); //Send password
		
		Quad++;
		}
/** ~ End of Q1 ~ **/

/** ~ Quadrant 2 - Follow Line ~ **/
	while (Quad = 2){
	void followLine(int error, int dv) // set motors based on the derivative of PID controller 
	{
	//Take picture
		take_picture();
		
	//Calculate Max, Min & Threshold
		unsigned char pix = get_pixel(120,i,3); //Get pixel on each for the line
		for (int i = 0; i < 320; i++) {
			unsigned char pix = get_pixel(160,i,3);
			if (max < pix) {
				max = pix;
				}
			if (pix < min) {
				min = pix;
				}
			}
		threshold = ((max-min)/2);
		
	//Calculate white pixels
		for (int i = 0; i < 320; i++) {
			if (pix < threshold) {
				pixelHor[i] = 0;
				}
			if (threshold < pix) {
				pixelHor[i] = 1;
				}
			}
	
	//Calculate and weigh error
		for (int i = 0; i < 320; i++) {
			//Pixels on the left hand side of the image added
			if (i >= 160) {
					if (i < 32){margin = 5;}
					if (32 =< i < 64) {margin = 4;}
					if (64 =< i < 96) {margin = 3;}
					if (96 =< i < 128) {margin = 2;}
					if (128 < i ) {margin = 1;}
				error = error + (margin*pixelHor[i]);
				}
			else if (x < 160) {
					if (x < 192){margin = 1;}
					if (192 =< i < 224) {margin = 2;}
					if (224 =< i < 256) {margin = 3;}
					if (256 =< i < 288) {margin = 4;}
					if (288 < i ) {margin = 5;}
				error = error - (margin*pixelHor[i]);
				}
			}
	//Time difference
	struct timeval t1;
	struct timeval t2; //structure with time
	gettimeofday(&t1, 0); // mark time

	sleep1(0, 0); // sleep 10 milliseconds

	gettimeofday(&t2, 0); // mark time now
	long elapsed = (t2.tv_sec - t1.tv_sec);
	printf("Time = %d\n", elapsed);
	
	//Deficate Calculation
	de = error - prevError;
	
	//Motor offset defined
		int motorOffset = (error*Kp)+((de/elapsed)*Kd);
		printf("%d \n", motorOffset);   
 	
 	// ~ NOTE ~ //
 	//Motor 1 is right, Motor 2 is left
	// if error value less than 0 then turn left
 		if (error > 0) {
			set_motor(1 , MOTOR_SPEED+motorOffset);
            set_motor(2, MOTOR_SPEED-motorOffset);
            sleep(0, 100);
            printf("Turning Left");
			} 
	// if error value greater than 0 then turn right
		else if (error < 0) {
			set_motor(1, MOTOR_SPEED+motorOffset);
            set_motor(2, MOTOR_SPEED-motorOffset);
            sleep(0, 100);
            printf("Turning Right");
			}
	// if no line is detected then reverse
		else if (error==0){
			set_motor(1, -MOTOR_SPEED);
			set_motor(2, -MOTOR_SPEED);
			sleep(0, 100);
			printf("Reversing\n");
 		}
 		
	}
	}
/** ~ End of Q2 ~ **/
}
