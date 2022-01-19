// PlaneGameView.cpp : CPlaneGameView 类的实现
//

#include "stdafx.h"
#include "PlaneGame.h"

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Ball.h"
#include "PlaneExplosion.h"
#include "UltraBoss.h"
#include "UltraBoss2.h"
#include "FirstAid.h"
#include "Explosion.h"
#include <atlimage.h>
#include "resource.h"
#include <Windows.h>
#include <iostream>
#include "string.h"
#include <process.h> 
#include<iomanip>
#include <MMSystem.h>
using namespace std;
#pragma comment (lib,"Winmm.lib");

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int flag = 0;

// CPlaneGameView 构造/析构

CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: 在此处添加构造代码
	level = score = 0;
	life = 100;
	bosslife = 100;
	isPause = 0;
	lboss = bboss = 0;
	ubosslife=400;
	MAXXXX = 0;
	upflag = 0;
	//m_bitmap = new CBitmap;
	//m_bitmap -> LoadBitmap(IDB_BITMAP7);
	//m_brushbackground.CreatePatternBrush(m_bitmap);
	m_bitmap2 = new CBitmap;
	m_bitmap2 -> LoadBitmap(IDB_BITMAP10);
	m_brushbackground2.CreatePatternBrush(m_bitmap2);
}

