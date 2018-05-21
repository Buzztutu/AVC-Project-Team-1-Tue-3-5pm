/*Program for tuning line detection ENGR101*/
/* A. Roberts, April 2018*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include "E101.h"

// Networking
char serverIP [15]= "130.195.6.196";
int Port = 1024;
char message [24]= "Please";

// Constants
#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
#define MOTOR_SPEED 100 // initial motor speed

unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminocity
// for pixel located at (row,column)
unsigned char getPixel( int row,int col, int color)
{
    // calculate address in 1D array of pixels
    int address = CAMERA_WIDTH*row*3 + col*3;
    if ((row < 0 ) || (row > CAMERA_HEIGHT) )
    {
        printf("row is out of range\n");
        return -1;
    }
    if ( (col< 0) || (col > CAMERA_WIDTH))
    {
        printf("column is out of range\n");
        return -1;
    }

    if (color==0)
    {
        return (pixels_buf[address]);
    }
    if (color==1)
    {
        return (pixels_buf[address + 1]);
    }
    if (color==2)
    {
        return (pixels_buf[address + 2]);
    }
    if (color==3)
    {
        unsigned char y = (pixels_buf[address] + pixels_buf[address+1] +pixels_buf[address+2])/3;
        return y;
    }
    printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
    return -2; //error
}

long measureTime() // measure time for PID Controller
{
        struct timeval t1;
        struct timeval t2; //structure with time
        gettimeofday(&t1, 0); // mark time
        for (int i = 0; i<20000; i++){ //something long
			for (int j = 0; j<20000; j++){
				//int ss = 9;
			}
		}
		gettimeofday(&t2, 0); // mark time now
		long elapsed = (t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec - t1.tv_usec);
		return elapsed;
}

void doCamera() // take picture with mounted camera and scan the image
				{
				take_picture("scan.txt"); // take a picture with the mounted camera
				save_picture[8]("scan.txt");
				
				/** save horizontal scan of the image into text file */
				FILE *scfile;
				scfile = fopen("scan.txt","w");
				int rightImg
				for (int i = 0; i <320;i++)
				{
					int pix = getPixel(120,i,3);
					fprintf(scfile,"%d %d\n",i,pix);
					
					if (i < 160)
					{
						
					}
				}
				fclose(scfile);
				}

void followLine(int error, int dv) // set motors based on the derivative of PID controller 
{
	int DEBUG = 1;
	int motorOffset = 0;
	if (error != 0)
	{
		//motorOffset = dv; // how much to change the speed of the motors by
		motorOffset = -error/3600;

		
	}
		motorOffset = dv;
		printf("%d \n", motorOffset);   
		if (DEBUG == 1) // if error value less than 0 then turn left
	    {
            set_motor(1, MOTOR_SPEED + motorOffset);
            set_motor(2, MOTOR_SPEED - motorOffset);
            printf("Turn left\n");
            //sleep1(0,1);
        }
		else if (DEBUG == 2) // if error value greater than 0 then turn right
		{
            set_motor(1, MOTOR_SPEED - motorOffset);
            set_motor(2, MOTOR_SPEED + motorOffset);
			//sleep1(0,1);
			printf("Turn right\n");
		}
		else if (DEBUG == 3) // if error value greater than 0 then go staight
		{
			set_motor(1, -10);
			set_motor(2, -10);
			//sleep1(0,100000);
			printf("Go Straight\n");
		}
}


