#include "framework.h"
#include "InputHandler.h"
#include <math.h>

InputHandler* InputHandler::instance = nullptr;

InputHandler::InputHandler()
{
	// ���� HWND ����
	mHwnd = GetActiveWindow();
	// ȭ�� ũ�� ����
	//ResizeScreen();
	// ���۽� ȣ�� �Լ� 
	Start();
}

InputHandler::~InputHandler()
{
	
}

void InputHandler::Start()
{
	SetActKeys();

	//==========================================================================
	// ĳ���� �⺻���� ==========================================================
	//==========================================================================
	AddNewAction("MoveForward", VirtualKey::Key_W);
	AddInputtoAction("MoveForward", VirtualKey::GamePad_DpadUp);

	AddNewAction("MoveBackward", VirtualKey::Key_S);
	AddInputtoAction("MoveBackward", VirtualKey::GamePad_DpadDown);

	AddNewAction("MoveRight", VirtualKey::Key_D);
	AddInputtoAction("MoveRight", VirtualKey::GamePad_DpadRight);

	AddNewAction("MoveLeft", VirtualKey::Key_A);
	AddInputtoAction("MoveLeft", VirtualKey::GamePad_DpadLeft);

	AddNewAction("Shoot", VirtualKey::MouseLeft);

	AddNewAction("ReLoad", VirtualKey::Key_R);

	AddNewAction("Dash", VirtualKey::LeftShift);

	AddNewAction("MouseUp", VirtualKey::UpArrow);
	AddNewAction("MouseDown", VirtualKey::DownArrow);
	//==========================================================================
	// �ӽ����� =================================================================
	//==========================================================================
	AddNewAction("Press1", VirtualKey::F1);
	AddNewAction("Press2", VirtualKey::F2);
	AddNewAction("Press3", VirtualKey::F3);
	AddNewAction("Press4", VirtualKey::F4);
	//==========================================================================
}

void InputHandler::MouseUpdate()
{
	GetCursorPos(&MousePos);

	ScreenSpaceMousePos = MousePos;

	// �ٷ� ���� ������ �ڵ鿡 client area ��ǥ������ ��ȯ�ȴ�.
	// �Ʒ��� �Լ� �� ����Ǹ� ��ü ��ũ����ǥ ���ؿ��� client area ������ ��ǥ�� ����ȴ�.
	ScreenToClient(mHwnd, &ScreenSpaceMousePos);

	// ���콺 ��Ÿ ��� 
	MousePosDelta.x = MousePos.x - OldMousePos.x;
	MousePosDelta.y = MousePos.y - OldMousePos.y;

	// ���Ѹ��콺 ���� üũ
	if (isInfiniteMouseOn)
	{
		// ���콺 ��ġ ���� ( ���Ѹ��콺 )
		MousePosSet();
	}

	// ���� ��Ÿ�� ���� ���� ���콺 ��ġ ���� ���콺 ��ġ�� ���
	OldMousePos = MousePos;
}

void InputHandler::SetActKeys()
{
	ActualKey =
	{
		0xff,
		0x01,
		0x02,
		0x04,
		0x05,
		0x06,
		0x08,
		0x09,
		0x0D,
		0x10,
		0x11,
		0x12,
		0x13,
		0x14,
		0x1B,
		0x20,
		0x21,
		0x22,
		0x23,
		0x24,
		0x25,
		0x26,
		0x27,
		0x28,
		0x2C,
		0x2D,
		0x2E,
		0x30,
		0x31,
		0x32,
		0x33,
		0x34,
		0x35,
		0x36,
		0x37,
		0x38,
		0x39,
		0x41,
		0x42,
		0x43,
		0x44,
		0x45,
		0x46,
		0x47,
		0x48,
		0x49,
		0x4A,
		0x4B,
		0x4C,
		0x4D,
		0x4E,
		0x4F,
		0x50,
		0x51,
		0x52,
		0x53,
		0x54,
		0x55,
		0x56,
		0x57,
		0x58,
		0x59,
		0x5A,
		0x5B,
		0x5C,
		0x6C,
		0x60,
		0x61,
		0x62,
		0x63,
		0x64,
		0x65,
		0x66,
		0x67,
		0x68,
		0x69,
		0x6A,
		0x6B,
		0x6E,
		0x6D,
		0x6F,
		0x70,
		0x71,
		0x72,
		0x73,
		0x74,
		0x75,
		0x76,
		0x77,
		0x78,
		0x79,
		0x7A,
		0x7B,
		0x90,
		0x91,
		0xA0,
		0xA1,
		0xA2,
		0xA3,
		0x0001,
		0x0002,
		0x0004,
		0x0008,
		0x0010,
		0x0020,
		0x0040,
		0x0080,
		0x0100,
		0x0200,
		0x1000,
		0x2000,
		0x4000,
		0x8000,
	};
}

