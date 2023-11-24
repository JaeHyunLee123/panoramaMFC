
// panoramaMFCDlg.h: 헤더 파일
//

#pragma once

#include<opencv2/opencv.hpp>

enum IMAGE_TYPE {
	LEFT,
	CENTER,
	RIGHT,
	PANORAMA
};

// CpanoramaMFCDlg 대화 상자
class CpanoramaMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CpanoramaMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PANORAMAMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	cv::Mat LeftImage, CenterImage, RightImage, PanoramaImage;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
	
public:
	CStatic LeftControl, CenterControl, RightControl, PanoramaControl;
	CRect rect;
	CDC* m_pDC;


	afx_msg void OnBnClickedButtonStitch();
	afx_msg void OnFileopenLeftImage();
	afx_msg void OnFileopenCenterImage();
	afx_msg void OnFileopenRightImage();

	void DisplayImage(cv::Mat image, IMAGE_TYPE imageType);
	void DisplayBitmap(CDC* pDC, CRect rect, cv::Mat displayImage);
	void ResizeImage(cv::Mat src, cv::Mat& dest, CRect rect);
};
