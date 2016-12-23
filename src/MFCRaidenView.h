
// MFCRaidenView.h : CMFCRaidenView ��Ľӿ�
//

#pragma once
#include <vector>
#include <list>
#include <sstream>
#include <mmsystem.h>
using namespace std;

#pragma comment(lib, "Winmm.lib")


class CMFCRaidenView : public CView
{
protected: // �������л�����
	CMFCRaidenView();
	DECLARE_DYNCREATE(CMFCRaidenView)

// ����
public:
	CMFCRaidenDoc* GetDocument() const;

	bool OnEraseBkgnd(CDC * pDC);


// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CMFCRaidenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	PlayerCraft playerCraft;
	//CBitmap backgroundIMG;
	CImage bgd, welcome, bossImg;
	CImage enemyImg, enemy4Img, enemy3Img;
	CDC bufferDC;
	CBitmap bufferBitmap;
	CSize client;
	CRect winRect;
	
	//game state
	int level, BossEngaged, score, bomb, Best;
	GAME_STATE state;
	Bullets bullets;
	Bullets enemybullets;
	Script scorebar;
	list<Script> script;

	//welcome screen
	CRect iconRect;
	CImage *icon;
	CImage startIconN, startIconY;
	bool isSelected;

	//enemy
	int enemyfre;
	int bossShot, bossSleep;
	list<EnemyCraft> enemylist;

	void Welcome();
	void win();
	void AddEnemy();
	void AddBoss();
	void UpdateBullet();
	void UpdateEnemy();
	void RunGame();
	void RenderToScreen();
	void DrawBullet();
	void DrawEnemy();
	void DrawString();
	void DamageCheck();
	void Bomb();
	void GameInit();


// ���ɵ���Ϣӳ�亯��
public:

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCRaidenView.cpp �еĵ��԰汾
inline CMFCRaidenDoc* CMFCRaidenView::GetDocument() const
   { return reinterpret_cast<CMFCRaidenDoc*>(m_pDocument); }
#endif

