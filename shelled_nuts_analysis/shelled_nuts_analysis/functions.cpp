
/**
 * functions.cpp file
 * @brief This file include all implementation details of whole project.
 * 
 * @author Mustafa ÇOKER
 * @version 2.1
 * @date 22.01.2013
 */

#include <stdio.h>
#include <stack>

#include "ml.h"
#include "cv.h"
#include "highgui.h"

#include "functions.h"

int createShelledNutsTree(const char const *highLevelPath, 
					      const char const *lowLevelPath, CvDTree** tree)
{
	IplImage *highLevel = cvLoadImage(highLevelPath),
			 *lowLevel = cvLoadImage(lowLevelPath);
	
	IplImage *tempImage = cvCloneImage(highLevel);

	if (NULL == highLevel || NULL == lowLevel || NULL == tempImage) {
		fprintf(stderr, "ERROR ! Image File not found\n");
		tree = NULL;
		return FILE_NOT_FOUND_ERROR;
	}
	
	*tree = new CvDTree;
	CvMat *data =NULL,
		  *resp =NULL;
	CvMat *variableType =NULL;

	float *dataIter =NULL, // Bu pointer iterator olarak kullanýlacak.
		  *respIter =NULL; // Bu pointer iterator olarak kullanýlacak.
	
	if (NULL == *tree) {
		fprintf(stderr, "%s",
			    "ERROR ! Tree allocation Error, no enough stack space !");
		return NOT_ENOUGH_STACK_SPACE;
	}

	uchar color1 =0, // high level X-Ray cýktýsýndan okunan renkleri temsil ediyor
		  color2 =0; // low level X-Ray cýktýsýndan okunan renkleri temsil ediyor
	int sampleNumber =0,
		sampleIndex =0;
	float pixelRatio =0.0; // (MAX_PIXEL_VAL - high Level Pixel Value)/ \
							  (MAX_PIXEL_VAL - low level Pixel Value)

	for (int i=IGNORED; i<highLevel->width-IGNORED; ++i)
		for (int j=0; j<highLevel->height; ++j) {
			color1 = MAX_PIXEL_VAL - 
					((uchar *)(highLevel->imageData + highLevel->widthStep*j))[i*3];
			color2 = MAX_PIXEL_VAL -
					((uchar *)(lowLevel->imageData + lowLevel->widthStep*j))[i*3];
			if (color1 != 0 && color2 != 0)
				++sampleNumber;
		}

	fprintf(stderr, "si ->%d\n", sampleNumber);
	
	data = cvCreateMat(sampleNumber+1, 3, CV_32F );
	resp = cvCreateMat(sampleNumber+1, 1, CV_32F );
	variableType = cvCreateMat(data->cols+1, 1, CV_8U);
	cvSet(variableType, cvScalarAll(CV_VAR_CATEGORICAL));
	
	for (int i=IGNORED; i<highLevel->width-IGNORED; ++i)
		for (int j=0; j<highLevel->height; ++j) {			
			color1 = MAX_PIXEL_VAL - 
					((uchar *)(highLevel->imageData + highLevel->widthStep*j))[i*3];
			color2 = MAX_PIXEL_VAL -
					((uchar *)(lowLevel->imageData + lowLevel->widthStep*j))[i*3];

			if (color1 == 0 || color2 == 0)
				continue;

			pixelRatio = ((float)color1) / ((float)color2);
			pixelRatio *= MULTIPLIER;
			
			dataIter = data->data.fl + (sampleIndex*3);
			respIter = resp->data.fl + (sampleIndex);
			++sampleIndex;

			*(dataIter) = (int) color1;
			*(++dataIter) = (int) color2;
			*(++dataIter) = (int) pixelRatio;
			

			if (color1>0 && color2>0) {
				if (pixelRatio > 0.65*MULTIPLIER && pixelRatio < 1.45*MULTIPLIER) {
					*(respIter) = SHELLED_NUTS;
					cvLine(tempImage, cvPoint(i,j), cvPoint(i,j), cvScalar(0,255,0) );
				}
									
				else
					*(respIter) = NOT_SHELLED_NUTS;
						
			} else
				*(respIter) = NOT_SHELLED_NUTS;
			
		} // end of inner for
	
	dataIter = data->data.fl + (sampleIndex*3);
	respIter = resp->data.fl + sampleIndex;

	*dataIter = 0;
	*(++dataIter) =0;
	*(++dataIter) =0;
	*(respIter) = NOT_SHELLED_NUTS;
	
	FILE* logFile = fopen("logFile.txt", "w");
	fprintf(logFile, "       color1       color2       pixelRatio      Response\n");
	float tempVal =0.0;
	
	for (int i=0; i<data->rows; ++i) {
		dataIter = data->data.fl + (i*3);
		respIter = resp->data.fl + i;

		tempVal = *(dataIter);
		fprintf(logFile, "%12.2f ", tempVal);
		
		tempVal = *(++dataIter);
		fprintf(logFile, "%12.2f ", tempVal);

		tempVal = *(++dataIter);
		fprintf(logFile, "%14.2f ", tempVal);
		
		tempVal = *(respIter);
		fprintf(logFile, "%12.2f\n", tempVal);
	}

	(*tree)->train(data, CV_ROW_SAMPLE, resp, 0, 0, variableType);
	cvSaveImage("highLevelResult.bmp", tempImage);
	cvReleaseImage(&highLevel);
	cvReleaseImage(&lowLevel);
	fclose(logFile);

	return NO_PROBLEM;
}/*********** end of createShelledNutsTree function **************************/

