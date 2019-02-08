
// MFC_Exp01View.h : CMFC_Exp01View Ŭ������ �������̽�
//

#pragma once


class CMFC_Exp01View : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC_Exp01View();
	DECLARE_DYNCREATE(CMFC_Exp01View)

// Ư���Դϴ�.
public:
	CMFC_Exp01Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC_Exp01View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC_Exp01View.cpp�� ����� ����
inline CMFC_Exp01Doc* CMFC_Exp01View::GetDocument() const
   { return reinterpret_cast<CMFC_Exp01Doc*>(m_pDocument); }
#endif

