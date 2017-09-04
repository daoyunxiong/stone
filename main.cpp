//包含的头文件
#include "tools.h"
#include "findCircleParameter.h"
#include "corrector.h"
#include "imagesStitcher.h"
#include "viewer.h"


using namespace std;
using namespace cv;
bool isDispCorrectRet = false;
bool saveResult = false;

static void help()
{
	printf("This is a navigator based on fisheye images.\n"
		"Usage: navigator\n"
		"     [-d]          # display the corrected fisheye images.\n"
		"     [-s]          # save the result of image process into files.\n"
		"     [-h]          # show the help information.\n"
		"\n");

}
void paserParams(int argc, char**argv)
{
	if (argc > 1)
	{
		for (int i = 1;i < argc;i++)
		{
			if (strcmp(argv[i], "-d")==0)
				isDispCorrectRet = true;
			else if (strcmp(argv[i], "-s")==0)
				saveResult = true;
			else if (strcmp(argv[i], "-h") == 0)
			{
				help();
				exit(0);
			}
		}
		saveResult = true;
	}
}

//CvCapture *capture;
void split_image2(Mat& img_l, Mat &img_r)
{
Mat img_l_f, img_l_s;
img_l_f = img_l(Range(0, img_l.rows), Range(0,(int)(img_l.cols/2)));
img_l_s = img_l(Range(0, img_l.rows), Range((int)(img_l.cols/2), img_l.cols));
imwrite("x1.jpg", img_l_f);
imwrite("x2.jpg", img_l_s);
Mat img_r_f, img_r_s;
img_r_f = img_r(Range(0, img_l.rows), Range(0,(int)(img_l.cols/2)));
img_r_s = img_r(Range(0, img_l.rows), Range((int)(img_l.cols/2), img_l.cols));
imwrite("x3.jpg", img_r_f);
imwrite("x4.jpg", img_r_s);
}
cv::VideoCapture video;
void split_image(Mat& image_l, Mat &image_r)
{
	Mat image;
	video >> image;
	
	image_l = image(Range(0, image.rows), Range(0,(int)(image.cols/2)));
        image_r = image(Range(0, image.rows), Range((int)(image.cols/2), image.cols));
        resize(image_l, image_l, Size(image_l.cols/2, image_l.rows/2));
        resize(image_r, image_r, Size(image_r.cols/2, image_r.rows/2));


}
bool open_video(int argc, char **argv)
{
//capture =  cvCreateFileCapture("./360_0080.MP4");
    if(!video.open(("./360_0080.MP4")))
    {
        printf("靠靠靠靠");
        return false;
    };
	Mat image;
	//while(1){
	//video >> image;	
	//imshow(" ", image);
	//waitKey(100);
	Mat image_l, image_r;
	split_image(image_l, image_r);
	//image_l = image(Range(0, image.rows), Range(0,(int)(image.cols/2)));
	//image_r = image(Range(0, image.rows), Range((int)(image.cols/2), image.cols));
	//resize(image_l, image_l, Size(image_l.cols/2, image_l.rows/2));
	//resize(image_r, image_r, Size(image_r.cols/2, image_r.rows/2));
//	imshow("left", image_l);
//	waitKey(100);
//	imshow("right", image_r);
}