int filter(IplImage *image)
{
	if (NULL == image) {
		fprintf(stderr, "image not set !\n");
		return VARIABLE_NOT_SET;
	}
	
	IplImage *resImage = NULL;
	resImage = cvCloneImage(image);

	uchar red =0,
		  green =0,
		  blue =0;

	for (int y=0; y<image->height; ++y) {
		for (int x=0; x<image->width; ++x) {
			blue = ((uchar *)(image->imageData + image->widthStep*y))[x*3];
			green = ((uchar *)(image->imageData + image->widthStep*y))[x*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*y))[x*3+2];

			if (255 == green && 0 == red && 0 == blue) {
				cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));

				if (true == controlNeigbor(image, cvPoint(x,y), W)){
					if (false == controlNeigbor(image, cvPoint(x,y), SW) ||
						false == controlNeigbor(image, cvPoint(x,y), NW))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));	
						continue;
					}
				} 
				if (true == controlNeigbor(image, cvPoint(x,y), NE)) {
					if (false == controlNeigbor(image, cvPoint(x,y), N) ||
						false == controlNeigbor(image, cvPoint(x,y), E))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), E)) {
					if (false == controlNeigbor(image, cvPoint(x,y), NE) ||
						false == controlNeigbor(image, cvPoint(x,y), SE))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), SE)) {
					if (false == controlNeigbor(image, cvPoint(x,y), E) ||
						false == controlNeigbor(image, cvPoint(x,y), S))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), S)) {
					if (false == controlNeigbor(image, cvPoint(x,y), SE) ||
						false == controlNeigbor(image, cvPoint(x,y), SW))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), SW)) {
					if (false == controlNeigbor(image, cvPoint(x,y), W) ||
						false == controlNeigbor(image, cvPoint(x,y), S))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), W)) {
					if (false == controlNeigbor(image, cvPoint(x,y), NW) ||
						false == controlNeigbor(image, cvPoint(x,y), SW))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				} 
				
				if (true == controlNeigbor(image, cvPoint(x,y), NW)) {
					if (false == controlNeigbor(image, cvPoint(x,y), W) ||
						false == controlNeigbor(image, cvPoint(x,y), N))
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,0,255));
					else {
						cvLine(resImage, cvPoint(x,y), cvPoint(x,y), cvScalar(0,255,0));
						continue;
					}
				}
			
			} // end of first if					
		}
	}
	
	cvSaveImage("res.bmp", resImage);

	return NO_PROBLEM;
}/************************ end of filter function ****************************/