void InputHandler::SetHwnd(HWND input_hwnd)
{
	// ���� HWND �缳��
	mHwnd = input_hwnd;
	// ȭ��ũ�� �缳��
	ResizeScreen();

	MosueCapture(isCapture);
}

void InputHandler::ResizeScreen()
{
	RECT rt;
	GetClientRect(mHwnd, &rt);

	ScreenSize = Vector2D(rt.right - rt.left, rt.bottom - rt.top);
}

void InputHandler::Update()
{
	if (isFocus)
	{
		mDwordxinput = XInputGetState(0, &mXinput);
		MouseUpdate();

		// �����ϴ� �׼��� ����ŭ �ݺ�
		for (int ActionIndex = 0; ActionIndex < mActions.size(); ActionIndex++)
		{
			if (mActions[ActionIndex].m_currentfocus != -1)
			{
				GetInputActual(ActionIndex, mActions[ActionIndex].m_currentfocus);

				if (InputKeyUp(ActionIndex))
				{
					mActions[ActionIndex].m_currentfocus = -1;
				}

				continue;
			}

			// ���� �׼ǿ� ����� ����Ű�� ����ŭ �ݺ�
			for (int KeyIndex = 0; KeyIndex < mActions[ActionIndex].InputKeys.size(); KeyIndex++)
			{
				if (mActions[ActionIndex].m_checked)
				{
					mActions[ActionIndex].m_checked = false;
					break;
				}

				GetInputActual(ActionIndex, KeyIndex);
			}
		}
	}
}

void InputHandler::AddNewAction(string input_InputName, VirtualKey input_virtualkey)
{
	// ���ο� �����Է� ����
	Action temp_input;

	// �����Է��� �̸� ����
	temp_input.ActionName = input_InputName;

	// ����Ű ����
	temp_input.InputKeys.push_back(input_virtualkey);

	// �����Է� �׼� ������ ����
	mActions.push_back(temp_input);
}

void InputHandler::AddInputtoAction(int input_index, VirtualKey input_virtualkey)
{
	// ��� �����Է¿� ����Ű ����
	mActions[input_index].InputKeys.push_back(input_virtualkey);
}

void InputHandler::AddInputtoAction(string input_name, VirtualKey input_virtualkey)
{
	for (int i = 0; i < mActions.size(); i++)
	{
		if (mActions[i].ActionName == input_name)
		{
			// ��� �����Է¿� ����Ű ����
			mActions[i].InputKeys.push_back(input_virtualkey);
		}
	}
}

bool InputHandler::InputKey(int input_index)
{
	return mActions[input_index].m_current;
}

bool InputHandler::InputKeyDown(int input_index)
{
	return !mActions[input_index].m_previous
		&& mActions[input_index].m_current;
}

bool InputHandler::InputKeyUp(int input_index)
{
	return mActions[input_index].m_previous
		&& !mActions[input_index].m_current;
}

bool InputHandler::InputKey(string input_name)
{
	for (int i = 0; i < mActions.size(); i++)
	{
		if (mActions[i].ActionName == input_name)
		{
			return mActions[i].m_current;
		}
	}

	return false;
}

