#include <iostream>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
using namespace std;

#define TextPosx 100
#define TextPosy 50
int flag = 1;

#define SIM


//ʵ�ֶ�ֵ����������ֵ��r+g+b��/3>128ʱ��Ϊ255,������ֵС��128ʱ����Ϊ0.
int main(int argc, char* argv[])
{
	double scale;
	CvCapture* capture;

	CvVideoWriter *writer = NULL;
	IplImage* frame;

	//�ϳ�����Ƶ����

	int nFrameW = 320;
	int nFrameH = 240;//��Ƶ֡��ʽ�Ĵ�С  
	double fps = 29;
	int iscolor = 1;




	//test bug
#ifdef SIM
	cout << "����һ����ֵ����ֵ�����Ƽ�128��" << endl;
	cin >> scale;//��ֵԽ�ߣ���ɫ����Խ��
	capture = cvCaptureFromFile("Maksim.avi");//���ļ�
	//capture = cvCaptureFromCAM(0);//������ͷ
	writer = cvCreateVideoWriter("out.avi", CV_FOURCC('D', 'I', 'V', 'X'), fps, cvSize(nFrameW, nFrameH), iscolor);
#else
	FILE *fp;
	//�жϲ����Ƿ���ȷ
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		cout << "�����ļ�������";
		return 1;
	}
	fclose(fp);

	scale = atoi(argv[2]);
	if (scale<0 || scale>255)
	{
		cout << "���뷧ֵ����";
		return 1;
	}

	if ((fp = fopen(argv[3], "w+")) == NULL)
	{
		cout << "���·�����Ϸ�";
		return 1;
	}

	fclose(fp);


	char *p;
	p = strstr(argv[3], ".");
	int res = strcmp(p, ".avi");
	if (res != 0)
	{
		cout << "����ļ���׺������";
		remove(argv[3]);
		return 1;
	}

	capture = cvCaptureFromFile(argv[1]);//���ļ�
	scale = atoi(argv[2]);
	writer = cvCreateVideoWriter(argv[3], CV_FOURCC('D', 'I', 'V', 'X'), fps, cvSize(nFrameW, nFrameH), iscolor);

#endif



	cvNamedWindow("avi");
	cvNamedWindow("origin");

	while (1)
	{
		frame = cvQueryFrame(capture);//��ȡ��һ֡����
		if (!frame)//��Ϊ��
			break;



		cvShowImage("origin", frame);//��ʾԭ��ͼ��

		//frame�൱��һ��ͼ��
		IplImage* bitimg = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);


		//��ֵ������
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
					s.val[0] = s.val[1] = s.val[2] = 255;//��ɫ
					cvSet2D(frame, i, j, s);
				}
				else
				{
					s.val[0] = s.val[1] = s.val[2] = 0;//��ɫ
					cvSet2D(frame, i, j, s);
				}
			}
		}


		//�����ֳ���
		//����������ʾ���ֵ�ͼ����� 
		IplImage* ImaTEXT = NULL;
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, 0.5, 0, 1, 8);


		cvPutText(frame, "3110000109", cvPoint(TextPosx, TextPosy), &font, CV_RGB(255, 0, 0));

		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0.5, 2, 8);
		cvPutText(frame, "Frank_Shaw", cvPoint(TextPosx, TextPosy + 30), &font, CV_RGB(0, 255, 0));

		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 1, 3, 8);
		cvPutText(frame, "Copy-Right", cvPoint(TextPosx, TextPosy + 60), &font, CV_RGB(0, 0, 255));



		cvShowImage("avi", frame);//��ʾͼ��




		cvWriteFrame(writer, frame);//����ͼƬΪ��Ƶ����ʽ





		char c = cvWaitKey(29);//����֡��

		if (c == 27)//Esc��ASCIIֵ
			break;
		if (c == ' ')//�ո��ASCIIֵ
			system("pause");
	}
	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&writer);


	cvDestroyWindow("avi");
	cvDestroyWindow("origin");
	return 0;
}