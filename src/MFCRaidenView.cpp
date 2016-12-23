
// MFCRaidenView.cpp : CMFCRaidenView 类的实现
//
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCRaiden.h"
#endif

#include "MFCRaidenDoc.h"
#include "MFCRaidenView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define REFRESHTIME 50
#define INCOMING_TIME 400
#define LEVEL2_SCORE 100
#define LEVEL3_SCORE 500
#define BOSS_SCORE 1000

// CMFCRaidenView

IMPLEMENT_DYNCREATE(CMFCRaidenView, CView)

BEGIN_MESSAGE_MAP(CMFCRaidenView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	//	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCRaidenView 构造/析构

CMFCRaidenView::CMFCRaidenView()
{
	// TODO: 在此处添加构造代码
	//client.SetSize(680, 1080);
	client.SetSize(WIN_WIDTH, WIN_HEIGHT);
	winRect.SetRect(0, 0, WIN_WIDTH, WIN_HEIGHT);
	//bgd.Load(_T("res\\bgd2.jpg"));
	bgd.Load(_T("res\\welcome2.png"));
	playerCraft.img.Load(_T("res\\craft4.png"));
	bullets.img.Load(_T("res\\bullet2.png"));
	enemybullets.img.Load(_T("res\\bullet3.png"));
	enemyImg.Load(_T("res\\enemy2.png"));
	enemy3Img.Load(_T("res\\enemy3.png"));
	enemy4Img.Load(_T("res\\enemy4.png"));
	welcome.Load(_T("res\\welcome2.png"));
	startIconN.Load(_T("res\\stn.png"));
	startIconY.Load(_T("res\\sty.png"));
	bossImg.Load(_T("res\\boss.png"));

	scorebar.posx = 0;
	scorebar.posy = 0;
	icon = NULL;
	isSelected = false;
	
	//start icon
	iconRect.SetRect(
		(WIN_WIDTH - startIconN.GetWidth()) / 2,
		WIN_HEIGHT / 3 * 2,
		(WIN_WIDTH + startIconN.GetWidth()) / 2,
		WIN_HEIGHT / 3 * 2 + startIconN.GetHeight()
		);
	//other init
	GameInit();
	//state = GAME_PLAYING;
	state = GAME_START;
}

void CMFCRaidenView::GameInit() {
	level = 1;	score = 0;	bomb = 3;
	BossEngaged = 0; bossShot = 0; bossSleep = 0;
	//init playerCraft
	int w = (client.cx - playerCraft.img.GetWidth()) / 2;
	playerCraft.rect.SetRect(w, client.cy - playerCraft.img.GetHeight() - 100, playerCraft.img.GetWidth() + w, client.cy - 100);
	
	enemylist.clear();
	bullets.maga.clear();
	enemybullets.maga.clear();
}

CMFCRaidenView::~CMFCRaidenView()
{
}

BOOL CMFCRaidenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCRaidenView 绘制

void CMFCRaidenView::OnDraw(CDC* pDC)
{
	CMFCRaidenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	bufferDC.CreateCompatibleDC(pDC);
	bufferBitmap.CreateCompatibleBitmap(pDC, WIN_WIDTH, WIN_HEIGHT);
	bufferDC.SelectObject(&bufferBitmap);
	bufferDC.FillSolidRect(0, 0, WIN_WIDTH, WIN_HEIGHT, RGB(0, 0, 0));
	bgd.BitBlt(bufferDC, 0, 0, SRCCOPY);
	DrawBullet();
	DrawEnemy();

	//state bar
	DrawString();
	//start icon
	if (state == GAME_START && icon != NULL) {
		icon->TransparentBlt(bufferDC, iconRect, RGB(255, 255, 255));
	}
	if (state != GAME_START) {
		playerCraft.img.TransparentBlt(bufferDC, playerCraft.rect, RGB(255, 255, 255));
	}
	//Sleep(1000);
	//RunGame();
	//MessageBox(_T("BitBlt!"));
	pDC->BitBlt(0, 0, client.cx, client.cy, &bufferDC, 0, 0, SRCCOPY);
	bufferBitmap.DeleteObject();
	bufferDC.DeleteDC();
}
void CMFCRaidenView::AddEnemy() {
	if (++enemyfre % 3 > level) {
		return;
	}
	int x = rand() * 7 % (WIN_WIDTH);
	EnemyCraft e(x, 0);
	e.rect.SetRect(x, 0, enemyImg.GetWidth() + x, enemyImg.GetWidth());
	e.Hp = e.Hp*level;
	e.enemyType = rand() % 3;
	enemylist.push_back(e);
}

void CMFCRaidenView::AddBoss() {
	Boss boss(0, WIN_WIDTH / 2);
	boss.rect.SetRect(0, bossImg.GetHeight(), bossImg.GetWidth(), 2 * bossImg.GetHeight());
	enemylist.push_back(boss);
	BossEngaged = 1;
}
void CMFCRaidenView::DrawBullet() {
	for each (BULLET b in bullets.maga){
		bullets.img.TransparentBlt(bufferDC, b.rect, RGB(255, 255, 255));
	}
	for each (BULLET b in enemybullets.maga){
		enemybullets.img.TransparentBlt(bufferDC, b.rect, RGB(255, 255, 255));
	}
}
void CMFCRaidenView::DrawEnemy() {
	for each(EnemyCraft e in enemylist) {
		if (e.enemyType != 100) {
			switch (e.enemyType)
			{
			case 0:
				enemyImg.TransparentBlt(bufferDC, e.rect, RGB(255, 255, 255));
				break;
			case 1:
				enemy3Img.TransparentBlt(bufferDC, e.rect, RGB(255, 255, 255));
				break;
			case 2:
				enemy4Img.TransparentBlt(bufferDC, e.rect, RGB(255, 255, 255));
				break;
			default:
				break;
			}
		}
		else if ( BossEngaged){
			bossImg.TransparentBlt(bufferDC, e.rect, RGB(255, 255, 255));
		}
	}
}
void CMFCRaidenView::Bomb() {
	if ((bomb) > 0) {
		score += enemylist.size() * HIT_SCORE;
		enemylist.clear();
		bomb--;
		//Sleep(50);
	}
}

void CMFCRaidenView::DrawString() {
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;
	CFont font;
	font.CreateFontIndirectW(&lf);
	bufferDC.SelectObject(font);
	bufferDC.SetTextColor(RGB(255, 255, 255));
	bufferDC.SetBkMode(TRANSPARENT);
	//bufferDC.TextOutW(x, y-200, str);
	bufferDC.DrawText(scorebar.str, CRect(0, 0, WIN_WIDTH, 30), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	list<Script>::iterator s;
	for (s = script.begin(); s != script.end(); s++) {
		bufferDC.DrawText(s->str, CRect(s->posx, s->posy, WIN_WIDTH, 30), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		s->time--;
	}
	script.remove_if([](Script s) {return s.time <= 0; });

	font.DeleteObject();
}
void CMFCRaidenView::UpdateBullet() {
	list<BULLET>::iterator it;
	for (it = bullets.maga.begin(); it != bullets.maga.end(); it++)
	{
		it->rect.top -= it->speedx;
		it->rect.bottom -= it->speedx;
		it->rect.left -= it->speedy;
		it->rect.right -= it->speedy;
		it->lifetime++;
		list<EnemyCraft>::iterator e;
		for (e = enemylist.begin(); e != enemylist.end(); e++) {
			if (e->rect.PtInRect(CPoint(it->rect.left, it->rect.top))) {
				score += HIT_SCORE;
				e->Hp--;
				it->lifetime = BULLIFE;
			}
		}
		if (!winRect.PtInRect((CPoint)(it->rect.top, it->rect.left))) {
			it->lifetime = BULLIFE;
		}
	}
	bullets.maga.remove_if([](BULLET n) {return n.lifetime >= BULLIFE; });
	//list<BULLET>::iterator it;
	for (it = enemybullets.maga.begin(); it != enemybullets.maga.end(); it++)
	{
		it->rect.top -= it->speedx;
		it->rect.bottom -= it->speedx;
		it->rect.left -= it->speedy;
		it->rect.right -= it->speedy;
		it->lifetime++;
		if (playerCraft.rect.PtInRect(CPoint(it->rect.left, it->rect.top))) {
			score += HIT_SCORE;
			playerCraft.Hp--;
			it->lifetime = BULLIFE;
		}
		if (!winRect.PtInRect((CPoint)(it->rect.top, it->rect.left))) {
			it->lifetime = BULLIFE;
		}
	}
	enemybullets.maga.remove_if([](BULLET n) {return n.lifetime >= BULLIFE; });

}
void CMFCRaidenView::UpdateEnemy() {
	list<EnemyCraft>::iterator it;
	for (it = enemylist.begin(); it != enemylist.end(); it++) {
		it->Move(it->speedx, it->speedy);
		//Boss
		if (it->enemyType == 100) {
			if (it->rect.right >= WIN_WIDTH || it->rect.left <= 0) {
				it->speedy = -it->speedy;
				it->Move(it->speedx, it->speedy);
			}
			if (it->Hp <= 0) {
				bossImg.Destroy();
				bossImg.Load(_T("res\\blow.png"));
				Invalidate(false);
				//Sleep(200);
				//BossEngaged = 0;

				//Invalidate(false);
				//Sleep(200);
				win();
			}

			if (bossShot > 0) {//shot
				enemybullets.addbullet(it->getX() +15, it->getY(), -20, 0);
				enemybullets.addbullet(it->getX() +15, it->getY()+ bossImg.GetWidth() , -20, 0);
				bossShot--;
			}
			else {//sleep
				if (bossSleep == 0) {
					bossShot = rand() % 30;
					bossSleep = rand() % 60;
				}
				bossSleep--;
			}
		}
		if (!winRect.PtInRect((CPoint)(it->rect.top, it->rect.left))) {
			it->Hp = 0;
		}
		if (it->rect.PtInRect(CPoint((playerCraft.rect.left + playerCraft.rect.right) / 2, (playerCraft.rect.top + playerCraft.rect.bottom) / 2))) {
			playerCraft.Hp--;
			it->Hp -= INI_ENEMY_HP;
		}

	}
	enemylist.remove_if([](EnemyCraft e) {	return  (e.Hp <= 0); });
}

void CMFCRaidenView::DamageCheck() {
	//Enemy Check
	if (playerCraft.Hp <= 0) {
		state = GAME_OVER;
	}
	//Level Check
	if (level == 1 && score > LEVEL2_SCORE) {
		level++;
		bomb = 3;
		Script s;
		s.str.Format(_T("UPGRADE!    LEVEL 2"));
		s.posx = 0;
		s.posy = WIN_HEIGHT / 2;
		s.time = 40;
		script.push_back(s);
		//Sleep(500);
	}
	if (level == 2 && score > LEVEL3_SCORE) {
		level++; bomb = 3;
		Script s;
		s.str.Format(_T("UPGRADE!    LEVEL 3"));
		s.posx = 0;
		s.posy = WIN_HEIGHT / 2;
		s.time = 40;
		//DrawString();
		script.push_back(s);
		enemyImg.Destroy();
		enemyImg.Load(_T("res\\enemy1.png"));
		//Sleep(500);
	}
	//Add Boss
	if (score > BOSS_SCORE && BossEngaged == 0) {
		AddBoss();
	}

	scorebar.str.Format(_T("HP: %3d  Bomb: %3d  Score: %6d  Level: %3d"), playerCraft.Hp, bomb, score, level);
}

void CMFCRaidenView::RunGame() {
	if (state == GAME_START) {
		if (isSelected == true) {
			//startIconY.TransparentBlt(bufferDC, iconRect, RGB(255, 255, 255));
			icon = &startIconY;
		}
		else {
			//startIconN.TransparentBlt(bufferDC, iconRect, RGB(255, 255, 255));
			icon = &startIconN;
		}
		Invalidate(false);
	}
	if (state == GAME_PLAYING) {
		//SetTimer(2, (int)(500 / level), NULL);
		//Update Enemy

		//Update Bullet
		//SetTimer(4, REFRESHTIME, NULL);
		if (KEYDOWN(VK_UP)) {
			playerCraft.Move(-20, 0);
		}
		if (KEYDOWN(VK_DOWN)) {
			playerCraft.Move(20, 0);
		}
		if (KEYDOWN(VK_LEFT)) {
			playerCraft.Move(0, -20);
		}
		if (KEYDOWN(VK_RIGHT)) {
			playerCraft.Move(0, 20);
		}
		if (KEYDOWN(VK_SPACE)) {
			switch (level)
			{
			case 1:
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() + playerCraft.img.GetWidth() / 2, 32, 0);
				break;
			case 2:
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() - 1, 32, 0);
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() + playerCraft.img.GetWidth(), 32, 0);
				break;
			case 3:
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() + playerCraft.img.GetWidth() / 2, 32, 16);
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() + playerCraft.img.GetWidth() / 2, 32, 0);
				bullets.addbullet(playerCraft.getX(), playerCraft.getY() + playerCraft.img.GetWidth() / 2, 32, -16);
				break;
			default:
				break;
			}
		}
		if (KEYDOWN(VK_OEM_PLUS)) {
			score += 100;
			//Sleep(100);
		}
		if (KEYDOWN(VK_RETURN)) {
			Bomb();
		}
		if (KEYDOWN(VK_ESCAPE)) {
			state = GAME_EXIT;
		}
		if (KEYDOWN(0x57)) {
			win();
		}
		UpdateBullet();
		UpdateEnemy();
		DamageCheck();
		//RenderToScreen();
	}
	if (state == GAME_OVER) {
		KillTimer(3);
		KillTimer(4);
		//afxMessageBox(_T("Game Over!"));
		CString sbuf = _T("");
		if (score > Best) {
			sbuf = sbuf + _T("New Record!  ");
			Best = score;
		}
		CString temp; 
		temp.Format(_T("Best: %d.  Your Score: %d."), Best, score);
		sbuf = sbuf + temp;
		int ret = MessageBox(sbuf, _T("Game Over!"), MB_RETRYCANCEL);
		if (ret == IDRETRY) {
			GameInit();
			SetTimer(3, INCOMING_TIME, NULL);
			SetTimer(4, REFRESHTIME, NULL);
		}
		else {
			OnClose();
		}
		while (true) {
			if (KEYDOWN(VK_RETURN)) {
				state = GAME_START;
				break;
			}
			if (KEYDOWN(VK_ESCAPE)) {
				state = GAME_EXIT;
				break;
			}
		}
	}

}

