# 基于OTSU理论的图像分割算法研究
# - **特点**
①无参数无监督<br>
②一维OTSU算法易受噪声影响<br>
③不受亮度和对比度影响<br>
④一维Fisher判别分析的离散化模拟<br>
⑤对目标大小敏感；<br>
⑥对类间方差为单峰的图像处理效果最好；<br>
# - **原理**
①类内方差最小<=>类间方差最大<br>
②类内方差最小时，前景与背景的错分概率最小<br>
# -**方法**
①计算出每个灰度级的直方图和概率；<br>
②初始化类概率w和类均值u的值；<br>
③遍历所有可能的阈值t=1：max;更新w,u;计算类间方差值；<br>
④取两个最大的类间方差值t1(此时阈值为threshold1),t2（此时阈值为threshold2）；<br>
⑤最佳阈值=(threshold1+threshold2)/2;<br>
 # - **算法改进思路**
 ①提高抗噪性能<br>
 ②减少运算量,提高运行速度，满足实时性-->（1）量化直方图：以合适的量化步长对原直方图进行量化，从而减少灰度级数目；<br>
 （2）运用递推算法:编程角度实现，每次计算不重头开始，而是保留上一次计算结果作为下一次运算初值；<br>
 （3）判决域约束法：最优阈值一般落在与对角线平行的距离为n的约束线范围内，在此区域搜索从而减少不必要的搜索空间。<br>
 # - **算法设计**
①定义变量：
总像素数：N<br>
前景像素数：fN<br>
背景像素数：bN<br>
前景像素灰度和：fSum<br>
背景像素灰度和：bSum<br>
前景像素平均灰度：fu<br>
背景像素平均灰度：bu<br>
图像总灰度值：Sum<br>
图像平均灰度：u<br>
前景像素占比：fw<br>
背景图像占比：bw<br>
类间方差：g<br>

前景像素占比：fw = fN/N<br>
背景像素占比：bw = bN/N<br>
前景、背景占比满足：fw + bw =1  (1)<br>

- 图片平均灰度值：
图片灰度直方图 ： Histogram[256]<br>
图片总灰度值 Sum ： for(i=0; i<N; i++){ Sum += Histogram[i];}<br>
图片平均灰度值 u ： Sum/N<br>

- 阈值为T时前景平均灰度：
阈值为T时前景像素数 fN : for(i=0; i<=T; i ++) { fN += Histogram[i];}<br>
阈值为T时前景像素灰度和 fSum ： for(i=0; i<=T; i++) { fSum += Histogram[i]*i;}<br>
阈值为T时前景平均灰度 fu ： fSum/fN<br>

- 平均灰度满足：
u = fu * fw + bu*bw  (2)<br>
- 类间方差：
类间方差 g：fw*(fu-u)^2+bw*(bu-u)^2  (3)<br>
将(1)(2)带入(3)式：<br>
g = bw* fw* (fu-bu)^2<br>

```c
#include <opencv2/opencv.hpp>  
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

using namespace std;
using namespace cv;

Mat otsuGray(const Mat src) {
    Mat img = src;
    int c = img.cols; //图像列数
    int r = img.rows; //图像行数
    int T = 0; //阈值
    uchar* data = img.data; //数据指针
    int ftNum = 0; //前景像素个数
    int bgNum = 0; //背景像素个数
    int N = c*r; //总像素个数
    int ftSum = 0; //前景总灰度值
    int bgSum = 0; //背景总灰度值
    int graySum = 0;
    double w0 = 0; //前景像素个数占比
    double w1 = 0; //背景像素个数占比
    double u0 = 0; //前景平均灰度
    double u1 = 0; //背景平均灰度
    double Histogram[256] = {0}; //灰度直方图
    double temp = 0; //临时类间方差
    double g = 0; //类间方差

    //灰度直方图
    for(int i = 0; i < r ; i ++) {
        for(int j = 0; j <c; j ++) {
            Histogram[img.at<uchar>(i,j)]++;
        }
    }
    //求总灰度值
    for(int i = 0; i < 256; i ++) {
        graySum += Histogram[i]*i;
    }

    for(int i = 0; i < 256; i ++) {
        ftNum += Histogram[i];  //阈值为i时前景个数
        bgNum = N - ftNum;      //阈值为i时背景个数
        w0 = (double)ftNum/N; //前景像素占总数比
        w1 = (double)bgNum/N; //背景像素占总数比
        if(ftNum == 0) continue;
        if(bgNum == 0) break;
        //前景平均灰度
        ftSum += i*Histogram[i];
        u0 = ftSum/ftNum;

        //背景平均灰度
        bgSum = graySum - ftSum;
        u1 = bgSum/bgNum;

        g = w0*w1*(u0-u1)*(u0-u1);
        if(g > temp) {
            temp = g;
            T = i;
        }
    }

    for(int i=0; i<img.rows; i++)
    {
        for(int j=0; j<img.cols; j++)
        {
            if((int)img.at<uchar>(i,j)>T)
                img.at<uchar>(i,j) = 255;
            else
                img.at<uchar>(i,j) = 0;
        }
    }
    return img;
}
```