bool controlNeigbor(IplImage *image, CvPoint point, int direction)
{
	if (NULL == image) {
		fprintf(stderr, "Image not Set !\n");
		return false;
	}
	
	int count =0;
	uchar red =0,
		  green =0,
		  blue =0;

	switch(direction){
	case N: {
		for (int i=0; i<DISTANCE; ++i) {
			if (point.y-i <=0)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case NE:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.y-i<=0 || point.x+i >= image->width)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x+i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x+i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x+i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case E:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.x+i >= image->width)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x+i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x+i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x+i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case SE:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.y+i>=image->height || point.x+i >= image->width)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x+i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x+i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x+i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case S:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.y+i>=image->height)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case SW:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.x-i <= 0 || point.y+i >= image->height)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x-i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x-i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y+i)))[(point.x-i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case W:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.x-i <= 0)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x-i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x-i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y)))[(point.x-i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
	}
	break;
	case NW:{
		for (int i=0; i<DISTANCE; ++i) {
			if (point.y-i <= 0 || point.x-i <= 0)
				continue;

			blue = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x-i)*3];
			green = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x-i)*3+1];
			red = ((uchar *)(image->imageData + image->widthStep*(point.y-i)))[(point.x-i)*3+2];

			if (255 == green && 0 == blue && 0 == red)
				++count;
		}

		if (count < DISTANCE-1)
			return false;
		else
			return true;
		
	}
	break;
	}
}/************* end of the controlNeigbor function ***************************/

int detectShelledNutsPixel(const char const *highLevelPath,
						   const char const *lowLevelPath, 
						   const CvDTree const *shelledNutsTree)
{
	IplImage *highLevel = cvLoadImage(highLevelPath),
		     *lowLevel = cvLoadImage(lowLevelPath);
	
	uchar color1 =0,
		  color2 =0;

	float pixelRatio =0.0;
	CvMat *currentPositionData = cvCreateMat(1, 3, CV_32F);
	CvDTreeNode *leaf;
	float *ddata;

	if (NULL == highLevel && NULL == lowLevel) {
		fprintf(stderr, "ERROR ! image file not found, please check the path !\n");
		return FILE_NOT_FOUND_ERROR; 
	}

	for (int i=IGNORED; i<highLevel->width-IGNORED; ++i)
		for (int j=0; j<highLevel->height; ++j) {
			color1 = MAX_PIXEL_VAL - 
					((uchar *)(highLevel->imageData + highLevel->widthStep*j))[i*3];
			color2 = MAX_PIXEL_VAL -
					((uchar *)(lowLevel->imageData + lowLevel->widthStep*j))[i*3];

			if (color1 == 0 || color2 == 0)
				continue;

			pixelRatio = ((float)color1) / ((float)color2);
			pixelRatio *= MULTIPLIER;
			
			ddata = currentPositionData->data.fl;
			*ddata = (int) color1;
			++ddata;
			*ddata = (int) color2;
			++ddata;
			*ddata = (int) pixelRatio;

			leaf = shelledNutsTree->predict(currentPositionData);
			
			// class_idx = 0 is ShelledNuts
			if (0 == leaf->class_idx)
				cvLine(lowLevel, cvPoint(i, j), cvPoint(i, j),
				cvScalar(0, 255, 0));
		}
	
	cvSaveImage("before_filter.jpg", lowLevel);
	filter(lowLevel);
	
	cvReleaseImage(&highLevel);
	cvReleaseImage(&lowLevel);

	return NO_PROBLEM;
}

