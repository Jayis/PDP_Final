#include <vector>
#include <opencv2/core/core.hpp>
#define NC 3
using namespace std;
using namespace cv;

void combineWorld(Mat worldImage, vector<Mat> mList);

void combineWorld2(Mat worldImage1, unsigned char * recvbuf, int scrwidth, int scrheight, int count);
void combineWorld3(Mat worldImage1, unsigned char * recvbuf, int scrwidth, int scrheight, int count);


