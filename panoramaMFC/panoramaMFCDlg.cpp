
// panoramaMFCDlg.cpp: 구현 파일
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <afx.h>
#include "pch.h"
#include "framework.h"
#include "panoramaMFC.h"
#include "panoramaMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpanoramaMFCDlg 대화 상자



CpanoramaMFCDlg::CpanoramaMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PANORAMAMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpanoramaMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEFT_IMAGE, LeftControl);
	DDX_Control(pDX, IDC_STATIC_CENTER_IMAGE, CenterControl);
	DDX_Control(pDX, IDC_STATIC_RIGHT_IMAGE, RightControl);
}

BEGIN_MESSAGE_MAP(CpanoramaMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STITCH, &CpanoramaMFCDlg::OnBnClickedButtonStitch)
	ON_COMMAND(ID_FILEOPEN_LEFT_IMAGE, &CpanoramaMFCDlg::OnFileopenLeftImage)
	ON_COMMAND(ID_FILEOPEN_CENTER_IMAGE, &CpanoramaMFCDlg::OnFileopenCenterImage)
	ON_COMMAND(ID_FILEOPEN_RIGHT_IMAGE, &CpanoramaMFCDlg::OnFileopenRightImage)
END_MESSAGE_MAP()


// CpanoramaMFCDlg 메시지 처리기

BOOL CpanoramaMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CpanoramaMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CpanoramaMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CpanoramaMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CpanoramaMFCDlg::OnBnClickedButtonStitch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (LeftImage.cols == 0 || CenterImage.cols == 0 || RightImage.cols == 0) {
		MessageBox((LPCTSTR)"Please open three images");
		return;
	}

	resize(LeftImage, LeftImage, Size(0, 0), 0.5, 0.5, INTER_LINEAR);
	resize(CenterImage, CenterImage, Size(0, 0), 0.5, 0.5, INTER_LINEAR);
	resize(RightImage, RightImage, Size(0, 0), 0.5, 0.5, INTER_LINEAR);

	int newCol = 0;
	double fixedRow = LeftImage.rows;

	Mat result = stitch_two_image(CenterImage, RightImage);
	newCol = (double)result.cols / (double)result.rows * fixedRow;
	resize(result, result, Size(newCol, fixedRow));

	flip(result, result, 1);
	flip(LeftImage, LeftImage, 1);
	result = stitch_two_image(result, LeftImage);
	flip(result, result, 1);
	newCol = (double)result.cols / (double)result.rows * fixedRow;
	resize(result, result, Size(newCol, fixedRow));

	PanoramaImage = result;

	DisplayImage(PanoramaImage, PANORAMA);

	imshow("result", result);
	waitKey(0);
}

void CpanoramaMFCDlg::OnFileopenLeftImage()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK) {
		//파일 확장자 확인
		CString fileExtention = dlg.GetFileExt();
		if (!(fileExtention == "png" || fileExtention == "PNG" || fileExtention == "jpg" || fileExtention == "JPG")) {
			MessageBox((LPCTSTR)"Please open png or jpg file");
		}

		CString cstr = dlg.GetPathName();
		const char* pathname = (LPCTSTR)cstr;
		LeftImage = cv::imread(pathname);

		DisplayImage(LeftImage, LEFT);
	}
}


void CpanoramaMFCDlg::OnFileopenCenterImage()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK) {
		//파일 확장자 확인
		CString fileExtention = dlg.GetFileExt();
		if (!(fileExtention == "png" || fileExtention == "PNG" || fileExtention == "jpg" || fileExtention == "JPG")) {
			MessageBox((LPCTSTR)"Please open png or jpg file");
		}

		CString cstr = dlg.GetPathName();
		const char* pathname = (LPCTSTR)cstr;
		CenterImage = cv::imread(pathname);

		DisplayImage(CenterImage, CENTER);
	}
}


