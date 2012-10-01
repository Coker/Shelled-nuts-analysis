
#include "functions.h"

/**
 * @brief This Recursive function should be used to ...
 *
 *
 *
 *
 */
void detechTheObject(IplImage img, unsigned int x, unsigned int y)
{
	unsigned int height =0,
		width =0,
		step =0,
		channels =0;
	
	unsigned int red =0,
		green =0,
		blue =0;	
	
	uchar *data;

	height    = img.height; // x aks
	width     = img.width;  // y aks
	step      = img.widthStep;
	channels  = img.nChannels;
	data      = (uchar *)img.imageData;

	printf("detechTheObject functions !\n");
}