
#include "functions.h"

/**
 * @brief This Recursive function should be used to ...
 *
 *
 *
 *
 */
void markTheObject(IplImage *img, unsigned int x, unsigned int y)
{
	unsigned int red = 0,
						green = 0,
						blue = 0;

	unsigned int width = 0,
						height = 0;
	
	unsigned int step=0;

	static uchar *imgData;

	width = img->width;
	height = img->height;
	imgData = (uchar *)img->imageData;
	step = img->widthStep;


	red = ((uchar*)(img->imageData + img->widthStep*y))[x*3];
	green = ((uchar*)(img->imageData + img->widthStep*y))[x*3+1];
	blue = ((uchar*)(img->imageData + img->widthStep*y))[x*3+2];
	
	if ( red<200 || green<200 || blue<200)
		cvLine(img, cvPoint(x, y), cvPoint(x, y), cvScalar(0,255,0), 1);
	else
		cvLine(img, cvPoint(x, y), cvPoint(x, y), cvScalar(0,255,0), 1);
}

void drawIndexNumberOnObject(IplImage *markedImg, IplImage *referenceImg)
{
	unsigned int red = 0,
				 green = 0,
				 blue = 0;

	unsigned int width = 0,
				 height = 0;
	
	unsigned int step=0;

	uchar *imgData;

	width = markedImg->width;
	height = markedImg->height;
	imgData = (uchar *)markedImg->imageData;
	step = markedImg->widthStep;

	for (int x=0; x<width; ++x) {
		for (int y=0; y<height; ++y) {
			red = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3];
			green = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+1];
			blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+2];

			if (red == 0 && green == 255 && blue == 0)
				detectAndSetIndex(markedImg, referenceImg, x, y, true);
		}
	}
	
}

void detectAndSetIndex(IplImage *markedImg, IplImage *referenceImg,
					   unsigned int x, unsigned int y, bool isNewObject)
{
	static unsigned int x1=x, x2=x, y1=y, y2=y;
	
	if (true == isNewObject) {
		x1 = x2 = x;
		y1 = y2 = y;
	}

	static unsigned int width = markedImg->width,
						height = markedImg->height;
	
	static unsigned int step = markedImg->widthStep;

	static uchar *imgData = (uchar *)markedImg->imageData;

	unsigned int red = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3],
				 green = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+1],
				 blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+2];

	cvLine(markedImg, cvPoint(x, y), cvPoint(x, y), cvScalar(255,0,0), 1);

	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x+1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x+1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x+1)*3+2];

	if (red == 0 && green == 255 && blue == 0) {
		if (x2 < (x+1))
			x2 = x+1;

		detectAndSetIndex(markedImg, referenceImg, x+1, y);		
	}
	
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x+1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x+1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x+1)*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (x2 < (x+1))
			x2 = x+1;
		
		if (y2 < (y+1))
			y2 = y+1;
		
		detectAndSetIndex(markedImg, referenceImg, (x+1), (y+1));
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[x*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[x*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[x*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (y2 < (y+1))
			y2 = y+1;

		detectAndSetIndex(markedImg, referenceImg, x, (y+1));
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x-1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x-1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y+1)))[(x-1)*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (x1 > (x-1))
			x1 = x-1;

		if (y2 < (y+1))
			y2 = y+1;

		detectAndSetIndex(markedImg, referenceImg, (x-1), (y+1));
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x-1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x-1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[(x-1)*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (x1 > (x-1))
			x1 = x-1;

		detectAndSetIndex(markedImg, referenceImg, (x-1), y);		
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x-1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x-1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x-1)*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (x1 > (x-1))
			x1 = x-1;

		if (y1 > (y-1))
			y1 = y-1;

		detectAndSetIndex(markedImg, referenceImg, (x-1), (y-1));		
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[x*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[x*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[x*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (y1 > (y-1))
			y1 = y-1;

		detectAndSetIndex(markedImg, referenceImg, x, (y-1));
	}
		
	red = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x+1)*3];
	green = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x+1)*3+1];
	blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*(y-1)))[(x+1)*3+2];
	
	if (red == 0 && green == 255 && blue == 0) {
		if (x2 < (x+1))
			x2 = x+1;

		if (y1 > (y-1))
			y1 = y-1;

		detectAndSetIndex(markedImg, referenceImg, (x+1), (y-1));
	}
	
	if (true == isNewObject) {
		cvLine(markedImg, cvPoint(x1-1, y1-1), cvPoint(x2+1, y1-1), cvScalar(0,0,0), 1);		
		cvLine(markedImg, cvPoint(x1-1, y2+1), cvPoint(x2+1, y2+1), cvScalar(0,0,0), 1);
		cvLine(markedImg, cvPoint(x1-1, y1-1), cvPoint(x1-1, y2+1), cvScalar(0,0,0), 1);
		cvLine(markedImg, cvPoint(x2+1, y1), cvPoint(x2+1, y2), cvScalar(0,0,0), 1);

		cvNamedWindow("detectorWin", CV_WINDOW_AUTOSIZE);
		cvShowImage("detectorWin", markedImg);
		cvWaitKey(0);
	}

	return;
}