void CpanoramaMFCDlg::OnFileopenRightImage()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK) {
		//파일 확장자 확인
		CString fileExtention = dlg.GetFileExt();
		if (!(fileExtention == "png" || fileExtention == "PNG" || fileExtention == "jpg" || fileExtention == "JPG")) {
			MessageBox((LPCTSTR)"Please open png or jpg file");
			return;
		}

		CString cstr = dlg.GetPathName();
		const char* pathname = (LPCTSTR)cstr;
		RightImage = cv::imread(pathname);

		DisplayImage(RightImage, RIGHT);
	}
}

void CpanoramaMFCDlg::DisplayImage(cv::Mat image, IMAGE_TYPE imageType) {
	CImage mfcImage;
	cv::Mat modifiedImage;
	cv::flip(image, modifiedImage, 0);

	if (imageType == LEFT) {
		GetDlgItem(IDC_STATIC_LEFT_IMAGE)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = LeftControl.GetDC();
		LeftControl.GetClientRect(&rect);

	}
	else if (imageType == CENTER) {
		GetDlgItem(IDC_STATIC_CENTER_IMAGE)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = CenterControl.GetDC();
		CenterControl.GetClientRect(&rect);
	}
	else if (imageType == RIGHT) {
		GetDlgItem(IDC_STATIC_RIGHT_IMAGE)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = RightControl.GetDC();
		RightControl.GetClientRect(&rect);
	}
	else {//panorama
		GetDlgItem(IDC_STATIC_PANORAMA_IMAGE)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = PanoramaControl.GetDC();
		PanoramaControl.GetClientRect(&rect);
	}

	ResizeImage(modifiedImage, modifiedImage, rect);
	DisplayBitmap(m_pDC, rect, modifiedImage);
}

void CpanoramaMFCDlg::ResizeImage(cv::Mat src, cv::Mat& dest, CRect rect) {
	int newWidth = src.cols * (rect.Height() / (double)src.rows);
	int newHeight = rect.Height();
	if (newWidth > rect.Width()) {
		newWidth = rect.Width();
		newHeight = src.rows * (rect.Width() / (double)src.cols);
	}
	cv::resize(src, dest, cv::Size(newWidth, newHeight));

}

void CpanoramaMFCDlg::DisplayBitmap(CDC* pDC, CRect rect, cv::Mat displayImage) {
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = displayImage.cols;
	bitmapInfo.bmiHeader.biHeight = displayImage.rows;

	bitmapInfo.bmiHeader.biBitCount = 3 * 8; // 채널 수(rgb) * 깊이(8bit)
	pDC->SetStretchBltMode(COLORONCOLOR);
	StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, displayImage.cols, displayImage.rows, displayImage.data, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}



