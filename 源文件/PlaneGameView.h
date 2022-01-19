// PlaneGameView.h : CPlaneGameView ��Ľӿ�
//


#pragma once

class CMyPlane;
class CPlaneGameView : public CView
{


	enum ObjType{enEnemy,enBomb,enBall,enExplosion, enUB1, enUB2, enFirstAid};

protected: // �������л�����
	CPlaneGameView();
	DECLARE_DYNCREATE(CPlaneGameView)

// ����
public:
	CPlaneGameDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CPlaneGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//�ڴ�DC
	CDC*        m_pMemDC;
	//�豸DC
	CClientDC*  m_pDC;
	//�ڴ�λͼ
	CBitmap*    m_pMemBitmap;


protected:
	//��ʼ����Ϸ
	BOOL InitGame();
	//������Ϸ
	void StopGame();

	//ˢ����Ϸ��֡����
	void UpdateFrame(CDC* pMemDC);


	void AI();

	//��ü���״̬1->down
	int GetKey(int nVirtKey)
	{
		return (GetKeyState(nVirtKey) & 0x8000)? 1 : 0;
	}
	
	CMyPlane*  m_pMe;
	CObList    m_ObjList[7];
	HANDLE hThread;  DWORD ThreadID;
	int isPause;
	static void ThreadFunc();
	public:
		int MAXXXX;
		int life;
		int level;
		int score;
		int lifenum;
		int bosslife;
		int ubosslife;
		int lboss;
		int bboss;
		int upflag;
		int pflag;
		//CBitmap *m_bitmap;
		CBitmap *m_bitmap2;
		//CBrush m_brushbackground;
		CBrush m_brushbackground2;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int Gameover(void);
};

#ifndef _DEBUG  // PlaneGameView.cpp �еĵ��԰汾
inline CPlaneGameDoc* CPlaneGameView::GetDocument() const
   { return reinterpret_cast<CPlaneGameDoc*>(m_pDocument); }
#endif