//主程度入口点
int main(int argc, char** argv)
{
	//Mat homography(3,3,CV_64F);
	double m[3][3] ={
	{0.8908620221861135, 0.1359951373351792, -13.62005157230035 },
	{-1.122926632112986, 1.150122689259188, -13.22936630814173 },
	{-0.001621983870912232, 0.0002001804446698442, 1 }
	};
	Mat homography(3,3,CV_64F,m);
//	open_video(argc, argv);
	//getchar();
	Mat image_l, image_r;
//	image_l = imread(argv[1],1);
//	image_r = imread(argv[2],1);
//	split_image2(image_l, image_r);
//	getchar();	

//	split_image(image_l, image_r);
	#if 0
	while(1){
	Mat image_l, image_r ;
	}
	#endif

	paserParams(argc, argv);

	correctParameters params;
	corrector adjuster;
	vector<Mat> outputs;
	imagesStitcher stitcher;

	//读图片到内存中
//	if (tools::readImage(argc, argv))
	{
		//vector<Mat>& inputs = tools::GetImages();

	//	Mat source_image = inputs[0];
		Mat source_image = imread(argv[1], 1);
	//	Mat source_image ;
	//	resize(inputs[0], source_image, Size(inputs[0].cols/2, inputs[0].rows/2));
		//imshow (" 22", source_image);
		//waitKey(100);

		if (findCircleParameter::init(source_image))
		{
	//		findCircleParameter::findCircle();

//#pragma region 校正参数设定区
			params.imgOrg = source_image;
			cout << "load ok " << endl;
	//		findCircleParameter::getCircleParatemer(params.center, params.radius);
			params.radius = 610;
			params.center = Point(640, 246);
			params.w_longtitude = PI / 2;
			params.w_latitude = PI / 2;
			params.distMap = LATITUDE_LONGTITUDE;
			params.theta_left = 0;
			params.phi_up = 0;
			params.camerFieldAngle = findCircleParameter::FOV;
			params.camProjMode = EQUIDISTANCE;
			params.typeOfCorrect = Reverse;
//#pragma endregion								

			cout << endl
				<< "Correct Image(s) with the same circular region params: " << endl
				<< "radius = " << params.radius << ", center = " << params.center << endl << endl;

//#pragma region 图像校正区
		//	corrector::correctMethod method = corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_HALF_PI;
			corrector::correctMethod method = corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL;

#if 0
		while(1)
		{
		int64 start_time = getTickCount();	
			split_image(image_l, image_r);
			params.imgOrg = image_l;
			Mat im_l = adjuster.correctImage(params, method,isDispCorrectRet);
			
			params.imgOrg = image_r;
			Mat im_r = adjuster.correctImage(params, method,isDispCorrectRet);
		 cout << "run time(s): " << (getTickCount() -start_time)/getTickFrequency() << endl;
		//	imshow("ll", im_l);
		//	waitKey(1);
		//	imshow("rr", im_r);
		
        cout << homography << endl;

	homography = Mat::eye(3,3, CV_64F);
	homography.at<double>(0,2) = 590;
	homography.at<double>(1,2) = 0;
        Mat transformed;
        warpPerspective(im_l, transformed, homography, Size(im_r.cols*2, im_r.rows));
	Mat result = transformed.clone();
	Mat half(transformed,Rect(0, 0, im_r.cols, im_r.rows));
	im_r.copyTo(half);
        imshow("transformed", transformed);
	waitKey(1);
		
	Mat result_linerblend = transformed.clone();
	double dblend = 0.0;
	int ioffset =im_r.cols-50;
	for(int i=0; i<50; i++)
	{
	result_linerblend.col(ioffset+i) = transformed.col(ioffset+i)*(1-dblend) + result.col(ioffset+i)*dblend;         
	dblend = dblend +0.02;
	}
	imshow("blend", result_linerblend);
	}

			outputs.push_back(
				adjuster.correctImage(params, method,
					isDispCorrectRet)
				);
#endif
  char folder_name[100];
  char *p_folder;
  p_folder = strrchr(argv[1], '/');
  strncpy(folder_name, argv[1], strlen(argv[1]) - strlen(p_folder));
  int seq_array =  strlen(argv[1]) - strlen(p_folder);
  folder_name[seq_array]  = '\0';
  cout  << "folder_name " << folder_name << endl;

	char dir_name[100];
	sprintf(dir_name ,"mkdir %s_corrected", folder_name);
	system(dir_name);
	//getchar();
	

			for (int i = 0; i < argc - 1 ; i++)
			{
				source_image = imread(argv[i+1], 1);
				params.imgOrg = source_image;

//				outputs.push_back(					adjuster.correctImage(params, method, isDispCorrectRet));

				Mat temp_buf = adjuster.correctImage(params, method, isDispCorrectRet);
			//	Mat temp = temp_buf(Range(350,830), Range(190,990));
				Mat temp = temp_buf(Range(460,910), Range(240,1040));
				char suffixx[100];
				char *p;
				p = strrchr(argv[i+1], '/'); 
/*				
				char folder_name[100] ;
 				strncpy(folder_name, argv[i+1] , strlen(argv[i+1] - strlen(p)));
				folder_name[strlen(argv[i+1] - strlen(p))] = '\0';
				cout << "folder_name " << folder_name << endl;
				getchar();
*/
				cout << argv[i+1] << endl;				

				sprintf(suffixx, "%s_corrected/%s", folder_name, p+1);
				cout << "suffixx" << suffixx << endl;
	//			getchar();
//                        string tmp(suffixx);
//                        filenamePath = tmp;
 	                       imwrite(suffixx, temp);
//                        images1.push_back(outputs[i]);

			}
		}
	}
//#pragma endregion
	return 0;
	getchar();
//#pragma region 图像接接部分
	vector<Mat> images1, images2;
	string filenamePath;
	char suffix[MAX_PATH];
	for (int i = 0;i < outputs.size();i++)
	{
	//	if (saveResult)
		{
			sprintf(suffix, "./corr_images/corrected%d.jpg", i);
			string tmp(suffix);
			filenamePath = tmp;
			imwrite(suffix, outputs[i]);
			images1.push_back(outputs[i]);
	//		imwrite(filenamePath, outputs[i]);
		}
//		if (i < outputs.size() - 1)
//			images1.push_back(outputs[i]);
//		else
//			images2.push_back(outputs[i]);
	}

	//if (outputs.size() > 1)
	if (0)
	{
		cout << endl << "Start Stitch images" << endl;

	//	Mat tmpRet = stitcher.stitchImages(images1);
		Stitcher s=Stitcher::createDefault(false);
		Mat panorama;
		Stitcher::Status status = s.stitch(outputs, panorama);
		imshow("2", panorama);
		imwrite("pana.jpg", panorama);
		//images2.insert(images2.begin(), tmpRet.clone());
		//stitcher.stitchImages(images2);
	//	stitcher.showPanorama();

		cout << "Stitch Images completes!" << endl;
//#pragma endregion
//#pragma region 浏览全景图像

		Mat panoramaImage = stitcher.getPanorama();

		if (!panoramaImage.empty())
		{
			if (saveResult)	imwrite("panorama.jpg", panoramaImage);

			viewer panoViewer(panoramaImage);
			panoViewer.showWindow();
		}
//#pragma endregion
	}

	#if 1
	homography = Mat::eye(3,3, CV_64F);
        homography.at<double>(0,2) = 1180;
        homography.at<double>(1,2) = 0;
        Mat transformed;
	Mat im_r = outputs[0].clone();
	Mat im_l = outputs[1].clone();
        warpPerspective(im_l, transformed, homography, Size(im_r.cols*2, im_r.rows));
        Mat result = transformed.clone();
        Mat half(transformed,Rect(0, 0, im_r.cols, im_r.rows));
        im_r.copyTo(half);
        imshow("transformed", transformed);
        waitKey(1);
                
        Mat result_linerblend = transformed.clone();
        double dblend = 0.0;
        int ioffset =im_r.cols-50;
        for(int i=0; i<50; i++)
        {
        result_linerblend.col(ioffset+i) = transformed.col(ioffset+i)*(1-dblend) + result.col(ioffset+i)*dblend;         
        dblend = dblend +0.02;
        }
        imshow("blend", result_linerblend);
	imwrite("blend.jpg", result_linerblend);
	#endif
	return 0;
}
