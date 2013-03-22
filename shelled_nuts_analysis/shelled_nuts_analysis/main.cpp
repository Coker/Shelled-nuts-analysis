
#include <stdio.h>

#include "ml.h"
#include "cv.h"
#include "highgui.h"

#include "functions.h"

int main()
{
	CvDTree *detectingNutsTree, *classifiedNutsTree;
	/*	
	createShelledNutsTree("highLevel.bmp",
						  "lowLevel.bmp", &detectingNutsTree);
	detectShelledNutsPixel("highLevel.bmp", "lowLevel.bmp", detectingNutsTree);
	detectShelledNuts("highLevel.bmp", "lowLevel.bmp", "res.bmp");
	*/
	
	applyFiveFoldsCrossValidation("walnuts.txt");

	return 0;
}