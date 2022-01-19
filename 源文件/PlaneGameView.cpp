// PlaneGameView.cpp : CPlaneGameView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int flag = 0;

// CPlaneGameView ����/����

CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView ����

void CPlaneGameView::OnDraw(CDC* pDC)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPlaneGameView ��ӡ

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPlaneGameView ���

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView ��Ϣ�������
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: �ڴ����ר�ô����/����û���
	//��ʼ����Ϸ
	InitGame();
	hThread=CreateThread(NULL,  0,  (LPTHREAD_START_ROUTINE)ThreadFunc,  NULL,  0,  &ThreadID);
	isPause=1;
	if(AfxMessageBox(L"�鿴��Ϸ˵���������ǡ�����ֱ�ӽ�����Ϸ�������񡱣�",MB_YESNO)==6)
	{
		AfxMessageBox(L"���������ս�����򣬿ո�������Z����ͣ��S����ʼ��U�������޵�ģʽ��.");
		
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
	
	//�������������
	srand( (unsigned)time( NULL ) );

	//�����豸DC
	m_pDC = new CClientDC(this);

	//�����ڴ�DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//�����ڴ�λͼ
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//��λͼѡ���ڴ�DC
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
	//��������(ս��)
	m_pMe = new CMyPlane;

	//������Ϸ
	SetTimer(1,30,NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{

	//�������
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
	CString str=_T("��ǰ�÷֣�" + d);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 0,str);

	CString d2;
	d2.Format(_T("%d"),life);
	CString str2=_T("��ǰ����ֵ��" + d2);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 20,str2);

	CString d3;
	d3.Format(_T("%d"),level);
	CString str3=_T("��ǰ�ؿ���" + d3);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_LEFT);
	pMemDC->SetTextColor(RGB(255,192,203));
	pMemDC->TextOut(0, 40,str3);

	if(GetKeyState('Z')<0){//��ͣ
		isPause=1;
	}
	if(GetKeyState('S')<0){//��ʼ
		isPause=0;
	}
	if(GetKeyState('U')<0){//�޵�
		MAXXXX = 1;
	}

	if(isPause ==0){
		//�����ҷ�ս��
		if(m_pMe!=NULL)
		{
		   m_pMe->Draw(m_pMemDC,FALSE);
		   if(flag == 1){
				flag = 0;
				KillTimer(1);
				if(AfxMessageBox(L"ս���ѱ����䣡���ȷ�����¿�ʼ��Ϸ��",MB_YESNO)==6)
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
			if(AfxMessageBox(L"�Ѷ������ˣ�",MB_OK))
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
			//���� ��������ը���л����ӵ�,UB1,UB2,FA
			for(int i=0;i<7;i++)
			{
				POSITION pos1,pos2;
				for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
				{
					CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );//���ص�ǰλ�õ�Ԫ��,Ȼ��pos1ָ����һ��Ԫ��
					if(!pObj->Draw(pMemDC,FALSE))
					{
						m_ObjList[i].RemoveAt(pos2);
						delete pObj;
					}
				}
			}
		}
	
	//�����ڴ�DC���豸DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	if(isPause ==0){
		static int nCreator = rand() %5+10;

		//��������л�
		if(nCreator<=0)
		{
			nCreator = rand()%5+10-level;
			m_ObjList[enEnemy].AddTail(new CEnemy);
			
		}
		nCreator--;
		
		static int nCreatorFA = rand() %5+10;

		//�������FA
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

		//����ĸ���������ƶ�ս��
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
	
		//����ս������
		if(GetKey(VK_SPACE)==1)//�����˿ո��
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

		//�л�UB1�����ӵ�
		CPoint PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enUB1].GetHeadPosition();ePos!=NULL;)
		{
			  
			UltraBoss* UB1 = (UltraBoss*)m_ObjList[enUB1].GetNext(ePos);
			  if(!UB1->Fired())
				  continue;
			  CPoint  ePt = UB1->GetPoint();

			  BOOL by=FALSE;

			  //�л���ս��ǰ��
			  if(UB1->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //�л���ս������
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

		//�л�UB2�����ӵ�
		PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enUB2].GetHeadPosition();ePos!=NULL;)
		{
			  
			UltraBoss* UB2 = (UltraBoss*)m_ObjList[enUB2].GetNext(ePos);
			  if(!UB2->Fired())
				  continue;
			  CPoint  ePt = UB2->GetPoint();

			  BOOL by=FALSE;

			  //�л���ս��ǰ��
			  if(UB2->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //�л���ս������
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
		//�л������ӵ�

		PlanePt = m_pMe->GetPoint();
		for(POSITION ePos=m_ObjList[enEnemy].GetHeadPosition();ePos!=NULL;)
		{
			  
			  CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
			  if(!pEnemy->Fired())
				  continue;
			  CPoint  ePt = pEnemy->GetPoint();

			  BOOL by=FALSE;

			  //�л���ս��ǰ��
			  if(pEnemy->GetMontion()==1 && ePt.y<PlanePt.y)
					 by=  TRUE;
			  //�л���ս������
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
		//�ɻ���ײ
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
				//��ӱ�ըЧ��
					m_ObjList[enExplosion].AddTail(
						new CExplosion(bRect.left,bRect.top)
						);
					mciSendString(L"open me_down.wav",NULL,0,NULL);
					mciSendString(L"play me_down.wav",NULL,0,NULL);
					//PlaySound((LPCTSTR)IDR_WAVE3, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
					//ɾ��ս��
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//ɾ������
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//ɾ������
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//ɾ������
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//ɾ��
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//ɾ��
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(bRect.left,bRect.top)
							);
						//ɾ��
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
		//�л��ӵ�ը��ս��
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
				if(life <= 0){//��ӱ�ըЧ��
					m_ObjList[enExplosion].AddTail(
						new PlaneExplosion(mRect.left,mRect.top)
						);
					mciSendString(L"open D:\\PlaneGame\\PlaneGame\\res\\sound\\me_down.wav",NULL,0,NULL);
					mciSendString(L"play D:\\PlaneGame\\PlaneGame\\res\\sound\\me_down.wav",NULL,0,NULL);
					//PlaySound((LPCTSTR)IDR_WAVE3, AfxGetInstanceHandle(), SND_RESOURCE | SND_NOSTOP | SND_SYNC);
					//ɾ���ӵ�
					m_ObjList[enBall].RemoveAt(bPos2);
					delete pBall;

					//ɾ��ս��
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
		//ս������ը���л�UB1
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//ɾ������
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;

						//ɾ���л�
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
		//ս������ը���л�UB2
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
						//��ӱ�ըЧ��
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mRect.left,mRect.top)
							);
						//ɾ������
						m_ObjList[enBomb].RemoveAt(mPos2);
						delete pBomb;

						//ɾ���л�
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

		//ս������ը���л�
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
					//��ӱ�ըЧ��
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left,mRect.top)
						);
					//ɾ������
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;

					//ɾ���л�
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
	//ˢ����Ϸ֡����: ���ڴ�DC�ϻ�ͼ
	
	UpdateFrame(m_pMemDC);
	AI();
	
	CView::OnTimer(nIDEvent);
}



void CPlaneGameView::OnDestroy()
{
	CView::OnDestroy();
	this->StopGame();
	// TODO: �ڴ˴������Ϣ����������
}


BOOL CPlaneGameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	return CView :: OnEraseBkgnd(pDC);
	//return 0;
}


int CPlaneGameView::Gameover(void)
{
	
	return 0;
}
