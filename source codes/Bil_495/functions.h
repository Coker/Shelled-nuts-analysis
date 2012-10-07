
#if !defined(__FUNCTIONS_H)
#define __FUNCTIONS_H

#include <cv.h>
#include <highgui.h>

#define PIXEL_DEPTH 225

void markTheObject(IplImage *img, unsigned int x, unsigned int y);
void drawIndexNumberOnObject(IplImage *markedImg, IplImage *referenceImg);
void detectAndSetIndex(IplImage *markedImg, IplImage *referenceImg,
					   unsigned int x, unsigned int y, bool isNewObject=false);
void classifyTheObject(IplImage *originalImg, CvPoint centerOfTheObject, int densityOfTheObject, int numberOfPixel);

#endif