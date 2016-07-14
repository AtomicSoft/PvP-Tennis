
// ChildView.h : CChildView Ŭ������ �������̽�
//


#pragma once


// CChildView â


//==============================================================================
// �������
//------------------------------------------------------------------------------
#define WIDTH 640				// ȭ���� �ʺ�
#define HEIGHT 480				// ȭ���� ����
#define GRAVITY_ACCELERATION 2	// �߷°��ӵ�=2pt/1frame^2
#define BALL_MAX_SPEED 16		// 
#define BALL_INITIAL_VELOSITY_X 20	// ���� ġ�� ������ �ӵ�(x����)
#define BALL_INITIAL_VELOSITY_Y -16	// ���� ġ�� ������ �ӵ�(y����)
#define BALL_RADIUS 2			// ���� ������
#define FRICTIONAL_FORCE 1		// ������

// �� ����
#define LAND_LEFT 0
#define LAND_RIGHT 640
#define LAND_TOP 460
//#define LAND_BOTTOM 480
// ��Ʈ ����
#define NET_LEFT 318
#define NET_RIGHT 321
#define NET_TOP 440
//#define NET_BOTTOM 480
// ��Ʈ ����
#define COURT_LEFT 99
#define COURT_RIGHT 541
//#define COURT_TOP 460
//#define COURT_BOTTOM 480

// ���񽺶��� ����
#define SERVICE_BOX_LEFT 192
#define SERVICE_BOX_RIGHT 448

// �÷��̾� ���� ����
#define PLAYER_MOVE_ACCELERATION 3	// ������ x�������� �̵��� ���� ���ӵ�
#define PLAYER_JUMP_VELOSITY -9		// ������ �ʱ�ӵ�
#define PLAYER_MAX_SPEED 9			// ������ x������ �ִ�ӵ�
#define PLAYER_WIDTH 48				// �÷��̾� ���� �ʺ�
#define PLAYER_HEIGHT 48			// �÷��̾� ���� ����

// �÷��̾� ���� ��ġ��
#define PLAYER1_SERVE_POS	21
#define PLAYER2_SERVE_POS	562

#define MAX_SET 2				// �� ������ 2��Ʈ�� ���� ȹ���ϸ� ���� ����
#define MAX_GAME 6				// �� ������ 6������ ���� ȹ���ϸ� 1��Ʈ ��*
#define LAST_SCORE 40			// �� ������ 40���� �ѱ�� 1���� ��*
// *���������� ���� ����
//==============================================================================

//==============================================================================
// Ű ���� ����ü
//------------------------------------------------------------------------------
struct KeySetting {
	UINT key_up;
	UINT key_left;
	UINT key_right;
	UINT key_swing;
};
//==============================================================================

//==============================================================================
// ���� ����ü
//------------------------------------------------------------------------------
struct Player {
	// ��ġ���� ������ �Ÿ��� ����ϴ� ���� ���� ����
	// ���� �Ʒ� ������ �������� �Ѵ�.
	CRect player_rect;		// �÷��̾ �����ϴ� �κ�
	CPoint ptSituation;		// ��ġ��
	CPoint ptVelosity;		// �ӵ���
	CPoint ptAcceleration;	// ���ӵ���
	CBitmap bmpPlayer;		// ���� ��Ʈ��
	bool bSwinging;			// ���� �������� üũ
	KeySetting key_set;		// Ű���� ���ð�
	int nScore[MAX_SET][MAX_GAME];	// ��Ʈ/���Ӻ� ����
	int nFootFault;			// Foot Fault Ƚ��
};
//==============================================================================

//==============================================================================
// �� ����ü
//------------------------------------------------------------------------------
struct Ball {
	// ��ġ���� ������ �Ÿ��� ����ϴ� ���� ���� ����
	// ���� �Ʒ� ������ �������� �Ѵ�.
	CPoint ptCenter;		// ��ġ��
	CPoint ptVelosity;		// �ӵ���
	CPoint ptAcceleration;	// ���ӵ���
	CRect ball_rect;		// ���� ����
	int nBound;			// ���� �ٿ�� Ƚ��(0~2)
};
//==============================================================================