Mat CpanoramaMFCDlg::blendImage(Mat image, vector<int> center, int blendingArea, int errorRange) {
	assert(image.rows <= center.size());

	Mat result = image.clone();
	

	for (int row = 0; row < image.rows; row++) {
		//행에 있는 데이터를 vector에 저장한다
		vector<RGB> inputValues = {};
		for (int col = 0; col < image.cols; col++) {
			RGB temp = getRGB(image, col, row);
			inputValues.push_back(temp);
		}
		//저장한 vector로 블렌딩을 진행한다
		vector<RGB> blendedValue = blendRow(inputValues, center[row], blendingArea, errorRange);

		//블렌딩한 값을 result에 넣는다
		for (int col = 0; col < image.cols; col++) {
			putRGB(result, blendedValue[col], col, row);
		}
	}

	return result;
}
vector<RGB> CpanoramaMFCDlg::blendRow(vector<RGB> input, int center, int _blendingArea, int errorRange) {
	int blendingArea = _blendingArea;

	if (center - blendingArea / 2 < 0) blendingArea = blendingArea / 2 + center;
	if (center + blendingArea / 2 > input.size()) blendingArea = (center + blendingArea / 2) - input.size();

	vector<RGB> result(input);

	//오차 범위 내 처리
	for (int i = 0; i < blendingArea / 2 - errorRange / 2; i++) {
		//center를 기준으로 대칭되는 픽셀의 값을 가져온다
		int leftIndex = center - blendingArea / 2 + i;
		int rightIndex = center + blendingArea / 2 - i;

		RGB leftRGB = input[leftIndex];
		RGB rightRGB = input[rightIndex];

		//가져온 픽셀 값을 hsi 값으로 바꾼다
		HSI leftHSI = rgb2hsi(leftRGB);
		HSI rightHSI = rgb2hsi(rightRGB);

		//intensity 값으로 알파 블렌딩을 진행한다.
		double alpha = (double)i / blendingArea;

		int tempIntensity = leftHSI.intensity;
		leftHSI.intensity = (1 - alpha) * leftHSI.intensity + alpha * rightHSI.intensity;
		rightHSI.intensity = alpha * tempIntensity + (1 - alpha) * rightHSI.intensity;


		//다시 rgb 값으로 바꾼 후 저장
		leftRGB = hsi2rgb(leftHSI);
		rightRGB = hsi2rgb(rightHSI);

		result[leftIndex] = leftRGB;
		result[rightIndex] = rightRGB;
	}

	//오차범위 바로 바깥의 intensity 값 평균으로 오차범위 내 intensity 조정
	RGB leftStandardRGB = result[center - errorRange / 2];
	RGB rightStandardRGB = result[center + errorRange / 2];

	HSI leftStandardHSI = rgb2hsi(leftStandardRGB);
	HSI rightStandardHSI = rgb2hsi(rightStandardRGB);
	int average = (leftStandardHSI.intensity + rightStandardHSI.intensity) / 2;

	for (int i = blendingArea / 2 - errorRange / 2; i <= blendingArea / 2; i++) {
		//center를 기준으로 대칭되는 픽셀의 값을 가져온다
		int leftIndex = center - blendingArea / 2 + i;
		int rightIndex = center + blendingArea / 2 - i;

		RGB leftRGB = result[leftIndex];
		RGB rightRGB = result[rightIndex];

		//가져온 픽셀 값을 hsi 값으로 바꾼다
		HSI leftHSI = rgb2hsi(leftRGB);
		HSI rightHSI = rgb2hsi(rightRGB);

		//오차범위 바로 바깥의 intensity의 평균값 사용
		leftHSI.intensity = average;
		rightHSI.intensity = average;

		//다시 rgb 값으로 바꾼 후 저장
		leftRGB = hsi2rgb(leftHSI);
		rightRGB = hsi2rgb(rightHSI);

		result[leftIndex] = leftRGB;
		result[rightIndex] = rightRGB;
	}
	return result;
}
RGB CpanoramaMFCDlg::getRGB(Mat image, int col, int row) {
	RGB result = { 0,0,0 };

	result.red = image.data[(col + row * image.cols) * 3 + 2];
	result.green = image.data[(col + row * image.cols) * 3 + 1];
	result.blue = image.data[(col + row * image.cols) * 3];

	return result;
}
void CpanoramaMFCDlg::putRGB(Mat& dest, RGB rgb, int col, int row) {
	dest.data[(col + row * dest.cols) * 3 + 2] = rgb.red;
	dest.data[(col + row * dest.cols) * 3 + 1] = rgb.green;
	dest.data[(col + row * dest.cols) * 3] = rgb.blue;
}
RGB CpanoramaMFCDlg::hsi2rgb(HSI input) {
	RGB result = { 0,0,0 };

	const double PI = 3.141592;

	//계산을 위한 변수 선언. 이 값들은 0~1 사이 값을 가짐. 나중에 255 곱해서 리턴
	double red = 0;
	double green = 0;
	double blue = 0;

	//계산을 위해 normalization
	double hue = input.hue * PI / 180.0;
	double saturation = input.saturation / 100.0;
	double intensity = input.intensity / 255.0;

	//공식 적용
	if (hue < 2 * PI / 3) {
		blue = intensity * (1 - saturation);
		red = intensity * (1 + saturation * cos(hue) / cos(PI / 3 - hue));
		green = 3 * intensity - (red + blue);
	}
	else if (2 * PI / 3 <= hue && hue < 4 * PI / 3) {
		hue -= 2 * PI / 3;

		red = intensity * (1 - saturation);
		green = intensity * (1 + saturation * cos(hue) / cos(PI / 3 - hue));
		blue = 3 * intensity - (red + green);
	}
	else {
		hue -= 4 * PI / 3;

		green = intensity * (1 - saturation);
		blue = intensity * (1 + saturation * cos(hue) / cos(PI / 3 - hue));
		red = 3 * intensity - (blue + green);
	}

	result.red = red * 255;
	result.green = green * 255;
	result.blue = blue * 255;

	return result;
}
HSI CpanoramaMFCDlg::rgb2hsi(RGB input) {
	HSI result = { 0,0,0 };
	const double PI = 3.141592;

	//계산을 위해 normalization
	double sum = input.red + input.green + input.blue;
	double red = input.red / sum;
	double green = input.green / sum;
	double blue = input.blue / sum;

	//계산을 위한 변수 선언. 이 값들은 0~1 사이 값을 가짐. 나중에 각각 180/PI와 100을 곱해서 리턴
	double hue = 0;
	double saturation = 0;

	//공식 적용
	double theta = acos(0.5 * ((red - green) + (red - blue)) / pow((pow(red - green, 2) + (red - blue) * (green - blue)), 0.5));
	if (blue > green) {
		hue = 2 * PI - theta;
	}
	else {
		hue = theta;
	}

	saturation = 1 - 3 * min(red, min(green, blue));

	result.hue = hue * 180 / PI;
	result.saturation = saturation * 100;
	result.intensity = (input.red + input.green + input.blue) / 3;

	return result;
}
Mat CpanoramaMFCDlg::stitch_two_image(Mat original_image, Mat object_image) {
	//객체 매칭 시 두 이미지의 크기를 똑같이 해서 잘못된 매칭의 수를 줄이려고 함
	Mat originalCutImage(object_image.size(), CV_8UC3);
	original_image(Rect(original_image.cols - originalCutImage.cols, 0, originalCutImage.cols, originalCutImage.rows)).
		copyTo(originalCutImage(Rect(0, 0, originalCutImage.cols, originalCutImage.rows)));

	//이미지를 내려서 변환할 때 100으로 모자랄 수 있기에 추후 쉽게 수정하기 위해 상수로 만듬
	int SHIFT = original_image.rows / 3;

	// ORB 특징점 검출기 초기화
	int minHessian = 1000;
	cv::Ptr<cv::ORB> orb = cv::ORB::create(minHessian);

	// 특징점 및 기술자 추출
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	orb->detectAndCompute(originalCutImage, noArray(), keypoints1, descriptors1);
	orb->detectAndCompute(object_image, noArray(), keypoints2, descriptors2);


	//-- Step 2: Matching descriptor vectors with a brute force matcher
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);

	//좋은 매칭점 선택
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


	//2. gradient로 나누기
	// 객체 위치 찾기
	std::vector<cv::Point2f> src_pts, dst_pts;
	int gradientValue[11] = { 0, }; // -0.5~ 0.5로 0.1씩
	std::vector<std::vector<int>> gradientIndex(11); //해당하는 인덱스 저장
	float tmp = 0.0f;
	int tmpIndex = 0;

	for (int i = 0; i < first_good_matches.size(); i++) {
		src_pts.push_back(keypoints1[first_good_matches[i].queryIdx].pt);
		dst_pts.push_back(keypoints2[first_good_matches[i].trainIdx].pt);

		//gradient 계산 및 저장
		tmp = ((float)(dst_pts[i].y - src_pts[i].y)) / ((float)(dst_pts[i].x - src_pts[i].x));
		tmpIndex = (int)(tmp * 10 + 5);
		if (tmpIndex >= 0 && tmpIndex < 11) {
			gradientValue[tmpIndex] = gradientValue[tmpIndex] + 1;
			gradientIndex[tmpIndex].push_back(i);
		}

	}

	//가장 큰 값이 저장된 index만 살리고 나머지는 버린다.
	int maxIndex = 0;
	for (int i = 1; i < 11; i++) {
		if (gradientValue[maxIndex] < gradientValue[i])
			maxIndex = i;
	}

	std::vector<DMatch> second_good_matches;
	for (int i = 0; i < gradientValue[maxIndex]; i++) {
		second_good_matches.push_back(first_good_matches[gradientIndex[maxIndex][i]]);
	}

	//3. 최단 거리를 구해 오차 제거(물리적 거리)
	std::vector<cv::DMatch> third_good_matches;
	double distanceMean = 0;
	double distanceTmp = 0;
	Point2f oriPoint, objPoint;
	for (int i = 0; i < second_good_matches.size(); i++) {
		oriPoint = keypoints1[second_good_matches[i].queryIdx].pt;
		objPoint = keypoints2[second_good_matches[i].trainIdx].pt;

		//거리 공식 사용
		distanceTmp = sqrt(pow(oriPoint.x - objPoint.x - originalCutImage.cols, 2) + pow(oriPoint.y - objPoint.y, 2));
		distanceMean += distanceTmp;
	}

	//평균의 +-20퍼센트 정도만 남기고 삭제
	distanceMean /= second_good_matches.size();

	for (int i = 0; i < second_good_matches.size(); i++) {
		oriPoint = keypoints1[second_good_matches[i].queryIdx].pt;
		objPoint = keypoints2[second_good_matches[i].trainIdx].pt;

		//거리 공식 사용
		distanceTmp = sqrt(pow(oriPoint.x - objPoint.x - originalCutImage.cols, 2) + pow(oriPoint.y - objPoint.y, 2));
		if (distanceTmp < distanceMean * 1.1 && distanceTmp > distanceMean * 0.9)
			third_good_matches.push_back(second_good_matches[i]);
	}

	// 좋은 매칭으로 객체 위치 찾기
	//매칭 결과가 너무 작으면 호모그래피를 찾을 수 없기에 이를 막기 위해 코드 추가
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



	// 변환 행렬 계산 -> CV_64F
	Mat H = findHomography(newDst_pts, newSrc_pts, cv::RANSAC);

	// 변환 행렬을 수정해서 0,100의 translate를 적용한다.
	// 100이라는 수치는 추후 조정할 수 있다. 100을 해도 이미지가 잘리는 경우가 존재할 수 있기 때문
	double set[9] = { 1,0, 0, 0, 1, SHIFT, 0, 0, 1 };
	Mat translateShift = Mat(H.size(), CV_64F, set);
	Mat translateH = translateShift * H;


	// 변환행렬을 적용해 object_on_original에 저장 
	Mat object_on_original;
	cv::warpPerspective(object_image, object_on_original, translateH, Size(object_image.cols * 2, object_image.rows * 2), INTER_CUBIC);
	//imshow("object_on_original", object_on_original);

	// 블랜딩을 위해 object_on_original의 경계 부분의 좌표를 구할 필요가 있다.
	// 외곽선을 구해주는 함수를 이용해서 이를 계산하겠다.
	Mat contoursMask;
	cvtColor(object_on_original, contoursMask, COLOR_BGR2GRAY); // RGB -> GRAY (채널 문제 때문);


	//이미지의 경계 찾기
	//contours에 외곽선의 좌표 저장
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(contoursMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	//contours에서 각 외곽선에 대해 꼭짓점을 찾습니다.
	//꼭짓점은 시작점을 기준으로 시계방향으로 저장된다.
	vector<vector<Point>> approxContours(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		double epsilon = 0.02 * arcLength(contours[i], true);
		approxPolyDP(contours[i], approxContours[i], epsilon, true);
	}


	// 좌표를 x값이 큰 순서대로 정렬 후 저장한다.
	// 오른쪽에서 col이 큰 포인트 순서대로 저장
	//이때 contours의 인덱스가 1이 아닌 경우도 있을 수 있으므로 크기가 가장 큰 index를 기준으로 해당 과정을 수행한다.
	Point rightMaxPoint, rightMinPoint, leftMaxPoint, leftMinPoint;
	int contoursIndex = 0;
	for (contoursIndex = 0; contoursIndex < approxContours.size(); contoursIndex++) {
		if (approxContours[contoursIndex].size() == 4) {
			std::sort(approxContours[contoursIndex].begin(), approxContours[contoursIndex].end(), compareX);
			leftMinPoint = approxContours[contoursIndex][0];
			leftMaxPoint = approxContours[contoursIndex][1];
			rightMinPoint = approxContours[contoursIndex][2];
			rightMaxPoint = approxContours[contoursIndex][3];
		}
	}
	contoursIndex--;

	//양쪽 좌표 중 y값의 크기를 비교하고 결과를 저장한다.
	bool isLeftYMax, isRightYMax;

	if (leftMinPoint.y < leftMaxPoint.y)
		isLeftYMax = true;
	else
		isLeftYMax = false;

	if (rightMinPoint.y < rightMaxPoint.y)
		isRightYMax = true;
	else
		isRightYMax = false;


	//이미지를 붙이고 필터를 붙이는 방식으로 구현하자
	Mat preCombineImg = Mat::zeros(object_on_original.size(), CV_8UC3);

	//먼저 originalCutImage를 붙인다.
	originalCutImage(Rect(0, 0, originalCutImage.cols, originalCutImage.rows)).
		copyTo(preCombineImg(Rect(0, SHIFT, originalCutImage.cols, originalCutImage.rows)));

	//object_and_origianl을 붙인다.
	for (int i = 0; i < object_on_original.rows; i++) {
		for (int j = 0; j < object_on_original.cols; j++) {
			//해당 점이 경계선 안에 있는 경우 값 대입
			if (pointPolygonTest(contours[contoursIndex], Point(j, i), false) > 0)
				preCombineImg.at<cv::Vec3b>(i, j) = object_on_original.at<cv::Vec3b>(i, j);
		}
	}
	//imshow("preCombineImg", preCombineImg);

	//이미지를 자르기
	//작은 범위로 자르기
	int top, right;
	if (isRightYMax) {
		top = max(SHIFT, rightMinPoint.y);
	}
	else {
		top = max(SHIFT, rightMaxPoint.y);
	}
	right = min(rightMaxPoint.x, rightMinPoint.x);

	//blending을 위한 경계선 벡터 생성
	vector<int> center;
	for (int i = top; i < top + original_image.rows; i++) {
		for (int j = 0; j < object_on_original.cols; j++) {
			//해당 점이 경계선 위에 있는 경우 값 넣기
			if (pointPolygonTest(contours[contoursIndex], Point(j, i), false) == 0)
			{
				//결과 영상에 대해 수행할 예정이기에 original_image를 붙인만큼의 길이를 계산해줘야 한다.
				center.push_back(j + original_image.cols - originalCutImage.cols);
				break;
			}
		}
	}


	// 결과를 저장할 mat 생성 후 데이터 옮기기
	//잘린 original과 object_on_original를 합쳐야 한다.
	//분기문을 넣은 이유는 original_image.cols - originalCutImage.cols가 0이면 오류가 발생하기 때문이다.
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

	//blending 수행
	result = blendImage(result, center, result.rows / 2, 2);
	return result;
}

bool compareX(const Point& p1, const Point& p2) { return p1.x < p2.x; }