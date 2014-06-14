#include <vector>
#define NC 3

// combine 4 images into 1, and update it into worldImage
void combine(mat worldImage, vector<mat> mList, vector<camera> cList)
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
		wArray[worldIndex + chnl] = mArray[cameraIndex + chnl];
	}
    }
}

