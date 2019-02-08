// MFC_Exp01Doc.h : CMFC_Exp01Doc Ŭ������ �������̽�
#include "Defect.h"

#pragma once
class CMFC_Exp01Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC_Exp01Doc();
	DECLARE_DYNCREATE(CMFC_Exp01Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CMFC_Exp01Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char *m_InImage, *m_OutImage; // �Է�, ��� �̹���
	unsigned int height, width;			// ����, �ʺ�
	unsigned char m_Threshold;
	afx_msg void OnTobinary();			// ����ȭ �Լ�
	afx_msg void OnBindilate();			// ���� ��â �Լ�
	afx_msg void OnBinerode();			// ���� ħ�� �Լ�
	afx_msg void OnTft();				// TFT-LCD Defect �����Լ�

	Defect* defects[10];				// ���� ��ü�� ��� �迭

	afx_msg void DefectMerge(Defect** defects); // �� ������ ��ġ�� �Լ�
	afx_msg void area(int x, int y, Defect *defect); // ������ �Ǵ��ϴ� �Լ�

	int num = 0;	// ������ ����
};
