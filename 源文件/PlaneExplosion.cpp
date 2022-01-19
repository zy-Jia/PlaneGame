#include "stdafx.h"
#include "PlaneExplosion.h"
#include "resource.h"

CImageList PlaneExplosion::m_Images;

PlaneExplosion::PlaneExplosion(int x,int y):CGameObject(x,y),m_nProcess(0)
{
}


PlaneExplosion::~PlaneExplosion(void)
{
}
BOOL PlaneExplosion::Draw(CDC* pDC,BOOL bPause)
{
	//ֻ��15��ͼƬ
	if(m_nProcess==15)
	{
		m_nProcess=0;
		return FALSE;
	}

	//����λ�û���ͼ��
	m_Images.Draw(pDC,m_nProcess,m_ptPos,ILD_TRANSPARENT);
	m_nProcess++;
	return TRUE;
}

BOOL PlaneExplosion::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_BITMAP8,RGB(0,0,0),102,126,1);
}