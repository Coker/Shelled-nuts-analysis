
#include "functions.h"

/**
 * @brief This Recursive function should be used to ...
 *
 *
 *
 *
 */
void detechTheObject(IplImage *img, unsigned int x, unsigned int y)
{
	static unsigned int red = 0,
						green = 0,
						blue = 0;

	static unsigned int width = 0,
						height = 0;
	
	static uchar *imgData;

	width = img->width;
	height = img->height;
	imgData = (uchar *)img->imageData;
	
	cvLine(img, cvPoint(x, y), cvPoint(x, y), cvScalar(0,255,0), 1);
}