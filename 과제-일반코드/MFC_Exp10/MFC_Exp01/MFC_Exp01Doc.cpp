// MFC_Exp01Doc.cpp : CMFC_Exp01Doc 클래스의 구현
//

#include "stdafx.h"
#include "time.h"
#include "Matrix.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_Exp01.h"
#endif

#include "MFC_Exp01Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFC_Exp01Doc

IMPLEMENT_DYNCREATE(CMFC_Exp01Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_Exp01Doc, CDocument)
	ON_COMMAND(ID_32771, &CMFC_Exp01Doc::OnArithmetic) // 산술 연산 함수 추가
	ON_COMMAND(ID_Histogram, &CMFC_Exp01Doc::OnHistogram)
	ON_COMMAND(ID_Binary, &CMFC_Exp01Doc::OnTobinary)
	ON_COMMAND(ID_Gonzalez, &CMFC_Exp01Doc::OnGonzalez)
	ON_COMMAND(ID_Otsu, &CMFC_Exp01Doc::OnOtsu)
	ON_COMMAND(ID_Lowpassfilter, &CMFC_Exp01Doc::OnLowpassfilter)
	ON_COMMAND(ID_Highpassfilter, &CMFC_Exp01Doc::OnHighpassfilter)
	ON_COMMAND(ID_Medianfilter, &CMFC_Exp01Doc::OnMedianfilter)
	ON_COMMAND(ID_Gaussian, &CMFC_Exp01Doc::OnGaussian)
	ON_COMMAND(ID_Prewittmask, &CMFC_Exp01Doc::OnPrewittmask)
	ON_COMMAND(ID_Sobel, &CMFC_Exp01Doc::OnSobel)
	ON_COMMAND(ID_DerivativeOfGaussian, &CMFC_Exp01Doc::OnDerivativeofgaussian)
	ON_COMMAND(ID_ZoomOut, &CMFC_Exp01Doc::OnZoomout)
	ON_COMMAND(ID_Rotation, &CMFC_Exp01Doc::OnRotation)
	ON_COMMAND(ID_Mirroring, &CMFC_Exp01Doc::OnMirroring)
	ON_COMMAND(ID_ZoomIn_Nn, &CMFC_Exp01Doc::OnZoominNn)
	ON_COMMAND(ID_BinDilate, &CMFC_Exp01Doc::OnBindilate)
	ON_COMMAND(ID_BinErode, &CMFC_Exp01Doc::OnBinerode)
	ON_COMMAND(ID_topHat, &CMFC_Exp01Doc::Ontophat)
	ON_COMMAND(ID_BinOpening, &CMFC_Exp01Doc::OnBinopening)
	ON_COMMAND(ID_BinClosing, &CMFC_Exp01Doc::OnBinclosing)
	ON_COMMAND(ID_GrayDilate, &CMFC_Exp01Doc::OnGraydilate)
	ON_COMMAND(ID_GrayErode, &CMFC_Exp01Doc::OnGrayerode)
	ON_COMMAND(ID_DCT, &CMFC_Exp01Doc::OnDct)
	ON_COMMAND(ID_IDCT, &CMFC_Exp01Doc::OnIdct)
	ON_COMMAND(ID_LMS, &CMFC_Exp01Doc::OnLms)
	ON_COMMAND(ID_HarrisCorner, &CMFC_Exp01Doc::OnHarriscorner)
	ON_COMMAND(ID_ADABOOST_INIT, &CMFC_Exp01Doc::OnAdaboostInit)
	ON_COMMAND(ID_adaboostnext, &CMFC_Exp01Doc::OnAdaboostNext)
	ON_COMMAND(ID_AdaboostTest, &CMFC_Exp01Doc::OnAdaboostTest)
END_MESSAGE_MAP()


// CMFC_Exp01Doc 생성/소멸

CMFC_Exp01Doc::CMFC_Exp01Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_InImage = NULL;	// 입력 이미지
	m_OutImage = NULL;	// 출력 이미지
	height = 0;			// 높이
	width = 0;			// 너비
}

CMFC_Exp01Doc::~CMFC_Exp01Doc()
{
	// 동적할당한 집합을 해제함
	if (m_InImage != NULL)
		delete(m_InImage);
	if (m_OutImage != NULL)
		delete(m_OutImage);
}

BOOL CMFC_Exp01Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CMFC_Exp01Doc serialization

void CMFC_Exp01Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) // 저장
	{
		// TODO: 여기에 저장 코드를 추가합니다. 
		// 영상처리 후 출력영상 m_OutImage[]를 raw 데이터로  저장  
		if (width != 0 && height != 0)
		{
			ar.Write(m_OutImage, width*height);
		}

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CString strTemp = ar.m_strFileName.Right(3);
		// 영상파일이 RAW로 저장되어 있는 경우 BMP 파일일 때
		if (toupper(strTemp[0]) == 'B' && toupper(strTemp[1]) == 'M' && toupper(strTemp[2]) == 'P')
		{
			// 입력할 화일의 포인트를 가져옴.
			CImage m_Image;  // 이미지 객체
			m_Image.Load(ar.m_strFileName); // 불러온 이미지로 초기화 
			// 높이와 너비, 입력 이미지와 출력 이미지 초기화
			width = m_Image.GetWidth();
			height = m_Image.GetHeight();
			m_InImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
			m_OutImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);

			// GetBPP() : 각 픽셀을 구성하는 비트 수
			int ws = width * (m_Image.GetBPP() / 8); // 한 라인의 크기(byte)
			// 임시 이미지 파일 동적 할당
			unsigned char *m_tmpImage = (unsigned char*)malloc(sizeof(unsigned char) * ws * height);

			if (m_Image.GetBPP() == 1) // 픽셀 비트가 1이면 못 읽음
			{
				AfxMessageBox((LPCTSTR)"읽을 수 있는 파일 형식이 아닙니다.");
				return;
			}
			else if (m_Image.GetBPP() == 8)// 픽셀 비트가 8
			{
				for (int y = 0; y < height; y++)
				{
					BYTE *srcImg = NULL;

					srcImg = (BYTE *)m_Image.GetPixelAddress(0, y);
					memcpy(&m_InImage[y * ws], srcImg, ws); // 이미지 복사
				}
			}
			else if (m_Image.GetBPP() == 24) // 픽셀 비트가 24 
			{
				for (int y = 0; y < height; y++)
				{
					BYTE * srcImg = NULL;

					srcImg = (BYTE *)m_Image.GetPixelAddress(0, y);
					memcpy(&m_tmpImage[y * ws], srcImg, ws);  // 이미지 복사
				}
				int iter = 0;
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						m_InImage[(y * width) + x] = (m_tmpImage[iter] * 0.21 + m_tmpImage[iter + 1] * 0.72
							+ m_tmpImage[iter + 2] * 0.07); // 이미지 복사
						iter += 3;
					}
				}
			}
		}
		// 입력파일이 BMP파일이 아닌경우 
		else
		{
			AfxMessageBox((LPCTSTR)"읽을 수 있는 파일 형식이 아닙니다.");
			return;
		}
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMFC_Exp01Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMFC_Exp01Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFC_Exp01Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFC_Exp01Doc 진단

