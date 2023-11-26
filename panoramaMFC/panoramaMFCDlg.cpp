
// panoramaMFCDlg.cpp: 구현 파일
//

#include <opencv2/opencv.hpp>
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
	if (!(LeftImage.cols == 0 || CenterImage.cols == 0 || RightImage.cols == 0)) {
		MessageBox((LPCTSTR)"Please three images");
		return;
	}


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