int detectShelledNuts(const char const *highLevelPath,
					  const char const *lowLevelPath,
					  const char const *resPath)
{	
	IplImage *highLevel = cvLoadImage(highLevelPath),
		     *lowLevel = cvLoadImage(lowLevelPath),
			 *detectedPixel = cvLoadImage(resPath);
	
	FILE *walnutSet = fopen("walnut_Log.txt", "w");

	if ( NULL == highLevel && NULL == lowLevel &&
		 NULL == detectedPixel && NULL == walnutSet ) {
		fprintf(stderr, "ERROR ! file not found please check the file path\n");
		return FILE_NOT_FOUND_ERROR;
	}
	
	fprintf(walnutSet, "%10s%10s%10s\n", "size", "ratio", "density");

	int averageDensity =0;
	std::stack<CvPoint> nutsPixel;
	uchar red =0,
		  green =0,
		  blue =0;

	for (int j=0; j<detectedPixel->height; ++j)
		for (int i=IGNORED; i<detectedPixel->width-IGNORED; ++i) {
			blue = ((uchar *)(detectedPixel->imageData +
					detectedPixel->widthStep*j))[i*3];
			green = ((uchar *)(detectedPixel->imageData +
				detectedPixel->widthStep*j))[i*3+1];
			red = ((uchar *)(detectedPixel->imageData +
				detectedPixel->widthStep*j))[i*3+2];

			if (blue == 0 && green == 255 && red == 0) {
				nutsPixel.push(cvPoint(i,j));
				
				int averagePixelDensityOfNuts =0,
					pixelSizeOfNuts =0,
					averageRatio =0;

				uchar color1 =0,
					  color2 =0;
				float pixelRatio =0.0;
				// this loop traverse all pixels of the nuts object by \
				using stack structure
				while (true) {
					if (true == nutsPixel.empty())
						break;
					
					CvPoint pixelPos = nutsPixel.top(); // get the element
					nutsPixel.pop(); // remove element
					
					int tempX = pixelPos.x,
						tempY = pixelPos.y;

					int tempBlue =0,
						tempGreen =0,
						tempRed =0;

					cvLine(detectedPixel, pixelPos, pixelPos, cvScalar(255,0,0));
					++pixelSizeOfNuts;
					color1 = ((uchar *)(highLevel->imageData +
						highLevel->widthStep*pixelPos.y))[pixelPos.x*3];
					color2 = ((uchar *)(lowLevel->imageData +
						lowLevel->widthStep*pixelPos.y))[pixelPos.x*3];
					
					averagePixelDensityOfNuts += color2;
					pixelRatio = ( (float)color1 / (float)color2);
					pixelRatio *= MULTIPLIER;
					averageRatio += (int) pixelRatio;
					
					tempBlue = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX+1)*3];
					tempGreen = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX+1)*3+1];
					tempRed = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX+1)*3+2];

					if (0 == tempBlue && 255 == tempGreen && 0 == tempRed)
						nutsPixel.push(cvPoint((tempX+1), tempY));

					tempBlue = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY+1)))[tempX*3];
					tempGreen = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY+1)))[tempX*3+1];
					tempRed = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY+1)))[tempX*3+2];
						
					if (0 == tempBlue && 255 == tempGreen && 0 == tempRed)
						nutsPixel.push(cvPoint(tempX, (tempY+1)));
					
					tempBlue = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX-1)*3];
					tempGreen = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX-1)*3+1];
					tempRed = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*tempY))[(tempX-1)*3+2];
					
					if (0 == tempBlue && 255 == tempGreen && 0 == tempRed)
						nutsPixel.push(cvPoint((tempX-1), tempY));
					
					tempBlue = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY-1)))[tempX*3];
					tempGreen = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY-1)))[tempX*3+1];
					tempRed = ((uchar *)(detectedPixel->imageData +
						detectedPixel->widthStep*(tempY-1)))[tempX*3+2];
						
					if (0 == tempBlue && 255 == tempGreen && 0 == tempRed)
						nutsPixel.push(cvPoint(tempX, (tempY-1)));

				} // end of while loop
				
				averagePixelDensityOfNuts /= pixelSizeOfNuts;
				averageRatio /= pixelSizeOfNuts;
				
				// :( 700 is a magic number, i am sorry
				if (pixelSizeOfNuts>500) {
					fprintf(stdout, "size %d, ratio %d, density %d\n",
						pixelSizeOfNuts, averageRatio, averagePixelDensityOfNuts);
					
					fprintf(walnutSet, "%10d%10d%10d\n", pixelSizeOfNuts,
						averageRatio, averagePixelDensityOfNuts);

					// cvShowImage("disp", detectedPixel);
					// cvWaitKey(0);
				}
			} // end of if				
		} // end of for loop

	// and everting is nice, releases all resources, say c u all resources ! :)
	fclose(walnutSet);
	cvReleaseImage(&highLevel);
	cvReleaseImage(&lowLevel);
	cvReleaseImage(&detectedPixel);
}

