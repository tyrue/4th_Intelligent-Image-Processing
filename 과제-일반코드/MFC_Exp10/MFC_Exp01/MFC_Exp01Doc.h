// MFC_Exp01Doc.h : CMFC_Exp01Doc 클래스의 인터페이스
#pragma once
#include "Classifier.h"
#include "Sample.h"
class CMFC_Exp01Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMFC_Exp01Doc();
	DECLARE_DYNCREATE(CMFC_Exp01Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMFC_Exp01Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char *m_InImage, *m_OutImage; // 입력, 출력 이미지
	unsigned int height, width;			// 높이, 너비
	unsigned char m_Threshold;
	afx_msg void OnArithmetic();		// 산술 연산 함수
	afx_msg void OnHistogram();			// 히스토 그램
	afx_msg void OnTobinary();			// 이진화 함수
	afx_msg void OnGonzalez();			// 곤잘레즈 함수
	afx_msg void OnOtsu();				// Otsu 함수
	afx_msg void OnLowpassfilter();		// 저역통과 필터
	afx_msg void OnHighpassfilter();	// 고역통과 필터
	afx_msg void OnMedianfilter();		// 미디언 필터
	afx_msg void OnGaussian();			// 가우시안 스무딩
	afx_msg void OnPrewittmask();			// Prewitt
	afx_msg void OnSobel();					// Sobel
	afx_msg void OnDerivativeofgaussian();  // 가우시안 함수를 미분한 마스크

	afx_msg void OnZoomout();			// 이미지 축소 함수
	afx_msg void OnRotation();			// 이미지 회전 함수
	afx_msg void OnMirroring();			// 이미지 미러링 함수
	afx_msg void OnZoominNn();			// 이미지 확대 함수

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
	double weight[16];//샘플마다 주어지는 가중치
	int turn;//학습 턴, t
	Classifier classifierList[100];//학습될 약분류기, T=100
	afx_msg void OnAdaboostInit();
	double AdaboostResult(void);
	afx_msg void OnAdaboostNext();
	afx_msg void OnAdaboostTest();
};
