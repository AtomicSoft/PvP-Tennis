
// ChildView.cpp : CChildView Ŭ������ ����
//

#include "stdafx.h"
#include "MfcTennis.h"
#include "ChildView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//=============================================================================
// Enumeration
//-----------------------------------------------------------------------------
enum tag_GameStatus { GAMEOVER, RUNNING, PAUSE, WAITINGSERVE, DOINGSERVE };
enum tag_ScorePoint { ZERO = 0, FIRST = 15, SECOND = 30, THIRD = 40, ADVANTAGE = 65 };
enum tag_RefreeMsg	{ NONE, FAULT, FOOT_FAULT, DEUCE };
tag_GameStatus GameStatus;
tag_ScorePoint ScorePoint;
tag_RefreeMsg RefreeMsg;
//=============================================================================



// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_GAME_START, &CChildView::OnGameStart)
	ON_COMMAND(ID_GAME_PAUSE, &CChildView::OnGamePause)
	ON_COMMAND(ID_GAME_END, &CChildView::OnGameEnd)
	ON_UPDATE_COMMAND_UI(ID_GAME_PAUSE, &CChildView::OnUpdateGamePause)
	ON_WM_CREATE()
	ON_WM_INITMENU()
	ON_WM_DESTROY()
	ON_WM_KEYUP()
	ON_WM_ERASEBKGND()
