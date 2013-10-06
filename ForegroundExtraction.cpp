#include "stdafx.h"

int main(){
	VideoCapture cap(0);
	Mat frame, foreground, background, diff, first_img;
	namedWindow("originalImage");
	namedWindow("diff");

	int flag = 0;
	int count = 0;

	vector<vector<Point>> contours;
	vector<Mat> channels;

	while (true)
	{
		count ++;
		cap >> frame;

		

		if(count > 20){
			if(flag == 0){
				first_img = frame.clone();
				flag = 1;
			}

			Mat mask = Mat::zeros(frame.rows, frame.cols, CV_8UC1);

			absdiff(frame, first_img, diff);

			split(diff, channels);
			for(int i = 0; i < channels.size(); i++){
				Mat tmp;
				threshold(channels[i], tmp, 45, 255, CV_THRESH_BINARY);
				mask |= tmp;
			}
			Mat img(frame.rows, frame.cols, CV_8UC3);
			img.setTo(Scalar(0, 0, 0));
			Mat kernel, e;
			getStructuringElement(MORPH_RECT, Size(10, 10));
			morphologyEx(mask, e, MORPH_CLOSE, kernel, Point(-1, -1));
			findContours(e, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			drawContours(e, contours, -1, Scalar(255), CV_FILLED);
			frame.copyTo(img, e);

			imshow("originalImage", frame);
			imshow("diff", img);
		}
		if(waitKey(10)>0)
			break;
	}
}