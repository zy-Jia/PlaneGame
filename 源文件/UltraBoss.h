#pragma once
#include "gameobject.h"
class UltraBoss:public CGameObject
{
public:
	UltraBoss(void);
	~UltraBoss(void);
	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+69,m_ptPos.y+99));
	}

	int GetMontion() const
	{
		return m_nMotion;
	}
	//�Ƿ���Կ������ӵ�
	BOOL Fired();
private:
	static const int ENEMY_HEIGHT = 69;
	static CImageList m_Images;
	int    m_nMotion;//���� 1->���� 0->ֹͣ -1->����
	//ͼ������
	int m_nImgIndex;
	//�ٶ�
	int m_V;
	int    m_nWait;//������ʱ
};

