// MFC_Exp01Doc.h : CMFC_Exp01Doc Ŭ������ �������̽�
#pragma once
#include "Classifier.h"
#include "Sample.h"
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
	afx_msg void OnArithmetic();		// ��� ���� �Լ�
	afx_msg void OnHistogram();			// ������ �׷�
	afx_msg void OnTobinary();			// ����ȭ �Լ�
	afx_msg void OnGonzalez();			// ���߷��� �Լ�
	afx_msg void OnOtsu();				// Otsu �Լ�
	afx_msg void OnLowpassfilter();		// ������� ����
	afx_msg void OnHighpassfilter();	// ����� ����
	afx_msg void OnMedianfilter();		// �̵�� ����
	afx_msg void OnGaussian();			// ����þ� ������
	afx_msg void OnPrewittmask();			// Prewitt
	afx_msg void OnSobel();					// Sobel
	afx_msg void OnDerivativeofgaussian();  // ����þ� �Լ��� �̺��� ����ũ

	afx_msg void OnZoomout();			// �̹��� ��� �Լ�
	afx_msg void OnRotation();			// �̹��� ȸ�� �Լ�
	afx_msg void OnMirroring();			// �̹��� �̷��� �Լ�
	afx_msg void OnZoominNn();			// �̹��� Ȯ�� �Լ�

	afx_msg void OnBindilate();			
	afx_msg void OnBinerode();
	afx_msg void Ontophat();
	afx_msg void OnBinopening();
	afx_msg void OnBinclosing();
	afx_msg void OnGraydilate();
	afx_msg void OnGrayerode();
	afx_msg void OnDct();
	afx_msg void OnIdct();
	double* m_Dct;
	afx_msg void OnLms();
	afx_msg void OnHarriscorner();

	Sample samples[16];
	double weight[16];//���ø��� �־����� ����ġ
	int turn;//�н� ��, t
	Classifier classifierList[100];//�н��� ��з���, T=100
	afx_msg void OnAdaboostInit();
	double AdaboostResult(void);
	afx_msg void OnAdaboostNext();
	afx_msg void OnAdaboostTest();
};
