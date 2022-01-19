#include "stdafx.h"
#include "background.h"
#include "resource.h"

background::background(void):x(0), y(0)
{
}
background::~background(void)
{
}
void background::drawmovebg(CDC* pDC, int y)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_BITMAP7);
	CBitmap* pbmpOld = memDC.SelectObject(&bmpDraw);
	pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, &memDC, 0, y, SRCCOPY);

	// TODO: 在此处添加实现代码.
}
