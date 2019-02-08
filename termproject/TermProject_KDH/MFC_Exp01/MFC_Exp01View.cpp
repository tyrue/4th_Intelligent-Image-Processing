
// MFC_Exp01View.cpp : CMFC_Exp01View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_Exp01View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_Exp01View ����/�Ҹ�

CMFC_Exp01View::CMFC_Exp01View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_Exp01View::~CMFC_Exp01View()
{
}

BOOL CMFC_Exp01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC_Exp01View �׸���
void CMFC_Exp01View::OnDraw(CDC* pDC)
{
	CMFC_Exp01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	// �Է� �̹����� ��� �̹����� ȭ�鿡 �����
	for(unsigned int i=0; i < pDoc->height; i++) 
	{
		for(unsigned int j=0 ; j < pDoc->width ; j++) 
		{
			unsigned char InVal = pDoc -> m_InImage[(i * pDoc->width) + j];
			unsigned char OutVal = pDoc -> m_OutImage[(i * pDoc->width) + j];
			pDC->SetPixel(j,i,RGB(InVal, InVal, InVal));  // �Է� �̹���
			pDC->SetPixel(j+ pDoc->width + 50 , i, RGB(OutVal, OutVal, OutVal)); // ��� �̹���
		}
	}

}


// CMFC_Exp01View �μ�


void CMFC_Exp01View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_Exp01View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_Exp01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_Exp01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMFC_Exp01View ����

#ifdef _DEBUG
void CMFC_Exp01View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_Exp01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_Exp01Doc* CMFC_Exp01View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_Exp01Doc)));
	return (CMFC_Exp01Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_Exp01View �޽��� ó����
