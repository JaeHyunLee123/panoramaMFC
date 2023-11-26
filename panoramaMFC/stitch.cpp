#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <array>
#include "stitch.h"
#include "blend.h"

using namespace std;
using namespace cv;

namespace stitch {
	Mat stitch_two_image(Mat original_image, Mat object_image) {
		//��ü ��Ī �� �� �̹����� ũ�⸦ �Ȱ��� �ؼ� �߸��� ��Ī�� ���� ���̷��� ��
		Mat originalCutImage(object_image.size(), CV_8UC3);
		original_image(Rect(original_image.cols - originalCutImage.cols, 0, originalCutImage.cols, originalCutImage.rows)).
			copyTo(originalCutImage(Rect(0, 0, originalCutImage.cols, originalCutImage.rows)));

		//�̹����� ������ ��ȯ�� �� 100���� ���ڶ� �� �ֱ⿡ ���� ���� �����ϱ� ���� ����� ����
		int SHIFT = original_image.rows / 3;

		// ORB Ư¡�� ����� �ʱ�ȭ
		int minHessian = 1000;
		cv::Ptr<cv::ORB> orb = cv::ORB::create(minHessian);

		// Ư¡�� �� ����� ����
		std::vector<cv::KeyPoint> keypoints1, keypoints2;
		cv::Mat descriptors1, descriptors2;
		orb->detectAndCompute(originalCutImage, noArray(), keypoints1, descriptors1);
		orb->detectAndCompute(object_image, noArray(), keypoints2, descriptors2);


		//-- Step 2: Matching descriptor vectors with a brute force matcher
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
		std::vector< std::vector<DMatch> > knn_matches;
		matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);

		//���� ��Ī�� ����
		//1.Filter matches using the Lowe's ratio test
		const float ratio_thresh = 0.8f;
		std::vector<DMatch> first_good_matches;
		for (size_t i = 0; i < knn_matches.size(); i++)
		{
			if (knn_matches[i].size() == 2) {
				if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
				{
					first_good_matches.push_back(knn_matches[i][0]);
				}
			}
		}


		//2. gradient�� ������
		// ��ü ��ġ ã��
		std::vector<cv::Point2f> src_pts, dst_pts;
		int gradientValue[11] = { 0, }; // -0.5~ 0.5�� 0.1��
		std::vector<std::vector<int>> gradientIndex(11); //�ش��ϴ� �ε��� ����
		float tmp = 0.0f;
		int tmpIndex = 0;

		for (int i = 0; i < first_good_matches.size(); i++) {
			src_pts.push_back(keypoints1[first_good_matches[i].queryIdx].pt);
			dst_pts.push_back(keypoints2[first_good_matches[i].trainIdx].pt);

			//gradient ��� �� ����
			tmp = ((float)(dst_pts[i].y - src_pts[i].y)) / ((float)(dst_pts[i].x - src_pts[i].x));
			tmpIndex = (int)(tmp * 10 + 5);
			if (tmpIndex >= 0 && tmpIndex < 11) {
				gradientValue[tmpIndex] = gradientValue[tmpIndex] + 1;
				gradientIndex[tmpIndex].push_back(i);
			}

		}

		//���� ū ���� ����� index�� �츮�� �������� ������.
		int maxIndex = 0;
		for (int i = 1; i < 11; i++) {
			if (gradientValue[maxIndex] < gradientValue[i])
				maxIndex = i;
		}

		std::vector<DMatch> second_good_matches;
		for (int i = 0; i < gradientValue[maxIndex]; i++) {
			second_good_matches.push_back(first_good_matches[gradientIndex[maxIndex][i]]);
		}

		//3. �ִ� �Ÿ��� ���� ���� ����(������ �Ÿ�)
		std::vector<cv::DMatch> third_good_matches;
		double distanceMean = 0;
		double distanceTmp = 0;
		Point2f oriPoint, objPoint;
		for (int i = 0; i < second_good_matches.size(); i++) {
			oriPoint = keypoints1[second_good_matches[i].queryIdx].pt;
			objPoint = keypoints2[second_good_matches[i].trainIdx].pt;

			//�Ÿ� ���� ���
			distanceTmp = sqrt(pow(oriPoint.x - objPoint.x - originalCutImage.cols, 2) + pow(oriPoint.y - objPoint.y, 2));
			distanceMean += distanceTmp;
		}

