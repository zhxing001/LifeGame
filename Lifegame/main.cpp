#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\core\core.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>


using namespace std;
using namespace cv;

//从string中解析数字，并记录最大的列数（除第一个数之外的数）
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

	}      //最后实际上多记录了一列，这里减掉
}

/*
init_image   最终用来显示的图像
CfgMat CfgMat   配置的矩阵坐标，这些位置是活的（255）
rmax   最大行号
cmax   最大列号
factor_r   行放大倍数
factor_c   列放大倍数

*/

void buildMap(Mat &init_image,vector<Point2d> &CfgMat, int &rmax, int &cmax,int factor_r,int factor_c)
{
	int rows = rmax*factor_r;
	int cols = cmax*factor_c;
	init_image = Mat::zeros(rows, cols, CV_8UC1);
	int r_drift = 0.5*rows - 0.5*rmax;
	int c_drift = 0.5*cols - 0.5*cmax;
	for (auto pos : CfgMat)
	{
		init_image.at<uchar>(pos.x + r_drift, pos.y + c_drift) = 255;
	}
	
}

/*
init_image    图片
loop_num      迭代次数
writeImg      是否保存图片
ms            显示延时(ms)
*/
void lifeGameEdge(Mat &init_image, int loop_num, bool writeImg,int ms)
{
	int rows = init_image.rows;
	int cols = init_image.cols;
	namedWindow("source", WINDOW_NORMAL);
	imshow("source", init_image);




	//k是迭代次数
	namedWindow("LIFE_GAME", WINDOW_NORMAL);
	for (int k = 0; k < loop_num; k++)
	{
		cout << k << endl;
		Mat tmp = Mat::zeros(rows, cols, CV_8UC1);
		uchar x1, x2, x3,
			x4, x6,
			x7, x8, x9;
		//九个位置的数
		for (int i = 0; i < rows; i++)
		{
			int count = 0;
			for (int j = 0; j < cols; j++)
			{
				//这里是处理不同的点，边界的点按照循环移位处理，这样的话可以理解为界面是无限大的
				//左上的点
				if (i == 0 && j == 0)
				{
					x1 = init_image.at<uchar>(rows - 1, cols - 1);
					x2 = init_image.at<uchar>(rows - 1, 0);
					x3 = init_image.at<uchar>(rows - 1, 1);
					x4 = init_image.at<uchar>(0, cols - 1);
					x6 = init_image.at<uchar>(0, 1);
					x7 = init_image.at<uchar>(1, cols - 1);
					x8 = init_image.at<uchar>(1, 0);
					x9 = init_image.at<uchar>(1, 1);
				}
				//右上的点
				else if (i == 0 && j == cols - 1)
				{
					x1 = init_image.at<uchar>(rows - 1, cols - 2);
					x2 = init_image.at<uchar>(rows - 1, cols - 1);
					x3 = init_image.at<uchar>(rows - 1, 0);
					x4 = init_image.at<uchar>(0, cols - 2);
					x6 = init_image.at<uchar>(0, 0);
					x7 = init_image.at<uchar>(1, cols - 2);
					x8 = init_image.at<uchar>(1, cols - 1);
					x9 = init_image.at<uchar>(1, 0);

				}
				//左下的点
				else if (i == rows - 1 && j == 0)
				{
					x1 = init_image.at<uchar>(rows - 2, cols - 1);
					x2 = init_image.at<uchar>(rows - 2, 0);
					x3 = init_image.at<uchar>(rows - 2, 1);
					x4 = init_image.at<uchar>(rows - 1, cols - 1);
					x6 = init_image.at<uchar>(rows - 1, 1);
					x7 = init_image.at<uchar>(0, cols - 1);
					x8 = init_image.at<uchar>(0, 0);
					x9 = init_image.at<uchar>(0, 1);
				}
				//右下的点
				else if (i == rows - 1 && j == cols - 1)
				{
					x1 = init_image.at<uchar>(rows - 2, cols - 2);
					x2 = init_image.at<uchar>(rows - 2, cols - 1);
					x3 = init_image.at<uchar>(rows - 2, 0);
					x4 = init_image.at<uchar>(rows - 1, cols - 2);
					x6 = init_image.at<uchar>(rows - 1, 0);
					x7 = init_image.at<uchar>(0, cols - 2);
					x8 = init_image.at<uchar>(0, cols - 1);
					x9 = init_image.at<uchar>(0, 0);
				}
				//第一行
				else if (i == 0)
				{
					x1 = init_image.at<uchar>(rows - 1, j - 1);
					x2 = init_image.at<uchar>(rows - 1, j);
					x3 = init_image.at<uchar>(rows - 1, j + 1);
					x4 = init_image.at<uchar>(i, j - 1);
					x6 = init_image.at<uchar>(i, j + 1);
					x7 = init_image.at<uchar>(i + 1, j - 1);
					x8 = init_image.at<uchar>(i + 1, j);
					x9 = init_image.at<uchar>(i + 1, j + 1);
				}
				//最后一行
				else if (i == rows - 1)
				{
					x1 = init_image.at<uchar>(i - 1, j - 1);
					x2 = init_image.at<uchar>(i - 1, j);
					x3 = init_image.at<uchar>(i - 1, j + 1);
					x4 = init_image.at<uchar>(i, j - 1);
					x6 = init_image.at<uchar>(i, j + 1);
					x7 = init_image.at<uchar>(0, j - 1);
					x8 = init_image.at<uchar>(0, j);
					x9 = init_image.at<uchar>(0, j + 1);
				}
				//第一列
				else if (j == 0)
				{
					x1 = init_image.at<uchar>(i - 1, cols - 1);
					x2 = init_image.at<uchar>(i - 1, 0);
					x3 = init_image.at<uchar>(i - 1, 1);
					x4 = init_image.at<uchar>(i, cols - 1);
					x6 = init_image.at<uchar>(i, 1);
					x7 = init_image.at<uchar>(i + 1, cols - 1);
					x8 = init_image.at<uchar>(i + 1, 0);
					x9 = init_image.at<uchar>(i + 1, 1);
				}
				//最后一列
				else if (j == cols - 1)
				{
					x1 = init_image.at<uchar>(i - 1, j - 1);
					x2 = init_image.at<uchar>(i - 1, j);
					x3 = init_image.at<uchar>(i - 1, 0);
					x4 = init_image.at<uchar>(i, j - 1);
					x6 = init_image.at<uchar>(i, 0);
					x7 = init_image.at<uchar>(i + 1, j - 1);
					x8 = init_image.at<uchar>(i + 1, j);
					x9 = init_image.at<uchar>(i + 1, 0);
				}
				//其他的点
				else
				{
					x1 = init_image.at<uchar>(i - 1, j - 1);
					x2 = init_image.at<uchar>(i - 1, j);
					x3 = init_image.at<uchar>(i - 1, j + 1);
					x4 = init_image.at<uchar>(i, j - 1);
					x6 = init_image.at<uchar>(i, j + 1);
					x7 = init_image.at<uchar>(i + 1, j - 1);
					x8 = init_image.at<uchar>(i + 1, j);
					x9 = init_image.at<uchar>(i + 1, j + 1);

				}


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
}

void lifeGame(Mat &init_image, int loop_num, bool writeImg,int ms)
{
	int rows = init_image.rows;
	int cols = init_image.cols;
	namedWindow("source", WINDOW_NORMAL);
	imshow("source", init_image);

	//k是迭代次数
	namedWindow("LIFE_GAME", 1);
	for (int k = 0; k < 100000; k++)
	{
		cout << k << endl;
		Mat tmp = Mat::zeros(rows, cols, CV_8UC1);
		uchar x1, x2, x3,
			x4, x6,
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
}

int main()
{
	string cfg("cfg//gun1.txt");     //读取配置文件
	vector<Point2d> CfgMat;                  //配置矩阵存储
	int cmax=0;
	int rmax=0;
	getPos(cfg, CfgMat, rmax, cmax);
	cout << rmax <<" "<<cmax << endl;
	Mat init_image;
	buildMap(init_image, CfgMat, rmax, cmax, 5, 2);
	lifeGame(init_image,1000,true,100);
	return 0;
}