#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

void DisplayYUV();
int main(int argc, char **argv)
{
	DisplayYUV();
	cout << "end "<< endl; 

}

void DisplayYUV()
{
        int w = 1280;
        int h = 720;
        printf("yuv file w: %d, h: %d \n", w, h);

        FILE* pFileIn = fopen("tmp.yuv", "rb+");
        int bufLen = w*h*3/2;
        unsigned char* pYuvBuf = new unsigned char[bufLen];
        int iCount = 0;
	
	unsigned char *yuv_start = (unsigned char *)malloc(1280*720*3/2);
	unsigned char *p = yuv_start;
	unsigned char *yuvbuf_p = pYuvBuf;
	long frame_size = 1280*720;
        fread(pYuvBuf, frame_size*sizeof(unsigned char)*3/2, 1, pFileIn);

	cout << "uchar size" << sizeof(unsigned char) << endl;

	cout << "start 1 " << endl;
	memcpy(p, yuvbuf_p, frame_size);
	yuvbuf_p = yuvbuf_p + frame_size;
	p = p + frame_size;

	memcpy(p, yuvbuf_p, frame_size/4);
	yuvbuf_p = yuvbuf_p + frame_size/4;
	p = p+ frame_size/4;

	
	memcpy(p, yuvbuf_p, frame_size/4);
	yuvbuf_p = yuvbuf_p + frame_size/4;
	p = p+ frame_size/4;

	cout << "start 2 " << endl;
	Mat testMat(h*3/2, w, CV_8UC1) ;
	Mat testrgb;
	memcpy(testMat.data, yuv_start, frame_size*3/2);
        cv::cvtColor(testMat, testrgb, CV_YUV2BGR_I420);
	imshow("ii", testrgb);
	waitKey(400);
	free(yuv_start);
	getchar();


	cout << "size " << sizeof(pFileIn) << endl;
       for(int i=0; i<2; i++)
        {
                fread(pYuvBuf, bufLen*sizeof(unsigned char), 1, pFileIn);
 //               pYuvBuf = fd;
                cv::Mat yuvImg;
                yuvImg.create(h*3/2, w, CV_8UC1);
                memcpy(yuvImg.data, pYuvBuf, bufLen*sizeof(unsigned char));
                cv::Mat rgbImg;
                cv::cvtColor(yuvImg, rgbImg, CV_YUV2BGR_I420);

              cv::imshow("img", rgbImg);
              cv::waitKey(300);
                imwrite("test.jpg", rgbImg);

                printf("%d \n", iCount++);
        }

        delete[] pYuvBuf;

	//free(yuv_start);
	yuv_start = NULL;
        fclose(pFileIn);
	getchar();
}

