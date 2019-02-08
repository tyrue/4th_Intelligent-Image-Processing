
// MFC_Exp01Doc.cpp : CMFC_Exp01Doc 클래스의 구현
//

#include "stdafx.h"
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
	ON_COMMAND(ID_Histogram, &CMFC_Exp01Doc::OnHistogram)  // 밝기 히스토그램 함수
	ON_COMMAND(ID_Binary, &CMFC_Exp01Doc::OnTobinary)	   // 2진화 함수
	ON_COMMAND(ID_Gonzalez, &CMFC_Exp01Doc::OnGonzalez)	   // 곤잘레즈 함수
	ON_COMMAND(ID_Otsu, &CMFC_Exp01Doc::OnOtsu)			   // Otsu 함수
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


void CMFC_Exp01Doc::OnHistogram() // 히스토그램
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
	m_Threshold = 80; // 원하는 임계값을 결정
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_InImage[(i*width) + j] >= m_Threshold)
				m_OutImage[(i*width) + j] = 255; // 영상 이진화: 밝기값이 임계값 보다 크면 255로
			else
				m_OutImage[(i*width) + j] = 0; // 밝기값이 임계값보다 작으면 0으로 이진화 처리한다.
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
	memset(hist, 0, sizeof(int) * 256); // 히스토그램 배열 0으로 초기화

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
	int N = 256 * 256;		// 영상의 총 화소수
	int L = 256;			// 밝기 최대값 0, 1, 2 ~ L-1
	int temp_threshold;		// 1차 임계값 검색과정에서 결정된 임계값

	double Mu_1, Mu_2;		// 두 클래스 각각의 밝기값 평균:
	double Mu_T;			// 밝기값 전체 평균 :
	double W1, W2;			// 두 클래스 발생 확률 :
	double Sigma_B;			// 클래스 간 분산(between class variance) :
	double Sigma_Bmax = 0;	// 클래스 간 분산 최대값 초기화 : 의 최대값
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
		W1 = H_cdf[k] / N;	// 그룹 1 확률 계산
		W2 = 1. - W1;		// 그룹 2 확률 계산
		if (H_cdf[k] == 0.) continue;

		Mu_1 = H_m[k] / H_cdf[k]; // 그룹 1의 밝기값 평균 :

		if ((N - H_cdf[k]) == 0.) continue; //(N-H_cdf[k]): 그룹 2에속한 전체화소수

		Mu_2 = (H_m[L - 1] - H_m[k]) / (N - H_cdf[k]); // 그룹 2의밝기값 평균
		Sigma_B = W1 * (Mu_1 - Mu_T)*(Mu_1 - Mu_T) + 
			W2 * (Mu_2 - Mu_T)*(Mu_2 - Mu_T); 
		if (Sigma_B > Sigma_Bmax) // 최대가 되는 때의 밝기값 k를 찾는 과정
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

