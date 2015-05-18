#include <iostream>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
using namespace std;

#define TextPosx 100
#define TextPosy 50
int flag = 1;

#define SIM


//实现二值化，当像素值（r+g+b）/3>128时设为255,当像素值小于128时设置为0.
int main(int argc, char* argv[])
{
	double scale;
	CvCapture* capture;

	CvVideoWriter *writer = NULL;
	IplImage* frame;

	//合成新视频参数

	int nFrameW = 320;
	int nFrameH = 240;//视频帧格式的大小  
	double fps = 29;
	int iscolor = 1;




	//test bug
#ifdef SIM
	cout << "输入一个二值化阀值，（推荐128）" << endl;
	cin >> scale;//阀值越高，黑色部分越多
	capture = cvCaptureFromFile("Maksim.avi");//打开文件
	//capture = cvCaptureFromCAM(0);//打开摄像头
	writer = cvCreateVideoWriter("out.avi", CV_FOURCC('D', 'I', 'V', 'X'), fps, cvSize(nFrameW, nFrameH), iscolor);
#else
	FILE *fp;
	//判断参数是否正确
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		cout << "输入文件不存在";
		return 1;
	}
	fclose(fp);

	scale = atoi(argv[2]);
	if (scale<0 || scale>255)
	{
		cout << "输入阀值有误";
		return 1;
	}

	if ((fp = fopen(argv[3], "w+")) == NULL)
	{
		cout << "输出路径不合法";
		return 1;
	}

	fclose(fp);


	char *p;
	p = strstr(argv[3], ".");
	int res = strcmp(p, ".avi");
	if (res != 0)
	{
		cout << "输出文件后缀名错误";
		remove(argv[3]);
		return 1;
	}

	capture = cvCaptureFromFile(argv[1]);//打开文件
	scale = atoi(argv[2]);
	writer = cvCreateVideoWriter(argv[3], CV_FOURCC('D', 'I', 'V', 'X'), fps, cvSize(nFrameW, nFrameH), iscolor);

#endif



	cvNamedWindow("avi");
	cvNamedWindow("origin");

	while (1)
	{
		frame = cvQueryFrame(capture);//读取下一帧内容
		if (!frame)//若为空
			break;



		cvShowImage("origin", frame);//显示原本图像

		//frame相当于一张图像
		IplImage* bitimg = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);


		//二值化程序
		CvScalar s;
		double sum;
		for (int i = 0; i<frame->height; i++)
		{
			for (int j = 0; j<frame->width; j++)
			{
				s = cvGet2D(frame, i, j);
				//cout<<s.val[0]<<" "<<s.val[1]<<" "<<s.val[2]<<endl;
				sum = (s.val[0] + s.val[1] + s.val[2]) / 3;
				if (sum > scale)
				{
					s.val[0] = s.val[1] = s.val[2] = 255;//白色
					cvSet2D(frame, i, j, s);
				}
				else
				{
					s.val[0] = s.val[1] = s.val[2] = 0;//黑色
					cvSet2D(frame, i, j, s);
				}
			}
		}


		//加文字程序
		//定义用于显示文字的图像变量 
		IplImage* ImaTEXT = NULL;
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, 0.5, 0, 1, 8);


		cvPutText(frame, "3110000109", cvPoint(TextPosx, TextPosy), &font, CV_RGB(255, 0, 0));

		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0.5, 2, 8);
		cvPutText(frame, "Frank_Shaw", cvPoint(TextPosx, TextPosy + 30), &font, CV_RGB(0, 255, 0));

		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 1, 3, 8);
		cvPutText(frame, "Copy-Right", cvPoint(TextPosx, TextPosy + 60), &font, CV_RGB(0, 0, 255));



		cvShowImage("avi", frame);//显示图像




		cvWriteFrame(writer, frame);//保存图片为视频流格式





		char c = cvWaitKey(29);//控制帧数

		if (c == 27)//Esc键ASCII值
			break;
		if (c == ' ')//空格键ASCII值
			system("pause");
	}
	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&writer);


	cvDestroyWindow("avi");
	cvDestroyWindow("origin");
	return 0;
}