bool InputHandler::InputKeyDown(string input_name)
{
	for (int i = 0; i < mActions.size(); i++)
	{
		if (mActions[i].ActionName == input_name)
		{
			return !mActions[i].m_previous
				&& mActions[i].m_current;
		}
	}

	return false;
}

bool InputHandler::InputKeyUp(string input_name)
{
	for (int i = 0; i < mActions.size(); i++)
	{
		if (mActions[i].ActionName == input_name)
		{
			return mActions[i].m_previous
				&& !mActions[i].m_current;
		}
	}

	return false;
}

void InputHandler::SetMouseRect(RECT input_rect)
{
	MouseRect = input_rect;
}

void InputHandler::InfiniteMouseActive(bool isactive)
{
	isInfiniteMouseOn = isactive;
}

void InputHandler::MousePosClippingActive(bool isactive)
{
	ResetMouseRect();
	isMouseClippingOn = isactive;

	if (isactive)
	{
		ClipCursor(&MouseRect);
	}
	else
	{
		ClipCursor(NULL);
	}
}

void InputHandler::MouseSetByBool()
{
	MousePosClippingActive(isMouseClippingOn);
	MouseCursorActive(isMouseCursorActive);
}

void InputHandler::ResetMouseRect()
{
	GetWindowRect(mHwnd, &ScreenRect);

	screen_width = ScreenRect.right - ScreenRect.left;
	screen_height = ScreenRect.bottom - ScreenRect.top;

	MouseRect = ScreenRect;

	MouseRect.left += ScreenRect.right / 90;
	MouseRect.right -= ScreenRect.right / 90;
	
	MouseRect.top += ScreenRect.bottom / 30;
	MouseRect.bottom -= ScreenRect.bottom / 90;

	//======================================================

	SmallMouseRect = ScreenRect;

	SmallMouseRect.left += ScreenRect.right / 3;
	SmallMouseRect.right -= ScreenRect.right / 3;

	SmallMouseRect.top += ScreenRect.bottom / 3;
	SmallMouseRect.bottom -= ScreenRect.bottom / 3;

	// ����� �뵵
	//cout << "// ScreenRect =======================" << endl;
	//cout << "Left : " << ScreenRect.left << endl;
	//cout << "Rigth : " << ScreenRect.right << endl;
	//cout << "Top : " << ScreenRect.top << endl;
	//cout << "Bottom : " << ScreenRect.bottom << endl;
	//cout << "//====================================" << endl;
	//cout << "// MouseRect =========================" << endl;
	//cout << "Left : " << MouseRect.left << endl;
	//cout << "Rigth : " << MouseRect.right << endl;
	//cout << "Top : " << MouseRect.top << endl;
	//cout << "Bottom : " << MouseRect.bottom << endl;
	//cout << "//====================================" << endl;
}

void InputHandler::SetFocus(bool isfocus)
{
	isFocus = isfocus;

	if (!isFocus)
	{
		ShowCursor(true);
		ClipCursor(NULL);
	}
	else
	{
		MouseSetByBool();
	}
}

bool InputHandler::IsMouseOnRect
(float relative_top, float relative_bottom, float relative_left, float relative_right)
{
	temp_x = ScreenRect.left + (screen_width * relative_left);
	temp_y = ScreenRect.top + (screen_height * relative_top);

	return
		MousePos.x > temp_x &&
		MousePos.x < temp_x + (screen_width * relative_right) &&
		MousePos.y > temp_y &&
		MousePos.y < temp_y + (screen_height * relative_bottom);
}

void InputHandler::MouseCursorActive(bool isactive)
{
	// ���콺 �����
	if (isactive)
	{
		ShowCursor(true);
		isMouseCursorActive = true;
	}
	else
	{
		ShowCursor(false);
		isMouseCursorActive = false;
	}
}

