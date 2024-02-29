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

// UI �⺻ ����
struct UIelement
{
	UIelement(UINT layer = 0)
		:layer(layer)
	{

	}

	// UI ũ�� ( Rect )
	FloatRect rect;
	// UI ĵ����
	Canvas* canvas;
	// UI Ÿ��
	UIType type;

	UINT layer;

	// Ȱ��ȭ ����
	bool isActive = true;

	// ������Ʈ
	virtual void Update(float deltatime) {};
	// ����
	virtual void Render() {};
};

// �ؽ�Ʈ 
struct Text : public UIelement
{
	// �ؽ�Ʈ ����
	wstring textContent;
	// ��Ʈ ũ��
	float textSize;
	// �ؽ�Ʈ �÷�
	XMFLOAT4 textColor;

	// ��ƮŸ��
	fontType fonttype;

	// ��Ʈ
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

// �ؽ�Ʈ 
struct TextAnimation : public UIelement
{
	// �ؽ�Ʈ ����
	wstring currenttext;
	// �ؽ�Ʈ �ִϸ��̼� ����
	vector<wstring> content_vec;
	// ���� �ε���
	int currentIndex = 0;
	// �ִϸ��̼� ����
	float animInterval;
	// ���� ����
	bool isLoop = false;

	// ��Ʈ ũ��
	float textSize;
	// �ؽ�Ʈ �÷�
	XMFLOAT4 textColor;

	// ��ƮŸ��
	fontType fonttype;

	// ��Ʈ
	DXTKFont* font;

	// �ð�üũ�� ����
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

// �̹��� �г�
struct ImgPanel : public UIelement
{
	// �̹��� ���ҽ�
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

// ���̹��� ������
struct Gauge_MultiImage : public UIelement
{
	// �� �̹���
	Resource<ID3D11ShaderResourceView> empty_img;
	// �� �̹���
	Resource<ID3D11ShaderResourceView> fill_img;

	// ĵ���� ����
	vector<Canvas*> canvas_vec;

	// �ִ� ũ��
	int maxsize;
	// ���� ũ��
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

// �̱��̹��� ������
struct Gauge_SingleImage : public UIelement
{
	// ��� �̹���
	Resource<ID3D11ShaderResourceView> back_img;
	// ������ �̹���
	vector<Resource<ID3D11ShaderResourceView>> fill_img;

	// ��� ��Ʈ
	FloatRect backrect;

	// ��� ĵ����
	Canvas* backcanvas;

	// ���� ũ��
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

// ��ư
struct Button : public UIelement
{
	// ��ư �̸�
	string name;
	// ���ڴ� ( ��ư�� ���ȴٴ°��� ����� ��Ʈ�� ������ )
	string& recorder;
	// ��Ʈ ũ��
	int fontsize;
	// Text ������
	FloatRect adjustrect;
	// ���� ��Ŀ�� ����
	bool is_on;
	bool is_oned;

	// ��ư �̹�����
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

// �ִϸ��̼� �̹���
struct AnimationImg : public UIelement
{
	// ���� ���� �ð�
	float currenttime;
	// ������ �� �ð�
	float framepertime;

	// ���� �ε���
	int currentindex = 0;
	// ���� �ִϸ��̼� ( �ð��� �帧�� ������ ���� ���� )
	bool isstatic = false;

	// �ִϸ��̼� ���ҽ� ( ������� ������ )
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

	// UI ������Ʈ��
	vector<UIelement*> element;
};