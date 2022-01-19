#include "stdafx.h"
#include "FirstAid.h"
#include "resource.h"
CImageList FirstAid::m_Images;

FirstAid::FirstAid(void)
{
	//随机确定X位置
	m_ptPos.x = rand()%(GAME_WIDTH-ENEMY_HEIGHT)+1;

	//随机确定图像索引
	m_nImgIndex = rand()%2;

	//根据图像索引确定方向
	m_nMotion=1;
	m_ptPos.y=-ENEMY_HEIGHT;
	if (m_nImgIndex%2!=0)//如果是图像索引是偶数
	{
		m_nMotion=1;
		m_ptPos.y = GAME_HEIGHT+ENEMY_HEIGHT;
	}
	//随机确定速度
	m_V = rand()%3+2;

	m_nWait=0;
}


FirstAid::~FirstAid(void)
{
}
BOOL FirstAid::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_BITMAP13,RGB(0,0,0),58,88,1);
}
BOOL FirstAid::Draw(CDC* pDC,BOOL bPause)
{
	m_nWait++;
	if(m_nWait>20)
		m_nWait=0;

	if(!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * m_V;
	}

	if(m_ptPos.y > GAME_HEIGHT+ENEMY_HEIGHT )
		return FALSE;
	if(m_ptPos.y < -ENEMY_HEIGHT)
		return FALSE;
	
	
	m_Images.Draw(pDC,m_nImgIndex,m_ptPos,ILD_TRANSPARENT);

	return TRUE;
}
BOOL FirstAid::Fired()
{
	if(m_nWait==0)
		return TRUE;
	else
		return FALSE;
}