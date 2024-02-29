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

	// 초기화
	InputHandler();
	~InputHandler();

	// HWND 세팅
	void SetHwnd(HWND input_hwnd);

	// 화면 크기 다시 불러오기
	void ResizeScreen();

	//업데이트
	void Update();

	// 액션 추가
	void AddNewAction(string input_InputName, VirtualKey input_virtualkey);
	// 이미있는 액션에 입력 추가 ( 인덱스 )
	void AddInputtoAction(int input_index, VirtualKey input_virtualkey);
	// 이미있는 액션에 입력 추가 ( 이름 )
	void AddInputtoAction(string input_name, VirtualKey input_virtualkey);

	// 입력 확인 ( 인덱스 )
	bool InputKey(int input_index);
	// 입력 눌림 확인 ( 인덱스 )
	bool InputKeyDown(int input_index);
	// 입력 뗌 확인 ( 인덱스 )
	bool InputKeyUp(int input_index);

	// 입력 확인 ( 이름 )
	bool InputKey(string input_name);
	// 입력 눌림 확인 ( 이름 )
	bool InputKeyDown(string input_name);
	// 입력 뗌 확인 ( 이름 )
	bool InputKeyUp(string input_name);

	// 마우스 제한 영역 임의로 설정
	void SetMouseRect(RECT input_rect);
	// 무한 마우스 여부 설정
	void InfiniteMouseActive(bool isactive);
	// 마우스 화면 고정 여부 설정
	void MousePosClippingActive(bool isactive);
	// 마우스 커서 비/활성화 여부 설정
	void MouseCursorActive(bool isactive);

	void MosueCapture(bool isactive);

	// 마우스 화면 고정 변수 확인후 적용
	void MouseSetByBool();
	// 마우스 화면 고정 영역 재설정
	void ResetMouseRect();

	// 포커스 여부 설정
	void SetFocus(bool isfocus);

	// 마우스가 렉트 범위내에 있는지 확인하는 함수
	bool IsMouseOnRect
	(
		float relative_top,
		float relative_bottom,
		float relative_left,
		float relative_right
	);
	// 마우스 렉트 반환 함수
	RECT GetMouseRect();
	// 화면 렉트 반환 함수
	RECT GetScreenRect();
	// 화면 크기 반환 함수
	int GetWidth();
	int GetHeight();

	// 마우스 위치 반환 함수
	POINT GetMousePos();
	// 화면기준 마우스 위치 반환 함수
	POINT GetScreenSpaceMousePos();
	// 마우스 델타 위치 반환 함수
	POINT GetMouseDelta();

	// 마우스 델타 위치
	POINT MousePosDelta;

	// 싱글톤 인스턴스
	static InputHandler* instance;

private:
	void MousePosSet();

	// 실질 입력 확인
	void GetInputActual(int input_actionindex,int input_keyindex);

	// 실질 키 초기화
	void SetActKeys();

	// 시작시 호출
	void Start();

	// 마우스 업데이트
	void MouseUpdate();

	// 대상 HWND
	HWND mHwnd;

	// 모든 가상입력 액션들
	vector<Action> mActions;

	// 콘솔 컨트롤러 입력을 위한 스테이트
	XINPUT_STATE mXinput;

	// XinputDWORD
	DWORD mDwordxinput = XInputGetState(0, &mXinput);

	// 실질적 키 ( 입력 관련 함수호출시 들어가는 내용들 ) 
	vector<int> ActualKey;

	// 화면 크기
	Vector2D ScreenSize;

	// 마우스
	POINT MousePos;
	POINT ScreenSpaceMousePos;
	POINT OldMousePos;

	// 마우스 이동 범위
	RECT MouseRect;
	// 마우스 이동 범위
	RECT SmallMouseRect;
	// 화면 범위
	RECT ScreenRect;
	// 화면 가로,세로 길이
	int screen_width, screen_height;

	// 마우스 클리핑 여부
	bool isMouseClippingOn = false;
	// 마우스 무한 드래그 여부
	bool isInfiniteMouseOn = false;
	// 마우스 가시 여부
	bool isMouseCursorActive = true;

	bool isCapture = false;

	bool isMouseMove = false;

	// 현재 게임창 포커싱 여부
	bool isFocus = true;

	float temp_x;
	float temp_y;
};