#ifdef _DEBUG
void CMFC_Exp01Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_Exp01Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CMFC_Exp01Doc::OnArithmetic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/************************** 1. 산술연산 처리 ********************************/
	int i, j; // 인덱스 처리릉 위한 변수 
	int temp; // 임시 데이터 저장을 위한 변수 
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// 1. 나눗셈 연산
			temp = m_InImage[(i*width) + j] / 2;
			// 2. 영상처리 결과영상을 화면에 출력하기 위한 처리
			if (temp > 255)
				m_OutImage[(i*width) + j] = 255; // 255를 넘어가면 최대값 255로
			else if (temp < 0)
				m_OutImage[(i*width) + j] = 0; // 0보다 작으면 최소값 0 으로
			else
				m_OutImage[(i*width) + j] = (unsigned char)temp;
		}
	}
	// 3. 현재 출력되어 있는 모든 화면을 갱신하여  다시 출력한다. 
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int Hist[256]; // 히스토그램을 저장하기 위한 배열
	int MaxVal = 0; // 히스토그램의 최대값 변수
	int i, j;
	// 히스토그램 내용을 모두 0으로 초기화
	memset(Hist, 0, sizeof(int) * 256);
	/***************************************************/
	// 1. 히스토그램 계산
	/***************************************************/
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			Hist[m_InImage[(i*width) + j]]++; // 현재 픽셀의 값을 히스토그램 배열에 저장
			if (MaxVal < Hist[m_InImage[(i*width) + j]])
				MaxVal = Hist[m_InImage[(i*width) + j]]; // 히스토그램에서의 최대값 계산
		}
	}
	/***************************************************/
	// 2. 히스토그램을 출력화면에 디스플레이 하기
	/***************************************************/
	// 출력화면 전체를 모두 하얗게(255 값으로) 초기화 한다.
	m_OutImage = (unsigned char*)malloc(sizeof(char) * height * width);
	memset(m_OutImage, 255, sizeof(unsigned char) * height * width);
	float w_ratio = (float)255 / (width - 1);
	float h_ratio = (float)(height - 1) / MaxVal;
	// X축은 밝기값으로 Y축은 히스토그램의 값으로 출력한다.
	for (i = 0; i < width; i++)
	{
		int idx = i * w_ratio;
		int value = Hist[idx] * h_ratio;	// 히스토그램의 최대 크기가 255가 되게한다.
		for (j = 0; j < value; j++)
			m_OutImage[(height - j - 1)*width + i] = 0; // 히스토그램의크기만큼 검은색(밝기값 0)으로 
														// 디스플레이한다.
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnTobinary()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 사용자가 임의의 임계값을 직접 결정해서 입력할 경우
	m_Threshold = 130; // 원하는 임계값을 결정
					   // 영상 이진화: 밝기값이 임계값 보다 크면 255로
					   // 밝기값이 임계값보다 작으면 0으로 이진화 처리한다.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_InImage[(i*width) + j] >= m_Threshold)
				m_OutImage[(i*width) + j] = 255;
			else
				m_OutImage[(i*width) + j] = 0;
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnGonzalez()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/***********************************************/
	// 1. 히스토그램 계산
	/***********************************************/

	int hist[256];
	// 히스토그램 배열 0으로 초기화
	memset(hist, 0, sizeof(int) * 256);

	// 히스토그램 계산
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			hist[m_InImage[(i*width) + j]]++; // 현재 픽셀의 값을 히스토그램 배열에 저장
		}
	}
	/***********************************************************/
	// 2. Gonzalez 알고리즘을 이용한 자동 임계값 결정 과정
	/************************************************************/
	int T, min, max;
	// 2.1 초기 Gonzalez 알고리즘의 임계값 결정
	for (int i = 0; i < 256; i++)
	{
		if (hist[i] > 0)
		{
			min = i; // 입력영상에서 가장 밝기값이 작은 값
			i = 257;
		}
	}
	for (int i = 255; i > 0; i--)
	{
		if (hist[i] > 0)
		{
			max = i; // 입력영상에서 가장 밝기값 큰 값
			i = -1;
		}
	}
	T = (min + max) / 2; // 초기 임계값 : 최소 밝기값과 최대 밝기값의 평균값
						 // 2.2 초기 결정된 임계값을 이용하여 Gonzalez 알고리즘 임계값 결정
	int Diff = 5; //새롭게 계산된 임계값과 이전임계값과의 차이에 대한 초기값
	while (Diff >= 1)
	{
		// 그룹 1에 속한 화소 집합의 평균계산
		int nSum = 0, nCnt = 0; // nSum: 그룹 1에 속하는 전체 화소밝기값
			// nCnt: 그룹 1의 전체 화소수
		for (int i = 0; i < T; i++)
		{
			nSum += hist[i] * i;
			nCnt += hist[i];
		}
		double Mu_1 = (double)(nSum / nCnt); // 그룹 1의 밝기값 평균
											 // 그룹 2에 속한 화소 집합의 평균계산
		nSum = 0, nCnt = 0; // nSum: 그룹 2에 속하는 전체 화소밝기값,
			// nCnt: 그룹 2의 전체 화소수
		for (int i = T; i < 256; i++)
		{
			nSum += hist[i] * i;
			nCnt += hist[i];
		}
		double Mu_2 = (double)(nSum / nCnt); // 그룹 2의 밝기값 평균
											 // 두 화소 그룹의 밝기값 평균을 이용한 임계값 결정
		int T_Last = (int)((Mu_1 + Mu_2) / 2);
		Diff = abs(T - T_Last); // 새롭게 계산된 임계값과 이전의 임계값과의차이계산
		T = T_Last;
	}

	m_Threshold = T; // 최종 Gonzalez 임계값 결정
					 /***********************************************************/
					 // 3. Gonzalez 알고리즘에 의한 영상 이진화
					 /************************************************************/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_InImage[(i*width) + j] >= m_Threshold)
				m_OutImage[(i*width) + j] = 255;
			else
				m_OutImage[(i*width) + j] = 0;
		}
	}
	UpdateAllViews(NULL);
	/***********************************************************/
	// 4. Gonzalez 자동 임계값을 메세지 박스에 표시
	/************************************************************/
	CString strTemp;
	strTemp.Format(_T("Gonzalez 임계값 = %d"), m_Threshold);
	AfxMessageBox(strTemp);
}


