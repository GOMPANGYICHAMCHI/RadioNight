#pragma once
#include <unordered_map>
#include "InputHandler.h";

enum class UIType
{
	type_Button,
	type_ImgPanel,
	type_AnimationImg,
	type_Gauge,
	type_Text
};

struct FloatRect
{
	float top;
	float left;
	float bottom;
	float right;

	FloatRect() : top(0), left(0), bottom(0), right(0) {}
	FloatRect(float input_top, float input_bottom, float input_left, float input_right)
	{
		top = input_top;
		bottom = input_bottom;
		left = input_left;
		right = input_right;
	}
};

// UI 기본 구성
struct UIelement
{
	UIelement(UINT layer = 0)
		:layer(layer)
	{

	}

	// UI 크기 ( Rect )
	FloatRect rect;
	// UI 캔버스
	Canvas* canvas;
	// UI 타입
	UIType type;

	UINT layer;

	// 활성화 여부
	bool isActive = true;

	// 업데이트
	virtual void Update(float deltatime) {};
	// 렌더
	virtual void Render() {};
};

// 텍스트 
struct Text : public UIelement
{
	// 텍스트 내용
	wstring textContent;
	// 폰트 크기
	float textSize;
	// 텍스트 컬러
	XMFLOAT4 textColor;

	// 폰트타입
	fontType fonttype;

	// 폰트
	DXTKFont* font;

	Text
	(
		wstring input_textcontent,
		FloatRect input_rect,
		DXTKFont* input_font,
		fontType input_type,
		UINT layer = 0,
		XMFLOAT4 input_color = { 1,1,1,1 },
		float input_textSize = 1
	)
		:UIelement(layer)
	{
		textContent = input_textcontent;
		rect = input_rect;
		textColor = input_color;
		textSize = input_textSize;
		font = input_font;
		fonttype = input_type;

		type = UIType::type_Text;
	};

	virtual void Update(float deltatime) override
	{
		if (isActive)
		{

		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			font->DrawTextColor
			(
				InputHandler::GetInstance()->GetWidth() * rect.left, 
				InputHandler::GetInstance()->GetHeight() * rect.top, 
				textColor, fonttype,(TCHAR*)textContent.c_str(), textSize
			);
		}
	}
};

// 텍스트 
struct TextAnimation : public UIelement
{
	// 텍스트 내용
	wstring currenttext;
	// 텍스트 애니메이션 벡터
	vector<wstring> content_vec;
	// 현재 인덱스
	int currentIndex = 0;
	// 애니메이션 간격
	float animInterval;
	// 루프 여부
	bool isLoop = false;

	// 폰트 크기
	float textSize;
	// 텍스트 컬러
	XMFLOAT4 textColor;

	// 폰트타입
	fontType fonttype;

	// 폰트
	DXTKFont* font;

	// 시간체크용 변수
	float timeCheck = 0;

	TextAnimation
	(
		vector<wstring> input_textcontent,
		FloatRect input_rect,
		bool input_isLoop,
		float input_animInterval,
		DXTKFont* input_font,
		fontType input_type,
		UINT layer = 0,
		XMFLOAT4 input_color = { 1,1,1,1 },
		float input_textSize = 1
	)
		:UIelement(layer)
	{
		content_vec = input_textcontent;
		currenttext = content_vec[0];
		isLoop = input_isLoop;
		animInterval = input_animInterval;
		textSize = input_textSize;
		rect = input_rect;
		textColor = input_color;
		font = input_font;
		fonttype = input_type;

		type = UIType::type_Text;
	};

	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			timeCheck += Time::Get()->Delta();
			if (timeCheck >= animInterval)
			{
				timeCheck = 0;

				if (currentIndex == content_vec.size() - 1)
				{
					if (isLoop)
					{
						currentIndex = 0;
					}
				}
				else
				{
					currentIndex++;
				}
				currenttext = content_vec[currentIndex];
			}
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			font->DrawTextColor
			(
				InputHandler::GetInstance()->GetWidth() * rect.left,
				InputHandler::GetInstance()->GetHeight() * rect.top,
				textColor, fonttype, (TCHAR*)currenttext.c_str(), textSize
			);
		}
	}
};

// 이미지 패널
struct ImgPanel : public UIelement
{
	// 이미지 리소스
	Resource<ID3D11ShaderResourceView> img;

	ImgPanel() { type = UIType::type_ImgPanel; };
	ImgPanel(Resource<ID3D11ShaderResourceView> input_img, FloatRect input_rect,
		UINT layer = 0)
		:UIelement(layer)
	{
		img = input_img;
		rect = input_rect;

		canvas = new Canvas(rect.left, rect.top, rect.right, rect.bottom, layer);
		canvas->SetSRV(&img);
		type = UIType::type_ImgPanel;
	};
	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			canvas->Update();
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			canvas->Render();
		}
	}
};

