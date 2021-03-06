#pragma once
#include "GameObject.h"

class CBall :public CGameObject
{
public:
	CBall(int x,int y,int nMontion);
	~CBall(void);

	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+5,m_ptPos.y+BALL_HEIGHT));
	}
private:
	static const int BALL_HEIGHT=11;
	static CImageList m_Images;
	int    m_nMotion;
};
