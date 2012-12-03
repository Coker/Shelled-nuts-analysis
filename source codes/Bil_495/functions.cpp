
#include "functions.h"
#include <stdio.h>
#include <stack>

std::stack<Object> objects;
unsigned int classA =0,
			 classB =0;

unsigned int massRatio = 0;

void startApplication(void)
{
	IplImage* originalImg = 0;
	IplImage* tempImg = 0;

	// load an image
	originalImg = cvLoadImage("references/ceviz.png");
	
	if(!originalImg){
		printf("Could not load image file\n");
		exit(-1);
	}
	
	cvNamedWindow("main window", CV_WINDOW_NORMAL);

	tempImg = findAllObject(originalImg, GET_REFERENCE_FOR_MASS);
	tempImg = findAllObject(originalImg, CLASSIFY);
	// GET_REFERENCE_FOR_MASS CLASSIFY
	// show the image
	cvShowImage("main window", originalImg);
	
#if defined(DEBUG_MODE)
	cvNamedWindow("debug window", CV_WINDOW_NORMAL);
	cvShowImage("debug window", tempImg);
#endif

	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&originalImg);
	cvReleaseImage(&tempImg);

	originalImg = cvLoadImage("class/3/last.png");
	
	if(!originalImg){
		printf("Could not load image file\n");
		exit(-1);
	}
	
	tempImg = findAllObject(originalImg, CLASSIFY);
	
	cvShowImage("main window", originalImg);
	cvShowImage("debug window", tempImg);

	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&originalImg);
	cvReleaseImage(&tempImg);

	originalImg = cvLoadImage("class/5/biber.png");
	
	if(!originalImg){
		printf("Could not load image file\n");
		exit(-1);
	}
	
	tempImg = findAllObject(originalImg, CLASSIFY);
	
	cvShowImage("main window", originalImg);
	cvShowImage("debug window", tempImg);
	
	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&originalImg);
	cvReleaseImage(&tempImg);
	
	return;
} // end of startApplication function

IplImage* findAllObject(IplImage *img, int mode)
{
	IplImage* tempImg = 0;
	Object object;
	
	int height=0,
		width=0,
		step=0,
		channels=0;
	
	uchar *data;
	int red=0,
		green=0,
		blue=0;

	tempImg = cvCloneImage(img);
	// get the image data
	height    = tempImg->height; // x aks
	width     = tempImg->width;  // y aks
	step      = tempImg->widthStep;
	channels  = tempImg->nChannels;
	data      = (uchar *)tempImg->imageData;
	
	for (int y=0; y<height; ++y) {
		for (int x=0; x<width; ++x) {
			red = ((uchar*)(tempImg->imageData + tempImg->widthStep*y))[x*3];
			green = ((uchar*)(tempImg->imageData + tempImg->widthStep*y))[x*3+1];
			blue = ((uchar*)(tempImg->imageData + tempImg->widthStep*y))[x*3+2];

			if (red<PIXEL_DEPTH && green<PIXEL_DEPTH && blue<PIXEL_DEPTH)
				markTheObject(tempImg, x, y);
		}
	}

	drawIndexNumberOnObject(tempImg, img);
#if defined(DEBUG_MODE)
	printf("*** color density ***\n");
#endif
	switch (mode){
	case GET_REFERENCE_FOR_MASS: {
		int sumOfTheMassRatio =0;
		while (false == objects.empty()) {
			object = objects.top();
			objects.pop();
			
			sumOfTheMassRatio += (255-object.density) * object.size;

#if defined(DEBUG_MODE)
			printf("%d\n", (255-object.density) * object.size);
#endif
		
		} // end of while
#if defined(DEBUG_MODE)
			printf("********\n");
			printf("%d is sum of the mass\n", sumOfTheMassRatio);
#endif
		massRatio = 
			calculateMassRatioOfTheIntensityOfColor(sumOfTheMassRatio);
		FILE* massRatioFPtr = fopen("massRatio.txt", "w");
		
		if (NULL == massRatioFPtr) {
			fprintf(stderr, "Mass Ratio file not createded !\n");
			std::exit(-1);
		}
		
		fprintf(massRatioFPtr, "%d", massRatio);
		fclose(massRatioFPtr);
	}
		break;
	case CLASSIFY: {

		while (false == objects.empty()) {
			object = objects.top();
			objects.pop();
			classifyTheObject(img, object.point, object.density, object.size);
		} // end of while
	}
		break;
	default:
		break;
	} 
	
	return tempImg;
}// end of findAllObject function

