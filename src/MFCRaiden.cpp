
// MFCRaiden.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCRaiden.h"
#include "MainFrm.h"

#include "MFCRaidenDoc.h"
#include "MFCRaidenView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCRaidenApp

BEGIN_MESSAGE_MAP(CMFCRaidenApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCRaidenApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMFCRaidenApp ����

CMFCRaidenApp::CMFCRaidenApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFCRaiden.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMFCRaidenApp ����

CMFCRaidenApp theApp;


// CMFCRaidenApp ��ʼ��

BOOL CMFCRaidenApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMFCRaidenDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMFCRaidenView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->SetWindowTextW(L"Raiden _by zyc");
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CMFCRaidenApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CMFCRaidenApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMFCRaidenApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

Craft::Craft() {
	pos_x = 100;
	pos_y = 100;
	Hp = INI_HP;
}
Craft::Craft(int x, int y)
{
	pos_x = x;
	pos_y = y;
	//rect.SetRect(x, y, x + img.GetWidth(), y + img.GetWidth());
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
	speedy = 10;
	//rect.SetRect(x, y, x+bossImg.GetWidth(),);
	enemyType = 100;
	isEngaged = false;
}

bool Craft::Move(int dx, int dy) {
	pos_x += dx;
	pos_y += dy;
	rect.left += dy;
	rect.right += dy;
	rect.bottom += dx;
	rect.top += dx;
	return true;
}
int Craft::getX() {
	return rect.top;
}
int Craft::getY() {
	return rect.left;
}
int Craft::getspeed() {
	return speed;
}

Bullets::Bullets() {
	//sizex = img.GetWidth();
	//sizey = img.GetHeight();
}
void Bullets::addbullet(int x, int y, int sx, int sy) {
	BULLET b;
	b.posx = x;
	b.posy = y;
 	b.rect.SetRect(y, x, y + img.GetWidth(), x + img.GetHeight());
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
// CMFCRaidenApp ��Ϣ�������



