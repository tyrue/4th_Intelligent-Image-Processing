// MFC_Exp01Doc.h : CMFC_Exp01Doc 클래스의 인터페이스
#pragma once

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
	unsigned char m_Threshold;			// 원하는 임계값을 결정
	afx_msg void OnArithmetic();		// 산술 연산 함수
	afx_msg void OnHistogram();			// 밝기 히스토그램 함수
	afx_msg void OnTobinary();			// 2진화 함수
	afx_msg void OnGonzalez();			// 곤잘레즈 함수
	afx_msg void OnOtsu();				// Otsu 함수
};