/** MAIN FUNCTION */
int main()
{
	init(); // initilize

	int quadrant = 1; // determine what allgorithm to run based on the quandrant of the course the robot is on

	while(true) // infinite loop
    {

		switch(quadrant)
		{
			case 1: /** Staight line. Upload to server */
			    {
					connect_to_server(serverIP,Port);
					send_to_server(message);
					receive_from_server(message);
					send_to_server(message);
					quadrant = 2;
			    }
				

			case 2: /** line following */
			{
				while(quadrant == 2)
				{
					doCamera(); // take picture and save horizontal scan
					
					
					/** calculate maximum value detected by a pixel and minimum value detected by a pixel for the middle row of pixels */
					int max = 0;
					int min =255;
					int scan_row = 120;
					for (int i = 0; i <320;i++) // do for each pixel in a row
					{
						int pix = getPixel(scan_row,i,3);
						if ( pix > max) // if the pixel value is greater than the value max then set max to the pixel value
						{
							max = pix;
						}
						if (pix < min) // if the pixel value is less than the value min then set min to the pixel value
						{
							min = pix;
						}
					}
					int threshold = (max+min)/2; // set the threshold for black and white pixels
					//printf(" min=%d max=%d threshold=%d\n", min, max,threshold);
					//int threshold = 60;


					/** Scan the middle row pixels of the image */
					int whitePixels[320];  // white pixels
					for (int i = 0; i <320;i++) // do for each pixel in a row
					{
						whitePixels[i]= 0 ;
						int pix = getPixel(scan_row,i,3); // get the white pixel value
						if ( pix > threshold) // if the pixels is greater than the mid value between the highest white and lowest black then the pixel is white
						{
							whitePixels[i] = 1;  // set the pixel at the index to white
						}
						/*if (max < 100) // if the max detected value of the pixels is less than 100 then all the pixels are black
						{
							whitePixels[i] = 0;  // set the pixel at the index to black
						}
						if (min > 100) // if the minimum detexted value of the pixel is greater than 100 then all the pixels are white
						{
							whitePixels[i] = 1;  // set the pixel at the index to white
						}*/
					}

					/** Do error calculation */
					double error = 0;
					double previousError = 0;
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

					for (int i = 0; i < 320; i++) // do for each pixel in row
					{
						error = error + (i - 160)*whitePixels[i];
						if (whitePixels[i] == 1)
						{
							np = np + 1;
						}
					}
					error = error/np;
					printf("Error: %d \n" , error); 

					double kp =2 ;
					double kd = 1;
					double derivative = (error - previousError)/(elapsed);
					unsigned char dv = error*kp + derivative*kd;
			printf("dv is %d\n", dv);
			followLine(error, dv);

			/*		if (min > threshold) // if robot is at the white tap mark then switch allgorithms
					{
						quadrant = 3;
					}
					else // Otherwise follow the line
					{
					followLine(error, dv);
					}*/
				}
			}

			case 3: /** line with shap turns */
			{
                doCamera(); // take picture and save horizontal scan


                 /** calculate maximum value detected by a pixel and minimum value detected by a pixel for the middle row of pixels */
				int max = 0;
				int min =255;
				int scan_row = 120;
				for (int i = 0; i < 320;i++)
				{
					int pix = getPixel(scan_row,i,3);
					if ( pix > max) // if the pixel value is greater than the value max then set the max to the pixel value
					{
						max = pix;
					}
					if (pix < min) // if the pixel value is less tahn the value min then set the min to the pixel value
					{
						min = pix;
					}
				}
				printf("the max is %d\n", max);
				printf("the min is %d\n", min);
				int threshold = (max+min)/2; // set the threshold for black and white pixels
				//printf(" min=%d max=%d threshold=%d\n", min, max,threshold);


                /** Check if there is a orange strip in front */
                int maxRed = 0;
                int minRed = 255;
                int maxGreen = 0;
                int minGreen = 255;
                for(int i = 0; i < 320; i++)
                {
                    int redPix = getPixel(scan_row, i, 0); // get red component of pixel colour
                    int greenPix = getPixel(scan_row, i, 1); // get green component of pixel colour
                    if (redPix > maxRed) // red component
                    {
                        maxRed = redPix;
                    }
                    if (redPix < minRed)
                    {
                        minRed = redPix;
                    }
                    if (greenPix > maxGreen) // green component
                    {
                        maxGreen = greenPix;
                    }
                    if (greenPix < minGreen)
                    {
                        minGreen = greenPix;
                    }
                }
                if (minRed > 150 && maxGreen < 80) // check if the colour of tape strip is orange
                {
                    quadrant = 4; // switch to next algorithm
                }
                else // otherwise do turning stuff
                {
                    /** Scan the middle row pixels of the image */
                    int whitePixels[320];  // white pixels
                    int scan_row = 120;
                    for (int i = 0; i <320;i++)
                    {
                        whitePixels[i]= 0 ;
                        int pix = getPixel(scan_row,i,3); //
                        if ( pix > threshold) // if the pixels is greater than the mid value between the highest white and lowest black then the pixel is white
                        {
                            whitePixels[i] = 1; // set the pixel at the index to white
                        }
                        if (max < threshold) // if the max detected value of the pixels is less than 100 then all the pixels are black
                        {
                            whitePixels[i] = 0; // set the pixel at the index to black
                        }
                        if (min > threshold)  // if the minimum detexted value of the pixel is greater than 100 then all the pixels are white
                        {
                            whitePixels[i] = 1; // set the pixel at the index to white
                        }
                    }

                    /** calculate maximum value detected by a pixel and minimum value detected by a pixel for upper image row */
                    int max2 = 0;
                    int min2 =255;
                    for (int i = 0; i <320;i++) // for all the pixels in a row
                    {
                        int pix = getPixel(50,i,3); // return the pixel at row 50, columb i, and input the colour white
                        if ( pix > max2)   // if the pixel value is greated than the maximum detected value by any pixel then the maximum value is the value of that pixel
                        {
                            max2 = pix;
                        }
                        if (pix < min2) // if the pixel value is less than the minimum detexted value by any pixel then the minimum value is the value of that pixel
                        {
                            min2 = pix;
                        }
                    }
                    int threshold2 = (max+min)/2; // set the threshold for black and white pixels

                    /** Scan the upper pixels of the image to determine whether there is a line in the higher portion of the image*/
                    int whitePixels2[320];  // white pixels
                    for (int i = 0; i <320;i++)
                    {
                        whitePixels2[i]= 0 ;
                        int pix = getPixel(50,i,3);
                        if ( pix > threshold2) // if the pixels is greater than the mid value between the highest white and lowest black then the pixel is white
                        {
                            whitePixels2[i] = 1; // set the pixel at the index to white
                        }
                        if (max2 < threshold2) // if the max detected value of the pixels is less than 100 then all the pixels are black
                        {
                            whitePixels2[i] = 0; // set the pixel at the index to black
                        }
                        if (min2 > threshold2) // if the minimum detexted value of the pixel is greater than 100 then all the pixels are white
                        {
                            whitePixels2[i] = 1; // set the pixel at the index to white
                        }
                    }


                    /** Do error calculation which determins how far the line is from the center of the camera*/
                    double error = 0;
                    double previousError = 0; // previous error when calculating the derivative for PID controller
                    int np = 0;
                    for (int i = 0; i < 320; i++) // do for each pixel in row
                    {
                        error = error + (i - 160)*whitePixels[i]; // Calculate error
                        if (whitePixels[i] == 1)
                        {
                            np = np + 1;
                        }
                    }
                    error = error/np;
                    previousError = error;

                    struct timeval t1;
                    struct timeval t2; //structure with time
                    gettimeofday(&t1, 0); // mark time

                    sleep1(0, 10000); // sleep 10 milliseconds

                    gettimeofday(&t2, 0); // mark time now
                    long elapsed = (t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec - t1.tv_usec);

                    double kp = 3;
                    double kd = 1;
                    double derivative = (error - previousError)/(elapsed);
                    unsigned char dv = error*kp;

                    ///////////////////////////////////////////////////////////////

                    if (min > 100) // if all pixels white then go forward a bit
                    {
                        set_motor(1, MOTOR_SPEED);
                        set_motor(2, MOTOR_SPEED);
                        sleep1(0, 1000); // time to wait to turn
                    }
                    if (max < 100) // if all pixels black then turn left
                    {
                        set_motor(1, -1*MOTOR_SPEED);
                        set_motor(2, MOTOR_SPEED);
                        sleep1(0, 2000); // time to wait to turn
                    }

                    if(error < 0 && max2 < 100) // if all pixels on the left are white and there are no white pixels in front then turn left
                    {
                        set_motor(0, 0);
                        set_motor(1, MOTOR_SPEED);
                        sleep1(0, 2000); // time to wait to turn
                    }
                    if(error > 0 && max2 < 100) // if all pixels on the right are white and there are no white pixels in front then turn right
                    {
                        set_motor(1, MOTOR_SPEED);
                        set_motor(2, 0);
                        sleep1(0, 2000); // time to wait to turn
                    }

                    followLine(error, dv); // function for adjusting the motor speeds based on the error and derivative calculations
                }
			}
				//break;

			case 4: /** Algorithm for navigating the maze */
			{
				/** if front sensor calue < 150
				 * 		if left sensor value < 150
				 * 			then: turn right
				 * }
				 * else {
				 * turn left
				 * }
				 * } else {
				 * go staight
				 *}
				 if the sesonr values on the side read too small then the robot needs to turn to avoid crashing into the side of the wall
				 */
				
				doCamera(); // take picture and save horizontal scan
				
				 /** Check if there is a red strip in front */
                int maxRed = 0;
                int minRed = 255;
                int maxGreen = 0;
                int minGreen = 255;
                int scan_row  = 120;
                for(int i = 0; i < 320;i++)
                {
                    int redPix = getPixel(scan_row, i, 0); // get red component of pixel colour
                    int greenPix = getPixel(scan_row, i, 1); // get green component of pixel colour
                    if (redPix > maxRed) // red component
                    {
                        maxRed = redPix;
                    }
                    if (redPix < minRed)
                    {
                        minRed = redPix;
                    }
                    if (greenPix > maxGreen) // green component
                    {
                        maxGreen = greenPix;
                    }
                    if (greenPix < minGreen)
                    {
                        minGreen = greenPix;
                    }
                }
                if (minRed > 180 && maxGreen < 50) // check if the colour of tape strip is red
                {
                    sleep1(0, 100000); // go forward a bit before stoping

                    // if gate is open then stop anyway incase gate comes down on robot
                    set_motor(1, 0);
                    set_motor(2, 0);
                    sleep1(4, 0);

                    while(read_analog(0) < 150) // check if the gate is shut
                    {
                        // Stop
                        set_motor(1, 0);
                        set_motor(2, 0);
                    }
                }

                /** navigate maze */
                if(read_analog(0) < 150) // if the measured distance to an object by front sensor value is less than 150 then determine whether to turn left or right, otherwise go staight 
                {
                    if(read_analog(1) < 150) // if the measured distance to an object by the left sensor value is less than 150
                    {                         // turn right
                        set_motor(1, 150);
                        set_motor(2, -150);
                    }
                    else         // turn left
                    {
                        set_motor(1, -150);
                        set_motor(2, 150);
                    }
                }
                else // go straight
                {
                    set_motor(1, MOTOR_SPEED);
                    set_motor(2, MOTOR_SPEED);
                }

			}
				break;
		}
    }

	return 0;
}

