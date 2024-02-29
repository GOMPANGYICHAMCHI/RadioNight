#pragma once
#include <Windows.h>
#include <Xinput.h>

#include "KeyState.h"

#pragma comment(lib, "xinput.lib")

class InputHandler
{
public:
	static InputHandler* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputHandler();
		}
		return instance;
	}

	// �ʱ�ȭ
	InputHandler();
	~InputHandler();

	// HWND ����
	void SetHwnd(HWND input_hwnd);

	// ȭ�� ũ�� �ٽ� �ҷ�����
	void ResizeScreen();

	//������Ʈ
	void Update();

	// �׼� �߰�
	void AddNewAction(string input_InputName, VirtualKey input_virtualkey);
	// �̹��ִ� �׼ǿ� �Է� �߰� ( �ε��� )
	void AddInputtoAction(int input_index, VirtualKey input_virtualkey);
	// �̹��ִ� �׼ǿ� �Է� �߰� ( �̸� )
	void AddInputtoAction(string input_name, VirtualKey input_virtualkey);

	// �Է� Ȯ�� ( �ε��� )
	bool InputKey(int input_index);
	// �Է� ���� Ȯ�� ( �ε��� )
	bool InputKeyDown(int input_index);
	// �Է� �� Ȯ�� ( �ε��� )
	bool InputKeyUp(int input_index);

	// �Է� Ȯ�� ( �̸� )
	bool InputKey(string input_name);
	// �Է� ���� Ȯ�� ( �̸� )
	bool InputKeyDown(string input_name);
	// �Է� �� Ȯ�� ( �̸� )
	bool InputKeyUp(string input_name);

	// ���콺 ���� ���� ���Ƿ� ����
	void SetMouseRect(RECT input_rect);
	// ���� ���콺 ���� ����
	void InfiniteMouseActive(bool isactive);
	// ���콺 ȭ�� ���� ���� ����
	void MousePosClippingActive(bool isactive);
	// ���콺 Ŀ�� ��/Ȱ��ȭ ���� ����
	void MouseCursorActive(bool isactive);

	void MosueCapture(bool isactive);

	// ���콺 ȭ�� ���� ���� Ȯ���� ����
	void MouseSetByBool();
	// ���콺 ȭ�� ���� ���� �缳��
	void ResetMouseRect();

	// ��Ŀ�� ���� ����
	void SetFocus(bool isfocus);

	// ���콺�� ��Ʈ �������� �ִ��� Ȯ���ϴ� �Լ�
	bool IsMouseOnRect
	(
		float relative_top,
		float relative_bottom,
		float relative_left,
		float relative_right
	);
	// ���콺 ��Ʈ ��ȯ �Լ�
	RECT GetMouseRect();
	// ȭ�� ��Ʈ ��ȯ �Լ�
	RECT GetScreenRect();
	// ȭ�� ũ�� ��ȯ �Լ�
	int GetWidth();
	int GetHeight();

	// ���콺 ��ġ ��ȯ �Լ�
	POINT GetMousePos();
	// ȭ����� ���콺 ��ġ ��ȯ �Լ�
	POINT GetScreenSpaceMousePos();
	// ���콺 ��Ÿ ��ġ ��ȯ �Լ�
	POINT GetMouseDelta();

	// ���콺 ��Ÿ ��ġ
	POINT MousePosDelta;

	// �̱��� �ν��Ͻ�
	static InputHandler* instance;

private:
	void MousePosSet();

	// ���� �Է� Ȯ��
	void GetInputActual(int input_actionindex,int input_keyindex);

	// ���� Ű �ʱ�ȭ
	void SetActKeys();

	// ���۽� ȣ��
	void Start();

	// ���콺 ������Ʈ
	void MouseUpdate();

	// ��� HWND
	HWND mHwnd;

	// ��� �����Է� �׼ǵ�
	vector<Action> mActions;

	// �ܼ� ��Ʈ�ѷ� �Է��� ���� ������Ʈ
	XINPUT_STATE mXinput;

	// XinputDWORD
	DWORD mDwordxinput = XInputGetState(0, &mXinput);

	// ������ Ű ( �Է� ���� �Լ�ȣ��� ���� ����� ) 
	vector<int> ActualKey;

	// ȭ�� ũ��
	Vector2D ScreenSize;

	// ���콺
	POINT MousePos;
	POINT ScreenSpaceMousePos;
	POINT OldMousePos;

	// ���콺 �̵� ����
	RECT MouseRect;
	// ���콺 �̵� ����
	RECT SmallMouseRect;
	// ȭ�� ����
	RECT ScreenRect;
	// ȭ�� ����,���� ����
	int screen_width, screen_height;

	// ���콺 Ŭ���� ����
	bool isMouseClippingOn = false;
	// ���콺 ���� �巡�� ����
	bool isInfiniteMouseOn = false;
	// ���콺 ���� ����
	bool isMouseCursorActive = true;

	bool isCapture = false;

	bool isMouseMove = false;

	// ���� ����â ��Ŀ�� ����
	bool isFocus = true;

	float temp_x;
	float temp_y;
};
