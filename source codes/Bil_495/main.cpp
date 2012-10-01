// compiled with: g++ circles.cpp -o circles `pkg-config --cflags --libs opencv`
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "functions.h"

#define PIXEL_DEPTH 230

int main()
{
	IplImage* img = 0; 
	int height,width,step,channels;
	uchar *data;
	int red=0,
		green=0,
		blue=0;	

	// load an image  
	img=cvLoadImage("siyah-beyaz.png");
	if(!img){
		printf("Could not load image file\n");
		exit(0);
	}

	// get the image data
	height    = img->height; // x aks
	width     = img->width;  // y aks
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;
	printf("Processing a %dx%d image with %d channels\n", height, width, channels); 

	// create a window
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	cvMoveWindow("mainWin", 100, 100);
	
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			red = ((uchar*)(img->imageData + img->widthStep*y))[x*3];
			green = ((uchar*)(img->imageData + img->widthStep*y))[x*3+1];
			blue = ((uchar*)(img->imageData + img->widthStep*y))[x*3+2];

			if (red<230 && green<230 && blue<230) {
				detechTheObject(img, x, y);
			}

		}
	}
	
	// show the image
	cvShowImage("mainWin", img );

	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&img);
	return 0;
}