		//����� +-20�ۼ�Ʈ ������ ����� ����
		distanceMean /= second_good_matches.size();

		for (int i = 0; i < second_good_matches.size(); i++) {
			oriPoint = keypoints1[second_good_matches[i].queryIdx].pt;
			objPoint = keypoints2[second_good_matches[i].trainIdx].pt;

			//�Ÿ� ���� ���
			distanceTmp = sqrt(pow(oriPoint.x - objPoint.x - originalCutImage.cols, 2) + pow(oriPoint.y - objPoint.y, 2));
			if (distanceTmp < distanceMean * 1.1 && distanceTmp > distanceMean * 0.9)
				third_good_matches.push_back(second_good_matches[i]);
		}

		// ���� ��Ī���� ��ü ��ġ ã��
		//��Ī ����� �ʹ� ������ ȣ��׷��Ǹ� ã�� �� ���⿡ �̸� ���� ���� �ڵ� �߰�
		std::vector<cv::Point2f> newSrc_pts, newDst_pts;
		if (third_good_matches.size() > 4) {
			for (int i = 0; i < third_good_matches.size(); i++) {
				newSrc_pts.push_back(keypoints1[third_good_matches[i].queryIdx].pt);
				newDst_pts.push_back(keypoints2[third_good_matches[i].trainIdx].pt);
			}
		}
		else {
			for (int i = 0; i < second_good_matches.size(); i++) {
				newSrc_pts.push_back(keypoints1[second_good_matches[i].queryIdx].pt);
				newDst_pts.push_back(keypoints2[second_good_matches[i].trainIdx].pt);
			}
		}



		// ��ȯ ��� ��� -> CV_64F
		Mat H = findHomography(newDst_pts, newSrc_pts, cv::RANSAC);

		// ��ȯ ����� �����ؼ� 0,100�� translate�� �����Ѵ�.
		// 100�̶�� ��ġ�� ���� ������ �� �ִ�. 100�� �ص� �̹����� �߸��� ��찡 ������ �� �ֱ� ����
		double set[9] = { 1,0, 0, 0, 1, SHIFT, 0, 0, 1 };
		Mat translateShift = Mat(H.size(), CV_64F, set);
		Mat translateH = translateShift * H;


		// ��ȯ����� ������ object_on_original�� ���� 
		Mat object_on_original;
		cv::warpPerspective(object_image, object_on_original, translateH, Size(object_image.cols * 2, object_image.rows * 2), INTER_CUBIC);
		//imshow("object_on_original", object_on_original);

		// ������ ���� object_on_original�� ��� �κ��� ��ǥ�� ���� �ʿ䰡 �ִ�.
		// �ܰ����� �����ִ� �Լ��� �̿��ؼ� �̸� ����ϰڴ�.
		Mat contoursMask;
		cvtColor(object_on_original, contoursMask, COLOR_BGR2GRAY); // RGB -> GRAY (ä�� ���� ����);