//==============================================================================
// ���� ����ü
// 
//------------------------------------------------------------------------------
struct TennisGame {
	bool bCurrentServe;	// ���� ����(false:p1, true:p2)
	bool bLastTouch;	// ���������� ���� ģ ���(false:p1, true:p2)
	int nCurrentGame;	// ���� ����
	int nCurrentSet;	// ���� ��Ʈ
	int nHResolution;	// ���� �ȼ���
	int nVResolution;	// ���� �ȼ���
	// �Ʒ� �������� ���� ������ �� �Ǵ� ����
	// �ʱ�ȭ�� ��, ���� ������ �¿���� 1pt�� �а� ��´�.
	CRect net;			// ��Ʈ ����
	CRect land;			// �� ����
	CRect court;		// ��Ʈ ����
	CRect service_box;		// ���� �ڽ� ����
};
//==============================================================================



class CChildView : public CWnd
{
// �����Դϴ�.
public:
	CChildView();

// Ư���Դϴ�.
public:
	Player m_Player[2];			// ���� ����ü
	Ball m_Ball;				// �� ����ü
	TennisGame m_TennisGame;	// ���� ����ü

// �۾��Դϴ�.
public:

// �������Դϴ�.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildView();

	// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGameStart();
	afx_msg void OnGamePause();
	afx_msg void OnGameEnd();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateGamePause(CCmdUI *pCmdUI);
	
	// ���� ��Ȳ�̸� ������Ű�� ���� ��� ���·� ��ȯ
	void CheckBall(void);

	// �־��� ��ǥ��(pt)�� x �Ǵ� y���� ���� �ػ󵵿� �´� �ȼ������� ��ȯ
	// axis�� : false=x, true=y
	CPoint FitToResolution(CPoint pt);

	// ���� ��ǥ���� �� ��ǥ���� ����
	//   1. �־��� ���ӵ��� �ӵ��� ����
	//   2. �־��� �ӵ��� ��ġ�� ����
	void IntegratePoint(CPoint addPoint, CPoint* currentPoint);

	//// �־��� ���ڰ� �������̰�
	//// ��ҹ��� ���� ���� ���� �������� �˻�
	//bool IsSameAlphabet(char c1, char c2);
	
	// ������ �÷��� ���� ���� �ִ��� �˻�
	int IsThereWall(int player_idx);

	// ���� �־��� ����(���簢��)�� ��ġ���� üũ
	bool OverlapCircleArea(CRect ellipse, CRect rect);

	// ������ ������ �� ����
	// �ش� ������ Ű�� �޾� ���ӵ����� ó��
	void PlayerMove(int player_idx, UINT command);

	//// x�� �Ǵ� y�� ��ǥ���� �Ÿ��� �ش� ���� �ִ�Ÿ��� �Է¹޾�
	//// �ȼ����� �´� �Ÿ��� ����
	//int ReCalcDistance(int dt, int maxDt);

	// ���� ���̰� ����
	int ReCalcWidth(int width);

	// ���� ���̰� ����
	int ReCalcHeight(int height);

	// ���� �ʱ�ȭ(���α׷� ���� ���� �Ǵ� ���� ����۽�)
	void ResetGame(void);

	// ���α׷� �ʱ�ȭ �Ǵ� �������� ��� ��
	// ������ ���� ���ӵ�, �ӵ�, ��ġ���� �ʱ�ȭ
	void ResetSituation(void);

	// ���ھ� �ʱ�ȭ
	void ResetScore(void);	
	
	// �ݿø�
	int Round(double d);

	// ���� ĥ �� ����
	// ���� ���߸� true, �� ���߸� false�� ��ȯ
	bool Swing(int player_idx);

	// ������ ���� �ӵ��� ��ġ�� ����
	void UpdateSituation(void);

	// ���� �����͸� �޾Ƽ� ���� �ҹ����̸� �빮�ڷ� ��ȯ�ϰ�
	// �ƴϸ� �״�� ��ȯ�Ѵ�.
	UINT UpperCase(UINT nChar);
};

