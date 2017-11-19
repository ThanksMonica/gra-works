int MyAutoFocusDll::otsuThreshold(IplImage *frame)  
{  
 const int GrayScale = 256;
 int width = frame->width;  
 int height = frame->height;  
 int pixelCount[GrayScale];  
 float pixelPro[GrayScale];  
 int i, j, pixelSum = width * height, threshold = 0;  
 uchar* data = (uchar*)frame->imageData;  //指向像素数据的指针

 for(i = 0; i < GrayScale; i++)  
 {  
  pixelCount[i] = 0;  
  pixelPro[i] = 0;  
 }  
 
 //统计灰度级中每个像素在整幅图像中的个数  
 for(i = 0; i < height; i++)  
 {  
  for(j = 0;j < width;j++)  
  {  
   pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
  }  
 }  
 
 //计算每个像素在整幅图像中的比例  
 float maxPro = 0.0;
 int kk=0;
 for(i = 0; i < GrayScale; i++)  
 {  
  pixelPro[i] = (float)pixelCount[i] / pixelSum;  
  if( pixelPro[i] > maxPro )
  {
   maxPro = pixelPro[i];
   kk = i;
  }
 }  


 //遍历灰度级[0,255]  
 float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;  
 for(i = 0; i < GrayScale; i++)     // i作为阈值
 {  
  w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;  
  for(j = 0; j < GrayScale; j++)  
  {  
   if(j <= i)   //背景部分  
   {  
    w0 += pixelPro[j];  
    u0tmp += j * pixelPro[j];  
   }  
   else   //前景部分  
   {  
    w1 += pixelPro[j];  
    u1tmp += j * pixelPro[j];  
   }  
  }  
  u0 = u0tmp / w0;  
  u1 = u1tmp / w1;  
  u = u0tmp + u1tmp;  
  deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);  
  if(deltaTmp > deltaMax)  
  {  
   deltaMax = deltaTmp;  
   threshold = i;  
  }  
 }  
 
 return threshold;  
} 