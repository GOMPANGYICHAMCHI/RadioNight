#include "framework.h"
#include "SceneManager.h"
#include "Script_SingleUI.h"

#include "EnemyScene.h"
#include "Script_SceneUI.h"
#include "DemoScene_ModelLoad.h"
#include "DemoScene.h"

Script_SceneUI::Script_SceneUI(GameObject* gameObject)
	:MonoBehavior(gameObject)
{
	Generate_MainMenu();
	Generate_Credit();
	Generate_Loading();
}

Script_SceneUI::~Script_SceneUI()
{
	for (auto ui : uiflow)
		SAFE_DELETE(ui.second);
}

void Script_SceneUI::Start()
{
}

void Script_SceneUI::Update()
{
	switch (currentstate)
	{
	case UI_State::Main:
	{
		Update_MainMenu();
	}break;
	case UI_State::Settings:
	{
		Update_Credit();
	}break;
	case UI_State::Loading:
	{

	}break;
	}

	current_ui->Update();

}

void Script_SceneUI::DrawUI()
{
	current_ui->Render();
}

void Script_SceneUI::Update_MainMenu()
{
	if (btn_recorder == "Start")
	{
		btn_recorder = "";

		currentstate = Loading;
		current_ui = uiflow["Loading"];

		//SceneManager::Get()->ChangeScene( new DemoScene_ModelLoad());
		SceneManager::Get()->ChangeScene(new DemoScene());
	}
	else if (btn_recorder == "Credit")
	{
		btn_recorder = "";
		currentstate = Settings;
		current_ui = uiflow["Settings"];
	}
	else if (btn_recorder == "Exit_main" && iscanbtnpress)
	{
		btn_recorder = "";
		exit(0);
	}
}

