#pragma once
class background
{
public:
	background(void);
	virtual ~background(void);
	void drawmovebg(CDC* pDC, int y);
	int x;
	int y;
};