		//�̹����� ��� ã��
		//contours�� �ܰ����� ��ǥ ����
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(contoursMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


		//contours���� �� �ܰ����� ���� �������� ã���ϴ�.
		//�������� �������� �������� �ð�������� ����ȴ�.
		vector<vector<Point>> approxContours(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			double epsilon = 0.02 * arcLength(contours[i], true);
			approxPolyDP(contours[i], approxContours[i], epsilon, true);
		}


		// ��ǥ�� x���� ū ������� ���� �� �����Ѵ�.
		// �����ʿ��� col�� ū ����Ʈ ������� ����
		//�̶� contours�� �ε����� 1�� �ƴ� ��쵵 ���� �� �����Ƿ� ũ�Ⱑ ���� ū index�� �������� �ش� ������ �����Ѵ�.
		Point rightMaxPoint, rightMinPoint, leftMaxPoint, leftMinPoint;
		int contoursIndex = 0;
		for (contoursIndex = 0; contoursIndex < approxContours.size(); contoursIndex++) {
			if (approxContours[contoursIndex].size() == 4) {
				sort(approxContours[contoursIndex].begin(), approxContours[contoursIndex].end(), compareX);
				leftMinPoint = approxContours[contoursIndex][0];
				leftMaxPoint = approxContours[contoursIndex][1];
				rightMinPoint = approxContours[contoursIndex][2];
				rightMaxPoint = approxContours[contoursIndex][3];
			}
		}
		contoursIndex--;

		//���� ��ǥ �� y���� ũ�⸦ ���ϰ� ����� �����Ѵ�.
		bool isLeftYMax, isRightYMax;

		if (leftMinPoint.y < leftMaxPoint.y)
			isLeftYMax = true;
		else
			isLeftYMax = false;

		if (rightMinPoint.y < rightMaxPoint.y)
			isRightYMax = true;
		else
			isRightYMax = false;


		//�̹����� ���̰� ���͸� ���̴� ������� ��������
		Mat preCombineImg = Mat::zeros(object_on_original.size(), CV_8UC3);

		//���� originalCutImage�� ���δ�.
		originalCutImage(Rect(0, 0, originalCutImage.cols, originalCutImage.rows)).
			copyTo(preCombineImg(Rect(0, SHIFT, originalCutImage.cols, originalCutImage.rows)));

		//object_and_origianl�� ���δ�.
		for (int i = 0; i < object_on_original.rows; i++) {
			for (int j = 0; j < object_on_original.cols; j++) {
				//�ش� ���� ��輱 �ȿ� �ִ� ��� �� ����
				if (pointPolygonTest(contours[contoursIndex], Point(j, i), false) > 0)
					preCombineImg.at<cv::Vec3b>(i, j) = object_on_original.at<cv::Vec3b>(i, j);
			}
		}
		//imshow("preCombineImg", preCombineImg);

		//�̹����� �ڸ���
		//���� ������ �ڸ���
		int top, right;
		if (isRightYMax) {
			top = max(SHIFT, rightMinPoint.y);
		}
		else {
			top = max(SHIFT, rightMaxPoint.y);
		}
		right = min(rightMaxPoint.x, rightMinPoint.x);

		//blending�� ���� ��輱 ���� ����
		vector<int> center;
		for (int i = top; i < top + original_image.rows; i++) {
			for (int j = 0; j < object_on_original.cols; j++) {
				//�ش� ���� ��輱 ���� �ִ� ��� �� �ֱ�
				if (pointPolygonTest(contours[contoursIndex], Point(j, i), false) == 0)
				{
					//��� ���� ���� ������ �����̱⿡ original_image�� ���θ�ŭ�� ���̸� �������� �Ѵ�.
					center.push_back(j + original_image.cols - originalCutImage.cols);
					break;
				}
			}
		}


		// ����� ������ mat ���� �� ������ �ű��
		//�߸� original�� object_on_original�� ���ľ� �Ѵ�.
		//�б⹮�� ���� ������ original_image.cols - originalCutImage.cols�� 0�̸� ������ �߻��ϱ� �����̴�.
		Mat result = Mat::zeros(original_image.rows, original_image.cols - originalCutImage.cols + right, CV_8UC3);

		if (original_image.cols - originalCutImage.cols == 0) {
			preCombineImg(Rect(0, top, right, result.rows)).
				copyTo(result(Rect(0, 0, right, result.rows)));
		}
		else {
			original_image(Rect(0, 0, original_image.cols - originalCutImage.cols, result.rows)).
				copyTo(result(Rect(0, 0, original_image.cols - originalCutImage.cols, result.rows)));
			preCombineImg(Rect(0, top, right, result.rows)).
				copyTo(result(Rect(original_image.cols - originalCutImage.cols, 0, right, result.rows)));
		}

		//blending ����
		result = blend::blendImage(result, center, result.rows / 2, 2);
		return result;
	}

	bool compareX(const Point& p1, const Point& p2) {
		return p1.x < p2.x;
	}
}