void Script_SceneUI::Generate_MainMenu()
{
	Script_SingleUI* temp_single = new Script_SingleUI;

	FloatRect temp_rect;

	vector<Resource<ID3D11ShaderResourceView>> temp_vec;
	// 시작 버튼===========
	temp_rect.top = 0.7f;
	temp_rect.bottom = 0.06f;
	temp_rect.left = 0.75f;
	temp_rect.right = 0.19f;

	Resource<ID3D11ShaderResourceView> startbutton_idle;
	Resource<ID3D11ShaderResourceView> startbutton_mouseon;
	Resource<ID3D11ShaderResourceView> startbutton_pressed;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(startbutton_idle, L"/UI/Default_Gamestart.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(startbutton_mouseon, L"MouseUp_Gamestart.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(startbutton_pressed, L"Click_Gamestart.png");

	Button* start_btn = new Button
	(temp_rect, startbutton_idle, startbutton_mouseon, startbutton_pressed, "Start", btn_recorder, 1);

	temp_single->element.push_back(start_btn);
	//========================================================================
	// 크레딧 버튼
	//========================================================================
	temp_rect.top = 0.78f;
	temp_rect.bottom = 0.06f;
	temp_rect.left = 0.75f;
	temp_rect.right = 0.19f;

	Resource<ID3D11ShaderResourceView> creditbutton_idle;
	Resource<ID3D11ShaderResourceView> creditbutton_mouseon;
	Resource<ID3D11ShaderResourceView> creditbutton_pressed;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(creditbutton_idle, L"Default_Credit.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(creditbutton_mouseon, L"MouseUp_Credit.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(creditbutton_pressed, L"Click_Credit.png");

	Button* setting_btn = new Button
	(temp_rect, creditbutton_idle, creditbutton_mouseon, creditbutton_pressed, "Credit", btn_recorder, 1);

	temp_single->element.push_back(setting_btn);
	//========================================================================
	// 나가기 버튼
	//========================================================================
	temp_rect.top = 0.86f;
	temp_rect.bottom = 0.06f;
	temp_rect.left = 0.75f;
	temp_rect.right = 0.19f;

	Resource<ID3D11ShaderResourceView> exitbutton_idle;
	Resource<ID3D11ShaderResourceView> exitbutton_mouseon;
	Resource<ID3D11ShaderResourceView> exitbutton_pressed;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_idle, L"Default_Qult.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_mouseon, L"MouseUp_Quit.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_pressed, L"Click_Quit.png");

	Button* exit_btn = new Button
	(temp_rect, exitbutton_idle, exitbutton_mouseon, exitbutton_pressed, "Exit_main", btn_recorder, 1);

	temp_single->element.push_back(exit_btn);
	//========================================================================
	// 배경화면
	//========================================================================
	temp_rect.top = 0.0f;
	temp_rect.bottom = 1.0f;
	temp_rect.left = 0.0f;
	temp_rect.right = 1.0f;

	Resource<ID3D11ShaderResourceView> background_image;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(background_image, L"Radio_night_maintitle.png");

	ImgPanel* background = new ImgPanel(background_image, temp_rect, 0);
	temp_single->element.push_back(background);

	UIelement* swap = temp_single->element[0];
	temp_single->element[0] = temp_single->element.back();
	temp_single->element.back() = swap;
	//========================================================================
	uiflow.insert(make_pair("MainMenu", temp_single));
	current_ui = temp_single;
	currentstate = Main;
}

void Script_SceneUI::Generate_Credit()
{
	Script_SingleUI* temp_single = new Script_SingleUI;

	FloatRect temp_rect;

	//========================================================================
	// 나가기 버튼
	//========================================================================

	Resource<ID3D11ShaderResourceView> exitbutton_idle;
	Resource<ID3D11ShaderResourceView> exitbutton_on;
	Resource<ID3D11ShaderResourceView> exitbutton_pressed;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_idle, L"Default_Qult.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_on, L"MouseUp_Quit.png");

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(exitbutton_pressed, L"Click_Quit.png");

	temp_rect.top = 0.86f;
	temp_rect.bottom = 0.06f;
	temp_rect.left = 0.75f;
	temp_rect.right = 0.19f;

	Button* exitbutton = new Button(temp_rect, exitbutton_idle, exitbutton_on, exitbutton_pressed, "Exit_Credit", btn_recorder, 1);
	temp_single->element.push_back(exitbutton);

	//========================================================================
	// 배경화면 
	//========================================================================
	temp_rect.top = 0.0f;
	temp_rect.bottom = 1.0f;
	temp_rect.left = 0.0f;
	temp_rect.right = 1.0f;

	Resource<ID3D11ShaderResourceView> background_image;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(background_image, L"blk.png");

	ImgPanel* temp_img = new ImgPanel(background_image, temp_rect, 0);
	temp_single->element.push_back(temp_img);

	//========================================================================
	uiflow.insert(make_pair("Settings", temp_single));
}

void Script_SceneUI::Update_Credit()
{
	if (btn_recorder == "Exit_Credit" && iscanbtnpress)
	{
		iscanbtnpress = false;
		currentstate = Main;
		current_ui = uiflow["MainMenu"];
	}
}

void Script_SceneUI::Generate_Loading()
{
	//========================================================================
	// 배경화면
	//========================================================================
	Script_SingleUI* temp_single = new Script_SingleUI;

	FloatRect temp_rect;

	temp_rect.top = 0.0f;
	temp_rect.bottom = 1.0f;
	temp_rect.left = 0.0f;
	temp_rect.right = 1.0f;

	Resource<ID3D11ShaderResourceView> background_image;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(background_image, L"Radio_night_maintitle.png");

	ImgPanel* background = new ImgPanel(background_image, temp_rect, 0);
	temp_single->element.push_back(background);
	//========================================================================
	uiflow.insert(make_pair("Loading", temp_single));
}

void Script_SceneUI::OnCollisionEnter(Collider* other)
{
}

void Script_SceneUI::OnCollisionPersist(Collider* other)
{
}

void Script_SceneUI::OnCollisionExit(Collider* other)
{
}

void Script_SceneUI::OnTriggerEnter(Collider* other)
{
}

void Script_SceneUI::OnTriggerPersist(Collider* other)
{
}

void Script_SceneUI::OnTriggerExit(Collider* other)
{
}
