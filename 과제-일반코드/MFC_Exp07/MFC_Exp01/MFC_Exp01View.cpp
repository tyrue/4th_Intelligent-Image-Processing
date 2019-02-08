
// MFC_Exp01View.cpp : CMFC_Exp01View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_Exp01.h"
#endif

#include "MFC_Exp01Doc.h"
#include "MFC_Exp01View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_Exp01View

IMPLEMENT_DYNCREATE(CMFC_Exp01View, CView)

BEGIN_MESSAGE_MAP(CMFC_Exp01View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_Exp01View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_Exp01View 생성/소멸

CMFC_Exp01View::CMFC_Exp01View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_Exp01View::~CMFC_Exp01View()
{
}

BOOL CMFC_Exp01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC_Exp01View 그리기
void CMFC_Exp01View::OnDraw(CDC* pDC)
{
	CMFC_Exp01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// 입력 이미지와 출력 이미지를 화면에 출력함
	for(unsigned int i=0; i < pDoc->height; i++) 
	{
		for(unsigned int j=0 ; j < pDoc->width ; j++) 
		{
			unsigned char InVal = pDoc -> m_InImage[(i * pDoc->width) + j];
			unsigned char OutVal = pDoc -> m_OutImage[(i * pDoc->width) + j];
			pDC->SetPixel(j,i,RGB(InVal, InVal, InVal));  // 입력 이미지
			pDC->SetPixel(j+ pDoc->width + 50 , i, RGB(OutVal, OutVal, OutVal)); // 출력 이미지
		}
	}

}


// CMFC_Exp01View 인쇄


void CMFC_Exp01View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_Exp01View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_Exp01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_Exp01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC_Exp01View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_Exp01View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_Exp01View 진단

#ifdef _DEBUG
void CMFC_Exp01View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_Exp01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_Exp01Doc* CMFC_Exp01View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_Exp01Doc)));
	return (CMFC_Exp01Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_Exp01View 메시지 처리기