// 다이미지 게이지
struct Gauge_MultiImage : public UIelement
{
	// 빈 이미지
	Resource<ID3D11ShaderResourceView> empty_img;
	// 찬 이미지
	Resource<ID3D11ShaderResourceView> fill_img;

	// 캔버스 벡터
	vector<Canvas*> canvas_vec;

	// 최대 크기
	int maxsize;
	// 현재 크기
	float currentamount;

	Gauge_MultiImage() { type = UIType::type_Gauge; };
	Gauge_MultiImage
	(
		Resource<ID3D11ShaderResourceView> input_emptyimg,
		Resource<ID3D11ShaderResourceView> input_fullimg,
		FloatRect input_rect,
		int input_maxamount,
		float angleincrease,
		float interval,
		UINT layer = 0
	) :UIelement(layer)
	{
		empty_img = input_emptyimg;
		fill_img = input_fullimg;
		
		rect = input_rect;
		maxsize = input_maxamount;

		float current_height = rect.top;
		float current_interval = rect.left;

		for (int i = 0; i < maxsize; i++)
		{
			Canvas* temp_canvas = new Canvas(current_interval, current_height, rect.right, rect.bottom, layer);
			temp_canvas->SetSRV(&empty_img);
			canvas_vec.push_back(temp_canvas);
		
			current_interval = current_interval + interval;
			current_height = current_height - angleincrease;
		}
		
		currentamount = 0;
		type = UIType::type_Gauge;
	};
	~Gauge_MultiImage()
	{
		for (auto temp_canvas : canvas_vec)
		{
			SAFE_DELETE(temp_canvas);
		}
	}

	void SetAmount(float input_amount)
	{
		if (input_amount > 1)
		{
			currentamount = 1;
		}
		else
		{
			currentamount = input_amount;
		}

		for (int i = 0; i < canvas_vec.size(); i++)
		{
			if (i == 0 && currentamount != 0)
			{
				canvas_vec[i]->SetSRV(&fill_img);
			}
			else
			{
				if (i + 1 <= currentamount * maxsize)
				{
				canvas_vec[i]->SetSRV(&fill_img);
				}
					else
				{
				canvas_vec[i]->SetSRV(&empty_img);
				}
			}
		}
	}

	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			for (int i = 0; i < canvas_vec.size(); i++)
			{
				canvas_vec[i]->Update();
			}
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			for (int i = 0; i < canvas_vec.size(); i++)
			{
				canvas_vec[i]->Render();
			}
		}
	}
};

// 싱글이미지 게이지
struct Gauge_SingleImage : public UIelement
{
	// 배경 이미지
	Resource<ID3D11ShaderResourceView> back_img;
	// 게이지 이미지
	vector<Resource<ID3D11ShaderResourceView>> fill_img;

	// 배경 렉트
	FloatRect backrect;

	// 배경 캔버스
	Canvas* backcanvas;

	// 현재 크기
	float currentamount;

	Gauge_SingleImage() { type = UIType::type_Gauge; };
	Gauge_SingleImage
	(
		Resource<ID3D11ShaderResourceView> input_backimg,
		vector<Resource<ID3D11ShaderResourceView>> input_fillimg,
		FloatRect input_rect,
		UINT layer = 0
	) :UIelement(layer)
	{
		back_img = input_backimg;
		fill_img = input_fillimg;
		rect = input_rect;
		backrect = input_rect;

		canvas = new Canvas(rect.left, rect.top, rect.right, rect.bottom, layer);
		backcanvas = new Canvas(rect.left, rect.top, rect.right, rect.bottom, layer);

		canvas->SetSRV(&fill_img[0]);
		backcanvas->SetSRV(&back_img);

		currentamount = 1;
		type = UIType::type_Gauge;
	};
	~Gauge_SingleImage()
	{
		SAFE_DELETE(canvas);
	}

	void SetImage(int index)
	{
		if (index > fill_img.size() - 1)
		{
			index = fill_img.size() - 1;
		}

		canvas->SetSRV(&fill_img[index]);
	}

	void SetAmount(float input_amount)
	{
		if (input_amount > 1)
		{
			currentamount = 1;
		}
		else
		{
			currentamount = input_amount;
		}

		canvas->ReSizeCanvas(rect.left, rect.top, rect.right * currentamount, rect.bottom);
	}

	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			backcanvas->Update();
			canvas->Update();
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			backcanvas->Render();
			canvas->Render();
		}
	}
};