int applyFiveFoldsCrossValidation(const char const *walnutsDataFilePath)
{
	FILE *walnutsDataFile = fopen(walnutsDataFilePath, "r");
	FILE *classificationResult = fopen("classRes.txt", "w");
	
	if (NULL == classificationResult) {
		fprintf(stderr, "ERROR ! File not created\n");
		return FILE_NOT_CREATED;
	}

	if (NULL == walnutsDataFile) {
		fprintf(stderr, "ERROR ! File not found\n");
		return FILE_NOT_FOUND_ERROR;
	}
	WalnutsData walnut;
	WalnutsData walnuts[60]; // We have 60 walnuts
	WalnutsData trainSet[55];
	WalnutsData testSet[5];
	float *dataIter = NULL;
	float *respIter = NULL;

	walnut.size =0,
	walnut.ratio =0,
	walnut.density =0,
	walnut.classOfNuts =0;
	
	int testPos = 0;

	// k folds cross validation. our k is 5.
	const int testCaseVariable = 5;
		
	// we get all walnuts data from file.
	for (int i=0; i<60; ++i) {
		fscanf(walnutsDataFile, "%d%d%d%d", &(walnut.size), &(walnut.ratio),
				&(walnut.density), &(walnut.classOfNuts));
		fprintf(stdout, "%d->%d %d %d %d\n",i, walnut.size, walnut.ratio,
				walnut.density, walnut.classOfNuts);
		
		walnuts[i].size = walnut.size;
		walnuts[i].ratio = walnut.ratio;
		walnuts[i].density = walnut.density;
		walnuts[i].classOfNuts = walnut.classOfNuts;
	} // end of for loop
	
	for (int i=0; i<(60/testCaseVariable); ++i) {
		int testWalnutNumber=0;
		int trainWalnutNumber=0;
		for (int j=0; j<60; ++j) {
			if (j>= i*testCaseVariable && j<i*testCaseVariable+testCaseVariable)
				testSet[testWalnutNumber++] = walnuts[j];
			else
				trainSet[trainWalnutNumber++] = walnuts[j];
		}

		for (int j=0; j<5; ++j)
			printf("test set %d: %d %d %d %d\n", j, testSet[j].size, testSet[j].ratio,
				testSet[j].density, testSet[j].classOfNuts);
		printf("**************\n");

		CvDTree *tree = new CvDTree;
		CvDTreeNode *leaf = NULL;
		CvMat *trainMatris = cvCreateMat(55, 2, CV_32F);
		CvMat *respMatris = cvCreateMat(55, 1, CV_32F);
		CvMat *testMatris = cvCreateMat(1, 2, CV_32F);
		CvMat *variableType = cvCreateMat(trainMatris->cols+1, 1, CV_8U);
		cvSet(variableType, cvScalarAll(CV_VAR_CATEGORICAL));
		float *priors = NULL;

		CvDTreeParams params =
			CvDTreeParams(25, // max depth
                           5, // min sample count
                           0, // regression accuracy: N/A here
                           false, // compute surrogate split, no missing data
                           15, // max number of categories (use sub-optimal \
								  algorithm for larger numbers)
                           5, // the number of cross-validation folds
                           false, // use 1SE rule => smaller tree
                           false, // throw away the pruned tree branches
                           priors // the array of priors
                           );
		
		if (NULL == trainMatris && NULL == tree && NULL == respMatris) {
			fprintf(stderr, "ERROR ! Not enough stack space !\n");
			return NOT_ENOUGH_STACK_SPACE;
		}

		for (int j=0; j<55; ++j) {
			dataIter = ((trainMatris->data.fl) + (j*2));
			respIter = ((respMatris->data.fl) + j);

			*dataIter = (int) trainSet[j].size;
			*(++dataIter) = (int) trainSet[j].density;
			*respIter = (int) trainSet[j].classOfNuts;
		} // end of for loop
		
		dataIter = NULL;
		respIter = NULL;
		
		// desicion tree is training ...
		tree->train(trainMatris, CV_ROW_SAMPLE, respMatris, NULL, NULL, variableType, NULL, params);
		
		for (int j=0; j<5; ++j) {
			dataIter = testMatris->data.fl;

			*dataIter = (int) testSet[j].size;
			*(++dataIter) = (int) testSet[j].density;

			leaf = tree->predict(testMatris);

			fprintf(classificationResult, "%d\n", (int) leaf->value);
		}// end of for loop
		
		
	} // end of for loop
	
	fclose(walnutsDataFile);
	fclose(classificationResult);

	return NO_PROBLEM;
}

