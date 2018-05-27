/*Program for tuning line detection ENGR101*/
/* A. Roberts, April 2018*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include "E101.h"


#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
int MOTOR_SPEED=70;
//#define MOTOR_SPEED 70
unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

void followLine(int error, int dv, bool black, int quadrant) // set motors based on the derivative of PID con$
{
	if (quadrant==3){
		MOTOR_SPEED =60;
	}
    int motorOffset = dv;
    printf("%d \n", motorOffset);   
	
    //move forward
	int v_right= MOTOR_SPEED+motorOffset;
                int v_left= MOTOR_SPEED-motorOffset;
    if (v_right>255){v_right=255;}
        if(v_right<-255){v_right=-255;}
	if (v_left>255){v_left=255;}
	if(v_left<-255){v_left=-255;}
                 
    if (error>0) // if error value less than 0 then turn left
	{
        //1 is right

		printf("Left: %d Right: %d \n", v_left, v_right);
		set_motor(1 ,-(v_left));
		set_motor(2, v_right);

		printf("Turn left\n");
	}
		
    else if(error<0) // if error value greater than 0 then turn right
    {
		
		printf("Left: %d Right: %d \n", v_left, v_right); 

		set_motor(1, -(v_left));
		set_motor(2, v_right);

    printf("Turn right\n");
    }
	else if (error==0&&black==false)
	{
		set_motor(1,-MOTOR_SPEED);
		set_motor(2, MOTOR_SPEED); 
		printf("Straight\n");
	}
	else if(black==true){
		set_motor(1,MOTOR_SPEED);
		set_motor(2,- MOTOR_SPEED);
	}
}


/** MAIN FUNCTION */
int main()
{	
	int quadrant = 1;
		
    init();
        
    switch(quadrant)
    {
			case 1: // Quadrant 1
			{
				
		
			}
			
			case 2: // Quadrant 2
			{
				while(true)
				{
					take_picture();
					//display_picture(1,0);
					int scan_row = 120;
					bool black= false;
					bool white= false;
					/*
					for (int i = 0; i <320;i++)
					{
						int pix = get_pixel(scan_row,i,3);
						//printf("i=%d col=%d\n",i,pix);
					}*/

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
						if (max<60)
						{
							whitePixels[i] = 0;
							black=true;
						}
						else if(min>100){
							whitePixels[i]=1;
							white= true;
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
                    
                    for (int i = 0; i < 320; i++) // do for each pixel in r$
                    {
                    previousError = previousError + (i - 160)*whitePixels[i];
                        if (whitePixels[i] == 1)
                        {
                            np = np + 1;
                        }
                    }
                    
                    if (np !=0){
                        previousError = previousError/np;
                    }
                    
                    printf("Previous Error: %d",previousError); 
                    struct timeval t1;
                    struct timeval t2; //structure with time
                    gettimeofday(&t1, 0); // mark time

					// sleep1(0, 10); // sleep 10 milliseconds

                    gettimeofday(&t2, 0); // mark time now
                    long elapsed = (t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec - t1.tv_usec);
                    printf("Time = %d\n", elapsed);                        

                    for (int i = 0; i < 320; i++) // do for each pixel in r
                    {
						error = error + (i - 160)*whitePixels[i];
                        if (whitePixels[i] == 1)
                        {
                            np = np + 1;
                        }
					}
					
					printf("Np is: %d", np);
                    if (np !=0)
                    {
                        error = error/np;
                    }
                    if (np >250){
			
                        error=40;
                    }

                    //printf("error is %d\n", error);
                    //printf("Error: %d \n" , error); 

                    /*if(error==0){
                        error= -10000;
                    }*/

                    printf("error is %d\n", error);
                    double kp = 4;//change kp so robot can follow the line
                    double kd = 0;//change kd until the movement is smooth
                    double derivative = (error - previousError)/(elapsed);
                    int dv = error*kp + derivative*kd;

					printf("dv is %d\n", dv);
					if (white==true){
						quadrant=3;
						break;
					}
					followLine(error, dv, black, quadrant);
					
					
					
                        
				}
			}
			
			case 3: // Quadrant 3
			{
				set_motor(1,-128);
				set_motor(2,128);
				sleep1(0,1 );
				bool straight= true;
				while(straight==true)
				{	
					straight= true;
					printf("Go Straight");
					take_picture();
					//display_picture(1,0);
					int scan_row = 120;
					bool black= false;
					set_motor(1,-100);
					set_motor(2,100);
					
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
					//printf(" min=%d max=%d threshold=%d\n", min, max,threshold);

					int whitePixels[320];  // white pixels
					for (int i = 0; i <320;i++)
					{
						whitePixels[i]= 0 ;
						int pix = get_pixel(scan_row,i,3);
						if (max<60)
						{
							whitePixels[i] = 0;
							black=true;
						}
						if(min>90){
							whitePixels[i]=1;
							straight=false;
							printf("Straight is false");
						}
						
						else if ( pix > threshold)
						{

							whitePixels[i] = 1;
						}
						

						
					}
				}
				
				bool turning= false;
				while(true){

						if (turning ==false){
							sleep1(1,0);
							printf("LEFT");
							set_motor(1,-128);
							set_motor(2, -128);
							turning= true;
						}
						
					take_picture();
		//display_picture(1,0);
        int scan_row = 120;
	bool black= false;
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
            if (max<60){
				whitePixels[i] = 0;
		black=true;
			}
			if(min>100){
				turning=false;
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
					if (np !=0){
						previousError = previousError/np;
					}
					printf("Previous Error: %d",previousError); 
					struct timeval t1;
					struct timeval t2; //structure with time
					gettimeofday(&t1, 0); // mark time

				//	sleep1(0, 10); // sleep 10 milliseconds

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
					
					printf("Np is: %d", np);
					if (np !=0){
						error = error/np;
					}
					if (np >250){
						error=20;
					}
                                        
					

					printf("error is %d\n", error);
					double kp = 4;//change kp so robot can follow the line
					double kd = 0;//change kd until the movement is smooth
					double derivative = (error - previousError)/(elapsed);
					int dv = error*kp + derivative*kd;
			
			printf("dv is %d\n", dv);
			if (turning==true){
			followLine(error, dv, black, quadrant);
		}
			
    
}
}

					
			
			case 4: // Quadrant 4
			{
				while(true)
				{
					
					
				}
			}
		return 0;	
	}
}
    

