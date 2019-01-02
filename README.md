# LifeGame

这个东西以前在看知乎的时候就看到过，感觉挺好玩的。最近又看到了，细细看了一下原理，恍然大悟这不就是一个空域滤波么？写一个应该很好玩吧？于是就动手了，为了显示方便用的Opencv的Mat数据结构来存取数据和显示。写了一下午差不多就可以了，后面再加了些配置文件的接口，并给了一些配置文件，这里记录一下。

程序：https://github.com/zhxing001/LifeGame
### 1.生命游戏
生命游戏也叫康威游戏，是一种细胞自动机，最初是由数学家约翰·何顿·康威在1970年发明的。

这个游戏是一个零玩家游戏，整个游戏会根据定义的规则自动执行下去。

生命游戏的游戏场地是一个二维的棋盘，每一个位置叫做一个细胞，有`生`, `死`两种状态，如果相邻方格活着的细胞数量过多，这个细胞会因为`资源匮乏`而死亡，相反，如果因为周围的细胞过少，这个细胞会因为太孤单而死去。实际中，这种规则是可以自定义的。有一点要注意：**棋牌上的所有细胞同时刷新状态。一个细胞生死变化不立即影响其他细胞**，在这种规则下，杂乱无序的的细胞会逐渐演化出各种精致，有型的结构。

