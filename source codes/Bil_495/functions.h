
#if !defined(__FUNCTIONS_H)
#define __FUNCTIONS_H

	#include <cv.h>
	#include <highgui.h>
	#include <vector>

	#define PIXEL_DEPTH 230
	#define MIN_OBJECT_SIZE 120

	// Debug mode constant
	#define DEBUG_MODE
	
	enum MODE {
		CLASSIFY = 0,
		GET_REFERENCE_FOR_MASS,
		CLASS_A,
		CLASS_B
	};

	typedef struct {
		CvPoint point;
		int density;
		int size;
	} Object;
 
	void markTheObject(IplImage *img, unsigned int x, unsigned int y);
	void drawIndexNumberOnObject(IplImage *markedImg, IplImage *referenceImg);
	void detectAndSetIndex(IplImage *markedImg, IplImage *referenceImg,
						   unsigned int x, unsigned int y, bool isNewObject=false);
	void classifyTheObject(IplImage *originalImg, CvPoint centerOfTheObject,
						   int densityOfTheObject, int numberOfPixel);
	IplImage* findAllObject(IplImage *img, int mode);
	void startApplication(void);
	void getReferences(void);
	int calculateMassRatioOfTheIntensityOfColor(int sumofTheMassRatio);

#endif