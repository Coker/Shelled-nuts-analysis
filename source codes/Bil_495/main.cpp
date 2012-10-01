// compiled with: g++ circles.cpp -o circles `pkg-config --cflags --libs opencv`
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "functions.h"

int main()
{
	IplImage* originalImg = 0;
	IplImage* tempImg = 0;
	
	int height=0,
		width=0,
		step=0,
		channels=0;
	
	uchar *data;
	int red=0,
		green=0,
		blue=0;	

	// load an image  
	originalImg = cvLoadImage("siyah-beyaz.png");
	
	if(!originalImg){
		printf("Could not load image file\n");
		exit(-1);
	}
	
	tempImg = cvCloneImage(originalImg);
	// get the image data
	height    = originalImg->height; // x aks
	width     = originalImg->width;  // y aks
	step      = originalImg->widthStep;
	channels  = originalImg->nChannels;
	data      = (uchar *)originalImg->imageData;

	printf("Processing a %dx%d image with %d channels\n", height, width, channels); 

	// create a window
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("tempWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			red = ((uchar*)(originalImg->imageData + originalImg->widthStep*y))[x*3];
			green = ((uchar*)(originalImg->imageData + originalImg->widthStep*y))[x*3+1];
			blue = ((uchar*)(originalImg->imageData + originalImg->widthStep*y))[x*3+2];

			if (red<PIXEL_DEPTH && green<PIXEL_DEPTH && blue<PIXEL_DEPTH)
				detechTheObject(tempImg, x, y);
		}
	}
	
	// show the image
	cvShowImage("tempWin", tempImg );
	cvShowImage("mainWin", originalImg );

	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&originalImg);
	cvReleaseImage(&tempImg);

	return 0;
}