有个软件，内置了各种规则以及初始状态，也不大，可以下载下来玩一下：[golly主页](http://golly.sourceforge.net/)，主页上的动图感受一下，这是一种比较复杂的初始状态了。还有一个网址可以在线玩：https://playgameoflife.com/

![ticker.gif](https://upload-images.jianshu.io/upload_images/5252065-441f5c67415d3662.gif?imageMogr2/auto-orient/strip)

我采取的是最原始的规则：（一个点周围的8个点为8邻域）
* 1． 如果一个细胞周围有3个细胞为生（一个细胞周围共有8个细胞），则该细胞为生（即该细胞若原先为死，则转为生，若原先为生，则保持不变） 。
* 2． 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
* 3． 在其它情况下，该细胞为死（即该细胞若原先为生，则转为死，若原先为死，则保持不变）

利用这个规则让其自动演化就可以了：

### 2. 常见种子。
* 滑翔机。
可以向右下方滑动：

![](https://upload-images.jianshu.io/upload_images/5252065-17550d4cfeb84032.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 滑翔者。

![](https://upload-images.jianshu.io/upload_images/5252065-69a8d3006aa5d429.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

每四个回合会向右走一格。
* 脉冲星。
周期为3，不断闪烁。

![](https://upload-images.jianshu.io/upload_images/5252065-7696b25350a34686.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

结果：

![](https://upload-images.jianshu.io/upload_images/5252065-e1ff1798527431b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 滑翔者枪

![](https://upload-images.jianshu.io/upload_images/5252065-fbd00b79b0cf8d6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这个玩意可以不断的发射滑翔者。
再有其他的复杂的图像就只能自己去发掘了，还有一种方法就是随机初始化种子：
* 随机初始化种子。
就是随机让一部分的细胞存活，然后执行游戏规则，有可能会产生出比较稳定的状态，当然这个也是有研究的，结果就发现随机激活37.5%的种子的时候产生比较稳定图案的概率比较大。这个我在代码里也给了，可以设置。

### 3. 实现过程。
其实主要的代码比较简单，就是空域滤波的锚点如何根据周围的点来决定自己的状态：
* **游戏规则实现：**
```cpp
void lifeGame(Mat &init_image, int loop_num, bool writeImg,int ms)
{
	int rows = init_image.rows;
	int cols = init_image.cols;
	namedWindow("source", WINDOW_NORMAL);
	imshow("source", init_image);

	//k是迭代次数
	namedWindow("LIFE_GAME", 2);
	for (int k = 0; k < loop_num; k++)
	{
		cout << k << endl;
		Mat tmp = Mat::zeros(rows, cols, CV_8UC1);
		uchar x1, x2, x3,
			  x4,       x6,
			  x7, x8, x9;

		for (int i = 1; i < rows - 1; i++)
		{
			int count = 0;
			for (int j = 1; j < cols - 1; j++)
			{
				x1 = init_image.at<uchar>(i - 1, j - 1);
				x2 = init_image.at<uchar>(i - 1, j);
				x3 = init_image.at<uchar>(i - 1, j + 1);
				x4 = init_image.at<uchar>(i, j - 1);
				x6 = init_image.at<uchar>(i, j + 1);
				x7 = init_image.at<uchar>(i + 1, j - 1);
				x8 = init_image.at<uchar>(i + 1, j);
				x9 = init_image.at<uchar>(i + 1, j + 1);
				count = x1 + x2 + x3
					+ x4 + x6
					+ x7 + x8 + x9;
				//生命游戏的核心代码，三个if代表三个规则
				if (count == 255 * 3)
					tmp.at<uchar>(i, j) = 255;
				else if (count == 255 * 2)
					tmp.at<uchar>(i, j) = init_image.at<uchar>(i, j);
				else
					tmp.at<uchar>(i, j) = 0;         //这一句也是可以不要的，因为本身就是0
			}
		}
		tmp.copyTo(init_image);
		tmp.release();
		imshow("LIFE_GAME", init_image);
		if (writeImg)
			imwrite("res//" + to_string(k) + ".jpg", init_image);
		waitKey(ms);
	}
```
这样的话生成的画布是固定大小的，自己设置，有的平移类的种子出了边界就不会再回来了，在此基础上又想了一种办法：**把左右两边相连，上下相连**，这样就可以变向的实现画布放大（当然这不是理想的解法），另外一点画布也是可以设置大一点的，因为算法简单，用C++写出来效率还是很高的，2000*2000的图像还是可以实现勉强实时的。

* **配置文件读取：**
配置文件以txt文件形式存储，然后读入，只存储`活着`点的坐标，每一行的第一个数表示该行的行坐标，后面是列坐标，比如：
```
1 5
2 4 5 6
3 3 4 5 6 7
4 2 3 4 5 6 7 8
5 1 2 3 4 5 6 7 8 9
6 2 3 4 5 6 7 8
7 3 4 5 6 7
8 4 5 6
9 5
```

对应的图片张这样：

![](https://upload-images.jianshu.io/upload_images/5252065-b4e77ef7a0429277.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

把所有的点移动到左上角来定位坐标，坐标初始位置从1开始。

解析的方法也比较简单，获取每一行的数字使用`getline`函数，每一行获取数字的时候使用`istringstream`,具体：
```
void getInt(string &s, vector<Point2d> &res,int &cmax,int &rmax)  //从一行中解析出整数,并记录最大行数
{
	istringstream iss(s);
	
	int num;
	int cnt=0;       //读第一个数的标志
	int line;        //行数
	int colmax = 0;
	while (iss >> num)
	{
		if (cnt == 0)     //每一行的第一个数是行号
		{
			line = num;
			rmax = line;       //记录行号
			cnt++;
		}
		else             //重构坐标存入res中
		{
			res.push_back(Point2d(line, num));
			if (num > colmax)
			{
				colmax = num;
			}
		}
	}
	cmax = colmax;
}
//从txt中提取坐标点，并记录最大的行和列
void getPos(string &file, vector<Point2d> &CfgMat, int &rmax,int &cmax)
{
	ifstream cfg(file);
	string s;
	int _cmax = 0;
	int _rmax = 0;
	while (getline(cfg, s))
	{
		cout << s << endl;
		getInt(s, CfgMat,_cmax,_rmax);
		if (_cmax > cmax)
			cmax = _cmax;
		if (_rmax > rmax)
			rmax = _rmax;
	} 
}
```
重构棋盘矩阵的时候会把棋牌扩大（根据记录的种子的最大行和列自定义行和列的放大系数）。
其他的就没什么了，在cfg文件里我存了几个比较经典的初始种子，可以读取来显示。

### 4. 效果展示。
* `X`型种子。

![](https://upload-images.jianshu.io/upload_images/5252065-aa417ffa86dfc6d9.gif?imageMogr2/auto-orient/strip)

* 滑翔者枪
这里有点小，程序里是可以调整显示大小的：

![test.gif](https://upload-images.jianshu.io/upload_images/5252065-44035f017939ab12.gif?imageMogr2/auto-orient/strip)
