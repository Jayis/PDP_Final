#include <vector>
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#define NC 3
#include <cv.h>
using namespace std;
using namespace cv;


// combine 4 images into 1, and update it into worldImage
/*
   void combine(Mat worldImage, vector<Mat> mList, vector<camera> cList)
   {
   unsigned char * wArray = worldImage.data;
   vector<unsigned char * > mArray;
   mArray.resize(mList.size());
   for(int i = 0; i < mList.size(); i++)
   mArray[i] = mList[i].data;

   for(int index = 0; index < mList.size(); index++)
   {
   int left = cList[index].center_x - cList[index].width / 2;
   int right = left + cList[index].width;
   int down = cList[index].center_y - cList[index].height / 2;
   int up = down + cList[index].height;

   for(int x = left; x < right; x++) for(int y = down; y < top; y++)
   {
   if( x < 0 || x >= worldImage.cols || y < 0 || y >= worldImage.rows) continue;
   int worldIndex = (x + y * worldImage.cols) * NC;
   int cameraIndex = ((x-left) + (y-down) * cList[index].width) * NC;
   for(int chnl = 0; chnl < NC; chnl++)
   wArray[worldIndex + chnl] = mArray[index][cameraIndex + chnl];
   }
   }
   }
 */

void combineWorld(Mat worldImage1, vector<Mat> mList)
{
	Mat* worldImage = &worldImage1;
    unsigned char * wArray = (unsigned char*) (*worldImage).data;
    vector<unsigned char * > mArray;
    mArray.resize(mList.size());
    for(int i = 0; i < mList.size(); i++)
	mArray[i] = (unsigned char*) mList[i].data;

    for(int x = 0; x < (*worldImage).cols; x++) for(int y = 0; y < (*worldImage).rows; y++)
    {
	int worldIndex = (x + y * (*worldImage).cols) * NC;
	vector<unsigned int> sum;
	sum.resize(NC);
	for(int i = 0; i < NC; i++) sum[i] = 0;
	int count = 0;

	for(int ind = 0; ind < mList.size(); ind++)
	{
	    bool has_Value = false;
	    for(int chnl = 0; chnl < NC; chnl++) if(mArray[ind][ worldIndex + chnl ] != 0)
	    {
		has_Value = true;
		sum[chnl] += (unsigned int) mArray[ind][ worldIndex + chnl ];
	    }
	    if(has_Value) count++;
	}
	
	for(int chnl = 0; chnl < NC; chnl++){
		if (count == 0){
			continue;
		}
		wArray[worldIndex + chnl] += (unsigned char)(sum[chnl] / count);
	}
    }
}