CPlaneGameView::~CPlaneGameView()
{

}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* pDC)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	//初始化游戏
	InitGame();
	hThread=CreateThread(NULL,  0,  (LPTHREAD_START_ROUTINE)ThreadFunc,  NULL,  0,  &ThreadID);
	isPause=1;
	if(AfxMessageBox(L"查看游戏说明请点击“是”，若直接进入游戏请点击“否”！",MB_YESNO)==6)
	{
		AfxMessageBox(L"方向键控制战机方向，空格键射击，Z键暂停，S键开始，U键开启无敌模式！.");
		
	}isPause=0;
}
void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
}
void CPlaneGameView::ThreadFunc(){
	//sndPlaySound(L"bgm.wav",SND_ASYNC); 
}
BOOL CPlaneGameView::InitGame()
{
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_LOOP | SND_NOSTOP | SND_ASYNC);
	CRect rc;
	GetClientRect(rc);
	
	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();
	PlaneExplosion::LoadImage();
	UltraBoss::LoadImage();
	UltraBoss2::LoadImage();
	FirstAid::LoadImage();
	//产生主角(战机)
	m_pMe = new CMyPlane;

	//启动游戏
	SetTimer(1,30,NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{

	//绘制天空
	//pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));
	//CView :: OnEraseBkgnd(pDC);
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int count = 1;
	if (count == 1 && pDoc->bg1.y == 0) {
		pDoc->bg2.y = GAME_HEIGHT;
		count = 0;
	}
	pDoc->bg1.drawmovebg(pMemDC, pDoc->bg1.y);
	pDoc->bg2.drawmovebg(pMemDC, pDoc->bg2.y);
	pDoc->bg1.y -= 5;
	pDoc->bg2.y -= 5;
	if (pDoc->bg1.y < -GAME_HEIGHT) {
		pDoc->bg1.y = GAME_HEIGHT;
	}
	if (pDoc->bg2.y < -GAME_HEIGHT) {
		pDoc->bg2.y = GAME_HEIGHT;
	}


	if(isPause == 0){
		CDC dcMem;
		CClientDC dc(this);
		HBITMAP hbit;

		dcMem.CreateCompatibleDC(&dc);
		//hbit = (HBITMAP)dcMem.SelectObject(m_bitmap);
		CRect rect;
		GetClientRect(&rect);
		//pMemDC -> FillRect(rect, &m_brushbackground);
	}
	else{
		CDC dcMem;
		CClientDC dc(this);
		HBITMAP hbit;

		dcMem.CreateCompatibleDC(&dc);
		hbit = (HBITMAP)dcMem.SelectObject(m_bitmap2);
		CRect rect;
		GetClientRect(&rect);
		pMemDC -> FillRect(rect, &m_brushbackground2);
	}
	CString d;
	d.Format(_T("%d"),score);
	CString str=_T("当前得分：" + d);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 0,str);

	CString d2;
	d2.Format(_T("%d"),life);
	CString str2=_T("当前生命值：" + d2);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 20,str2);

	CString d3;
	d3.Format(_T("%d"),level);
	CString str3=_T("当前关卡：" + d3);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 40,str3);

	if(GetKeyState('Z')<0){//暂停
		isPause=1;
	}
	if(GetKeyState('S')<0){//开始
		isPause=0;
	}
	if(GetKeyState('U')<0){//无敌
		MAXXXX = 1;
	}

	if(isPause ==0){
		//绘制我方战机
		if(m_pMe!=NULL)
		{
		   m_pMe->Draw(m_pMemDC,FALSE);
		   if(flag == 1){
				flag = 0;
				KillTimer(1);
				if(AfxMessageBox(L"战机已被击落！点击确认重新开始游戏！",MB_YESNO)==6)
				{
					for(int i = 0; i < 7; ++i)
						m_ObjList[i].RemoveAll();
					m_pMe->Draw(m_pMemDC,FALSE);
					level = score = 0;
					life = 100;
					lboss = 0;
					bosslife = 100;
					ubosslife=400;
					SetTimer(1,30,NULL);
				}
			}
		}
		if(upflag == 1){
			upflag = 0;
			KillTimer(1);
			if(AfxMessageBox(L"难度提升了！",MB_OK))
					{
						for(int i = 0; i < 7; ++i)
							m_ObjList[i].RemoveAll();
						m_pMe->Draw(m_pMemDC,FALSE);
						life = 100;
						lboss = 0;
						bosslife = 100;
						ubosslife=400;
						SetTimer(1,30,NULL);			
					}		
			}
			//绘制 导弹、爆炸、敌机、子弹,UB1,UB2,FA
			for(int i=0;i<7;i++)
			{
				POSITION pos1,pos2;
				for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
				{
					CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );//返回当前位置的元素,然后将pos1指向下一个元素
					if(!pObj->Draw(pMemDC,FALSE))
					{
						m_ObjList[i].RemoveAt(pos2);
						delete pObj;
					}
				}
			}
		}
	
	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	if(isPause ==0){
		static int nCreator = rand() %5+10;

		//随机产生敌机
		if(nCreator<=0)
		{
			nCreator = rand()%5+10-level;
			m_ObjList[enEnemy].AddTail(new CEnemy);
			
		}
		nCreator--;
		
		static int nCreatorFA = rand() %5+10;

		//随机产生FA
		if(nCreatorFA<=0)
		{
			nCreatorFA = rand()%5+100;
			m_ObjList[enFirstAid].AddTail(new FirstAid);
			
		}
		nCreatorFA--;


		static int nCreatorUB1 = rand() %5+300;
		if(nCreatorUB1<=0)
		{
			
			nCreatorUB1 = rand()%5+300;
			m_ObjList[enUB1].AddTail(new UltraBoss);
			
		}
		nCreatorUB1--;

		static int nCreatorUB2 = rand() %5+400;
		if(nCreatorUB2<=0)
		{
			
			nCreatorUB2 = rand()%5+400;
			m_ObjList[enUB2].AddTail(new UltraBoss2);
			level += 1;
			life = 100;
		}
		nCreatorUB2--;

		if(m_pMe==NULL)
			return;

		//检测四个方向键，移动战机
		//for(int i=0;i<4;i++)
		//{
			int nMeMotion=0;
			m_pMe->SetVerMotion(0);
			m_pMe->SetHorMotion(0);

			nMeMotion = GetKey(VK_UP);
			if(nMeMotion==1)
				m_pMe->SetVerMotion(1);
		    
			nMeMotion = GetKey(VK_DOWN);
			if(nMeMotion==1)
				m_pMe->SetVerMotion(-1);

			nMeMotion = GetKey(VK_RIGHT);
			if(nMeMotion==1)
				m_pMe->SetHorMotion(1);

			nMeMotion = GetKey(VK_LEFT);
			if(nMeMotion==1)
				m_pMe->SetHorMotion(-1);
		//}
	
		//产生战机导弹
		if(GetKey(VK_SPACE)==1)//按下了空格键
		{

			if(m_pMe!=NULL && m_pMe->Fired())
			{
				
				CPoint pt = m_pMe->GetPoint();
				//mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				//mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				//PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
				m_ObjList[enBomb].AddTail(new CBomb(pt.x+35,pt.y+10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x+65,pt.y+10));
			}
		}
		static int nbullet = rand() %5+10;

		//敌机UB1发射子弹
		CPoint PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enUB1].GetHeadPosition();ePos!=NULL;)
		{
			  
			UltraBoss* UB1 = (UltraBoss*)m_ObjList[enUB1].GetNext(ePos);
			  if(!UB1->Fired())
				  continue;
			  CPoint  ePt = UB1->GetPoint();

			  BOOL by=FALSE;

			  //敌机在战机前面
			  if(UB1->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //敌机在战机后面
			  //if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
					 //by=  TRUE;
			  if(nbullet<=0)
			  {
				  nbullet = rand()%5;
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+15,ePt.y+20,UB1->GetMontion()));
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+55,ePt.y+40,UB1->GetMontion()));
			  }
				nbullet--;
			  //if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH)
			  //{
				  //mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);

			  //}
		}

		//敌机UB2发射子弹
		PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enUB2].GetHeadPosition();ePos!=NULL;)
		{
			  
			UltraBoss* UB2 = (UltraBoss*)m_ObjList[enUB2].GetNext(ePos);
			  if(!UB2->Fired())
				  continue;
			  CPoint  ePt = UB2->GetPoint();

			  BOOL by=FALSE;

			  //敌机在战机前面
			  if(UB2->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //敌机在战机后面
			  //if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
					 //by=  TRUE;

			  //if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH)
			  //{
				  //mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
			  			  if(nbullet<=0)
			  {
				  nbullet = rand()%5;
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+35,ePt.y+10,UB2->GetMontion()));
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+135,ePt.y+10,UB2->GetMontion()));
			  }
				nbullet--;

			  //}
		}
		//敌机发射子弹

		PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enEnemy].GetHeadPosition();ePos!=NULL;)
		{
			  
			  CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
			  if(!pEnemy->Fired())
				  continue;
			  CPoint  ePt = pEnemy->GetPoint();

			  BOOL by=FALSE;

			  //敌机在战机前面
			  if(pEnemy->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //敌机在战机后面
			  //if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
					 //by=  TRUE;

			  if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH)
			  {
				  //mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\bullet.wav",NULL,0,NULL);
				  //PlaySound((LPCTSTR)IDR_WAVE2, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+15,ePt.y+10,pEnemy->GetMontion()));
				  m_ObjList[enBall].AddTail(new CBall(ePt.x+40,ePt.y+10,pEnemy->GetMontion()));
			  }
		}

		//FA GET
		POSITION bPos1=NULL,bPos2=NULL;
		CRect mRect = m_pMe->GetRect();
		for(bPos1=m_ObjList[enFirstAid].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
		{
			FirstAid* pFirstAid = (FirstAid*)m_ObjList[enFirstAid].GetNext(bPos1);
			CRect bRect = pFirstAid->GetRect();
			CRect tmpRect;
			if(tmpRect.IntersectRect(&bRect,mRect))
			{
				if(life+40 >=100)	
					life = 100;
				else
					life+=40;
				m_ObjList[enFirstAid].RemoveAt(bPos2);
				delete pFirstAid;
			}
		}
		//飞机碰撞
		bPos1=NULL,bPos2=NULL;
		mRect = m_pMe->GetRect();
		for(bPos1=m_ObjList[enEnemy].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(bPos1);
			CRect bRect = pEnemy->GetRect();
			CRect tmpRect;
			if(tmpRect.IntersectRect(&bRect,mRect))
			{
				if(MAXXXX == 0){
					life -= 10;
				}
				//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(bRect.left,bRect.top)
						);
					mciSendString(L"open me_down.wav",NULL,0,NULL);
					mciSendString(L"play me_down.wav",NULL,0,NULL);
					//PlaySound((LPCTSTR)IDR_WAVE3, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
					//删除战机
					m_ObjList[enEnemy].RemoveAt(bPos2);
					delete pEnemy;
											pflag = 1;
						break;

				}
							if(pflag == 1){
				pflag = 0;
				break;
			}
		}
		POSITION mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enEnemy].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(mPos1);
			CRect bRect = pEnemy->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB1].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				UltraBoss* pUltraBoss = (UltraBoss*)m_ObjList[enUB1].GetNext(ePos1);
				CRect mRect = pUltraBoss->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//删除导弹
						m_ObjList[enEnemy].RemoveAt(mPos2);
						//delete pEnemy;
												pflag = 1;
						break;

				}
							if(pflag == 1){
				pflag = 0;
				break;
			}

			}
		}
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enUB1].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			UltraBoss* pUltraBoss = (UltraBoss*)m_ObjList[enUB1].GetNext(mPos1);
			CRect bRect = pUltraBoss->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB2].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				UltraBoss2* pUltraBoss2 = (UltraBoss2*)m_ObjList[enUB2].GetNext(ePos1);
				CRect mRect = pUltraBoss2->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//删除导弹
						m_ObjList[enUB1].RemoveAt(mPos2);
						//delete pUltraBoss;
						pflag = 1;
						
						break;

				}
							if(pflag == 1){
				pflag = 0;
				break;
			}

			}
		}
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enEnemy].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(mPos1);
			CRect bRect = pEnemy->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB2].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				UltraBoss2* pUltraBoss2 = (UltraBoss2*)m_ObjList[enUB2].GetNext(ePos1);
				CRect mRect = pUltraBoss2->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//删除导弹
						m_ObjList[enEnemy].RemoveAt(mPos2);
						//delete pEnemy;
						pflag = 1;break;
						
				}

			}
						if(pflag == 1){
				pflag = 0;
				break;
			}
		}
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enEnemy].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(mPos1);
			CRect bRect = pEnemy->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enEnemy].GetHeadPosition();mPos2 != ePos1 && (ePos2=ePos1)!=NULL;)
			{
				CEnemy* pEnemy2 = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy2->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//删除
						m_ObjList[enEnemy].RemoveAt(ePos2);
						//delete pEnemy;
						pflag = 1;break;
						
				}

			}
						if(pflag == 1){
				pflag = 0;
				break;
			}
		}
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enUB1].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			UltraBoss* pUltraBoss = (UltraBoss*)m_ObjList[enUB1].GetNext(mPos1);
			CRect bRect = pUltraBoss->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB1].GetHeadPosition();mPos2 != ePos1 && (ePos2=ePos1)!=NULL;)
			{
				UltraBoss* pUltraBoss2 = (UltraBoss*)m_ObjList[enUB1].GetNext(ePos1);
				CRect mRect = pUltraBoss2->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//删除
						m_ObjList[enUB1].RemoveAt(ePos2);
						//delete pEnemy;
						pflag = 1;break;
						
				}

			}
			if(pflag == 1){
				pflag = 0;
				break;
			}
		}
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enUB2].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			UltraBoss2* pUltraBoss2 = (UltraBoss2*)m_ObjList[enUB2].GetNext(mPos1);
			CRect bRect = pUltraBoss2->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB2].GetHeadPosition();mPos2 != ePos1 && (ePos2=ePos1)!=NULL;)
			{
				UltraBoss2* pUltraBoss22 = (UltraBoss2*)m_ObjList[enUB2].GetNext(ePos1);
				CRect mRect = pUltraBoss22->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//删除
						m_ObjList[enUB2].RemoveAt(mPos2);
						//delete pEnemy;
						pflag = 1;
						break;

				}
							if(pflag == 1){
				pflag = 0;
				break;
			}
			}
		}
		//敌机子弹炸掉战机
		bPos1=NULL,bPos2=NULL;
		mRect = m_pMe->GetRect();
		for(bPos1=m_ObjList[enBall].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
		{
			CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
			CRect bRect = pBall->GetRect();
			CRect tmpRect;
			if(tmpRect.IntersectRect(&bRect,mRect))
			{
				if(MAXXXX == 0){
					life -= 10;
				}
				if(life <= 0){//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new PlaneExplosion(mRect.left,mRect.top)
						);
					mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\me_down.wav",NULL,0,NULL);
					mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\me_down.wav",NULL,0,NULL);
					//PlaySound((LPCTSTR)IDR_WAVE3, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
					//删除子弹
					m_ObjList[enBall].RemoveAt(bPos2);
					delete pBall;

					//删除战机
					//delete m_pMe;
					//m_pMe=NULL;
					flag = 1;
					break;
				}
				else{
					m_ObjList[enBall].RemoveAt(bPos2);
					delete pBall;

				}

			}
		}
		//战机导弹炸掉敌机UB1
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enBomb].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB1].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				UltraBoss* pUltraBoss = (UltraBoss*)m_ObjList[enUB1].GetNext(ePos1);
				CRect mRect = pUltraBoss->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
					bosslife -= 2;
					if(bosslife <= 0){
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//删除导弹
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;

						//删除敌机
						m_ObjList[enUB1].RemoveAt(ePos2);
						delete pUltraBoss;
						lboss -= 1;
						score += 10;
						bosslife = 100;
						
						
						
					}
					else{
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;
					}
				}

			}
		}
		//战机导弹炸掉敌机UB2
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enBomb].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enUB2].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				UltraBoss2* pUltraBoss2 = (UltraBoss2*)m_ObjList[enUB2].GetNext(ePos1);
				CRect mRect = pUltraBoss2->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
					ubosslife -= 10;
					if(ubosslife <= 0){
						//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//删除导弹
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;

						//删除敌机
						m_ObjList[enUB2].RemoveAt(ePos2);
						delete pUltraBoss2;
						ubosslife=400;
						score += 30;
						lboss -= 1;
						upflag = 1;
						break;
					}
					else{
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;
					}
				}
			}
		}

		//战机导弹炸掉敌机
		mPos1=NULL,mPos2=NULL;
		for(mPos1=m_ObjList[enBomb].GetHeadPosition();(mPos2=mPos1)!=NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1=NULL,ePos2=NULL;
			for(ePos1=m_ObjList[enEnemy].GetHeadPosition();(ePos2=ePos1)!=NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if(tmpRect.IntersectRect(&bRect,mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left,mRect.top)
						);
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;

					//删除敌机
					m_ObjList[enEnemy].RemoveAt(ePos2);
					delete pEnemy;

					score += 1;
					
					break;

				}
			}
		}
	}
}
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	//刷新游戏帧画面: 在内存DC上绘图
	
	UpdateFrame(m_pMemDC);
	AI();
	
	CView::OnTimer(nIDEvent);
}



void CPlaneGameView::OnDestroy()
{
	CView::OnDestroy();
	this->StopGame();
	// TODO: 在此处添加消息处理程序代码
}


BOOL CPlaneGameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	return CView :: OnEraseBkgnd(pDC);
	//return 0;
}


int CPlaneGameView::Gameover(void)
{
	
	return 0;
}