void CMFCRaidenView::Welcome() {
	mciSendString(_T("Open res\\TakeMyBreathAway.mp3 alias BGM"), NULL, 0, NULL);
	mciSendString(_T("Play BGM repeat"), NULL, 0, NULL);
}

void CMFCRaidenView::win() {
	KillTimer(3);
	KillTimer(4);
	CString temp;
	temp.Format(_T("Congratulations! \nScore.: %d New Record!"), score);
	MessageBox(temp);
}
void CMFCRaidenView::RenderToScreen() {
	Invalidate(true);
}


// CMFCRaidenView 诊断

#ifdef _DEBUG
void CMFCRaidenView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCRaidenView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCRaidenDoc* CMFCRaidenView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCRaidenDoc)));
	return (CMFCRaidenDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCRaidenView 消息处理程序
bool CMFCRaidenView::OnEraseBkgnd(CDC* pDC) {
	return true;
}

void CMFCRaidenView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (state != GAME_EXIT)
		switch (nIDEvent)
		{
		case 3:
			if (state == GAME_PLAYING) {
				AddEnemy();
			}
			break;
		case 4:
			RunGame();
			Invalidate(false);
			break;
		default:
			break;
		}
	CView::OnTimer(nIDEvent);
}


int CMFCRaidenView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//srand(GetTickCount());

	//SetTimer(1, 20, NULL);
	if (state == GAME_START) {
		//DrawString(200, 200, _T("START!"));
		//Sleep(3000);

		Welcome();
		//state = GAME_PLAYING;
		SetTimer(3, INCOMING_TIME, NULL);
		SetTimer(4, REFRESHTIME, NULL);
	}
	if (state == GAME_PLAYING) {

	}
	if (state == GAME_OVER) {
		KillTimer(3);
		KillTimer(4);
	}
	return 0;
}


//void CMFCRaidenView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CView::OnKeyDown(nChar, nRepCnt, nFlags);
//}


void CMFCRaidenView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (state == GAME_START) {
		if (iconRect.PtInRect(point)) {
			isSelected = true;
		}
		else {
			isSelected = false;
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CMFCRaidenView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (state)
	{
	case GAME_EXIT:
		break;
	case GAME_START:
		if (iconRect.PtInRect(point)) {
			state = GAME_PLAYING;
			bgd.Destroy();
			bgd.Load(_T("res\\bgd2.jpg"));
			Invalidate(false);
		}
		break;
	case GAME_PLAYING:
		break;
	case GAME_OVER:
		break;
	default:
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}