void CMFC_Exp01Doc::OnOtsu()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/***********************************************/
	// 1. 히스토그램 계산 과정
	/***********************************************/
	int hist[256]; // 밝기 히스토그램
	memset(hist, 0, sizeof(int) * 256); // 히스토그램 배열 0으로 초기화
										// 히스토그램 계산
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			hist[m_InImage[(i*width) + j]]++; // 현재픽셀의 값을 히스토그램 배열에 저장
		}
	}

	/***********************************************************/
	// 2. Otsu 알고리즘을 이용한 자동 임계값 결정 과정
	/************************************************************/
	int N = 256 * 256; // 영상의 총 화소수
	int L = 256; // 밝기 최대값 0, 1, 2 ~ L-1
	int temp_threshold; // 1차 임계값 검색과정에서 결정된 임계값
	double Mu_1, Mu_2; // 두 클래스 각각의 밝기값 평균:
	double Mu_T; // 밝기값 전체 평균 :
	double W1, W2; // 두 클래스 발생 확률 :
	double Sigma_B; // 클래스 간 분산(between class variance) :
	double Sigma_Bmax = 0; // 클래스 간 분산 최대값 초기화 : 의 최대값
	double H_m[256], H_cdf[256]; // H_m[i]: 밝기값 0~i 까지의 전체총 누적밝기값
	// H_cdf[i] : 밝기값 0 ~ i까지의 전체 화소수
	H_m[0] = 0; H_cdf[0] = hist[0]; // 밝기값 0 에서의 초기값 설정
	for (int i = 1; i < L; i++)
	{
		H_m[i] = H_m[i - 1] + hist[i] * (double)i;
		H_cdf[i] = H_cdf[i - 1] + hist[i];
	}
	Mu_T = H_m[255] / N; // 전체 밝기값에 대한 평균 계산 :
		/***********************************************************/
		// 2.1 계산 시간을 절약하기 위해 밝기값 0 ~ L-1 사이에서 듬성듬성하게
		// 임계값 검색
		/***********************************************************/
	for (int k = 3; k < L; k += 4)
	{
		W1 = H_cdf[k] / N; // 그룹 1 확률 계산
		W2 = 1. - W1; // 그룹 2 확률 계산
		if (H_cdf[k] == 0.) continue;

		Mu_1 = H_m[k] / H_cdf[k]; // 그룹 1의 밝기값 평균 :

		if ((N - H_cdf[k]) == 0.) continue; //(N-H_cdf[k]): 그룹 2에속한 전체화소수

		Mu_2 = (H_m[L - 1] - H_m[k]) / (N - H_cdf[k]); // 그룹 2의밝기값 평균
		Sigma_B = W1 * (Mu_1 - Mu_T)*(Mu_1 - Mu_T) +
			W2 * (Mu_2 - Mu_T)*(Mu_2 - Mu_T);
		// 가 최대가 되는 때의 밝기값 k를 찾는 과정
		if (Sigma_B > Sigma_Bmax)
		{
			Sigma_Bmax = Sigma_B;
			temp_threshold = k;
		}
	}
	/***********************************************************/
	// 2.2 1차로 결정된 임계값을 중심으로 좀 더 세밀하게 임계값 검색
	/***********************************************************/
	for (int k = temp_threshold - 3; k <= temp_threshold + 3; k++)
	{
		W1 = H_cdf[k] / N; // 그룹 1의 확률
		W2 = 1. - W1; // 그룹 2의 확률
		Mu_1 = H_m[k] / H_cdf[k]; // 그룹 1의 밝기값 평균
		Mu_2 = (H_m[L - 1] - H_m[k]) / (N - H_cdf[k]); // 그룹 2의밝기값 평균
		Sigma_B = W1 * (Mu_1 - Mu_T)*(Mu_1 - Mu_T) +
			W2 * (Mu_2 - Mu_T)*(Mu_2 - Mu_T);

		if (Sigma_B > Sigma_Bmax)
		{
			Sigma_Bmax = Sigma_B;
			m_Threshold = k; // 최종 임계값 결정
		}
	}
	/***********************************************************/
	// 3. Otsu 임계값에 의한 영상 이진화
	/************************************************************/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_InImage[(i*width) + j] >= m_Threshold)
				m_OutImage[(i*width) + j] = 255;
			else
				m_OutImage[(i*width) + j] = 0;
		}
	}
	UpdateAllViews(NULL);
	/***********************************************************/
	// 4. Otsu 자동 임계값을 사용자가 보기위해 메세지 박스에 표시
	/************************************************************/
	CString strTemp;
	strTemp.Format(_T("Otsu 임계값 = %d"), m_Threshold);
	AfxMessageBox(strTemp);
}