//	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView �޽��� ó����

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CBrush brush;
	CRect rect;
	GetClientRect(&rect);
	CString str;

	// ���� ����ü�� ����� �ȼ����� ����
	m_TennisGame.nHResolution = rect.Width();
	m_TennisGame.nVResolution = rect.Height();

	dc.SelectStockObject(BLACK_BRUSH);
	dc.SelectStockObject(NULL_PEN);
	// ��, ��Ʈ, ��Ʈ �׸���� ���ȭ�� ���� �Ŀ��� �����Ѵ�.
	dc.Rectangle(ReCalcWidth(LAND_LEFT), ReCalcHeight(LAND_TOP),
		ReCalcWidth(LAND_RIGHT), ReCalcHeight(HEIGHT));		// �� ����
	dc.SelectStockObject(BLACK_BRUSH);
	dc.SelectStockObject(BLACK_PEN);
	dc.Rectangle(ReCalcWidth(NET_LEFT), ReCalcHeight(NET_TOP),
		ReCalcWidth(NET_RIGHT), ReCalcHeight(HEIGHT));			// ��Ʈ ����

	brush.CreateSolidBrush(RGB(0, 0, 255));
	dc.SelectObject(brush);
	dc.SelectStockObject(NULL_PEN);
	dc.Rectangle(ReCalcWidth(COURT_LEFT), ReCalcHeight(LAND_TOP),
		ReCalcWidth(COURT_RIGHT), ReCalcHeight(HEIGHT));	// ��Ʈ ����
	brush.DeleteObject();

	brush.CreateSolidBrush(RGB(255, 0, 0));
	dc.SelectObject(brush);
	dc.Rectangle(ReCalcWidth(SERVICE_BOX_LEFT), ReCalcHeight(LAND_TOP),
		ReCalcWidth(SERVICE_BOX_RIGHT), ReCalcHeight(HEIGHT));	// ���� ����
	brush.DeleteObject();
	
	BITMAP bmpinfo[2];
	CDC dcmem[2];

	for(int i = 0; i < 2; i++) {
		// ���� ǥ��
		m_Player[0].bmpPlayer.GetBitmap(&bmpinfo[i]);
		dcmem[i].CreateCompatibleDC(&dc);
		dcmem[i].SelectObject(&m_Player[i].bmpPlayer);
		// ������ ũ�Ⱑ �⺻���̸� ���� ũ��� ����ϰ�
		// �⺻���� �ƴϸ� ũ�� ����
		if( m_TennisGame.nHResolution == WIDTH &&
			m_TennisGame.nVResolution == HEIGHT)
			dc.BitBlt(m_Player[i].player_rect.left,
				m_Player[i].player_rect.top, PLAYER_WIDTH, PLAYER_HEIGHT,
				&dcmem[i], 0, 0, SRCCOPY);
		else
			dc.StretchBlt(ReCalcWidth(m_Player[i].player_rect.left),
				ReCalcHeight(m_Player[i].player_rect.top),
				ReCalcWidth(PLAYER_WIDTH), ReCalcHeight(PLAYER_HEIGHT),
				&dcmem[i], 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT, SRCCOPY);
		// ��� �׸��� �ϼ��Ǹ� ������ �׸��ڵ� ������ش�.
	}

	// �� ǥ��
	dc.SelectStockObject(NULL_PEN);
	dc.SelectStockObject(BLACK_BRUSH);
	dc.Ellipse(
		ReCalcWidth(m_Ball.ptCenter.x - BALL_RADIUS - 1),
		ReCalcHeight(m_Ball.ptCenter.y - BALL_RADIUS - 1),
		ReCalcWidth(m_Ball.ptCenter.x + BALL_RADIUS + 1),
		ReCalcHeight(m_Ball.ptCenter.y + BALL_RADIUS + 1));
	brush.CreateSolidBrush(RGB(0, 255, 0));
	dc.SelectObject(brush);
	dc.Ellipse(
		ReCalcWidth(m_Ball.ptCenter.x - BALL_RADIUS),
		ReCalcHeight(m_Ball.ptCenter.y - BALL_RADIUS),
		ReCalcWidth(m_Ball.ptCenter.x + BALL_RADIUS),
		ReCalcHeight(m_Ball.ptCenter.y + BALL_RADIUS));
	brush.DeleteObject();

	// ���ھ� ���� ǥ��
	/*str.Format(TEXT("%s : %d"), m_Player[0].strName, m_Player[0].nScore);
	dc.DrawText(str,
		CRect(ReCalcWidth(40), ReCalcHeight(30),
		ReCalcWidth(40) + 100, ReCalcHeight(30) + 50), DT_SINGLELINE);
	str.Format("%s : %d", m_Player[1].strName, m_Player[1].nScore);
	dc.DrawText(str,
		CRect(ReCalcWidth(600) - 100, ReCalcHeight(30),
		ReCalcWidth(600), ReCalcHeight(30) + 50),
		DT_RIGHT | DT_SINGLELINE);*/
	dc.SelectStockObject(BLACK_BRUSH);
	dc.SelectStockObject(WHITE_PEN);
	dc.Rectangle(ReCalcWidth(200), ReCalcHeight(30),
		ReCalcWidth(440), ReCalcHeight(210));
	dc.MoveTo(ReCalcWidth(200), ReCalcHeight(90));
	dc.LineTo(ReCalcWidth(440), ReCalcHeight(90));
	dc.MoveTo(ReCalcWidth(200), ReCalcHeight(110));
	dc.LineTo(ReCalcWidth(320), ReCalcHeight(110));
	dc.MoveTo(ReCalcWidth(200), ReCalcHeight(130));
	dc.LineTo(ReCalcWidth(320), ReCalcHeight(130));
	dc.MoveTo(ReCalcWidth(200), ReCalcHeight(150));
	dc.LineTo(ReCalcWidth(440), ReCalcHeight(150));
	dc.MoveTo(ReCalcWidth(224), ReCalcHeight(90));
	dc.LineTo(ReCalcWidth(224), ReCalcHeight(150));
	dc.MoveTo(ReCalcWidth(248), ReCalcHeight(90));
	dc.LineTo(ReCalcWidth(248), ReCalcHeight(150));
	dc.MoveTo(ReCalcWidth(272), ReCalcHeight(90));
	dc.LineTo(ReCalcWidth(272), ReCalcHeight(150));
	dc.MoveTo(ReCalcWidth(296), ReCalcHeight(90));
	dc.LineTo(ReCalcWidth(296), ReCalcHeight(150));
	dc.MoveTo(ReCalcWidth(320), ReCalcHeight(30));
	dc.LineTo(ReCalcWidth(320), ReCalcHeight(210));
	dc.MoveTo(ReCalcWidth(360), ReCalcHeight(30));
	dc.LineTo(ReCalcWidth(360), ReCalcHeight(210));
	dc.MoveTo(ReCalcWidth(400), ReCalcHeight(30));
	dc.LineTo(ReCalcWidth(400), ReCalcHeight(210));
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SetBkColor(RGB(0, 0, 0));
	rect.SetRect(201, 31, 319, 89);
	dc.DrawText(m_Player[0].strName, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rect.SetRect(201, 151, 319, 209);
	dc.DrawText(m_Player[1].strName, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	rect.SetRect(201, 111, 223, 128);
	dc.DrawText("0", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rect.SetRect(225, 111, 247, 128);
	dc.DrawText("15", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rect.SetRect(249, 111, 271, 128);
	dc.DrawText("30", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rect.SetRect(273, 111, 295, 128);
	dc.DrawText("40", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	rect.SetRect(297, 111, 319, 128);
	dc.DrawText("A", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// ���� �޽��� ǥ��
	if(m_TennisGame.strRefree != "")
		dc.DrawText(m_TennisGame.strRefree,
			CRect(ReCalcWidth(120), ReCalcHeight(200),
			ReCalcWidth(120) + 100, ReCalcHeight(200) + 50), DT_SINGLELINE);

	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()�� ȣ������ ���ʽÿ�.
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	// ���α׷� ���۽ÿ��� ������ �ڵ�� �� �Լ� �ȿ� �ְ�
	// �������� ResetGame() �ȿ� �ִ´�.
	//CRect crt;
	//SetRect(&crt, 0, 0, WIDTH, HEIGHT);
	////AdjustWindowRect(&crt, GetStyle(), FALSE);
	//AdjustWindowRect(&crt, GetWindowLong(GetSafeHwnd(), GWL_STYLE), FALSE);

	// ������ Ű���� ������ ���� ����
	// Ű���� ���� �޴��� ������ ��
	//   Ű ���� ��ġ�� �ʵ��� �� ��
	// �����ڷ� ������ ��� �ݵ�� �빮�ڷ� �� ��
	m_Player[0].key_set.key_left = 'A';
	m_Player[0].key_set.key_right = 'D';
	m_Player[0].key_set.key_swing = VK_CONTROL;
	m_Player[0].key_set.key_up = 'W';
	m_Player[1].key_set.key_left = VK_LEFT;
	m_Player[1].key_set.key_right = VK_RIGHT;
	m_Player[1].key_set.key_swing = VK_SPACE;
	m_Player[1].key_set.key_up = VK_UP;

	// ������ �ʱ�ȭ
	m_Player[0].strName = "Player1";
	m_Player[1].strName = "Player2";

	m_TennisGame.nHResolution = WIDTH;
	m_TennisGame.nVResolution = HEIGHT;

	// ��Ʈ�� �ҷ�����
	// ���� ��Ʈ�ʸ� �ʿ�, ���� Ellipse()�� ǥ��
	m_Player[0].bmpPlayer.LoadBitmap(IDB_PLAYER1);
	m_Player[1].bmpPlayer.LoadBitmap(IDB_PLAYER2);

	// �� ������ �ʱ�ȭ�Ѵ�.
	// ���� ��� �ͱ��� üũ�� �� �ֵ���
	// �������� �����¿� 1pt�� �а� ��´�(��Ʈ, ���񽺹ڽ� ���δ� ����).
	m_TennisGame.court.SetRect(COURT_LEFT, LAND_TOP - 1, COURT_RIGHT, HEIGHT);
	m_TennisGame.land.SetRect(0, LAND_TOP - 1, WIDTH, HEIGHT);
	m_TennisGame.net.SetRect(NET_LEFT - 1, NET_TOP - 1, NET_RIGHT + 1, HEIGHT);
	m_TennisGame.service_box.SetRect(SERVICE_BOX_LEFT, LAND_TOP - 1, SERVICE_BOX_RIGHT, HEIGHT);

	// ���� �ʱ�ȭ(���� ����۽ÿ��� ����)
	ResetGame();

	return 0;
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);
}


void CChildView::OnGameStart()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//if(GameStatus != GAMEOVER) return;
	ResetGame();	// ���� �ʱ�ȭ
	GameStatus = WAITINGSERVE;
	//SetTimer(1, 100, NULL);
}


void CChildView::OnGamePause()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���°� running�� ���� pause�� ��ȯ ����
	if(GameStatus == WAITINGSERVE || GameStatus == DOINGSERVE || GameStatus == RUNNING) {
		GameStatus = PAUSE;
		KillTimer(1);
	}
	else if(GameStatus == PAUSE) {
		GameStatus = RUNNING;
		SetTimer(1, 100, NULL);
	}
}


void CChildView::OnGameEnd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� �Ͻ�����
	if(MessageBox("������ ������ �ʱ� ȭ������ ���ư��ϴ�.", "���� �ߴ�", MB_OKCANCEL) == MB_OK) { 
		KillTimer(1);
		ResetGame();
		GameStatus = GAMEOVER;
	}
}


void CChildView::OnInitMenu(CMenu* pMenu)
{
	CWnd::OnInitMenu(pMenu);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// ���߿� �� ���� ���� ������ �� ����. ���� ������ ����.
	if(m_Player[0].player_rect.bottom >= LAND_TOP &&
		(GameStatus == RUNNING || GameStatus == DOINGSERVE ||
		GameStatus == WAITINGSERVE && m_TennisGame.bCurrentServe)) {
		if( UpperCase(nChar) == m_Player[0].key_set.key_left && 
			GetAsyncKeyState(UpperCase(m_Player[0].key_set.key_right) & 0x8000) == 0x0000)
			PlayerMove(0, VK_LEFT);
		if(UpperCase(nChar) == m_Player[0].key_set.key_right &&
			GetAsyncKeyState(UpperCase(m_Player[0].key_set.key_left) & 0x8000) == 0x0000)
			PlayerMove(0, VK_RIGHT);
		if(UpperCase(nChar) == m_Player[0].key_set.key_up)
			PlayerMove(0, VK_UP);
	}
	if(	UpperCase(nChar) == m_Player[0].key_set.key_swing) {
		switch(GameStatus) {
		case DOINGSERVE:
			m_TennisGame.bLastTouch = false;
			// ���� ���� ����� �ٽ� ġ�� fault
			if(m_TennisGame.bCurrentServe == m_TennisGame.bLastTouch) {
				Fault(0);
				break;
			}
			else {
				GameStatus = RUNNING;
				m_TennisGame.in_area.SetRect(m_TennisGame.net.right, LAND_TOP,
					m_TennisGame.court.right, HEIGHT);
				m_TennisGame.bLastTouch = false;
				m_Player[0].bmpPlayer.DeleteObject();
				m_Player[0].bmpPlayer.LoadBitmap(IDB_PLAYER1_SWING);
				if(!m_Player[0].bSwinging)
					if(Swing(0)) {
						m_Ball.ptVelosity.x = BALL_INITIAL_VELOSITY_X + m_Player[0].ptVelosity.x;
						m_Ball.ptVelosity.y = BALL_INITIAL_VELOSITY_Y + m_Player[0].ptVelosity.y;
					}
				break;
			}
		case WAITINGSERVE:
			if(GameStatus != WAITINGSERVE) break;
			if(m_TennisGame.bCurrentServe) break;
			m_TennisGame.bLastTouch = false;
			GameStatus = DOINGSERVE;
			SetTimer(1, 100, NULL);
			// �� �б����� �ش��� ���
			// �Ʒ� �ڵ�� ������ �����ϹǷ� break�� ���� �ʴ´�.
		case RUNNING:
			m_TennisGame.bLastTouch = false;
			m_Player[0].bmpPlayer.DeleteObject();
			m_Player[0].bmpPlayer.LoadBitmap(IDB_PLAYER1_SWING);
			if(!m_Player[0].bSwinging)
				if(Swing(0)) {
					m_Ball.ptVelosity.x = BALL_INITIAL_VELOSITY_X + m_Player[0].ptVelosity.x;
					m_Ball.ptVelosity.y = BALL_INITIAL_VELOSITY_Y + m_Player[0].ptVelosity.y;
				}
			break;
		}
	}

	if(m_Player[1].player_rect.bottom >= LAND_TOP &&
		(GameStatus == RUNNING || GameStatus == DOINGSERVE ||
		GameStatus == WAITINGSERVE && !m_TennisGame.bCurrentServe)) {
		if( UpperCase(nChar) == m_Player[1].key_set.key_left && 
			GetAsyncKeyState(UpperCase(m_Player[1].key_set.key_right) & 0x8000) == 0x0000)
			PlayerMove(1, VK_LEFT);
		if(UpperCase(nChar) == m_Player[1].key_set.key_right &&
			GetAsyncKeyState(UpperCase(m_Player[1].key_set.key_left) & 0x8000) == 0x0000)
			PlayerMove(1, VK_RIGHT);
		if(UpperCase(nChar) == m_Player[1].key_set.key_up)
			PlayerMove(1, VK_UP);
	}
	if(	UpperCase(nChar) == m_Player[1].key_set.key_swing) {
		switch(GameStatus) {
		case DOINGSERVE:
			m_TennisGame.bLastTouch = true;
			// ���� ���� ����� �ٽ� ġ�� fault
			if(m_TennisGame.bCurrentServe == m_TennisGame.bLastTouch) {
				Fault(1);
				break;
			}
			else {
				GameStatus = RUNNING;
				m_TennisGame.in_area.SetRect(m_TennisGame.court.left, LAND_TOP,
					m_TennisGame.net.left, HEIGHT);
				m_TennisGame.bLastTouch = true;
				m_Player[1].bmpPlayer.DeleteObject();
				m_Player[1].bmpPlayer.LoadBitmap(IDB_PLAYER2_SWING);
				if(!m_Player[1].bSwinging)
					if(Swing(1)) {
						m_Ball.ptVelosity.x = -BALL_INITIAL_VELOSITY_X + m_Player[1].ptVelosity.x;
						m_Ball.ptVelosity.y = BALL_INITIAL_VELOSITY_Y + m_Player[1].ptVelosity.y;
					}
				break;
			}
		case WAITINGSERVE:
			if(GameStatus != WAITINGSERVE) break;
			if(!m_TennisGame.bCurrentServe) break;
			m_TennisGame.bLastTouch = true;
			GameStatus = DOINGSERVE;
			m_TennisGame.bLastTouch = true;
			SetTimer(1, 100, NULL);
			// �� �б����� �ش��� ���
			// �Ʒ� �ڵ�� ������ �����ϹǷ� break�� ���� �ʴ´�.
		case RUNNING:
			m_TennisGame.bLastTouch = true;
			m_Player[1].bmpPlayer.DeleteObject();
			m_Player[1].bmpPlayer.LoadBitmap(IDB_PLAYER2_SWING);
			if(!m_Player[1].bSwinging)
				if(Swing(1)) {
					m_Ball.ptVelosity.x = -BALL_INITIAL_VELOSITY_X + m_Player[1].ptVelosity.x;
					m_Ball.ptVelosity.y = BALL_INITIAL_VELOSITY_Y + m_Player[1].ptVelosity.y;
				}
			break;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(GameStatus == WAITINGSERVE || GameStatus == DOINGSERVE || GameStatus == RUNNING) {
		if(UpperCase(nChar) == m_Player[0].key_set.key_left) {
			m_Player[0].ptAcceleration.x = 0;
		}
		else if(UpperCase(nChar) == m_Player[0].key_set.key_right) {
			m_Player[0].ptAcceleration.x = 0;
		}
		else if(UpperCase(nChar) == m_Player[0].key_set.key_swing) {
			m_Player[0].bmpPlayer.DeleteObject();
			m_Player[0].bmpPlayer.LoadBitmap(IDB_PLAYER1);
			m_Player[0].bSwinging = false;
		}
		else if(UpperCase(nChar) == m_Player[1].key_set.key_left) {
			m_Player[1].ptAcceleration.x = 0;
		}
		else if(UpperCase(nChar) == m_Player[1].key_set.key_right) {
			m_Player[1].ptAcceleration.x = 0;
		}
		else if(UpperCase(nChar) == m_Player[1].key_set.key_swing) {
			m_Player[1].bmpPlayer.DeleteObject();
			m_Player[1].bmpPlayer.LoadBitmap(IDB_PLAYER2);
			m_Player[1].bSwinging = false;
		}
	}





	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}


//void CChildView::OnSize(UINT nType, int cx, int cy)
//{
//	CWnd::OnSize(nType, cx, cy);
//
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_TennisGame.nHResolution = cx;
//	m_TennisGame.nVResolution = cy;
//	ResetSituation();
//}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(GameStatus == WAITINGSERVE || GameStatus == DOINGSERVE || GameStatus == RUNNING)
		UpdateSituation();

	CWnd::OnTimer(nIDEvent);
}


void CChildView::OnUpdateGamePause(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(GameStatus == PAUSE);
}


//==============================================================================
// �߰� �Լ���
//------------------------------------------------------------------------------

void CChildView::AddGame(bool bPlayer)
{
	m_Player[bPlayer].nScore = 0;
	m_Player[bPlayer].nGame[m_TennisGame.nCurrentSet]++;
	// ȹ�� ���� ���� 6�̸� 1��Ʈ ȹ��
	if(m_Player[bPlayer].nGame[m_TennisGame.nCurrentSet] == 6) {
		m_TennisGame.nCurrentSet++;
		// 3��Ʈ���� �������� ���� ����
		if(m_TennisGame.nCurrentSet == MAX_SET) {
			m_TennisGame.strRefree = "Game Over!";
			GameStatus = GAMEOVER;
			Invalidate();
			Sleep(2000);
			m_TennisGame.strRefree.Format("%s won!", m_Player[bPlayer].strName);
			ResetSituation();
			return;
		}
	}

	GameStatus = WAITINGSERVE;
	ResetSituation();
}


void CChildView::AddScore(bool bPlayer)
{
	// �� �ٿ�� ���� �ʱ� ���·� �ǵ�����
	// ������ ��ȿ�� ������ ����
	m_Ball.nBound = 0;
	switch(m_Player[bPlayer].nScore) {
	case ZERO:
		m_Player[bPlayer].nScore = FIRST;
		break;
	case FIRST:
		m_Player[bPlayer].nScore = SECOND;
		break;
	case SECOND:
		m_Player[bPlayer].nScore = THIRD;
		// ��� ������ 40���� ��� �ེ
		if(m_Player[!bPlayer].nScore == THIRD) {
			m_TennisGame.strRefree = "Deuce!";
			m_Player[bPlayer].nScore = ADVANTAGE;
			Invalidate();
			Sleep(2000);
			GameStatus = WAITINGSERVE;
			m_TennisGame.strRefree = "";
			ResetSituation();
		}
		break;
	case THIRD:
		// ��� ������ 30�� �����̸�, ������ ������ 1���� ȹ��
		if(m_Player[!bPlayer].nScore < THIRD)
			AddGame(bPlayer);
		// �ེ
		else {
			m_TennisGame.strRefree = "Deuce!";
			m_Player[bPlayer].nScore = ADVANTAGE;
			// ��� ������ Advantage���� ��� 40������ ������.
			if(m_Player[!bPlayer].nScore == ADVANTAGE)
				m_Player[!bPlayer].nScore = THIRD;

			Invalidate();
			Sleep(2000);
			GameStatus = WAITINGSERVE;
			m_TennisGame.strRefree = "";
			ResetSituation();
		}
		break;
	case ADVANTAGE:
		// 1������ ������, 6������ ��� 1��Ʈ ȹ��
		AddGame(bPlayer);
		break;
	}
}


void CChildView::CheckBall(void)
{
	// ���� ���� �������� ��
	if(m_Ball.ball_rect.bottom >= LAND_TOP - 1) {
		// ù��° �ٿ��
		if(m_Ball.nBound == 0)
		{
			m_Ball.nBound++;

			// ���� ��ȿ���� �ȿ� �������� in
			if(m_Ball.ptCenter.x >= m_TennisGame.in_area.left ||
				m_Ball.ptCenter.x <= m_TennisGame.in_area.right) {
				return;
			}

			// ���� ��ȿ���� �ۿ� �������� fault �Ǵ� �ƿ�
			else {
					// �������̾��� ������ fault
					if(GameStatus == DOINGSERVE) {
						Fault(m_TennisGame.bLastTouch);
					}
					// �������� �ƴϾ��� ���� out
					// ���������� ģ ������ ������� ����
					else {
						AddScore(!m_TennisGame.bLastTouch);
						return;
					}
			}
		}

		// �� ��° �ٿ���� ��� ������ ��ȿ�� �����̹Ƿ�
		// ���������� ģ ������ ����
		else {
			AddScore(m_TennisGame.bLastTouch);
			return;
		}
	}

	// ���� ��Ʈ�� �¾��� ���
	// ���������� ģ ������ ������� ����
	if(OverlapCircleArea(m_Ball.ball_rect, m_TennisGame.net)) {
		AddScore(!m_TennisGame.bLastTouch);
		return;
	}
}


void CChildView::Fault(bool player_idx)
{
	// �������� ���
	if(GameStatus == DOINGSERVE) {
		// �������
		m_Ball.nBound = 0;
		m_TennisGame.strRefree = "Fault!";
		Invalidate();
		Sleep(2000);
		m_TennisGame.strRefree = "";
		Invalidate();
		GameStatus = WAITINGSERVE;
		ResetSituation();

		// ù��° fault�� ���
		if(!m_TennisGame.bFault) {
			m_TennisGame.bFault = true;
			return;
		}
		// �ι�° fault�� ���
		else if(m_TennisGame.bFault) {
			m_TennisGame.bFault = false;
			AddScore(!m_TennisGame.bCurrentServe);
			return;
		}
		// ���ܻ�Ȳ
		else {

		}
	}
}


void CChildView::IntegratePoint(CPoint addPoint, CPoint* currentPoint)
{
	currentPoint->x += addPoint.x;
	currentPoint->y += addPoint.y;
}


int CChildView::IsThereWall(int player_idx)
{
	// ��ȯ��
	//   0: ��ֹ��� ���� ���
	//   1: ��ֹ��� ���ʿ� ���� ���
	//   2: ��ֹ��� �����ʿ� ���� ���
	if(	!player_idx && m_Player[0].ptSituation.x <= 0 || 
		player_idx && m_Player[1].ptSituation.x < NET_RIGHT)
		return 1;
	if(	!player_idx && m_Player[0].ptSituation.x >= NET_LEFT - PLAYER_WIDTH ||
		player_idx &&  m_Player[1].ptSituation.x > WIDTH - PLAYER_WIDTH)
		return 2;
	return 0;
}


bool CChildView::OverlapCircleArea(CRect ellipse, CRect rect)
{
	CPoint center;
	center.SetPoint(	Round((ellipse.left + ellipse.right) / 2.0),
						Round((ellipse.top + ellipse.bottom) / 2.0));
	int radius = Round((ellipse.right - ellipse.left) / 2.0);

	// ellipse�� �߽��� rect ���� �ȿ� ������ ������ ��ģ��.
	if(	center.x >= rect.left && center.x <= rect.right &&
		center.y >= rect.top && center.y <= rect.bottom)
		return true;
	// �߽��� �ۿ� ������ rect�� ���ʰ� ������ ���̿� ���� ���
	if(center.x >= rect.left && center.x <= rect.right) {
		// ellipse�� top�̳� bottom�� rect�� �ȿ� ������ ��ģ��.
		if(	ellipse.top >= rect.top && ellipse.top <= rect.bottom ||
			ellipse.bottom >= rect.top && ellipse.bottom <= rect.bottom)
			return true;
	}
	// �߽��� �ۿ� ������ rect�� ���� �Ʒ� ���̿� ���� ���
	if(center.y >= rect.top && center.y <= rect.bottom) {
		// ellipse�� �����̳� �������� rect�� �ȿ� ������ ��ģ��.
		if( ellipse.left >= rect.left && ellipse.right <= rect.right ||
			ellipse.right >= rect.left && ellipse.right <= rect.right)
			return true;
	}
	// rect�� ����� ���� ���� �ۿ� ellipse�� ��ġ�� ���
	// ellipse�� ���� ����� rect�� �������� ellipse�� �߽��� �Ÿ���
	// ellipse�� ���������� ª���� ��ģ��.
	if(ellipse.top < rect.top && ellipse.left < rect.left &&
		sqrt(pow((double)(rect.left - ellipse.left), 2) + pow((double)(rect.top - ellipse.top), 2)) < radius)
		return true;
	if(ellipse.top < rect.top && ellipse.right > rect.right &&
		sqrt(pow((double)(ellipse.right - ellipse.right), 2) + pow((double)(rect.top - ellipse.top), 2)) < radius)
		return true;
	if(ellipse.bottom > rect.bottom && ellipse.left < rect.left &&
		sqrt(pow((double)(rect.left - ellipse.left), 2) + pow((double)(ellipse.bottom - rect.bottom), 2)) < radius)
		return true;
	if(ellipse.bottom > rect.bottom && ellipse.right > rect.right &&
		sqrt(pow((double)(ellipse.right - rect.right), 2) + pow((double)(ellipse.bottom - rect.bottom), 2)) < radius)
		return true;

	return false;
}


bool CChildView::OverlapRectArea(CRect rect1, CRect rect2)
{
	// rect1�� ������ ���� x���� rect2�� ���� ���� x���� ���ų� ������ ��
	if(rect1.right <= rect2.left) return false;

	// rect2�� ������ ���� x���� rect1�� ���� ���� x���� ���ų� ������ ��
	if(rect2.right <= rect1.left) return false;

	// rect1�� �Ʒ��� ���� y���� rect2�� ���� ���� y���� ���ų� ���� ��
	if(rect1.bottom <= rect2.top) return false;

	// rect2�� �Ʒ��� ���� y���� rect1�� ���� ���� y���� ���ų� ���� ��
	if(rect2.bottom <= rect1.top) return false;

	// ���� ��쿡 ��� �ش� �ȵǸ� ��ħ
	return true;
}


void CChildView::PlayerMove(int player_idx, UINT command)
{
	// ��ֹ��� �ε����� ���� y ���⸸ �����δ�.(����)
	if(m_Player[player_idx].ptSituation.y < LAND_TOP ||
		!(GameStatus == WAITINGSERVE || GameStatus == DOINGSERVE || GameStatus == RUNNING))
		return;
	switch(command) {
	case VK_LEFT:
			m_Player[player_idx].ptAcceleration.x = -PLAYER_MOVE_ACCELERATION;
		break;
	case VK_RIGHT:
			m_Player[player_idx].ptAcceleration.x = PLAYER_MOVE_ACCELERATION;
		break;
	case VK_UP:
		m_Player[player_idx].ptVelosity.y = PLAYER_JUMP_VELOSITY;
		break;
	}
}


int CChildView::ReCalcWidth(int width)
{
	return Round(width * m_TennisGame.nHResolution / (double)WIDTH);
}


int CChildView::ReCalcHeight(int height)
{
	return Round(height * m_TennisGame.nVResolution / (double)HEIGHT);
}


void CChildView::ResetGame(void)
{
	ResetScore();		// ���ھ� �ʱ�ȭ
	ResetSituation();	// ����, �� ��ġ�� �ʱ�ȭ

	// ���� �Ӽ��� �ʱ�ȭ
	// ��ġ���� ������ �⺻ ũ�⸦ �������� �Ѵ�.
	// �ȼ��� ������ OnPaint �Լ����� ����
	m_TennisGame.nCurrentSet = 1;
	m_TennisGame.nCurrentGame = 1;
	m_TennisGame.bCurrentServe = false;
	m_TennisGame.bLastTouch = false;
	m_TennisGame.bFault = false;
	m_TennisGame.strRefree = "";

	// �� �Ӽ��� �ʱ�ȭ
	m_Ball.nBound = 0;

	// ��ȿ���� �ʱ�ȭ
	// ������ڰ� P1�� ��� P2�� ���񽺹ڽ�,
	// ������ڰ� P2�� ��� P1�� ���񽺹ڽ�
	//m_TennisGame.in_area.CopyRect(m_TennisGame.service_box);
	m_TennisGame.in_area.SetRect(
		m_TennisGame.bCurrentServe ? m_TennisGame.service_box.left : m_TennisGame.net.right,
		LAND_TOP,
		m_TennisGame.bCurrentServe ? m_TennisGame.net.left : m_TennisGame.service_box.right,
		HEIGHT);

	// �ڵ����� ���� ����(������)
	GameStatus = WAITINGSERVE;
	SetTimer(1, 100, NULL);
}


void CChildView::ResetScore(void)
{
	m_Player[0].nScore = 0;
	m_Player[1].nScore = 0;
	for(int i = 0; i < MAX_SET; i++) {
		m_Player[0].nGame[i] = 0;
		m_Player[1].nGame[i] = 0;
	}
}


void CChildView::ResetSituation(void)
{
	// �� �ڵ�� ���α׷� ���� �Ǵ� ���� ���� ���� ����ȴ�!
	// ������ ũ�� ���濡 ���� �ȼ��� �������� OnPaint �Լ����� ����
	// ������ �ڵ�(������ ũ��������� ���� ��)
	for(int i = 0; i < 2; i++) {
		m_Player[i].bSwinging = false;
		m_Player[i].ptAcceleration.x = 0;
		m_Player[i].ptAcceleration.y = GRAVITY_ACCELERATION;
		m_Player[i].ptSituation.y = LAND_TOP;
		m_Player[i].ptVelosity.x = 0;
		m_Player[i].ptVelosity.y = 0;
		m_Player[i].player_rect.bottom = m_Player[i].ptSituation.y;
		m_Player[i].player_rect.top = m_Player[i].ptSituation.y - 48;
	}
	// ������ x ��ġ���� ���� ����
	m_Player[0].ptSituation.x = PLAYER1_SERVE_POS;
	m_Player[1].ptSituation.x = PLAYER2_SERVE_POS;
	m_Player[0].player_rect.left = m_Player[0].ptSituation.x;
	m_Player[0].player_rect.right = m_Player[0].ptSituation.x + 48;
	m_Player[1].player_rect.left = m_Player[1].ptSituation.x;
	m_Player[1].player_rect.right = m_Player[1].ptSituation.x + 48;

	// �� �Ӽ��� �ʱ�ȭ
	m_Ball.ptAcceleration.x = 0;
	m_Ball.ptAcceleration.y = GRAVITY_ACCELERATION;
	m_Ball.ptCenter.x = PLAYER1_SERVE_POS + 24;
	m_Ball.ptCenter.y = LAND_TOP - 40;
	m_Ball.ptVelosity.x = 0;
	m_Ball.ptVelosity.y = 0;

	Invalidate();
}


int CChildView::Round(double d)
{
	if((int)(d * 10) % 10 < 5)
		return (int)d;
	
	return (int)d + 1;
}


bool CChildView::Swing(int player_idx)
{
	if(m_Player[player_idx].bSwinging)
		return false;
	m_Player[player_idx].bSwinging = true;
	if(OverlapCircleArea(m_Ball.ball_rect, m_Player[player_idx].player_rect))
		return true;
	return false;
}


void CChildView::UpdateSituation(void)
{
	
	// ���� �� : ������ ũ��������� ���� �� �ڵ�
	// �ӵ� ����
	IntegratePoint(m_Player[0].ptAcceleration, &m_Player[0].ptVelosity);
	IntegratePoint(m_Player[1].ptAcceleration, &m_Player[1].ptVelosity);
	IntegratePoint(m_Ball.ptAcceleration, &m_Ball.ptVelosity);

	// ������ �ӵ��� �ְ� �ӵ��� ���� �ʵ���
	if(m_Player[0].ptVelosity.x > PLAYER_MAX_SPEED)
		m_Player[0].ptVelosity.x = PLAYER_MAX_SPEED;
	else if(m_Player[0].ptVelosity.x < -PLAYER_MAX_SPEED)
		m_Player[0].ptVelosity.x = -PLAYER_MAX_SPEED;
	if(m_Player[1].ptVelosity.x > PLAYER_MAX_SPEED)
		m_Player[1].ptVelosity.x = PLAYER_MAX_SPEED;
	else if(m_Player[1].ptVelosity.x < -PLAYER_MAX_SPEED)
		m_Player[1].ptVelosity.x = -PLAYER_MAX_SPEED;


	// ���� �������� �ӵ��� �پ��
	if(m_Player[0].ptSituation.y >= LAND_TOP)
		if(GetKeyState('D') >= 0 && GetKeyState('d') >= 0 && m_Player[0].ptVelosity.x > 0)
			m_Player[0].ptVelosity.x -= FRICTIONAL_FORCE;
		else if(GetKeyState('A') >= 0 && GetKeyState('a') >= 0 && m_Player[0].ptVelosity.x < 0)
			m_Player[0].ptVelosity.x += FRICTIONAL_FORCE;
	if(m_Player[1].ptSituation.y >= LAND_TOP)
		if(GetKeyState(VK_RIGHT) >= 0 && m_Player[1].ptVelosity.x > 0)
			m_Player[1].ptVelosity.x -= FRICTIONAL_FORCE;
		else if(GetKeyState(VK_LEFT) >= 0 && m_Player[1].ptVelosity.x < 0)
			m_Player[1].ptVelosity.x += FRICTIONAL_FORCE;
	if(m_Ball.ptCenter.y >= LAND_TOP - BALL_RADIUS)
		if(m_Ball.ptVelosity.x > 0)
			m_Ball.ptVelosity.x -= FRICTIONAL_FORCE;
		else if(m_Ball.ptVelosity.x < 0)
			m_Ball.ptVelosity.x += FRICTIONAL_FORCE;

	// ���� �� ���������� y ������ �ӵ��� 0����
	// ����Ű�� ������ ���� ����
	if(m_Player[0].ptSituation.y >= LAND_TOP && GetKeyState('W') >= 0 && GetKeyState('w') >= 0 && !IsThereWall(0))
			m_Player[0].ptVelosity.y = 0;
	if(m_Player[1].ptSituation.y >= LAND_TOP && GetKeyState(VK_UP) >= 0 && !IsThereWall(1))
			m_Player[1].ptVelosity.y = 0;
	// ���� ���� �������� �ٿ��Ǿ�� �Ѵ�.
	if(m_Ball.ptCenter.y >= LAND_TOP - BALL_RADIUS) {
		m_Ball.ptVelosity.y = Round(-m_Ball.ptVelosity.y * 0.555118);
		if(m_Ball.ptVelosity.y > 0)
			m_Ball.nBound++;
	}

	// ��ġ ����
	// �÷��� ������ ����� ���� x ������ ��ġ ��ȭ�� ��ȿ�� �Ѵ�.
	// ���� ���� �ٷ� ������ �����ϰ� ������ ����ؾ� �ϹǷ� ��ġ ������ �ʿ����
	IntegratePoint(m_Player[0].ptVelosity, &m_Player[0].ptSituation);
	IntegratePoint(m_Player[1].ptVelosity, &m_Player[1].ptSituation);
	IntegratePoint(m_Ball.ptVelosity, &m_Ball.ptCenter);
	if(IsThereWall(0) == 1)
		m_Player[0].ptSituation.x = 0;
	else if(IsThereWall(0) == 2)
		m_Player[0].ptSituation.x = NET_LEFT - PLAYER_WIDTH;
	if(IsThereWall(1) == 1)
		m_Player[1].ptSituation.x = NET_RIGHT;
	else if(IsThereWall(1) == 2)
		m_Player[1].ptSituation.x = WIDTH - PLAYER_WIDTH;
	

	// �������� ������ �ʵ��� ��ġ ����
	if(m_Player[0].ptSituation.y > LAND_TOP)
		m_Player[0].ptSituation.y = LAND_TOP;
	if(m_Player[1].ptSituation.y > LAND_TOP)
		m_Player[1].ptSituation.y = LAND_TOP;
	if(m_Ball.ptCenter.y > LAND_TOP - BALL_RADIUS)
		m_Ball.ptCenter.y = LAND_TOP - BALL_RADIUS;

	// ���� ��� ������ ���� ���� ��ġ�� ������ ������ ��ġ�� ���󰣴�.
	if(GameStatus == WAITINGSERVE) {
		m_Ball.ptCenter.x = m_Player[(int)m_TennisGame.bCurrentServe].ptSituation.x + 25;
		m_Ball.ptCenter.y = m_Player[(int)m_TennisGame.bCurrentServe].ptSituation.y - 40;
	}

	// ���� ������ ����
	for(int i = 0; i < 2; i++) {
		m_Player[i].player_rect.left = m_Player[i].ptSituation.x;
		m_Player[i].player_rect.right = m_Player[i].ptSituation.x + 48;
		m_Player[i].player_rect.bottom = m_Player[i].ptSituation.y;
		m_Player[i].player_rect.top = m_Player[i].ptSituation.y - 48;
	}

	// �� ������ ����
	m_Ball.ball_rect.left = m_Ball.ptCenter.x - BALL_RADIUS;
	m_Ball.ball_rect.right = m_Ball.ptCenter.x + BALL_RADIUS;
	m_Ball.ball_rect.top = m_Ball.ptCenter.y - BALL_RADIUS;
	m_Ball.ball_rect.bottom = m_Ball.ptCenter.y + BALL_RADIUS;

	Invalidate();

	// ���� ��Ȳ���� �˻�
	CheckBall();
}


UINT CChildView::UpperCase(UINT nChar)
{
	if(nChar >= 'a' && nChar <= 'z')
		return nChar - 32;
	return nChar;
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	return CWnd::OnEraseBkgnd(pDC);
}



