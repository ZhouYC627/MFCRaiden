
// MFCRaiden.h : MFCRaiden Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// CMFCRaidenApp:
// �йش����ʵ�֣������ MFCRaiden.cpp
//

class CMFCRaidenApp : public CWinApp
{
public:
	CMFCRaidenApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

#define WIN_WIDTH 680
#define WIN_HEIGHT 1080

#include <list>
using namespace std;
#define INI_HP 3
#define INI_ENEMY_HP 3
#define INI_PLAYER_HP 3
#define INI_BOSS_HP 100
#define ENEMY_SPEED 16
#define BULLIFE 100
#define HIT_SCORE 3
#define BOMB_NUM 3

enum GAME_STATE {
	GAME_EXIT = 0, GAME_START = 1, GAME_PLAYING = 2, GAME_OVER = 3
};

struct ImgCraft
{
	CImage img;
	CRect rect;
};
class Craft
{
public:
	Craft();
	Craft(int x, int y);
	~Craft();
	bool Move(int dx, int dy);
	int getX();
	int getY();
	int getspeed();
	int Hp;
	int bomb;
	//int setHp(int);
	//ImgCraft img;
	CImage img;
	CRect rect;

protected:
	int pos_x, pos_y;
	int speed;
};

class PlayerCraft : public Craft
{
public:
	PlayerCraft();
	PlayerCraft(int x, int y);
	~PlayerCraft();
	//int score;

private:

};
class EnemyCraft : public Craft
{
public:
	EnemyCraft();
	EnemyCraft(int x, int y);
	int speedx, speedy;
	int enemyType;
	//~EnemyCraft();

private:

};
class Boss : public EnemyCraft {
public:
	Boss(int, int);
	//void BossIni(int, int);
	bool isEngaged;
private:
};
struct BULLET
{
	int posx, posy;
	CRect rect;
	int speedx, speedy;
	int lifetime;
};
class Bullets
{
private:
	//int sizex, sizey;
	//char* imgstr;
	CImage img;
	list<BULLET> maga;
public:
	Bullets();
	friend class CMFCRaidenView;
	void addbullet(int, int, int, int);
	void update();
};
class Script
{
public:
	CString str;
	int posx, posy;
	int time;
};

extern CMFCRaidenApp theApp;