void CMFC_Exp01Doc::OnLowpassfilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/************************************************************
	// 1. 저역통과 필터링 마스크 처리
	**********************************************************/
	static int mask[3][3] = { { 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } };
	int temp;
	// 1.1 마스크 처리가 가능한 영역
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			temp = 0; // 0으로 초기화
			for (int r = 0; r < 3; r++)
			{
				// 마스크와 입력 영상을 합친다.
				for (int c = 0; c < 3; c++)
					temp += (mask[r][c] * m_InImage[((i + r - 1) * width) + j + c - 1]);
			}
			m_OutImage[(i*width) + j] = (unsigned char)(temp / 9); // 출력 영상
		}
	}
	// 1.2 영상의 최외각 라인 및 행에 대한 처리
	for (int i = 0; i < width; i++) {
		m_OutImage[i] = m_InImage[i];
		m_OutImage[((height - 2)*width) + i] = m_InImage[((height - 2)*width) + i];
	}
	for (int i = 0; i < height; i++) {
		m_OutImage[((i)*width)] = m_InImage[((i)*width)];
		m_OutImage[((i + 1)*width - 1)] = m_InImage[((i + 1)*width - 1)];
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnHighpassfilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/************************************************************
	// 1. 고역통과 필터링 마스크 처리
	**********************************************************/
	static int mask[3][3] = { { -1, -1, -1 },{ -1, 9, -1 },{ -1, -1, -1 } };
	int temp, Max = -256 * 256, Min = 256 * 256; // 마스크 처리을 위해 필요한 변수,
	int i, j, r, c;
	int *Result = (int*)malloc(sizeof(int) * width * height); //[256][256]; // 마스크 처리된 이미지를 저장하는 배열
		// 1.1 마스크 처리가 가능한 영역 처리
	for (i = 1; i < height; i++)
	{
		for (j = 1; j < width; j++)
		{
			temp = 0; // 0으로 초기화
			for (r = 0; r < 3; r++)
			{
				for (c = 0; c < 3; c++)
					temp += (mask[r][c] * m_InImage[((i + r - 1)*width) + j + c - 1]); //마스크 처리
			}
			if (temp > Max) Max = temp;//마스크 처리된 값 중 최대값 검색
			if (temp < Min) Min = temp;//마스크 처리된 값 중 최소값 검색
			Result[(i*width) + j] = temp;
		}
	}
	int size = width * height;
	// 1.2 영상의 최외각 라인 및 행에 대한 처리
	for (int i = 0; i < width; i++)
	{
		m_OutImage[i] = m_InImage[i];
		m_OutImage[size - i - 1] = m_InImage[size - i - 1];
	}
	for (int i = 0; i < height; i++)
	{
		m_OutImage[((i)*width)] = m_InImage[((i)*width)];
		m_OutImage[((i + 1)*width - 1)] = m_InImage[((i + 1)*width - 1)];
	}
	// 1.3 마스크 처리된 결과를 화면에 출력하는 방법
	// 1) 결과를 그대로 출력해 보자.
	/*
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			m_OutImage[(i*width) + j] = (unsigned
				char)Result[(i*width) + j];
		}
	}*/
	/*
	// 2) 화면 출력을 위해 출력 화소값을 0~255로 클리핑(Clipping) 한다.
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (Result[(i*width) + j] > 255)
				m_OutImage[(i*width) + j] = 255;
			else if (Result[(i*width) + j] < 0)
				m_OutImage[(i*width) + j] = 0;
			else
				m_OutImage[(i*width) + j] = (unsigned char)Result[(i*width) + j];
		}
	}*/

	//3) 화면 출력을 위해 출력화소값을 0~255로 매핑시킨다.
	  // 상수값 미리 계산

	double C1 = (double)(255.0 / (Max - Min));
	double C2 = (double)(-255.0*Min / (Max - Min));
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			m_OutImage[(i*width) + j] = (unsigned char)(C1*Result[(i*width) + j] + C2);
		}
	}
	//이때 마스크 처리된 최대값과 최소값이 어떻게 되는지 화면에 출력해보자
	CString strTemp;
	strTemp.Format(_T("최대값 = %d , 최소값 = %d"), Max, Min);
	AfxMessageBox(strTemp);
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnMedianfilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//미디언 필터를¸ 적용할 정방형 마스크의 가로화소
	const int winSize = 3; // 영역처리하는 마스크 크기
						   //마스크의 전체 크기의 배열을 할당한다. Sorting 된 밝기값을 저장하기 위해
	unsigned char* arr = new unsigned char[winSize * winSize];
	//영상 각각의 픽셀에 대해
	for (int y = 0 + winSize / 2; y < height - winSize / 2; y++)
	{
		for (int x = 0 + winSize / 2; x < width - winSize / 2; x++)
		{
			int total = 0; // 정렬되는 화소수의 Index 값
			for (int by = 0; by < winSize; by++)
			{
				for (int bx = 0; bx < winSize; bx++)
				{
					arr[total] = m_InImage[((y + (by -
						winSize / 2)) * width) + x + (bx - winSize / 2)];
					for (int i = total; i > 0; i--)
					{
						//새롭게 추가된 화소의 밝기값을 정렬한다.
						if (arr[i] < arr[i - 1])
						{
							unsigned char temp = arr[i];
							arr[i] = arr[i - 1];
							arr[i - 1] = temp;
						}
					}
					total++;
				}
			}
			//정렬된 밝기값의 중간값을 출력이미지로 처리한다.
			m_OutImage[(y*width) + x] = arr[winSize * winSize / 2];
		}
	}
	delete[] arr; // 정렬을 위해 할당된 메모리를 해제한다.
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 가우시안 필터를 적용할 정방형 마스크의 가로화소
	double sigma = 5.0; // 표준편차
	double center = 0.0; // 평균 값 위치
	double PI = 3.14159; // pi값
	const int winSize = 5; // 영역처리하는 마스크 크기

	clock_t begin, end; // 시간 측정용 변수
	begin = clock(); // 시작

	// 2차원 배열 마스크 동적 할당
	double **mask = (double **)malloc(sizeof(double *) * winSize);
	for (int i = 0; i < winSize; i++)
	{
		mask[i] = (double *)malloc(sizeof(double) * winSize);
	}

	double sum = 0; // 마스크 값을 저장할 변수
	// 마스크에 값을 저장한다.
	for (int i = 0; i < winSize; i++)
	{
		for (int j = 0; j < winSize; j++)
		{
			mask[i][j] = (1.0 / sqrt(2.0*PI*sigma*sigma)) *
				exp(-((i - center)*(i - center) + (j - center)*(j - center))* (1.0 / (2.0*sigma*sigma)));
			sum += mask[i][j];
		}
	}

	int temp; // 임시 변수
	int i, j, r, c; // 반복자 변수
	// 출력 영상에 마스크 필터링
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			temp = 0; // 0으로 초기화
			for (int r = 0; r < winSize; r++)
			{
				for (int c = 0; c < winSize; c++)
					temp += (mask[r][c] * m_InImage[((i + r - 1) * width) + j + c - 1]);
			}
			m_OutImage[(i*width) + j] = (unsigned char)(temp / sum);
		}
	}
	// 영상의 최외각 라인 및 행에 대한 처리
	for (int i = 0; i < width; i++) {
		m_OutImage[i] = m_InImage[i];
		m_OutImage[((height - 2)*width) + i] = m_InImage[((height - 2)*width) + i];
	}
	for (int i = 0; i < height; i++) {
		m_OutImage[((i)*width)] = m_InImage[((i)*width)];
		m_OutImage[((i + 1)*width - 1)] = m_InImage[((i + 1)*width - 1)];
	}
	end = clock();

	CString strTemp;
	strTemp.Format(_T("수행 시간 = %dms"), (end - begin));
	AfxMessageBox(strTemp);
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnPrewittmask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: Add your command handler code here
	// Prewitt 마스크
	clock_t begin, end; // 시간 측정용 변수
	begin = clock(); // 시작

	// XY 마스크
	static int maskX[3][3] = { -1, 0, 1,-1, 0, 1,-1, 0, 1 };
	static int maskY[3][3] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

	int amplitude; // 에지강도를 위한 변수
	m_Threshold = 130; // 에지검출을 위한 이진화 임계값

	//1.1 Prewitt 마스크 연산
	for (int y = 1; y < height; y++)
	{
		for (int x = 1; x < width; x++)
		{
			//블록단위처리
			int sumX = 0;
			int sumY = 0;
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					sumX += maskX[by][bx] * m_InImage[(y + by - 1)*width + x + bx - 1];
					sumY += maskY[by][bx] * m_InImage[(y + by - 1)*width + x + bx - 1];
				}
			}
			//계산된 에지 강도값
			//amplitude = (int)sqrt((double)sumX * sumX + sumY * sumY); //방법 1
			amplitude = abs(sumX) + abs(sumY); // 방법2

			//1. 에지강도값을 0~255의 값으로 처리(클리핑)
			//if (amplitude > 255) m_OutImage[y*width + x] = 255;
			//else if (amplitude < 0) m_OutImage[y*width + x] = 0;
			//else m_OutImage[y*width + x] = (unsigned char)amplitude;

			//// 2. 에지강도값을 임의의 임계값으로 이진화
			if (amplitude > m_Threshold) m_OutImage[y*width + x] = 255;
			else m_OutImage[y*width + x] = 0;
		}
	}
	end = clock();

	CString strTemp;
	strTemp.Format(_T("수행 시간 = %dms"), (end - begin));
	AfxMessageBox(strTemp);

	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// Sobel 마스크
	// XY 마스크
	static int maskX[3][3] = { -1, 0, 1,-2, 0, 2,-1, 0, 1 };
	static int maskY[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	int amplitude; // 에지강도를 위한 변수
	m_Threshold = 130; // 에지검출을 위한 이진화 임계값

	//1.1 Prewitt 마스크 연산
	for (int y = 1; y < height; y++)
	{
		for (int x = 1; x < width; x++)
		{
			//블록단위처리
			int sumX = 0;
			int sumY = 0;
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					sumX += maskX[by][bx] * m_InImage[(y + by - 1)*width + x + bx - 1];
					sumY += maskY[by][bx] * m_InImage[(y + by - 1)*width + x + bx - 1];
				}
			}
			//계산된 에지 강도값
			//amplitude = (int)sqrt((double)sumX * sumX + sumY * sumY); //방법 1
			amplitude = abs(sumX) + abs(sumY); // 방법2

			//1. 에지강도값을 0~255의 값으로 처리(클리핑)
			//if (amplitude > 255) m_OutImage[y*width + x] = 255;
			//else if (amplitude < 0) m_OutImage[y*width + x] = 0;
			//else m_OutImage[y*width + x] = (unsigned char)amplitude;

			//// 2. 에지강도값을 임의의 임계값으로 이진화
			if (amplitude > m_Threshold) m_OutImage[y*width + x] = 255;
			else m_OutImage[y*width + x] = 0;
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnDerivativeofgaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const double PI = 3.14159; // pi
	const double sigma = 1.0; // Gaussian 분포의 표준편차, sigma =0.4, 1.0, 2.0

	int y, x, r, c;
	int M = (int)(6.0 * sigma) + 1; // 마스크 크기 M x M
	if (M % 2 == 0) M = M + 1; // 마스크의 크기 M 짝수이면 홀수로 만든다.

	int center = (int)M / 2; // 마스크의 중심
	int scale = 10; // 마스크의 전체 가중치를 Scaling 하는 변수
	int amplitude; // 에지강의 크기를 저장

	clock_t begin, end; // 시간 측정용 변수
	begin = clock(); // 시작

	//X, Y 마스크를 위한 메모리 크기할당
	double **MaskX = new double*[M];
	double **MaskY = new double*[M];
	for (int i = 0; i < M; i++)
	{
		MaskX[i] = new double[M];
		MaskY[i] = new double[M];
	}

	//[1] 가우시안함수를 미분한 X 축, Y 축 마스크 계산
	for (y = -center; y <= center; y++)
	{
		for (x = -center; x <= center; x++)
		{
			MaskX[y + center][x + center] = scale * (-x * exp(-(x * x + y * y) /
				(2.0 * sigma * sigma)) / (2.0 * PI * sigma * sigma * sigma * sigma));
			MaskY[y + center][x + center] = scale * (-y * exp(-(x * x + y * y) /
				(2.0 * sigma * sigma)) / (2.0 * PI * sigma * sigma * sigma * sigma));
		}
	}
	for (y = 0; y < height; y++) for (x = 0; x < width; x++) m_OutImage[y*width + x] = 0;

	// [2] 마스크를 이용한 에지강도 검출
	for (y = center; y < height - center; y++)
	{
		for (x = center; x < width - center; x++)
		{
			int sumX = 0;
			int sumY = 0;
			for (r = 0; r < M; r++)
			{
				for (c = 0; c < M; c++)
				{
					sumX += MaskX[r][c] * m_InImage[(y + r - (int)(M / 2))*width + x + c - (int)(M / 2)];
					sumY += MaskY[r][c] * m_InImage[(y + r - (int)(M / 2))*width + x + c - (int)(M / 2)];
				}
			}
			//에지강도값 계산
			amplitude = (int)sqrt((double)sumX * sumX + sumY * sumY);
			// 에지강도값을 0 ~ 255로 처리(clipping)
			if (amplitude > 255) m_OutImage[y*width + x] = 255;
			else if (amplitude < 0) m_OutImage[y*width + x] = 0;
			else m_OutImage[y*width + x] = (unsigned char)amplitude;
		}
	}
	delete[] MaskX;
	delete[] MaskY;
	end = clock();

	CString strTemp;
	strTemp.Format(_T("수행 시간 = %dms"), (end - begin));
	AfxMessageBox(strTemp);

	UpdateAllViews(NULL);
}

