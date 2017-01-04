//该程序用于寻找一个“OK”手势中，食指与拇指的焦点以及“孔洞”的中心点。
//其中用红色的圆点标出特征点
//利用HSV将手从环境中分割出来（HSV数值根据不同图片的不同背景环境可能需要一定的调整）
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;  
using namespace cv;  



//找到轮廓的中心
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
    split(hsv,a);//分离色彩通道
    Mat h=a[0];
    Mat s=a[1];
    Mat v=a[2];
    //调节HSV值
    inRange(h,6,25,a[0]);
    inRange(s,66,200,a[1]);
    inRange(v,60,200,a[2]);
    //合并调节好的三个单通道图
    bitwise_and(a[0],a[1],imb);bitwise_and(a[2],imb,imb);
    //膨胀腐蚀
    dilate(imb, imb, Mat(15, 15, CV_8UC1), Point(-1, -1));    
    erode(imb, imb, Mat(15, 15, CV_8UC1), Point(-1, -1)); 
    
    vector<vector<Point> > contours; 
    vector<Vec4i> hierarchy;
    findContours(imb,contours,hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE); 

    //寻找圈的中心点
    vector<Point> p1=contours[0];//手的外轮廓
    vector<Point> p2=contours[1];//手的内轮廓
    _Center(contours[1],img);
    //寻找内外轮廓距离最小的点
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
    imshow("结果图",img);
    waitKey();  
    system("pause");
    return 0;  
}  
