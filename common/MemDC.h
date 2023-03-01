//如何使用
//#include"MemDCEx.h"
//1,加入类MemDC.h(我的文档\Resources\Skill_vc\Memdc.h)

//2
//OnDraw中
//CMemDCEx MemDC(pDC);
//并将绘制部分的pDC更换为MemDC

//或者在OnPaint中
//CPaintDC	dc(this);
//CMemDCEx	memDC(&dc);
//并将绘制部分的dc更换为memDC

//3,OnEraseBkgnd中
//return TRUE;
//直接返回真值
//如果还闪，就在所有和当前对话框有关的窗口中，把OnEraseBkgnd都改为return TRUE;

// 如果不是MFC环境，而是WTL或win32，则::InvalidateRect(hwnd, NULL, FALSE); 最后一个值要传FALSE，否则会闪

//注:
//更改背景颜色重载 虚拟函数 OnPrepareDC,
//pDC -> SetBkColor(RGB(..,..,..));
//


#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CMemDCEx - memory DC
//
// Author: Keith Rule
// Email: keithr@europa.com
// Copyright 1996-1999, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
// Added print support. - KR
//
// 11/3/99 Fixed most common complaint. Added
// background color fill. - KR
//
// 11/3/99 Added support for mapping modes other than
// MM_TEXT as suggested by Lee Sang Hun. - KR
//
// This class implements a memory Device Context which allows
// flicker free drawing.
#include "../thirdParty/WTL/atlgdi.h"

class CMemDCEx : public CDC
{
private:
	CBitmap m_bitmap;		 // Offscreen bitmap
	CBitmap m_oldBitmap;	 // bitmap originally found in CMemDCEx
	CDC* m_pDC;				 // Saves CDC passed in constructor
	CRect m_rect;			 // Rectangle of drawing area.
	BOOL m_bMemDC;			 // TRUE if CDC really is a Memory DC.

public:

	CMemDCEx(CDC* pDC, const CRect* pRect = NULL) : CDC()
	{
		//ASSERT(pDC != NULL);
		ATLASSERT(pDC != NULL); // WTL 是这个函数

		// Some initialization
		m_pDC = pDC;
		//m_oldBitmap = NULL;

//#ifndef _WIN32_WCE_NO_PRINTING
//        m_bMemDC = !pDC->IsPrinting();
//#else
//        m_bMemDC = FALSE;
//#endif

		m_bMemDC = TRUE;

		// Get the rectangle to draw
		if (pRect == NULL) 
		{
			pDC-> GetClipBox(&m_rect);
		} 
		else 
		{
			m_rect = *pRect;
		}

		if (m_bMemDC) 
		{
			// Create a Memory DC
			CreateCompatibleDC(pDC->m_hDC);
			pDC-> LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC->m_hDC, m_rect.Width(), m_rect.Height());
			//m_oldBitmap = SelectObject(&m_bitmap);
			m_oldBitmap = SelectBitmap(m_bitmap);

			SetMapMode(pDC->GetMapMode());
			pDC-> DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		} 
		//else 
		//{
		//	// Make a copy of the relevent parts of the current DC for printing
		//	m_bPrinting = pDC-> m_bPrinting;
		//	m_hDC = pDC-> m_hDC;
		//	m_hAttribDC = pDC-> m_hAttribDC;
		//}

		// Fill background
		FillSolidRect(m_rect, pDC->GetBkColor());
	}

	~CMemDCEx()
	{
		if (m_bMemDC) 
		{
			// Copy the offscreen bitmap onto the screen.
			//m_pDC-> BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			//	this, m_rect.left, m_rect.top, SRCCOPY);
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this->m_hDC, m_rect.left, m_rect.top, SRCCOPY);

			//Swap back the original bitmap.
			//SelectObject(m_oldBitmap);
			SelectBitmap(m_oldBitmap);
		} 
		else 
		{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			//m_hDC = m_hAttribDC = NULL;
            m_hDC = NULL;
		}
	}

	// Allow usage as a pointer
	CMemDCEx* operator-> ()
	{
		return this;
	}

	// Allow usage as a pointer
	operator CMemDCEx*()
	{
		return this;
	}
};

#endif