void CMFC_Exp01Doc::OnZoominNn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/*********************************************************
	// 1. 영상 확대
	***********************************************************/
	double scale = 3.5; // 영상을 1.6배 확대한다.
	int X_max = (int)(width*scale); // 확대된 영상의 새로운 폭 계산
	int Y_max = (int)(height*scale); // 확대된 영상의 새로운 높이 계산

	double x, y, p, q; // 역방향 사상된 좌표의 분수값
	int x_org, y_org, x2, y2, value; // 역방향 사상된 정수 좌표값

	unsigned char *ZoomImage;
	// 확대된 영상을 임시로 저장할 공간 동적할당
	ZoomImage = new unsigned char[Y_max*X_max];
	for (int y_new = 0; y_new < Y_max; y_new++)
	{
		for (int x_new = 0; x_new < X_max; x_new++)
		{
			x = x_new / scale;
			y = y_new / scale;

			// 1.1 가장 인접한 이웃화소 보간법으로 좌표값을 계산

		/*	x_org = (int)(x + 0.5);
			y_org = (int)(y + 0.5);*/

			// 1.2 양선형 보간법으로 좌표값을 계산

			// 가장 가까운 정수로 옮김
			x_org = (int)x;
			y_org = (int)y;

			p = x - x_org;
			q = y - y_org;

			// 밝기값을 계산한다.
			value = (1.0 - p) * (1.0 - q) * m_InImage[y_org * height + x_org]
				+ p * (1.0 - q) * m_InImage[y_org * height + (x_org + 1)]
				+ (1.0 - p) * q * m_InImage[(y_org + 1) * height + x_org]
				+ p * q * m_InImage[(y_org + 1) * height + (x_org + 1)];

			// 범위 조사: 원영상의 크기 영역을 벗어나는 경우
			if (x_org < 0 || x_org > width || y_org < 0 || y_org > height)
			{
				ZoomImage[y_new*X_max + x_new] = 0;
			}
			// 원 이미지의 범위내에 존재하는 경우 가장 인접한 이웃화소 보간법
			else
			{
				ZoomImage[y_new*X_max + x_new] = m_InImage[y_org*height + x_org];
			}
		}
	}
	// 1.3 확대된 영상을 원래크기의 영상사이즈 만큼 출력한다.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			m_OutImage[height * i + j] = ZoomImage[(i + (Y_max - height) / 2)*X_max + (j + (X_max - width) / 2)]; // 중앙 확대
			//m_OutImage[height * i + j] = ZoomImage[i*X_max + j]; // 왼쪽 위 구석에서 부터 확대
		}
	}
	delete[] ZoomImage; // 동적 할당된 메모리 해제
	UpdateAllViews(NULL);
}

