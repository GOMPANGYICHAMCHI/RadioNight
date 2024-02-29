#pragma once
#include <string>
#include <vector>

using namespace std;

// 가상 키
enum class VirtualKey
{
	/// None
	//
	None = 0,

	// Mouse virtual keys
	//
	MouseLeft = 1,
	MouseRight = 2,
	MouseMiddle = 3,
	// mouse extension button 1
	Button1 = 4,
	// mouse extension button 2
	Button2 = 5,

	/// Keyboard virtual keys
	//
	Backspace = 6,
	Tab = 7,
	// VK_RETURN
	Enter = 8,
	Shift = 9,
	Control = 10,
	// VK_MENU
	Alt = 11,
	Pause = 12,
	CapsLock = 13,
	// VK_ESCAPE
	Escape = 14,
	SpaceBar = 15,
	// VK_PRIOR
	PageUp = 16,
	// VK_NEXT
	PageDown = 17,
	End = 18,
	Home = 19,
	LeftArrow = 20,
	UpArrow = 21,
	RightArrow = 22,
	DownArrow = 23,
	// VK_SNAPSHOT
	PrintScreen = 24,
	Insert = 25,
	Delete = 26,
	Key_0 = 27,
	Key_1 = 28,
	Key_2 = 29,
	Key_3 = 30,
	Key_4 = 31,
	Key_5 = 32,
	Key_6 = 33,
	Key_7 = 34,
	Key_8 = 35,
	Key_9 = 36,
	Key_A = 37,
	Key_B = 38,
	Key_C = 39,
	Key_D = 40,
	Key_E = 41,
	Key_F = 42,
	Key_G = 43,
	Key_H = 44,
	Key_I = 45,
	Key_J = 46,
	Key_K = 47,
	Key_L = 48,
	Key_M = 49,
	Key_N = 50,
	Key_O = 51,
	Key_P = 52,
	Key_Q = 53,
	Key_R = 54,
	Key_S = 55,
	Key_T = 56,
	Key_U = 57,
	Key_V = 58,
	Key_W = 59,
	Key_X = 60,
	Key_Y = 61,
	Key_Z = 62,
	// VK_LWIN
	LeftWindow = 63,
	// VK_RWIN
	RightWindow = 64,
	// Key ','
	Separator = 65,
	Numpad_0 = 66,
	Numpad_1 = 67,
	Numpad_2 = 68,
	Numpad_3 = 69,
	Numpad_4 = 70,
	Numpad_5 = 71,
	Numpad_6 = 72,
	Numpad_7 = 73,
	Numpad_8 = 74,
	Numpad_9 = 75,
	// Numpad '*'
	Multiply = 76,
	// Key '+'
	Add = 77,
	// Numpad '.'
	Decimal = 78,
	// Key '-'
	Subtract = 79,
	// Key '/'
	Divide = 80,
	F1 = 81,
	F2 = 82,
	F3 = 83,
	F4 = 84,
	F5 = 85,
	F6 = 86,
	F7 = 87,
	F8 = 88,
	F9 = 89,
	F10 = 90,
	F11 = 91,
	F12 = 92,
	NumLock = 93,
	ScrollLock = 94,
	LeftShift = 95,
	RightShift = 96,
	LeftControl = 97,
	RightControl = 98,

	// Xbox GamePad Inputs
	GamePad_DpadUp = 99,
	GamePad_DpadDown = 100,
	GamePad_DpadLeft = 101,
	GamePad_DpadRight = 102,
	GamePad_Start = 103,
	GamePad_Back = 104,
	GamePad_LeftThumb = 105,
	GamePad_RightThumb = 106,
	GamePad_LeftShoulder = 107,
	GamePad_RightShoulder = 108,
	GamePad_A = 109,
	GamePad_B = 110,
	GamePad_X = 111,
	GamePad_Y = 112,

	GamePad_LeftTrigger = 113,
	GamePad_RightTrigger = 114,

	EndNone = 115
};

// 가상 액션
// 지정된 키 입력을 특정 액션으로 들어오도록 처리
struct Action
{
	// 가상행동 명칭
	string ActionName;

	// 현재 키입력
	bool m_current = false;

	// 이전 키입력
	bool m_previous = false;

	// 해당 액션이 이미 처리되었는지 확인
	bool m_checked = false;

	// 현재 입력을 받고 있는 가상 키
	int m_currentfocus = -1;

	// 눌린 정도 ( 게임패드 트리거 )
	float m_pressamount;

	// 대상되는 가상 키
	vector<VirtualKey> InputKeys;
};

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0;
		y = 0;
	};
	Vector2D(float input_x, float input_y)
	{
		x = input_x;
		y = input_y;
	}
};
