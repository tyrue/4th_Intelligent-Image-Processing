// MFC_Exp01Doc.h : CMFC_Exp01Doc Ŭ������ �������̽�
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
	afx_msg void OnArithmetic();		// ��� ���� �Լ�
};