void CMFC_Exp01Doc::OnZoomout()
{
	double scale = 0.5; // 영상을 0.5배 축소한다.
	int X_max = (int)(width*scale); // 축소된 영상의 새로운 폭 계산
	int Y_max = (int)(height*scale); // 축소된 영상의 새로운 높이 계산

	double x, y; // 역방향 사상된 좌표의 분수값
	int x_org, y_org; // 역방향 사상된 정수 좌표값

	unsigned char *ZoomImage;
	// 확대된 영상을 임시로 저장할 공간 동적할당
	ZoomImage = new unsigned char[Y_max*X_max];
	for (int y_new = 0; y_new < Y_max; y_new++)
	{
		for (int x_new = 0; x_new < X_max; x_new++)
		{
			x = x_new / scale;
			y = y_new / scale;
			// 가장 인접한 이웃화소 좌표값으로 매핑
			x_org = (int)(x + 0.5);
			y_org = (int)(y + 0.5);
			// 범위 조사: 원영상의 크기 영역을 벗어나는 경우
			if (x_org < 0 || x_org > width || y_org < 0 || y_org > height)
			{
				ZoomImage[y_new*X_max + x_new] = 0;
			}
			// 원 이미지의 범위내에 존재하는 경우 가장 인접한 이웃화소 보간법
			else
				ZoomImage[y_new*X_max + x_new] = m_InImage[y_org*height + x_org];
		}
	}
	// 축소된 영상을 원래크기의 영상사이즈 만큼 출력한다.
	for (int i = 0; i < Y_max; i++)
	{
		for (int j = 0; j < X_max; j++)
		{
			m_OutImage[i*height + j] = ZoomImage[i*X_max + j];
		}
	}
	delete[] ZoomImage; // 동적 할당된 메모리 해제
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnRotation()
{
	double rotationAngle = 135; // 시계방향 영상회전 각도(degree)
	int x_center = width / 2; // X축 영상 회전중심
	int y_center = height / 2; // Y축 영상 회전중심

	double rotationAngleRad = (rotationAngle*3.14159265 / 180); //영상회전각도 (radian)
	double cosValue, sinValue; // cosValue=cos(rotationAngleRad)이다.

	double x, y; // 역방향 사상된 좌표의 분수값
	int x_org, y_org; // 역방향 사상된 정수 좌표값
	for (int y_new = 0; y_new < height; y_new++)
	{
		for (int x_new = 0; x_new < width; x_new++)
		{
			cosValue = cos(rotationAngleRad);
			sinValue = sin(rotationAngleRad);
			// 영상회전 전의 원영상에서의 좌표값 계산
			x = cosValue * (x_new - x_center) + sinValue * (y_new - y_center) + x_center;
			y = -sinValue * (x_new - x_center) + cosValue * (y_new - y_center) + y_center;
			// 가장 인접한 이웃화소 좌표값으로 매핑
			x_org = (int)(x + 0.5);
			y_org = (int)(y + 0.5);
			// 범위 조사: 원영상의 크기 영역을 벗어나는 경우
			if (x_org < 0 || x_org > width || y_org < 0 || y_org > height)
			{
				m_OutImage[y_new*height + x_new] = 0;
			}
			// 원 이미지의 범위내에 존재하는 경우 가장 인접한 이웃화소 보간법
			else
				m_OutImage[y_new*height + x_new] =
				m_InImage[y_org*height + x_org];
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnMirroring()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//m_OutImage[i*height + j] = m_InImage[i*height + (width - 1 - j)]; // 수평 미러링
			m_OutImage[(height - 1 - i)*height + j] = m_InImage[i*height + j]; // 수직 미러링
		}
	}
	UpdateAllViews(NULL);
}



void CMFC_Exp01Doc::OnBindilate() // 팽창 연산
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int y = 1; y < height; y++)
	{
		for (int x = 1; x < width; x++)
		{
			//x, y 주변의 화소를 검사한다. 
			bool found = false;
			for (int by = y - 1; by <= y + 1; by++)
			{
				for (int bx = x - 1; bx <= x + 1; bx++)
				{
					//하나라도 true라면 반복을 그만둔다.
					if (m_InImage[by*width + bx] > 0)
					{
						found = true;
						break;
					}
				}
				if (found == true)
				{
					break;
				}
			}
			//주변에서 화소를 하나라도 발견했다면 밝게 함
			if (found == true)
			{
				m_OutImage[y*width + x] = 255;
			}
			else // 아니면 어둡게 한다.
			{
				m_OutImage[y*width + x] = 0;
			}
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnBinerode() // 침식 연산
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int y = 1; y < height; y++)
	{
		for (int x = 1; x < width; x++)
		{
			//x, y 주변의 화소를 검사한다. 
			bool found = false;
			for (int by = y - 1; by <= y + 1; by++)
			{
				for (int bx = x - 1; bx <= x + 1; bx++)
				{
					//하나라도 0을 발견하면 반복을 그만둔다.
					if (m_InImage[by*width + bx] == 0)
					{
						found = true;
						break;
					}
				}
				if (found == true)
				{
					break;
				}
			}
			//주변의 모든 화소가 true라면 해당 화소는 밝게한다.
			if (found == false)
			{
				m_OutImage[y*width + x] = 255;
			}
			else // 하나라도 어두운 화소가 있다면 해당 화소는 어둡게 한다.
			{
				m_OutImage[y*width + x] = 0;
			}
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnBinopening() // 열림 연산(침식 + 팽창)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int n = 2; // 반복 횟수
	int *temp = (int*)malloc(sizeof(int) * width * height); // 원본영상을 저장할 임시 배열

	// 임시 배열에 입력 영상을 저장(원본)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp[y * width + x] = m_InImage[y * width + x];
		}
	}

	// 반복 횟수 만큼 반복한다.
	for (int i = 0; i < n; i++)
	{
		OnBinerode(); // 침식 연산
		// 입력영상에 출력 영상을 저장한다.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	for (int i = 0; i < n; i++)
	{
		OnBindilate(); // 팽창 연산
		// 입력영상에 출력 영상을 저장한다.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}

	// 입력 영상에 원본 영상을 저장한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = temp[y * width + x];
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnBinclosing() // 닫힘 연산(팽창 + 침식)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int n = 2; // 반복 횟수
	int *temp = (int*)malloc(sizeof(int) * width * height); // 원본영상을 저장할 임시 배열

	// 임시 배열에 입력 영상을 저장(원본)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp[y * width + x] = m_InImage[y * width + x];
		}
	}

	// 반복횟수 만큼 연산을 반복한다.
	for (int i = 0; i < n; i++)
	{
		OnBindilate(); // 팽창 연산
		// 입력 영상에 출력 영상을 저장한다.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	for (int i = 0; i < n; i++)
	{
		OnBinerode(); // 침식 연산
		// 입력 영상에 출력 영상을 저장한다.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}

	// 입력 영상에 원본 영상을 저장한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = temp[y * width + x];
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnGraydilate() // 그레이 팽창 연산
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int y = 1; y < height; y++) // 영상의 높이
	{
		for (int x = 1; x < width; x++) // 영상의 길이
		{
			int max = 0;//찾아낼 최대값, 초기값으로 0을 설정
			// 주변 화소에서 최대 밝기 값을 찾는다.
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					if (max < m_InImage[(y + (by - 1))*width + x + (bx - 1)])
					{
						max = m_InImage[(y + (by - 1))*width + x + (bx - 1)];
					}
				}
			}
			//주변 화소 중 가장 큰 값을 저장
			m_OutImage[y*width + x] = max;
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnGrayerode() // 그레이 침식 연산
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int y = 1; y < height; y++) // 영상의 높이
	{
		for (int x = 1; x < width; x++) // 영상의 길이
		{
			int min = 255; //찾아낼 최소값, 초기값으로 255을 설정
			// 주변 화소에서 최소 밝기 값을 찾는다.
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					if (min > m_InImage[(y + (by - 1))*width + x + (bx - 1)])
					{
						min = m_InImage[(y + (by - 1))*width + x + (bx - 1)];
					}
				}
			}
			//주변 화소 중 가장 작은 값을 저장
			m_OutImage[y*width + x] = min;
		}
	}
	UpdateAllViews(NULL);
}

