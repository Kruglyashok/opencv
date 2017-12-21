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
	int areaNum1, areaNum2;
	areaNum1 = areaInd(vec, x1, y1);
	areaNum2 = areaInd(vec, x2, y2);
	for (int i = vec[areaNum2].size() - 1; i >-1 ; i-- ) {
		vec[areaNum1].push_back(vec[areaNum2][i]);
		vec[areaNum2].pop_back();
	}
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
void changecolor(int &color) {
	if (color == 0) color = 255;
	else color = 0;
}
void mergeArea(Mat &source, Mat &merged) {
	vector<vector<pair<int, int>>> vec(source.cols*source.rows); // vector of areas, [] - num of area, contains pixels corresponding to the area
	//fill vector of areas with single pixels
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows; j++) {
			vec[i + j*source.cols].push_back(make_pair(i, j));
		}
	}

	int Tavg = 0; // average intense
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows; j++) {
			Tavg += source.at<uchar>(i, j);
		}
	}
	//Tavg /= (source.cols*source.rows);
	Tavg = 20;
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

	int color = 0;
	for (int i = 0; i < source.cols; i++) {
		for (int j = 0; j < source.rows; j++) {
			if (vec[i + j*source.cols].size() != 0) {
				for (int g = 0; g < vec[i + j*source.cols].size(); g++) {
					merged.at<uchar>(vec[i + j*source.cols][g].first, vec[i + j*source.cols][g].second) = color;
				}
				changecolor(color);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	Mat source, merged;
	source = imread("test128.png", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("important", CV_WINDOW_NORMAL);
	source.copyTo(merged);

	imshow("source", source);
	mergeArea(source, merged);
	waitKey(0);
	imshow("source", merged);
	imwrite("result20.png", merged);
	waitKey(0);
	return 0;

}
