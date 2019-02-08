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
	ON_COMMAND(ID_Binary, &CMFC_Exp01Doc::OnTobinary)
	ON_COMMAND(ID_BinDilate, &CMFC_Exp01Doc::OnBindilate)
	ON_COMMAND(ID_BinErode, &CMFC_Exp01Doc::OnBinerode)
	ON_COMMAND(ID_TFT, &CMFC_Exp01Doc::OnTft)
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


void area()
{

}

void CMFC_Exp01Doc::OnTobinary()
{
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

// 영역 탐색을 위한 좌표 방향 배열
int dx[4] = { 1, 0 , -1, 0};
int dy[4] = { 0, 1 , 0, -1};

void CMFC_Exp01Doc::DefectMerge(Defect** defects) // 결함을 합치는 함수
{
	int num = -1;
	for (int y = 1; y < height-1; y++)
	{
		for (int x = 1; x < width-1; x++)
		{
			if (m_OutImage[y * width + x] == 255) // 결함을 발견함
			{
				num++; // 결함 번호 증가
				defects[num] = new Defect(num); 
				area(x, y, defects[num]); // num번째 결함 영역 탐색
				this->num++; // 결함 개수 증가
			}
		}
	}
}

void CMFC_Exp01Doc::area(int x, int y, Defect *defect) // 결함 영역 탐색 함수
{
	if (m_OutImage[y * width + x] == 255) // 결함인 영역일 때
	{
		m_OutImage[y * width + x] = defect->num + 100; // 눈에 잘보이기 위해

		// 결함의 크기 갱신
		if (x < defect->left) defect->left = x;
		if (x > defect->right) defect->right = x;
		if (y < defect->up) defect->up = y;
		if (y > defect->down) defect->down = y;

		// 상하좌우 방향으로 결함 탐색
		for (int i = 0; i < 4; i++)
		{
			// 이미지 크기를 벗어나면 안된다.
			if ((0 <= x + dx[i] && x + dx[i] < width) && (0 <= y + dy[i] && y + dy[i] < height))
				area(x + dx[i], y + dy[i], defect);
		}
	}
}


void CMFC_Exp01Doc::OnTft() // TFT-LCD Defect 검출함수
{
	int *max = (int*)malloc(sizeof(int) * width * height); // 원본 영상의 최대 화소를 저장하는 배열 
	int *min = (int*)malloc(sizeof(int) * width * height); // 원본 영상의 최소 화소를 저장하는 배열
	int *D = (int*)malloc(sizeof(int) * width * height); // Dynamic edge tolerance algorithm을 적용한 차영상
	int *F = (int*)malloc(sizeof(int) * width * height); // AB Compare 알고리즘을 사용하여 Ghost Defect 제거한 영상

	int *temp = (int*)malloc(sizeof(int) * width * height); // 원본 영상을 저장할 임시 배열
	// 임시 배열에 입력 영상을 저장
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			temp[y * width + x] = m_InImage[y * width + x];
		}
	}

	// 원본영상에 대한 각 좌표의 최소값과 최대값 구함
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// 원본 영상의 범위를 벗어나는 것을 방지함
			if (j == 0 || j == width - 1 || i == 0 || i == height - 1) 
			{
				// 각 배열의 값을 초기화
				max[i * width + j] = 0;
				min[i * width + j] = 0;
				D[i * width + j] = 0;
				F[i * width + j] = 0;
				continue;
			}
			double max_n = -1;
			double min_n = 999;

			// 현재 좌표의 3 X 3 범위에서 최소, 최대 값 화소를 구한다.
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					max_n = max_n > m_InImage[(i + k) * width + (j + l)] ? max_n : m_InImage[(i + k) * width + (j + l)];
					min_n = min_n < m_InImage[(i + k) * width + (j + l)] ? min_n : m_InImage[(i + k) * width + (j + l)];
				}
			}

			// 최대 값과 최소값을 저장함
			max[i * width + j] = m_InImage[i * width + j] + 0.8 * (max_n - m_InImage[i * width + j]);
			min[i * width + j] = m_InImage[i * width + j] - 0.8 * (m_InImage[i * width + j] - min_n);
		}
	}

	// Dynamic edge tolerance algorithm을 적용하여 차영상을 구한다.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j <= 500) // 맨 우측 패턴을 제외한 나머지 패턴일 때
			{
				// 각 패턴은 100픽셀 간격으로 반복된다.
				// 현재 패턴과 우측 패턴과 비교한다. 만약 다른 점이 있으면 차영상 배열에 저장한다.
				if (m_InImage[i * width + j] < min[i * width + j + 100])
				{
					D[i * width + j] = min[i * width + j + 100] - m_InImage[i * width + j];
				}
				else if (m_InImage[i * width + j] > max[i * width + j + 100])
				{
					D[i * width + j] = m_InImage[i * width + j] - max[i * width + j + 100];
				}
				else
				{
					D[i * width + j] = 0;
				}
			}
			else // 맨 우측 패턴일 때
			{
				// 각 패턴은 100픽셀 간격으로 반복된다.
				// 바로 왼쪽 패턴과 비교한다. 만약 다른 점이 있으면 차영상 배열에 저장한다.
				if (m_InImage[i * width + j] < min[i * width + (j - 100)])
				{
					D[i * width + j] = min[i * width + (j - 100)] - m_InImage[i * width + j];
				}
				else if (m_InImage[i * width + j] > max[i * width + (j - 100)])
				{
					D[i * width + j] = m_InImage[i * width + j] - max[i * width + (j - 100)];
				}
				else
				{
					D[i * width + j] = 0;
				}
			}
		}
	}
	// 이때 Ghost Defect가 발생한다.

	// AB Compare 알고리즘을 사용하여 Ghost Defect 제거함
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j >= 100) // 맨 좌측 패턴을 제외한 나머지 패턴일 때
			{
				// 현재 패턴과 좌측 패턴을 AND연산하여 F배열에 저장한다.
				// Ghost Defect는 제거 되고, 원래 결함이 있던 부분은 남게 된다. 
				if (D[i * width + j] && D[i * width + j - 100])
				{
					F[i * width + j] = D[i * width + j];
				}
				else
				{
					F[i * width + j] = 0;
				}
			}
			else // 맨 좌측 패턴일 때
			{
				// 현재 패턴과 가장 우측에서 두 번째 패턴을 AND연산하여 F배열에 저장한다.
				// Ghost Defect는 제거 되고, 원래 결함이 있던 부분은 남게 된다. 
				if (D[i * width + j] && D[i * width + j + 400])
				{
					F[i * width + j] = D[i * width + j];
				}
				else
				{
					F[i * width + j] = 0;
				}
			}
		}
	}

	// 입력 영상에 Ghost Defect을 제거한 영상을 저장함(이진 연산을 하기 위해)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = F[y * width + x];
		}
	}

	
	// 입력 영상을 이진화 하고 다시 결과 영상을 저장한다.
	OnTobinary(); 
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}

	// 이진 팽창을 3번, 이진 침식을 2번 실행한다.
	OnBindilate(); // 이진 팽창
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	OnBindilate(); // 이진 팽창
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	OnBindilate(); // 이진 팽창
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}

	OnBinerode();	// 이진 침식
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	OnBinerode();	// 이진 침식
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = m_OutImage[y * width + x];
		}
	}
	
	DefectMerge(defects); // 모든 결함 부분을 탐색하고 합친다.

	int size = 2; // 결함 부분 표시 사각형 두께

	// 결과 영상엔 원본 영상 위에 결함 부분을 표시한다.
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_InImage[y * width + x] = temp[y * width + x]; // 원본 영상으로 되돌린다.
			m_OutImage[y * width + x] = temp[y * width + x]; // 원본 영상으로 되돌린다.

			// 결함 부분을 사각형으로 표시한다.
			for (int i = 0; i < this->num; i++) // 결함의 개수 만큼 표시
			{
				if (((defects[i]->up - size <= y && y <= defects[i]->up) || 
					(defects[i]->down <= y && y <= defects[i]->down + size)) &&
					((defects[i]->left <= x && x <= defects[i]->right)))
				{
					m_OutImage[y * width + x] = 255;
				}
				else if(((defects[i]->left - size <= x && x <= defects[i]->left) || 
					(defects[i]->right <= x && x <= defects[i]->right + size)) &&
					((defects[i]->up <= y && y <= defects[i]->down)))
				{
					m_OutImage[y * width + x] = 255;
				}
			}
		}
	}
	
	// 결함의 중심 좌표를 출력한다.
	for (int i = 0; i < this->num; i++)
	{
		defects[i]->center_x = (defects[i]->right + defects[i]->left) / 2;
		defects[i]->center_y = (defects[i]->down + defects[i]->up) / 2;

		CString strTemp;
		strTemp.Format(_T("num : %d, center_x = %d, center_y = %d"), i + 1, defects[i]->center_x, defects[i]->center_y);
		AfxMessageBox(strTemp);
	}
	UpdateAllViews(NULL);
}