// 버튼
struct Button : public UIelement
{
	// 버튼 이름
	string name;
	// 레코더 ( 버튼이 눌렸다는것을 기록할 스트링 포인터 )
	string& recorder;
	// 폰트 크기
	int fontsize;
	// Text 조정값
	FloatRect adjustrect;
	// 현재 포커싱 여부
	bool is_on;
	bool is_oned;

	// 버튼 이미지들
	Resource<ID3D11ShaderResourceView> img_idle;
	Resource<ID3D11ShaderResourceView> img_on;
	Resource<ID3D11ShaderResourceView> img_onclicked;

	Button
	(
		FloatRect input_rect,
		Resource<ID3D11ShaderResourceView> inputimg_idle,
		Resource<ID3D11ShaderResourceView> inputimg_on,
		Resource<ID3D11ShaderResourceView> inputimg_onclicked,
		string input_name,
		string& input_btnrecord,
		UINT layer = 0
	)
		:UIelement(layer), is_on(false), is_oned(false), recorder(input_btnrecord)
	{
		name = input_name;
		recorder = input_btnrecord;
		img_idle = inputimg_idle;
		img_on = inputimg_on;
		img_onclicked = inputimg_onclicked;

		rect.top = input_rect.top;
		rect.bottom = input_rect.bottom;
		rect.left = input_rect.left;
		rect.right = input_rect.right;

		canvas = new Canvas(rect.left, rect.top, rect.right, rect.bottom, layer);
		canvas->SetSRV(&img_idle);
		
		type = UIType::type_Button;
	}
	~Button()
	{
		
	}
	void SetFont
	(string input_name,int input_fontsize,FloatRect input_adjustrect = FloatRect(0,0,0,0))
	{
		name = input_name;
		fontsize = input_fontsize;
		adjustrect = input_adjustrect;
	}
	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			canvas->Update();

			is_on = false;

			is_on = InputHandler::GetInstance()->
				IsMouseOnRect(rect.top, rect.bottom, rect.left, rect.right);
			if (is_on)
			{
				canvas->SetSRV(&img_on);
				if (InputHandler::GetInstance()->InputKey("Shoot"))
				{
					canvas->SetSRV(&img_onclicked);
					recorder = name;
				}
			}
			else
			{
				canvas->SetSRV(&img_idle);
			}

			if (is_on && !is_oned)
			{
				SoundManager::GetInstance()->PlaySFT("UI_mouse_on");
			}

			is_oned = is_on;
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			canvas->Render();
		}
	}
};

// 애니메이션 이미지
struct AnimationImg : public UIelement
{
	// 현재 측정 시간
	float currenttime;
	// 프레임 당 시간
	float framepertime;

	// 현재 인덱스
	int currentindex = 0;
	// 정적 애니메이션 ( 시간의 흐름에 영향을 받지 않음 )
	bool isstatic = false;

	// 애니메이션 리소스 ( 순서대로 넣을것 )
	vector<Resource<ID3D11ShaderResourceView>> animation;

	AnimationImg(){type = UIType::type_AnimationImg;}
	~AnimationImg()
	{
		for (auto temp : animation)
		{
			
		}
	}
	AnimationImg
	(
		FloatRect input_rect, 
		float input_framepertime, 
		vector<Resource<ID3D11ShaderResourceView>> input_anim,
		UINT layer = 0
	)
		:UIelement(layer)
	{
		rect = input_rect;
		canvas = new Canvas(rect.left, rect.top, rect.right, rect.bottom, layer);
		framepertime = input_framepertime;
		animation = input_anim;

		canvas->SetSRV(&animation[0]);
		type = UIType::type_AnimationImg;
	}

	void SetAnimIndex(int index)
	{
		currentindex = index;

		if (currentindex >= animation.size() - 1)
		{
			currentindex = animation.size() - 1;
		}

		canvas->SetSRV(&animation[currentindex]);
	}

	void SetAnimNext()
	{
		currentindex += 1;

		if (currentindex >= animation.size() - 1)
		{
			currentindex = animation.size() - 1;
		}

		canvas->SetSRV(&animation[currentindex]);
	}

	virtual void Update(float deltatime) override
	{
		if (isActive)
		{
			canvas->Update();
			if (!isstatic && animation.size() != 0)
			{
				currenttime += deltatime;

				if (framepertime <= currenttime)
				{
					if (currentindex == animation.size() - 1)
					{
						currentindex = 0;
					}
					else
					{
						currentindex++;
					}
					canvas->SetSRV(&animation[currentindex]);
					currenttime = 0;
				}
			}
		}
	}
	virtual void Render() override
	{
		if (isActive)
		{
			canvas->Render();
		}
	}
};

class Script_SingleUI
{
public:
	Script_SingleUI();
	~Script_SingleUI();

	void Update();
	void Render();

	// UI 엘레멘트들
	vector<UIelement*> element;
};