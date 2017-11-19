# gra-works
@(病虫害检测)[图像分割|降维|识别]
# 马铃薯病虫害检测步骤
# - **分离叶片与背景（提取叶片）**
  ① HSI，选取I通道，进行`中值滤波`操作->去噪；<br>
  ② Lab，选取a通道，进行`K-means聚类`->得到绿色叶片部分；<br>
  ③ `区域填充法`和`最小移除法`->填充病斑；<br>
  ④ 消除绿叶与背景；<br>
  ⑤ `腐蚀-Hough变换`->拟合<br>
  ### 图像分割方法
    基于阈值： 
[直方图双峰法(mode)](http://blog.csdn.net/hh555800/article/details/42342687);<br>
[OTSU（自适应阈值)](https://zh.wikipedia.org/zh-hans/%E5%A4%A7%E6%B4%A5%E7%AE%97%E6%B3%95)<br>
`缺点`：当图像复杂时（灰度差异小，信噪比低，目标面积小等）分割效果较差 ；
```matlab
function level = otsu(histogramCounts, total)
%% OTSU automatic thresholding method
sumB = 0;
wB = 0;
maximum = 0.0;
threshold1 = 0.0;
threshold2 = 0.0;
sum1 = sum((1:256).*histogramCounts); 
% the above code is replace with this single line
for ii=1:256
    wB = wB + histogramCounts(ii);
    if (wB == 0)
        continue;
    end
    wF = total - wB;
    if (wF == 0)
        break;
    end
    sumB = sumB +  ii * histogramCounts(ii);
    mB = sumB / wB;
    mF = (sum1 - sumB) / wF;
    between = wB * wF * (mB - mF) * (mB - mF);
    if ( between >= maximum )
        threshold1 = ii;
        if ( between > maximum )
            threshold2 = ii;
        end
        maximum = between;
    end
end
level = (threshold1 + threshold2 )/(2);
end
```



             [固定阈值/半阈值/迭代阈值法]
    基于边缘：边缘检测方法
    基于区域：[分水岭算法]
             [区域生长]
             [区域分类合并]
    基于图论：
    基于能量泛函:
   <!-- 1.K-means
   2.Hough变幻
   3.超像素分割算法
   4.Graphcut -->

# - **分离病斑与叶片（提取病斑）**
  Lab，选取a通道，进行`二维OTSU分割`->提取病斑；
# - **特征提取**
    颜色：选取H,S,V的均值，方差，三阶矩等;
    形状：`几何区与描述符`;
    纹理:`灰度共生矩阵`，0°，45°，90°，135°，四个方向的特征;
# - **特征融合（优化特征）**
    ① `小波变换`->优化特征提取；
    ② SPSS19.0-Common Factor Analysis
     `PCA`->主分量提取-加权融合；
# - **识别**

    ① <code>SVM</code>
    ② `费歇尔判别函数方程`


  -  `SVM`<br>
  - `费歇尔判别函数方程`



    
