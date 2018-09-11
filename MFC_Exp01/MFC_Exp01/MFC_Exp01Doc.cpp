
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
		if(m_InImage != NULL)
			delete(m_InImage);
		if(m_OutImage != NULL)
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
			if(width != 0 && height != 0) 
			{ 
				ar.Write(m_OutImage, width*height); 
			}

		}
		else
		{
			// TODO: 여기에 로딩 코드를 추가합니다.
			CString strTemp = ar.m_strFileName.Right(3);
			// 영상파일이 RAW로 저장되어 있는 경우 BMP 파일일 때
			if (toupper( strTemp[ 0 ] ) == 'B' && toupper( strTemp[ 1 ] ) == 'M' && toupper( strTemp[ 2 ] ) == 'P' ) 
			{ 
				// 입력할 화일의 포인트를 가져옴.
				CImage m_Image;  // 이미지 객체
				m_Image.Load(ar.m_strFileName); // 불러온 이미지로 초기화 
				// 높이와 너비, 입력 이미지와 출력 이미지 초기화
				width = m_Image.GetWidth();		
				height = m_Image.GetHeight();
				m_InImage = (unsigned char*) malloc(sizeof(unsigned char) * width * height);
				m_OutImage = (unsigned char*) malloc(sizeof(unsigned char) * width * height);

				// GetBPP() : 각 픽셀을 구성하는 비트 수
				int ws = width * (m_Image.GetBPP() / 8); // 한 라인의 크기(byte)
				// 임시 이미지 파일 동적 할당
				unsigned char *m_tmpImage = (unsigned char*) malloc(sizeof(unsigned char) * ws * height);

				if(m_Image.GetBPP() == 1) // 픽셀 비트가 1이면 못 읽음
				{
					AfxMessageBox((LPCTSTR)"읽을 수 있는 파일 형식이 아닙니다.");
					return;
				} 
				else if(m_Image.GetBPP() == 8)// 픽셀 비트가 8
				{ 
					for (int y= 0; y < height; y++)
					{
						BYTE *srcImg = NULL;                   
						
						srcImg = (BYTE *) m_Image.GetPixelAddress(0,y);
						memcpy(&m_InImage[y * ws], srcImg, ws); // 이미지 복사
					}
				} 
				else if(m_Image.GetBPP() == 24) // 픽셀 비트가 24 
				{ 
					for (int y= 0; y <height; y++)
					{
						BYTE * srcImg = NULL;                   
						
						srcImg = (BYTE *) m_Image.GetPixelAddress(0,y);
						memcpy(&m_tmpImage[y * ws], srcImg, ws);  // 이미지 복사
					}
					int iter = 0; 
					for(int y=0; y<height; y++)
					{
						for(int x=0; x<width; x++)
						{
							m_InImage[(y * width) + x] = ( m_tmpImage[iter] * 0.21 + m_tmpImage[iter + 1] * 0.72 
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

		CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
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
		for(i = 0; i < height; i++)
		{ 
			for(j = 0; j < width; j++)
			{ 
				// 1. 나눗셈 연산
				temp = m_InImage[(i*width) + j] / 2; 
				// 2. 영상처리 결과영상을 화면에 출력하기 위한 처리
				if(temp > 255) 
					m_OutImage[(i*width) + j] = 255; // 255를 넘어가면 최대값 255로
				else if(temp < 0) 
					m_OutImage[(i*width) + j] = 0; // 0보다 작으면 최소값 0 으로
				else 
					m_OutImage[(i*width) + j] = (unsigned char)temp; 
			}
		}     
		// 3. 현재 출력되어 있는 모든 화면을 갱신하여  다시 출력한다. 
		UpdateAllViews(NULL); 
	}
