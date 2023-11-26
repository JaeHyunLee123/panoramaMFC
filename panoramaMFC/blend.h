#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

namespace blend {
	struct RGB {
		int red;
		int green;
		int blue;
	};

	struct HSI
	{
		int hue; // 0 ~ 180/PI ���� ��
		int saturation; //0 ~ 100
		int intensity; // 0 ~ 255
	};

	//mergedImage: blending ��� �̹���
	//center: center �� ����
	//blendingArea: blending �ϴ� ����
	//errorRange: ��������
	Mat blendImage(Mat image, vector<int> center, int blendingArea, int errorRange);

	vector<RGB> blendRow(vector<RGB> input, int center, int blendingArea, int errorRange);

	RGB getRGB(Mat image, int col, int row);
	void putRGB(Mat& dest, RGB rgb, int col, int row);

	RGB hsi2rgb(HSI input);
	HSI rgb2hsi(RGB input);
}