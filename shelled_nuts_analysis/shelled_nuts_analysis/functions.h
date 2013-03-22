
#if !defined(_FUNCTIONS_H)
#define _FUNCTIONS_H
	
	#define DEBUG_LEVEL_1
	
	#if !defined(DEBUG_LEVEL_1) && !defined(DEBUG_LEVEL_2) && !defined(DBG) 
		#define __RELEASE	
	#endif
	
	#define MAX_PIXEL_VAL 255

	// Bu constant X-Ray çýktýsýnýn saðýnda ve solunda bulunan siyah þeriti
	// atlamak için tanýmlanmýþtýr
	#define IGNORED 40
	
	
	#define MULTIPLIER 1000

	#define SHELLED_NUTS 1.0
	#define	NOT_SHELLED_NUTS 2.0
	
	#define DISTANCE 5
	
	enum DIRECTION {
		N =1, // North
		NE,   // North East
		E,    // East
		SE,   // South East
		S,    // South
		SW,   // South West
		W,    // West 
		NW	  // North West
	};

	enum RETURN_VALUES {
		NO_PROBLEM =0,
		FILE_NOT_FOUND_ERROR,
		FILE_NOT_CREATED,
		NOT_ENOUGH_HEAP_SPACE,
		NOT_ENOUGH_STACK_SPACE,
		VARIABLE_NOT_SET
	};

	typedef struct {
		int size;
		int ratio;
		int density;
		int classOfNuts;
	} WalnutsData;

	int createShelledNutsTree(const char const *highLevelPath, 
							  const char const *lowLevelPath, CvDTree** tree);
	int filter(IplImage *image);
	int detectShelledNutsPixel(const char const *highLevelPath,
							   const char const *lowLevelPath, 
							   const CvDTree const *shelledNutsTree);
	bool controlNeigbor(IplImage *image, CvPoint point, int direction);
	int detectShelledNuts(const char const *highLevelPath,
					      const char const *lowLevelPath,
					      const char const *resPath);
	int applyFiveFoldsCrossValidation(const char const *walnutsDataFilePath);

#endif