void CMFC_Exp01Doc::Ontophat()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int *temp = (int*)malloc(sizeof(int) * width * height); // 원본 영상을 저장할 임시 배열
	// 임시 배열에 입력 영상을 저장
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp[y * width + x] = m_InImage[y * width + x];
		}
	}
	OnGraydilate(); // 그레이 팽창

	// 입력 영상에 출력 영상을 저장한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}

	OnGrayerode(); // 그레이 침식
	// 입력 영상에 원본 영상을 저장한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = temp[y * width + x];
		}
	}
	// Top Hat 연산 
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Light object, 원본 영상에 열림 연산한 영상을 뺀다.
			//m_OutImage[y*width + x] = m_InImage[y*width + x] - m_OutImage[y*width + x];
			// Dark object, 닫힘 연산한 영상에 원본 영상을 뺀다.
			m_OutImage[y*width + x] = m_OutImage[y*width + x] - m_InImage[y*width + x];
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnDct()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: Add your command handler code here
	////////////////////////////////////////////////////////////////////////////
	// 1. 먼저 (height)x(width) 입력영상을 반으로 축소한다.
	////////////////////////////////////////////////////////////////////////////
	const double PI = 3.14159265;
	const double scale = 1 / 2.; //영상을 0.5배 축소한다.
	const int N = (width < height ? width : height) * scale; // 축소된 영상의 새로운 넓이 계산
	m_Dct = (double*)malloc(sizeof(double) * N*N);
	unsigned char *ZoomImage; // 축소된 영상을 저장하는 메모리
	ZoomImage = new unsigned char[sizeof(char) * N * N]; //축소된 영상을 저장할 공간동적할당

	for (int y_new = 0; y_new < N; y_new++)
	{
		for (int x_new = 0; x_new < N; x_new++)
		{
			//역방향 사상된 좌표값
			double x = x_new / scale;
			double y = y_new / scale;
			// 가장 인접한 이웃화소 좌표값으로 매핑
			int x_org = (int)(x + 0.5);
			int y_org = (int)(y + 0.5);
			ZoomImage[y_new * N + x_new] = m_InImage[y_org * width + x_org];
		}
	}
	/////////////////////////////////////////////////////////////
	//// 2. M x N영상을 DCT 변환한다. -> DCT[M][N]
	////////////////////////////////////////////////////////////
	int u_max, v_max;
	double Sum, Cu, Cv, DCT_max = 0.;
	for (int u = 0; u < N; u++)
	{
		if (u == 0) Cu = sqrt(1. / 2.); else Cu = 1;
		for (int v = 0; v < N; v++)
		{
			Sum = 0;
			if (v == 0) Cv = sqrt(1. / 2.); else Cv = 1;
			for (int y = 0; y < N; y++)
			{
				for (int x = 0; x < N; x++)
				{
					double cosValue1 = cos(v * PI * (2 * y + 1) / (2 * N));
					double cosValue2 = cos(u * PI * (2 * x + 1) / (2 * N));
					Sum += (double)ZoomImage[y*N + x] * cosValue1*cosValue2;
				}
			}
			m_Dct[u*N + v] = 2. / N * Cu*Cv*Sum;
			// DCT계수의 절대값이 가장 큰 값(DCT_max)과
			// 그때의 주파수(u_max, v_max)를 찾아보자.
			if (abs((int)m_Dct[u*N + v]) > DCT_max)
			{
				DCT_max = abs((int)m_Dct[u*N + v]);
				u_max = u;
				v_max = v;
			}
		}
	}
	// DCT 변환한 후, DCT 계수중 가장 큰 계수값을 메세지 박스에 표시한다.
	CString strTemp;
	strTemp.Format(_T("(u,v)=(%d,%d)일때 DCT_max값=%5.3f"), u_max, v_max, DCT_max);
	AfxMessageBox(strTemp);
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 3. DCT[M][N] 계수값을 0 ~ 255 까지의 화소값으로 변환한다.
	// 이과정에서 DCT계수의 절대값이 threshold ~ DCT_max 사이의 계수
	// 값은 밝기값 255로, DCT 계수의 절대값이 0 ~ threshold 사이의 값은
	// 0~255 사이의 밝기값으로 매핑한다.
	const int threshold = DCT_max * 0.05;
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (abs((int)m_Dct[y*N + x]) > threshold)
				m_OutImage[y*width + x] = 255;
			else
				m_OutImage[y*width + x] = abs((int)m_Dct[y*N + x]) * 255 / threshold;
		}
	}
	// 입력영상 저장 메모리를 0으로 초기화
	memset(m_InImage, 0, sizeof(unsigned char) * (width*height));
	// 입력영상 출력부분을 256x256을 128x 128로 변경해서 출력
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			m_InImage[y*width + x] = ZoomImage[y*N + x];
		}
	}
	delete[] ZoomImage; // 축소된 영상을 저장하기 위한 동적 저장공간 해제
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnIdct()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: Add your command handler code here
	const double PI = 3.14159265;
	const double scale = 1 / 2.;
	const int N = (width < height ? width : height) * scale; // IDCT 변환 열
	double Sum, Cu, Cv, temp;
	/**********************************************************
	// ///// 1. IDCT 변환 과정
	***********************************************************/
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			Sum = 0;
			for (int u = 0; u < N; u++)
			{
				for (int v = 0; v < N; v++)
				{
					if (v == 0) Cv = sqrt(1. / 2.); else Cv = 1;
					if (u == 0) Cu = sqrt(1. / 2.); else Cu = 1;
					double cosValue1 = cos(v * PI * (2 * y + 1) / (2 * N));
					double cosValue2 = cos(u * PI * (2 * x + 1) / (2 * N));
					Sum += Cu * Cv*m_Dct[u*N + v] * cosValue1*cosValue2;
				}
			}
			temp = Sum * 2. / N; // IDCT 변환 후 원래의 화소값
			m_OutImage[y*width + x] = (unsigned char)temp;
		}
	}
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnLms()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const int size = width * height; // 영상의 크기
	double * dat_a = new double[size * 3]; // 입력영상의 좌표 데이터
	double * dat_b = new double[size]; // 입력영상 x,y의 밝기값 데이터
	int idx = 0, count = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dat_a[idx++] = y;
			dat_a[idx++] = x;
			dat_a[idx++] = 1;
			dat_b[count++] = (double)m_InImage[(y*width) + x];
		}
	}

	// 입력 데이터를 통해 행렬을 생성한다
	CMatrix matA(dat_a, size, 3); // 좌표값 행렬
	CMatrix matB(dat_b, size, 1); // 밝기값 행렬
	// pseudo inverse matrix 계산
	CMatrix pinv = (matA.transpose() * matA).inverse() * matA.transpose();
	// 최소제곱 해 행렬 계산
	CMatrix matX = pinv * matB;
	// 모델링된 밝기 계산
	CMatrix matI = matA * matX;
	// residual 행렬 계산
	CMatrix matR = matB - matI;

	int *temp = (int*)malloc(sizeof(int) * width * height); // 원본영상을 저장할 임시 배열
	// 임시 배열에 입력 영상을 저장(원본)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp[y * width + x] = m_InImage[y * width + x];
		}
	}

	// 보정 이미지 출력
	count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			m_InImage[i*width + j] = abs(m_InImage[i*width + j] - matI.data[count++]);
		}
	}
	OnOtsu();
	// 입력 영상에 원본 영상을 저장한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = temp[y * width + x];
		}
	}

	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnHarriscorner()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// sobel 마스크
	int maskX[3][3] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	int maskY[3][3] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int* dxImage = (int*)malloc(sizeof(int)*width*height);
	int* dyImage = (int*)malloc(sizeof(int)*width*height);
	//dx 이미지 생성
	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			int value = 0;
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					value += maskX[by][bx] * m_InImage[(y + by - 1) * width + x + bx - 1];
				}
			}
			dxImage[y*width + x] = value;
		}
	}
	//dy 이미지 생성
	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			int value = 0;
			for (int by = 0; by < 3; by++)
			{
				for (int bx = 0; bx < 3; bx++)
				{
					value += maskY[by][bx] * m_InImage[(y + by - 1) * width + x + bx - 1];
				}
			}
			dyImage[y*width + x] = value;
		}
	}
	//출력을 위한 구문 
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_OutImage[y*width + x] = dyImage[y*width + x];
		}
	}

	int wSize = 5; //코너를 구하기 위한 윈도우 사이즈
	for (int y = wSize / 2; y < height - wSize / 2; y++)
	{
		for (int x = wSize / 2; x < width - wSize / 2; x++)
		{
			//해시안 매트릭스의 a, b, c, d ... (식 2)의 구현
			double a = 0, b = 0, c = 0, d = 0;
			for (int wy = 0; wy < wSize; wy++)
			{
				for (int wx = 0; wx < wSize; wx++)
				{
					int dx = dxImage[(y + wy - wSize / 2) *
						width + x + wx - wSize / 2];
					int dy = dyImage[(y + wy - wSize / 2) *
						width + x + wx - wSize / 2];
					a += dx * dx;
					b += dx * dy;
					c += dx * dy;
					d += dy * dy;
				}
			}
			//고유값을 구하기 위한 구문 ... (식 3)의 구현
			double aplusd = (a + d);
			double root = sqrt(aplusd * aplusd - 4 * (a * d - b * c));
			double lambda1 = (aplusd + root) / 2;
			double lambda2 = (aplusd - root) / 2;

			// 두개의 고유값중 작은 값을 이용한 방법
			int value = (int)(min(lambda1, lambda2) * 0.001);

			// Measure of corner response을 이용한 방법
			double det = lambda1 * lambda2;
			double trace = lambda1 + lambda2;
			double k = 0.05;
			int r = (int)(det - k * trace * trace);
			/*
			//1. 출력을 위한 클리핑
			if (value > 255)
			{
				value = 255;
			}
			m_OutImage[y * width + x] = value;
			*/
			//2. 출력을 위한 클리핑

			if (r < 0)
			{
				r = 0;
			}
			else if (r > 255)
			{
				r = 255;
			}
			m_OutImage[y * width + x] = r;
		}
	}
	UpdateAllViews(NULL);
}

