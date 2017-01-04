//�ó�������Ѱ��һ����OK�������У�ʳָ��Ĵָ�Ľ����Լ����׶��������ĵ㡣
//�����ú�ɫ��Բ����������
//����HSV���ִӻ����зָ������HSV��ֵ���ݲ�ͬͼƬ�Ĳ�ͬ��������������Ҫһ���ĵ�����
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;  
using namespace cv;  



//�ҵ�����������
void _Center(vector<Point> p,Mat src)
{
    Point tmp;    
        int i;  
        for (i = 0; i < p.size(); i++)    
        {    
            tmp.x += p[i].x;
            tmp.y += p[i].y;
        }
        tmp.x /=i;
        tmp.y /=i;
        circle(src, tmp, 8 ,Scalar(0, 0, 255), CV_FILLED); 
}

int main(int argc, char* argv[])  
{  
    Mat hsv;
    Mat imb;
    Mat imy;
    Mat iml;
    const char* imagename = "OK.jpg"; 
    Mat img = imread(imagename);
    
    if(img.empty())  
    {  
        fprintf(stderr, "Can not load image %s\n", imagename);  
        return -1;  
    }   
    cvtColor(img,hsv,CV_BGR2HSV);
    vector<Mat> a;
    split(hsv,a);//����ɫ��ͨ��
    Mat h=a[0];
    Mat s=a[1];
    Mat v=a[2];
    //����HSVֵ
    inRange(h,6,25,a[0]);
    inRange(s,66,200,a[1]);
    inRange(v,60,200,a[2]);
    //�ϲ����ںõ�������ͨ��ͼ
    bitwise_and(a[0],a[1],imb);bitwise_and(a[2],imb,imb);
    //���͸�ʴ
    dilate(imb, imb, Mat(15, 15, CV_8UC1), Point(-1, -1));    
    erode(imb, imb, Mat(15, 15, CV_8UC1), Point(-1, -1)); 
    
    vector<vector<Point> > contours; 
    vector<Vec4i> hierarchy;
    findContours(imb,contours,hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE); 

    //Ѱ��Ȧ�����ĵ�
    vector<Point> p1=contours[0];//�ֵ�������
    vector<Point> p2=contours[1];//�ֵ�������
    _Center(contours[1],img);
    //Ѱ����������������С�ĵ�
    int test,min=5;
    Point temp;
    for(int j=0;j<p1.size();j++)
    {
        for(int i=0;i<p2.size();i++)
        {
            test=(p1[j].x-p2[i].x)*(p1[j].x-p2[i].x)-(p1[j].y-p2[i].y)*(p1[j].y-p2[i].y);
            if(test<min)
            {   min=test;
                temp.x=p2[i].x;temp.y=p2[i].y;
            }
        }
    }
    circle(img, temp, 8 ,Scalar(0, 0, 255), CV_FILLED);
    imshow("���ͼ",img);
    waitKey();  
    system("pause");
    return 0;  
}  