void InputHandler::MosueCapture(bool isactive)
{
	if (isactive)
	{
		SetCapture(mHwnd);
		isCapture = true;
	}
	else
	{
		ReleaseCapture();
		isCapture = false;
	}
}

void InputHandler::MousePosSet()
{
	if (
		MousePos.x <= MouseRect.left + 5 ||
		MousePos.x >= MouseRect.right - 5||
		MousePos.y <= MouseRect.top + 5 ||
		MousePos.y >= MouseRect.bottom - 5
		)
	{
		SetCursorPos((ScreenRect.left + ScreenRect.right) / 2, (ScreenRect.top + ScreenRect.bottom) / 2);
		GetCursorPos(&MousePos);
	}
}

void InputHandler::GetInputActual(int input_actionindex, int input_keyindex)
{
	// Ű���� �Է� �� ���,
	if ((int)mActions[input_actionindex].InputKeys[input_keyindex] < 99)
	{
		mActions[input_actionindex].m_previous = mActions[input_actionindex].m_current;
		mActions[input_actionindex].m_current =
			HIWORD(GetAsyncKeyState(ActualKey[(int)mActions[input_actionindex].InputKeys[input_keyindex]]));

		if (mActions[input_actionindex].m_current)
		{
			mActions[input_actionindex].m_checked = true;
			mActions[input_actionindex].m_currentfocus = input_keyindex;
		}
	}

	// �����е� �Է� �� ���,
	else if ((int)mActions[input_actionindex].InputKeys[input_keyindex] < 113)
	{
		mActions[input_actionindex].m_previous = mActions[input_actionindex].m_current;
		mActions[input_actionindex].m_current =
			mXinput.Gamepad.wButtons & ActualKey[(int)mActions[input_actionindex].InputKeys[input_keyindex]];

		if (mActions[input_actionindex].m_current)
		{
			mActions[input_actionindex].m_checked = true;
			mActions[input_actionindex].m_currentfocus = input_keyindex;
		}
	}

	// ���� Ʈ����
	else if ((int)mActions[input_actionindex].InputKeys[input_keyindex] == 113)
	{
		mActions[input_actionindex].m_previous = mActions[input_actionindex].m_current;
		mActions[input_actionindex].m_pressamount = mXinput.Gamepad.bLeftTrigger;

		if (mActions[input_actionindex].m_pressamount == 0)
		{
			mActions[input_actionindex].m_current = false;
		}
		else
		{
			mActions[input_actionindex].m_current = true;
		}

		if (mActions[input_actionindex].m_current)
		{
			mActions[input_actionindex].m_checked = true;
			mActions[input_actionindex].m_currentfocus = input_keyindex;
		}
	}

	// ������ Ʈ����
	else if ((int)mActions[input_actionindex].InputKeys[input_keyindex] == 114)
	{
		mActions[input_actionindex].m_previous = mActions[input_actionindex].m_current;
		mActions[input_actionindex].m_pressamount = mXinput.Gamepad.bRightTrigger;

		if (mActions[input_actionindex].m_pressamount == 0)
		{
			mActions[input_actionindex].m_current = false;
		}
		else
		{
			mActions[input_actionindex].m_current = true;
		}

		if (mActions[input_actionindex].m_current)
		{
			mActions[input_actionindex].m_checked = true;
			mActions[input_actionindex].m_currentfocus = input_keyindex;
		}
	}
}

RECT InputHandler::GetMouseRect()
{
	return MouseRect;
}

RECT InputHandler::GetScreenRect()
{
	return ScreenRect;
}

int InputHandler::GetWidth()
{
	return screen_width;
}

int InputHandler::GetHeight()
{
	return screen_height;
}

POINT InputHandler::GetMousePos()
{
	return MousePos;
}

POINT InputHandler::GetScreenSpaceMousePos()
{
	return ScreenSpaceMousePos;
}
 
POINT InputHandler::GetMouseDelta()
{
	return MousePosDelta;
}