double CMFC_Exp01Doc::AdaboostResult(void)
{
	//배경을 검은색으로 초기화
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			m_InImage[(y*width) + x] = 0;
		}
	}
	//입력 샘플 출력
	for (int i = 0; i < 16; i++)
	{
		int x = samples[i].x_u;
		int y = samples[i].x_v;

		if (samples[i].y == 1)
		{
			m_InImage[(y*width) + x] = 255;
			m_InImage[(y*width) + x + 1] = 255;
			m_InImage[((y + 1)*width) + x] = 255;
			m_InImage[((y + 1)*width) + x + 1] = 255;
		}
		else
		{
			m_InImage[(y*width) + x] = 80;
			m_InImage[(y*width) + x + 1] = 80;
			m_InImage[((y + 1)*width) + x] = 80;
			m_InImage[((y + 1)*width) + x + 1] = 80;
		}
	}

	//출력 이미지 배경을 검은색으로 초기화
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			m_OutImage[(y*width) + x] = 0;
		}
	}

	//약분류기들의 직선을 화면에 표시
	for (int t = 0; t < turn; t++)
	{
		if (classifierList[t].Direction == 1)
		{
			int thX = classifierList[t].Th;
			for (int y = 0; y < 256; y++)
			{
				m_OutImage[(y*width) + thX] = 128;
			}
		}
		else
		{
			int thY = classifierList[t].Th;
			for (int x = 0; x < 256; x++)
			{
				m_OutImage[(thY*width) + x] = 128;
			}
		}
	}

	int countError = 0;//총 에러 샘플 수
	//각각의 샘플에 대해
	for (int i = 0; i < 16; i++)
	{
		//3. 현재까지의 강분류기 결과 도출
		double result = 0;
		double totalAlpha = 0;
		for (int t = 0; t < turn; t++)
		{
			double error = classifierList[t].error;
			result += classifierList[t].alpha *
				classifierList[t].Classify(samples[i]);
		}
		//분류 결과에 따라 화면에 색깔을 달리하여 표시
		int x = samples[i].x_u;
		int y = samples[i].x_v;

		if (result > 0)
		{
			m_OutImage[(y*width) + x] = 255;
			m_OutImage[(y*width) + x + 1] = 255;
			m_OutImage[((y + 1)*width) + x] = 255;
			m_OutImage[((y + 1)*width) + x + 1] = 255;
		}
		else
		{
			m_OutImage[(y*width) + x] = 80;
			m_OutImage[(y*width) + x + 1] = 80;
			m_OutImage[((y + 1)*width) + x] = 80;
			m_OutImage[((y + 1)*width) + x + 1] = 80;
		}
		//강분류기의 결과가 틀렸다면 에러에 대한 카운트 증가
		if ((result < 0 && samples[i].y > 0) || (result > 0 && samples[i].y < 0))
		{
			countError++;
		}
	}
	//전체 샘플에 대한 에러율을 리턴
	return countError / (double)16;
}

void CMFC_Exp01Doc::OnAdaboostInit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	width = 256;
	height = 256;
	m_InImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	m_OutImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);

	for (int i = 0; i < width * height; i++) 
	{
		m_InImage[i] = 0;
		m_OutImage[i] = 0;
	}

	//샘플 추가
	samples[0] = Sample(5, 8, -1);
	samples[1] = Sample(34, 29, 1);
	samples[2] = Sample(159, 63, -1);
	samples[3] = Sample(82, 36, 1);
	samples[4] = Sample(165, 217, 1);
	samples[5] = Sample(169, 84, 1);
	samples[6] = Sample(190, 5, 1);
	samples[7] = Sample(198, 153, 1);
	samples[8] = Sample(30, 163, -1);
	samples[9] = Sample(80, 15, 1);
	samples[10] = Sample(200, 89, -1);
	samples[11] = Sample(138, 153, -1);
	samples[12] = Sample(54, 140, -1);
	samples[13] = Sample(7, 50, 1);
	samples[14] = Sample(81, 170, -1);
	samples[15] = Sample(210, 10, -1);

	//가중치 초기화
	for (int i = 0; i < 16; i++)
	{
		weight[i] = 1 / (double)16;
	}
	turn = 0;

	AdaboostResult();
	UpdateAllViews(NULL);
}


void CMFC_Exp01Doc::OnAdaboostNext()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (turn == 100) return;

	Classifier minClassifier;//최소 에러값을 가지는 가장 강한 약분류기
	minClassifier.error = 1;

	//2.1 모든 분류기 중 가장 에러가 작은 분류기를 선택한다.
	//1이면 u축, 2이면 v축
	for (int axis = 0; axis <= 1; axis++)
	{
		for (int direction = -1; direction <= 1; direction += 2)
		{
			for (int th = 0; th < 256; th++)
			{
				//분류기 생성 
				Classifier classifier(direction, th, axis);

				//생성된 분류기를 모든 샘플에 테스트하여 에러 도출
				classifier.error = 0;
				for (int i = 0; i < 16; i++)
				{
					if (classifier.Classify(samples[i]) != samples[i].y)
					{
						classifier.error += weight[i];
					}
				}
				//에러를 최소로 하는 분류기 선택
				if (classifier.error < minClassifier.error)
				{
					minClassifier = classifier;
				}
			}
		}
	}

	//2.2 h_t에 대한 알파값 획득
	minClassifier.alpha = 0.5 * log((1 + minClassifier.error) / (minClassifier.error));

	//2.3 가중치 업데이트
	double totalWeight = 0;
	for (int i = 0; i < 16; i++)
	{
		weight[i] = weight[i] * exp(-minClassifier.alpha * samples[i].y *
			minClassifier.Classify(samples[i]));
		totalWeight += weight[i];
	}
	//가중치 초기화
	for (int i = 0; i < 16; i++)
	{
		weight[i] /= totalWeight;
	}

	//찾아낸 분류기를 리스트에 저장
	classifierList[turn] = minClassifier;
	turn++;	//학습된 횟수 증가

	//3. 최종 분류기 결과 도출
	double errorRate = AdaboostResult();
	UpdateAllViews(NULL);

	CString string;
	string.Format(_T("에러율 : %.2lf"), errorRate);
	MessageBox(NULL, string, _T(""), MB_OK);
}



void CMFC_Exp01Doc::OnAdaboostTest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++)
			m_OutImage[y*width + x] = 0;

	//격자 간격의 테스트 샘플에 대해
	for (int y = 10; y <= 250; y += 15)
	{
		for (int x = 10; x <= 250; x += 15)
		{
			//강 분류기에 의한 분류
			double result = 0;
			for (int t = 0; t < turn; t++)
			{
				double error = classifierList[t].error;
				result += classifierList[t].alpha *
					classifierList[t].Classify(Sample(x, y, 1));
			}
			if (result > 0)
			{
				m_OutImage[(y*width) + x] = 255;
				m_OutImage[(y*width) + x + 1] = 255;
				m_OutImage[((y + 1)*width) + x] = 255;
				m_OutImage[((y + 1)*width) + x + 1] = 255;
			}
			else
			{
				m_OutImage[(y*width) + x] = 80;
				m_OutImage[(y*width) + x + 1] = 80;
				m_OutImage[((y + 1)*width) + x] = 80;
				m_OutImage[((y + 1)*width) + x + 1] = 80;
			}
		}
	}
	UpdateAllViews(NULL);
}
