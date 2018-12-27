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

//��string�н������֣�����¼��������������һ����֮�������
void getInt(string &s, vector<Point2d> &res,int &cmax,int &rmax)  //��һ���н���������,����¼�������
{
	istringstream iss(s);
	
	int num;
	int cnt=0;       //����һ�����ı�־
	int line;        //����
	int colmax = 0;
	while (iss >> num)
	{
		if (cnt == 0)     //ÿһ�еĵ�һ�������к�
		{
			line = num;
			rmax = line;       //��¼�к�
			cnt++;
		}
		else             //�ع��������res��
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

//��txt����ȡ����㣬����¼�����к���
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

	}      //���ʵ���϶��¼��һ�У��������
}

/*
init_image   ����������ʾ��ͼ��
CfgMat CfgMat   ���õľ������꣬��Щλ���ǻ�ģ�255��
rmax   ����к�
cmax   ����к�
factor_r   �зŴ���
factor_c   �зŴ���

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
init_image    ͼƬ
loop_num      ��������
writeImg      �Ƿ񱣴�ͼƬ
ms            ��ʾ��ʱ(ms)
*/
void lifeGameEdge(Mat &init_image, int loop_num, bool writeImg,int ms)
{
	int rows = init_image.rows;
	int cols = init_image.cols;
	namedWindow("source", WINDOW_NORMAL);
	imshow("source", init_image);




	//k�ǵ�������
	namedWindow("LIFE_GAME", WINDOW_NORMAL);
	for (int k = 0; k < loop_num; k++)
	{
		cout << k << endl;
		Mat tmp = Mat::zeros(rows, cols, CV_8UC1);
		uchar x1, x2, x3,
			x4, x6,
			x7, x8, x9;
		//�Ÿ�λ�õ���
		for (int i = 0; i < rows; i++)
		{
			int count = 0;
			for (int j = 0; j < cols; j++)
			{
				//�����Ǵ���ͬ�ĵ㣬�߽�ĵ㰴��ѭ����λ���������Ļ��������Ϊ���������޴��
				//���ϵĵ�
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
				//���ϵĵ�
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
				//���µĵ�
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
				//���µĵ�
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
				//��һ��
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
				//���һ��
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
				//��һ��
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
				//���һ��
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
				//�����ĵ�
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
				//������Ϸ�ĺ��Ĵ��룬����if������������
				if (count == 255 * 3)
					tmp.at<uchar>(i, j) = 255;
				else if (count == 255 * 2)
					tmp.at<uchar>(i, j) = init_image.at<uchar>(i, j);
				else
					tmp.at<uchar>(i, j) = 0;         //��һ��Ҳ�ǿ��Բ�Ҫ�ģ���Ϊ�������0
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

	//k�ǵ�������
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
				//������Ϸ�ĺ��Ĵ��룬����if������������
				if (count == 255 * 3)
					tmp.at<uchar>(i, j) = 255;
				else if (count == 255 * 2)
					tmp.at<uchar>(i, j) = init_image.at<uchar>(i, j);
				else
					tmp.at<uchar>(i, j) = 0;         //��һ��Ҳ�ǿ��Բ�Ҫ�ģ���Ϊ�������0
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
	string cfg("cfg//gun1.txt");     //��ȡ�����ļ�
	vector<Point2d> CfgMat;                  //���þ���洢
	int cmax=0;
	int rmax=0;
	getPos(cfg, CfgMat, rmax, cmax);
	cout << rmax <<" "<<cmax << endl;
	Mat init_image;
	buildMap(init_image, CfgMat, rmax, cmax, 5, 2);
	lifeGame(init_image,1000,true,100);
	return 0;
}