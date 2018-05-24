#include "E101.h"

//Make Q3 true when more than 90px
if (np > 200) {
	Q3 == true;
	Q2 == false;
	}

//Q3 Variables
int pastAnsHor = 0;
int pastAnsVert = 0;
int whiteTotalHor = 0;
int whiteTotalVert = 0;

//When Q3 is true
while (Q3 = true) {
int whitePixelsVert[240];  // white pixels
        for (int j = 0; j < 240; j++)
        {
            whitePixelsVert[j]= 0 ;
            int pix = get_pixel(j,160,3);
            if (max<100)
            {
				whitePixelsVert[j] = 0;
			}
            
            else if ( pix > threshold)
            {
                whitePixels[j] = 1;
            }

for (int i = 0; i < whitePixels[320]; i++){
	whiteTotalHor = whitePixels[i] + pastAnsHor;
	pastAnsHor = whitePixels[i];
	i++;
}
for (int j = 0; j < WhitePixelsVert[240]; j++){
	whiteTotalVert = whitePixels[j] + pastAnsVert;
	pastAnsVert = whitePixels[j];
	j++;
}

if (whiteTotalHor > 280 && whiteTotalVert < 120;){
	//Set motor to Turn Left
	}
if (whiteTotalVert > 160)
	//Set motor to go Straight
}
}
