#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;
//returns number of area, corresponding to pixel 
int areaInd(vector<vector<pair<int, int>>> vec, int x, int y) {
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < vec[i].size(); j++) {
			if (vec[i][j] == make_pair(x, y)) return i; //i - num of area, j - num of pair
		}
	}
	return -1;
}
//to fill, merges 2 areas, we must put pixel in correspondin elem of vec
void merge(int x1, int y1, int x2, int y2, vector<vector<pair<int, int>>> &vec) {
	int areaNum;
	areaNum = areaInd(vec, x1, y1);

}

//merge all of the right neighbours
void mergeRight(Mat &source, int x, int y, int T, vector<vector<pair<int, int>>> &vec) {
	if (abs(source.at<uchar>(x, y) - source.at<uchar>(x + 1, y)) < T) {
		merge(x, y, x + 1, y, vec);
		if (x < source.cols - 2) mergeRight(source, x + 1, y, T, vec);
	}
}
//merge all of the bot  neighbours
void mergeBot(Mat &source, int x, int y, int T, vector<vector<pair<int, int>>> &vec) {
	if (abs(source.at<uchar>(x, y) - source.at<uchar>(x, y + 1)) < T) {
		merge(x, y, x, y + 1, vec);
		if (y < source.rows - 2) mergeBot(source, x, y + 1, T, vec);
	}
}
void mergeArea(Mat &source, Mat &merged) {
	vector<vector<pair<int, int>>> vec(source.cols*source.rows); // vector of areas, [] - num of area, contains pixels corresponding to the area
	//fill vector of areas with single pixels
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows; j++) {
			vec[i + j*source.cols].push_back(make_pair(i, j));
		}
	}
	for (int i = 0; i < 10; i++) {
		cout << "(" << vec[i][0].first << "," << vec[i][0].second << ")" << endl;
	}
	for (int i = 65526; i < 65536; i++) {
		cout << "(" << vec[i][0].first << "," << vec[i][0].second << ")" << endl;
	}

	int Tavg = 0; // average intense
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows; j++) {
			Tavg += source.at<uchar>(i, j);
		}
	}
	Tavg /= (source.cols*source.rows);
	cout << "Tavg = " << Tavg << endl;
	//merging 
	for (int i = 0; i < source.rows; i++) {
		for (int j = 0; j < source.cols - 1; j++) {
			mergeRight(source, j, i, Tavg, vec);
		}
	}
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows - 1; j++) {
			mergeBot(source, i, j, Tavg, vec);
		}
	}
}

int main(int argc, char *argv[])
{
	Mat source, merged;
	source = imread("test.png", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("important", CV_WINDOW_NORMAL);
	imshow("source", source);
	mergeArea(source, merged);
	waitKey(0);
	imshow("source", source);
	waitKey(0);
	return 0;

}
