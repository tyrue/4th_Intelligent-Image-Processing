
// MFC_Exp01Doc.cpp : CMFC_Exp01Doc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
		ON_COMMAND(ID_32771, &CMFC_Exp01Doc::OnArithmetic) // ��� ���� �Լ� �߰�
	END_MESSAGE_MAP()


	// CMFC_Exp01Doc ����/�Ҹ�

	CMFC_Exp01Doc::CMFC_Exp01Doc()
	{
		// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
		m_InImage = NULL;	// �Է� �̹���
		m_OutImage = NULL;	// ��� �̹���
		height = 0;			// ����
		width = 0;			// �ʺ�
	}

	CMFC_Exp01Doc::~CMFC_Exp01Doc()
	{
		// �����Ҵ��� ������ ������
		if(m_InImage != NULL)
			delete(m_InImage);
		if(m_OutImage != NULL)
			delete(m_OutImage);
	}

	BOOL CMFC_Exp01Doc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
		// SDI ������ �� ������ �ٽ� ����մϴ�.

		return TRUE;
	}

	// CMFC_Exp01Doc serialization

	void CMFC_Exp01Doc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring()) // ����
		{
			// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�. 
			// ����ó�� �� ��¿��� m_OutImage[]�� raw �����ͷ�  ����  
			if(width != 0 && height != 0) 
			{ 
				ar.Write(m_OutImage, width*height); 
			}

		}
		else
		{
			// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
			CString strTemp = ar.m_strFileName.Right(3);
			// ���������� RAW�� ����Ǿ� �ִ� ��� BMP ������ ��
			if (toupper( strTemp[ 0 ] ) == 'B' && toupper( strTemp[ 1 ] ) == 'M' && toupper( strTemp[ 2 ] ) == 'P' ) 
			{ 
				// �Է��� ȭ���� ����Ʈ�� ������.
				CImage m_Image;  // �̹��� ��ü
				m_Image.Load(ar.m_strFileName); // �ҷ��� �̹����� �ʱ�ȭ 
				// ���̿� �ʺ�, �Է� �̹����� ��� �̹��� �ʱ�ȭ
				width = m_Image.GetWidth();		
				height = m_Image.GetHeight();
				m_InImage = (unsigned char*) malloc(sizeof(unsigned char) * width * height);
				m_OutImage = (unsigned char*) malloc(sizeof(unsigned char) * width * height);

				// GetBPP() : �� �ȼ��� �����ϴ� ��Ʈ ��
				int ws = width * (m_Image.GetBPP() / 8); // �� ������ ũ��(byte)
				// �ӽ� �̹��� ���� ���� �Ҵ�
				unsigned char *m_tmpImage = (unsigned char*) malloc(sizeof(unsigned char) * ws * height);

				if(m_Image.GetBPP() == 1) // �ȼ� ��Ʈ�� 1�̸� �� ����
				{
					AfxMessageBox((LPCTSTR)"���� �� �ִ� ���� ������ �ƴմϴ�.");
					return;
				} 
				else if(m_Image.GetBPP() == 8)// �ȼ� ��Ʈ�� 8
				{ 
					for (int y= 0; y < height; y++)
					{
						BYTE *srcImg = NULL;                   
						
						srcImg = (BYTE *) m_Image.GetPixelAddress(0,y);
						memcpy(&m_InImage[y * ws], srcImg, ws); // �̹��� ����
					}
				} 
				else if(m_Image.GetBPP() == 24) // �ȼ� ��Ʈ�� 24 
				{ 
					for (int y= 0; y <height; y++)
					{
						BYTE * srcImg = NULL;                   
						
						srcImg = (BYTE *) m_Image.GetPixelAddress(0,y);
						memcpy(&m_tmpImage[y * ws], srcImg, ws);  // �̹��� ����
					}
					int iter = 0; 
					for(int y=0; y<height; y++)
					{
						for(int x=0; x<width; x++)
						{
							m_InImage[(y * width) + x] = ( m_tmpImage[iter] * 0.21 + m_tmpImage[iter + 1] * 0.72 
								+ m_tmpImage[iter + 2] * 0.07); // �̹��� ����
							iter += 3;
						}
					}
				}
			}
			// �Է������� BMP������ �ƴѰ�� 
			else
			{
				AfxMessageBox((LPCTSTR)"���� �� �ִ� ���� ������ �ƴմϴ�.");
				return;
			}
		}
	}

#ifdef SHARED_HANDLERS

	// ����� �׸��� �����մϴ�.
	void CMFC_Exp01Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

	// �˻� ó���⸦ �����մϴ�.
	void CMFC_Exp01Doc::InitializeSearchContent()
	{
		CString strSearchContent;
		// ������ �����Ϳ��� �˻� �������� �����մϴ�.
		// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

		// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

	// CMFC_Exp01Doc ����

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
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		/************************** 1. ������� ó�� ********************************/
		int i, j; // �ε��� ó���� ���� ���� 
		int temp; // �ӽ� ������ ������ ���� ���� 
		for(i = 0; i < height; i++)
		{ 
			for(j = 0; j < width; j++)
			{ 
				// 1. ������ ����
				temp = m_InImage[(i*width) + j] / 2; 
				// 2. ����ó�� ��������� ȭ�鿡 ����ϱ� ���� ó��
				if(temp > 255) 
					m_OutImage[(i*width) + j] = 255; // 255�� �Ѿ�� �ִ밪 255��
				else if(temp < 0) 
					m_OutImage[(i*width) + j] = 0; // 0���� ������ �ּҰ� 0 ����
				else 
					m_OutImage[(i*width) + j] = (unsigned char)temp; 
			}
		}     
		// 3. ���� ��µǾ� �ִ� ��� ȭ���� �����Ͽ�  �ٽ� ����Ѵ�. 
		UpdateAllViews(NULL); 
	}
