 #include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cstdio>

using namespace cv;
using namespace std;

void Imagecapture()
{
VideoCapture cap(0); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "ERROR: Cannot open the video file" << endl;
        
    }
 
   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH,1080); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT,512); //get the height of frames of the video

   cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

   vector<int> compression_params; //vector that stores the compression parameters of the image

   compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); //specify the compression technique

   compression_params.push_back(95); //specify the jpeg quality

	 
	 
   Mat img(dWidth, dHeight, CV_8UC1);
   cap.read(img);
	 
   bool bSuccess = imwrite("/media/card/img.jpg", img, compression_params); //write the image to file


   if ( !bSuccess )
   {
        cout << "ERROR : Failed to save the image" << endl;
   }
}

int main()
{
	Imagecapture();
	return 0;
}
