#pragma once
#include <list>
using namespace std;
#define INI_HP 1
#define INI_ENEMY_HP 1
#define INI_PLAYER_HP 3
#define INI_BOSS_HP 50
#define ENEMY_SPEED 1
#define BULLIFE 100
#define HIT_SCORE 3
#define BOMB_NUM 3

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
class EnemyCraft: public Craft
{
public:
	EnemyCraft();
	EnemyCraft(int x, int y);
	int speedx, speedy;
	int enemyType;
	//~EnemyCraft();

private:

};
class Boss: public EnemyCraft {
public:
	Boss(int, int);
	//void BossIni(int, int);
	bool isEngaged;
private:
};
struct BULLET
{
	int posx, posy;
	int speedx, speedy;
	int lifetime;
};
class Bullets
{
private:
	int sizex, sizey;
	char* imgstr;
	list<BULLET> maga;
public:
	Bullets();
	friend class Control;
	void addbullet(int, int, int, int);
	void update();
};

