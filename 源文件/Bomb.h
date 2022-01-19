#pragma once
#include "gameobject.h"

class CBomb :
	public CGameObject
{
public:
	CBomb(int x,int y);
	~CBomb(void);

	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+5,m_ptPos.y+BOMB_HEIGHT));
	}
private:
	static const int BOMB_HEIGHT = 11;
	static CImageList m_Images;

};