void classifyTheObject(IplImage *originalImg, CvPoint centerOfTheObject,
					   int densityOfTheObject, int numberOfPixel)
{
	static CvFont font;
	static double hScale=0.2;
	static double vScale=0.5;
	static double lineWidth=0.06;
	char mass[10];
	double estimatedMass = 0;
	static int massRatio =0;
	int massMultiplier = (255-densityOfTheObject)*numberOfPixel;
	
	if (0 == massRatio) {
		FILE* massRatioFPtr = fopen("massRatio.txt", "r");	
	
		if (NULL == massRatioFPtr) {
			fprintf(stderr, "mass Ratio file Not Opened !\n");
			std::exit(-1);
		}
			
		fscanf(massRatioFPtr, "%d", &massRatio);
		fclose(massRatioFPtr);
	}
	

#if defined(DEBUG_MODE)
	printf("mass Ratio is %d\n", massRatio);
#endif

	centerOfTheObject.y += 5;
	centerOfTheObject.x -= 3;

	cvInitFont(&font, CV_FONT_HERSHEY_TRIPLEX, hScale, vScale, 0, lineWidth);
	estimatedMass = (double)massMultiplier/massRatio;

	sprintf(mass, "%.2lf", estimatedMass);
	cvPutText(originalImg, mass, centerOfTheObject, &font, cvScalar(125,125,0));

	/*
	if (densityOfTheObject<188) {
		cvPutText(originalImg, "A", centerOfTheObject, &font, cvScalar(125,125,0));
		return;
	} else if (densityOfTheObject<198) {
		cvPutText(originalImg, "B", centerOfTheObject, &font, cvScalar(125,125,0));
		return;	
	} else {
		cvPutText(originalImg, "C", centerOfTheObject, &font, cvScalar(125,125,0));
		return;
	}*/
} // end of classifyTheObject function

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
}// end of markTheObject function

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
	// This variable is used to detecting the objects' border
	// x1 means -x axes, x2 means +x axes. Y is the same of the x
	static unsigned int x1=x, x2=x, y1=y, y2=y;

	static unsigned int numberOfPixel = 0;
	static unsigned long sumOfPixel = 0;
	
	if (true == isNewObject) {
		x1 = x2 = x;
		y1 = y2 = y;
		numberOfPixel =0;
		sumOfPixel =0;
	}

	static unsigned int width = markedImg->width,
						height = markedImg->height;
	
	static unsigned int step = markedImg->widthStep;

	static uchar *imgData = (uchar *)markedImg->imageData;

	unsigned int red = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3],
				 green = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+1],
				 blue = ((uchar*)(markedImg->imageData + markedImg->widthStep*y))[x*3+2];
	
	sumOfPixel += ((uchar*)(referenceImg->imageData + referenceImg->widthStep*y))[x*3];
	++numberOfPixel;

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
		if (numberOfPixel<MIN_OBJECT_SIZE)
			return;
#ifdef DEBUG_MODE_2
		cvLine(referenceImg, cvPoint(x1-1, y1-1), cvPoint(x2+1, y1-1), cvScalar(0,0,0), 1);		
		cvLine(referenceImg, cvPoint(x1-1, y2+1), cvPoint(x2+1, y2+1), cvScalar(0,0,0), 1);
		cvLine(referenceImg, cvPoint(x1-1, y1-1), cvPoint(x1-1, y2+1), cvScalar(0,0,0), 1);
		cvLine(referenceImg, cvPoint(x2+1, y1), cvPoint(x2+1, y2), cvScalar(0,0,0), 1);

		cvNamedWindow("detectorWin", CV_WINDOW_AUTOSIZE);
		cvShowImage("detectorWin", markedImg);
		cvWaitKey(0);
#endif
		Object object;
		object.density = sumOfPixel/numberOfPixel;
		object.size = numberOfPixel;
		object.point = cvPoint((x1+x2)/2, (y1+y2)/2);
		objects.push( object );
	}

	return;
}

int calculateMassRatioOfTheIntensityOfColor(int sumofTheMassRatio)
{
	int massRatio =0;
	double temp =0.0;

	FILE* massOfTheWalnut = fopen("references/mass.txt", "r");
	
	if (massOfTheWalnut == NULL) {
		printf("File reading error\n");
		return -1;
	}

	while ( !feof(massOfTheWalnut) ) {
		fscanf(massOfTheWalnut, "%lf", &temp);
		massRatio += temp;
#if defined(DEBUG_MODE)
		printf("%lf\n", temp);
#endif
	} // end of while
	
	massRatio = sumofTheMassRatio/massRatio;

	return massRatio;
} // end of calculateMassRatioOfTheIntensityOfColor function