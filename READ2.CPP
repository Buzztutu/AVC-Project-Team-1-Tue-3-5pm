/*Program for tuning line detection ENGR101*/
/* A. Roberts, April 2018*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include "E101.h"


#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
#define MOTOR_SPEED 150
unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

void followLine(int error, int dv) // set motors based on the derivative of PID controller 
{
	
		int motorOffset = dv;
		printf("%d \n", motorOffset);   
		if (error<0) // if error value less than 0 then turn left
	    {	
			//1 is right
            /*set_motor(1, MOTOR_SPEED+motorOffset);
            set_motor(2, MOTOR_SPEED-motorOffset);*/
            printf("Turn left\n");
        }
		else if(error>0) // if error value greater than 0 then turn right
		{
            /*set_motor(1,  MOTOR_SPEED-motorOffset);
            set_motor(2, MOTOR_SPEED+motorOffset);*/
			printf("Turn right\n");
		}
		else if (error==0){
			/*set_motor(1,  -MOTOR_SPEED);
			set_motor(2,  -MOTOR_SPEED);*/
			printf("Reversing\n");
		}
}


/** MAIN FUNCTION */
int main()
{	
	init();
	
	while(true){
		take_picture();
		display_picture(1,0);
        int scan_row = 120;
        for (int i = 0; i <320;i++)
        {
            int pix = get_pixel(scan_row,i,3);
            //printf("i=%d col=%d\n",i,pix);
           

        }


        int max = 0;
        int min =255;
        for (int i = 0; i <320;i++)
        {
            int pix = get_pixel(scan_row,i,3);
            if ( pix > max)
            {
                max = pix;
            }
            if (pix < min)
            {
                min =pix;
            }
        }
        int threshold = (max+min)/2; // set the threshold for black and white pixels
        printf(" min=%d max=%d threshold=%d\n", min, max,threshold);

        int whitePixels[320];  // white pixels
        for (int i = 0; i <320;i++)
        {
            whitePixels[i]= 0 ;
            int pix = get_pixel(scan_row,i,3);
            if (max<100){
				whitePixels[i] = 0;
			}
            
            else if ( pix > threshold)
            {
                whitePixels[i] = 1;
            }
            

            
        }

        
        
        /** Do error calculation */
					int error = 0;
					int previousError = 0;
					int np = 0;
					for (int i = 0; i < 320; i++) // do for each pixel in row
					{
						previousError = previousError + (i - 160)*whitePixels[i];
						if (whitePixels[i] == 1)
						{
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

					for (int i = 0; i < 320; i++) // do for each pixel in row
					{
						error = error + (i - 160)*whitePixels[i];
						if (whitePixels[i] == 1)
						{
							np = np + 1;
						}
					}
					error = error/np;
					//printf("error is %d\n", error);
					//printf("Error: %d \n" , error); 
					
					/*if(error==0){
						error= -10000;
					}*/
					printf("error is %d\n", error);
					double kp = 0.01;//change kp so robot can follow the line
					double kd = 1;//change kd until the movement is smooth
					double derivative = (error - previousError)/(elapsed);
					int dv = error*kp + derivative*kd;
			
			printf("dv is %d\n", dv);
			followLine(error, dv);
			sleep1(1,0);
    }
    
    
}

