#include "Craft.h"

Craft::Craft() {
	pos_x = 100;
	pos_y = 100;
	Hp = INI_HP;
}
Craft::Craft(int x, int y)
{
	pos_x = x;
	pos_y = y;
	speed = ENEMY_SPEED;
}

Craft::~Craft()
{
}


PlayerCraft::PlayerCraft(int x, int y) : Craft(x, y)
{
	Hp = INI_PLAYER_HP;
	//score = 0;
	bomb = BOMB_NUM;
}
PlayerCraft::PlayerCraft() :Craft() {
	Hp = INI_PLAYER_HP;
	//score = 0;
	bomb = BOMB_NUM;
}

PlayerCraft::~PlayerCraft()
{
}
EnemyCraft::EnemyCraft() {
}
EnemyCraft::EnemyCraft(int x, int y) :Craft(x, y) {
	Hp = INI_ENEMY_HP;
	speedx = ENEMY_SPEED;
	speedy = 0;
	enemyType = 0;
}

Boss::Boss(int x, int y) {
	Hp = INI_BOSS_HP;
	speedx = 0;
	speedy = 1;
	enemyType = 1;
	isEngaged = false;
}

bool Craft::Move(int dx, int dy) {
	pos_x += dx;
	pos_y += dy;
	return true;
}
int Craft::getX() {
	return pos_x;
}
int Craft::getY() {
	return pos_y;
}
int Craft::getspeed() {
	return speed;
}

Bullets::Bullets() {
	sizex = 1;
	sizey = 1;
}
void Bullets::addbullet(int x, int y, int sx, int sy) {
	BULLET b;
	b.posx = x;
	b.posy = y;
	b.speedx = sx;
	b.speedy = sy;
	b.lifetime = 0;
	maga.push_back(b);
}
void Bullets::update() {
	list<BULLET>::iterator it;
	for (it = maga.begin(); it != maga.end(); it++)
	{
		it->posx -= it->speedx;
		it->posy -= it->speedy;
	}
	maga.remove_if([](BULLET n) {return n.lifetime >= BULLIFE; });
}