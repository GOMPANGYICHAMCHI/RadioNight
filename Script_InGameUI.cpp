#include "framework.h"
#include "Script_InGameUI.h"
#include "PlayerStat.h"

#include "MainMenuScene.h"

Script_InGameUI::Script_InGameUI(GameObject* gameObject)
	: MonoBehavior(gameObject), textdelay(1.5f), timecheck(0), isEndTextAllout(false), isEnd(false), endTimeCheck(0.0f)
{
	font = RenderEngine::Get()->GetFont();
	GenerateInGameUI();
	GenerateDeadUI();
	GenerateEndUI();
}

Script_InGameUI::~Script_InGameUI()
{
	SAFE_DELETE(ingame_ui);
	SAFE_DELETE(dead_ui);
}

void Script_InGameUI::SetTextAmmo(int input_ammo)
{
	ammo_text->textContent = to_wstring(input_ammo);
}

void Script_InGameUI::SetHealth(int input_health)
{
	health_text->textContent = to_wstring(input_health);
	health_gauge->SetAmount((float)input_health / (float)PLAYER_MAXHEALTH);
}

void Script_InGameUI::SetFeverExp(int input_exp, int input_maxexp, int input_feverLevel)
{
	switch (input_feverLevel)
	{
	case 0:
		{
			combomul_text->textContent = FEVER_0_TEXT;
			combostate_img->SetAnimIndex(0);
			combo_gauge->SetImage(0);
		}break;
	case 1:
		{
			combomul_text->textContent = FEVER_1_TEXT;
			combostate_img->SetAnimIndex(1);
			combo_gauge->SetImage(1);
		}break;
	case 2:
		{
			combomul_text->textContent = FEVER_2_TEXT;
			combostate_img->SetAnimIndex(2);
			combo_gauge->SetImage(2);
		}break;
	case 3:
		{
			combomul_text->textContent = FEVER_3_TEXT;
			combostate_img->SetAnimIndex(3);
			combo_gauge->SetImage(3);
		}break;
	case 4:
		{
			combomul_text->textContent = FEVER_4_TEXT;
			combostate_img->SetAnimIndex(4);
			combo_gauge->SetImage(4);
		}break;
	case 5:
		{
			combomul_text->textContent = FEVER_5_TEXT;
			combostate_img->SetAnimIndex(5);
			combo_gauge->SetImage(5);
		}break;
	}

	combo_gauge->SetAmount((float)input_exp / (float)input_maxexp);
}

void Script_InGameUI::SetScore(int input_score)
{
	score_text->textContent = to_wstring(input_score);
}

void Script_InGameUI::SetGameEnd
(int input_accumulateExp, int input_killcount, int input_perfectshot, int input_score)
{
	accumulateExp = input_accumulateExp;
	killCount = input_killcount;
	perfectshot = input_perfectshot;
	score = input_score;

	AddText();

	current_ui = end_ui;
	state = GameUIState::ui_end;
}

void Script_InGameUI::Dead()
{
	current_ui = dead_ui;
	state = GameUIState::ui_dead;

	timecheck = 0;
}

void Script_InGameUI::Start()
{
}

void Script_InGameUI::Update()
{
	if (state == GameUIState::ui_end)
	{
		UpdateEndUI();
		//endTimeCheck += TIME->Delta();
	}
	else if (state == GameUIState::ui_dead)
	{
		UpdateDeadUI();
		//endTimeCheck += TIME->Delta();
	}

	//if (endTimeCheck >= 3.0f)
	//{
	//	endTimeCheck = 0.0f;
	//	RenderEngine::Get()->GetCamera()->transform.parent = nullptr;
	//	SceneManager::Get()->ChangeScene(new MainMenuScene());
	//}


	current_ui->Update();
}

void Script_InGameUI::DrawUI()
{
	current_ui->Render();

	//wstring temp_str = to_wstring((int)state);
	//
	//font->DrawTextColor(100, 800, { 0,1,0,1 }, (TCHAR*)temp_str.c_str(), 1);
	//
	//temp_str = to_wstring(ui_element.size());
	//
	//font->DrawTextColor(100, 900, { 0,1,0,1 }, (TCHAR*)temp_str.c_str(), 1);
}

void Script_InGameUI::OnCollisionEnter(Collider* other)
{
}

void Script_InGameUI::OnCollisionPersist(Collider* other)
{
}

void Script_InGameUI::OnCollisionExit(Collider* other)
{
}

void Script_InGameUI::OnTriggerEnter(Collider* other)
{
}

void Script_InGameUI::OnTriggerPersist(Collider* other)
{
}

void Script_InGameUI::OnTriggerExit(Collider* other)
{
}

void Script_InGameUI::GenerateInGameUI()
{
	ingame_ui = new Script_SingleUI();
	current_ui = ingame_ui;
	state = GameUIState::ui_ingame;

	FloatRect temp_rect;

	vector<wstring> text_vec;

	vector<Resource<ID3D11ShaderResourceView>> temp_imgvec;

	//========================================================================
	// �޺� ������ �ִϸ��̼�
	//========================================================================
	temp_rect.top = 29.5f / 35.0f;
	temp_rect.bottom = 3.8f / 35.0f;
	temp_rect.left = 15.0f / 35.0f;
	temp_rect.right = 5.0f / 35.0f;

	temp_imgvec.clear();

	Resource<ID3D11ShaderResourceView> comboanim_img;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox.png");
	temp_imgvec.push_back(comboanim_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox_color1.png");
	temp_imgvec.push_back(comboanim_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox_color2.png");
	temp_imgvec.push_back(comboanim_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox_color3.png");
	temp_imgvec.push_back(comboanim_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox_color4.png");
	temp_imgvec.push_back(comboanim_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(comboanim_img, L"Ingame/ingame_gaugeBox_color5.png");
	temp_imgvec.push_back(comboanim_img);

	AnimationImg* combogage = new AnimationImg(temp_rect, 0.5f, temp_imgvec, 0);
	ingame_ui->element.push_back(combogage);
	combostate_img = combogage;
	combostate_img->isstatic = true;
	//========================================================================
	// �޺� �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 905.0f / 1920.0f;
	temp_rect.top = 910.0f /1080.0f;

	Text* combotext = new Text(L"X 0.0", temp_rect, font, fontType::playpretend_25, 0);
	ingame_ui->element.push_back(combotext);
	combomul_text = combotext;
	//========================================================================
	// �޺� ������
	//========================================================================
	temp_rect.top = 77.0f / 80.0f;
	temp_rect.bottom = 1.0f / 100.0f;
	temp_rect.left = 15.0f / 35.0f;
	temp_rect.right = 5.0f / 35.0f;

	temp_imgvec.clear();

	Resource<ID3D11ShaderResourceView> combogauge_img;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_img, L"Ingame/ingame_gauge_color1.png");
	temp_imgvec.push_back(combogauge_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_img, L"Ingame/ingame_gauge_color2.png");
	temp_imgvec.push_back(combogauge_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_img, L"Ingame/ingame_gauge_color3.png");
	temp_imgvec.push_back(combogauge_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_img, L"Ingame/ingame_gauge_color4.png");
	temp_imgvec.push_back(combogauge_img);

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_img, L"Ingame/ingame_gauge_color5.png");
	temp_imgvec.push_back(combogauge_img);

	Resource<ID3D11ShaderResourceView> combogauge_back;
	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(combogauge_back, L"Ingame/ingame_gauge_Off.png");

	Gauge_SingleImage* combogauge = new Gauge_SingleImage
	(combogauge_back, temp_imgvec, temp_rect);

	ingame_ui->element.push_back(combogauge);
	combo_gauge = combogauge;
	//========================================================================
	// �� ������
	//========================================================================
	temp_rect.top = 17.0f / 20.0f;
	temp_rect.bottom = 1.0f / 10.0f;
	temp_rect.left = 22.0f / 25.0f;
	temp_rect.right = 1.0f / 11.0f;

	Resource<ID3D11ShaderResourceView> guniconimg;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(guniconimg, L"Ingame/ingame_gun.png");

	ImgPanel* gunicon = new ImgPanel(guniconimg, temp_rect, 0);
	ingame_ui->element.push_back(gunicon);
	//========================================================================
	// ��ų �ִϸ��̼�
	//========================================================================
	//temp_rect.top = 171.0f / 200.0f;
	//temp_rect.bottom = 1.0f / 11.0f;
	//temp_rect.left = 4.0f / 5.0f;
	//temp_rect.right = 1.0f / 20.0f;
	//
	//temp_imgvec.clear();
	//
	//Resource<ID3D11ShaderResourceView> skillanim_img;
	//
	//DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	//(skillanim_img, L"../../4_Resources/UI/Ingame/ingame_skill_Off.png");
	//temp_imgvec.push_back(skillanim_img);
	//
	//DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	//(skillanim_img, L"../../4_Resources/UI/Ingame/ingame_skill_On.png");
	//temp_imgvec.push_back(skillanim_img);
	//
	//AnimationImg* skillanim = new AnimationImg(temp_rect, 0.5f, temp_imgvec, 0);
	//ingame_ui->element.push_back(skillanim);
	//skill_img = skillanim;
	//skill_img->isstatic = true;
	//========================================================================
	// ���ھ� ��Ī �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 1770.0f /1920.0f;
	temp_rect.top = 20.0f /1080.0f;
		
	Text* scorenametext = new Text(L"score", temp_rect, font, fontType::squarefont_21, 0, { 0.8f,0.8f,0.8f,1 });
	ingame_ui->element.push_back(scorenametext);
	//========================================================================
	// ���ھ� �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 1750.0f /1920.0f;
	temp_rect.top = 60.0f /1080.0f;

	Text* scoreamounttext = new Text(to_wstring(0), temp_rect, font, fontType::squarefont_29, 0);
	ingame_ui->element.push_back(scoreamounttext);
	score_text = scoreamounttext;
	//========================================================================
	// �Ѿ� �ִ뷮 �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 1765.0f /1920.0f;
	temp_rect.top = 840.0f /1080.0f;

	Text* maxammotext = new Text(L"    / 6", temp_rect, font, fontType::squarefont_24, 0, { 0.6f,0.6f,0.6f,1 });
	ingame_ui->element.push_back(maxammotext);
	//========================================================================
	// �Ѿ� �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 1765.0f /1920.0f;
	temp_rect.top = 833.0f /1080.0f;

	Text* ammotext = new Text(L"8", temp_rect, font, fontType::squarefont_29, 0);
	ingame_ui->element.push_back(ammotext);
	ammo_text = ammotext;
	//========================================================================
	// ü�� �ִ뷮 �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 60.0f /1920.0f;
	temp_rect.top = 868.0f /1080.0f;

	Text* maxhealthtext = new Text(L"        / " + to_wstring(PLAYER_MAXHEALTH), temp_rect, font, fontType::squarefont_24, 0, { 0.6f,0.6f,0.6f,1 });
	ingame_ui->element.push_back(maxhealthtext);
	//========================================================================
	// ü�� �ؽ�Ʈ
	//========================================================================
	temp_rect.left = 60.0f /1920.0f;
	temp_rect.top = 865.0f /1080.0f;

	Text* healthtext = new Text(L"100", temp_rect, font, fontType::squarefont_29, 0);
	ingame_ui->element.push_back(healthtext);
	health_text = healthtext;
	//========================================================================
	// ü�� ������
	//========================================================================
	temp_rect.top = 179.0f / 200.0f;
	temp_rect.bottom = 1.0f / 25.0f;
	temp_rect.left = 1.0f / 35.0f;
	temp_rect.right = 1.0f / 92.0f;

	Resource<ID3D11ShaderResourceView> healthgauge_fill;
	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(healthgauge_fill, L"Ingame/ingame_HP_On.png");

	Resource<ID3D11ShaderResourceView> healthgauge_empty;
	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(healthgauge_empty, L"Ingame/ingame_HP_Off.png");

	Gauge_MultiImage* healthgauge = new Gauge_MultiImage(healthgauge_empty, healthgauge_fill, temp_rect, 20, 1.0f / 1000.0f , 1.0f / 120.0f);
	ingame_ui->element.push_back(healthgauge);
	health_gauge = healthgauge;
	health_gauge->SetAmount(0.5f);
}

void Script_InGameUI::GenerateDeadUI()
{
	dead_ui = new Script_SingleUI();

	FloatRect temp_rect;

	//========================================================================
	// ���� ��ũ��
	//========================================================================
	temp_rect.top = 0;
	temp_rect.bottom = 1;
	temp_rect.left = 0;
	temp_rect.right = 1;

	Resource<ID3D11ShaderResourceView> deadscreenimg;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(deadscreenimg, L"Gameover.png");

	ImgPanel* dead_img = new ImgPanel(deadscreenimg, temp_rect, 0);
	dead_ui->element.push_back(dead_img);
}

void Script_InGameUI::GenerateEndUI()
{
	end_ui = new Script_SingleUI();

	FloatRect temp_rect;

	vector<wstring> text_vec;

	//========================================================================
	// ���ȭ��
	//========================================================================
	temp_rect.top = 0.0f;
	temp_rect.bottom = 1.0f;
	temp_rect.left = 0.0f;
	temp_rect.right = 1.0f;

	Resource<ID3D11ShaderResourceView> background_image;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(background_image, L"blk.png");

	ImgPanel* background = new ImgPanel(background_image, temp_rect, 0);
	end_ui->element.push_back(background);

	//========================================================================
	// ����Ϸ��� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 90.0f / 100.0f;
	temp_rect.left = 130.0f / 200.0f;
	
	wstring temp_text = L"���콺 Ŭ���� ���θ޴���";
	
	Text* continuetext = new Text
	(temp_text, temp_rect, font, fontType::esamanru_Light_30, 0, { 1,1,1,1 });
	continuetext->isActive = false;
	end_ui->element.push_back(continuetext);
	continue_text = continuetext;

	//========================================================================
	// 0 ������ ���� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 77.0f / 100.0f;
	temp_rect.left = 68.0f / 200.0f;

	temp_text = L"                     ��Ʈ�δ�";

	Text* finalscoretext_text = new Text
	(temp_text, temp_rect, font, fontType::esamanru_Light_50, 0, { 1,0,0,1 });
	finalscoretext_text->isActive = false;
	ui_element.push_back(finalscoretext_text);
	end_ui->element.push_back(finalscoretext_text);

	//========================================================================
	// 1 ���ھ ���� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 77.0f / 100.0f;
	temp_rect.left = 68.0f / 200.0f;

	temp_text = L"����� �޸�";

	Text* scoretext_text = new Text(temp_text, temp_rect, font, fontType::esamanru_Light_50);
	scoretext_text->isActive = false;
	ui_element.push_back(scoretext_text);
	end_ui->element.push_back(scoretext_text);

	//========================================================================
	// 2 ���� ���ھ� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 68.0f / 100.0f;
	temp_rect.left = 69.0f / 200.0f;

	text_vec.clear();
	text_vec.push_back(L"F");
	text_vec.push_back(L"Fi");
	text_vec.push_back(L"Fin");
	text_vec.push_back(L"Fina");
	text_vec.push_back(L"Final");
	text_vec.push_back(L"Final ");
	text_vec.push_back(L"Final S");
	text_vec.push_back(L"Final Sc");
	text_vec.push_back(L"Final Sco");
	text_vec.push_back(L"Final Scor");
	text_vec.push_back(L"Final Score");
	text_vec.push_back(L"Final Score ");

	TextAnimation* finalscore_text = new TextAnimation
	(text_vec, temp_rect, false, 0.1f, font, fontType::squarefont_24, 0, { 1,1,1,1 }, 1);
	finalscore_text->isActive = false;
	ui_element.push_back(finalscore_text);
	end_ui->element.push_back(finalscore_text);

	//========================================================================
	// �ϴܰ��ι�
	//========================================================================
	temp_rect.top = 66.0f / 100.0f;
	temp_rect.bottom = 1.0f / 500.0f;
	temp_rect.left = 7.0f / 20.0f;
	temp_rect.right = 1.0f / 3.0f;

	Resource<ID3D11ShaderResourceView> horizbarImg;

	DX11Renderer::Get()->GetResources()->srvs->GetFromFile
	(horizbarImg, L"wht.png");

	ImgPanel* horizontalBar_2 = new ImgPanel(horizbarImg, temp_rect, 0);
	//horizontalBar_2->isActive = false;
	end_ui->element.push_back(horizontalBar_2);

	//========================================================================
	// 3 �Ͼ� ųī��Ʈ �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 59.0f / 100.0f;
	temp_rect.left = 69.0f / 200.0f;

	text_vec.clear();
	text_vec.push_back(L"K");
	text_vec.push_back(L"Ki");
	text_vec.push_back(L"Kil");
	text_vec.push_back(L"Kill");
	text_vec.push_back(L"Kill ");
	text_vec.push_back(L"Kill N");
	text_vec.push_back(L"Kill Ni");
	text_vec.push_back(L"Kill Nie");
	text_vec.push_back(L"Kill Nier");
	text_vec.push_back(L"Kill Nier ");
	text_vec.push_back(L"Kill Nier X");
	text_vec.push_back(L"Kill Nier X ");

	TextAnimation* killcount_text = new TextAnimation
	(text_vec, temp_rect, false, 0.1f, font, fontType::squarefont_24, 0, { 1,1,1,1 }, 1);
	killcount_text->isActive = false;
	ui_element.push_back(killcount_text);
	end_ui->element.push_back(killcount_text);

	//========================================================================
	// 4 ����Ʈ�� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 54.0f / 100.0f;
	temp_rect.left = 69.0f / 200.0f;

	text_vec.clear();
	text_vec.push_back(L"P");
	text_vec.push_back(L"Pe");
	text_vec.push_back(L"Per");
	text_vec.push_back(L"Perf");
	text_vec.push_back(L"Perfe");
	text_vec.push_back(L"Perfec");
	text_vec.push_back(L"Perfect");
	text_vec.push_back(L"Perfect ");
	text_vec.push_back(L"Perfect S");
	text_vec.push_back(L"Perfect Sh");
	text_vec.push_back(L"Perfect Sho");
	text_vec.push_back(L"Perfect Shot");
	text_vec.push_back(L"Perfect Shot ");
	text_vec.push_back(L"Perfect Shot x");
	text_vec.push_back(L"Perfect Shot x ");

	TextAnimation* perfectshot_text = new TextAnimation
	(text_vec, temp_rect, false, 0.1f, font, fontType::squarefont_24, 0, { 1,1,1,1 }, 1);
	perfectshot_text->isActive = false;
	ui_element.push_back(perfectshot_text);
	end_ui->element.push_back(perfectshot_text);

	//========================================================================
	// 5 �ǹ�����Ʈ �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 49.0f / 100.0f;
	temp_rect.left = 69.0f / 200.0f;

	text_vec.clear();
	text_vec.push_back(L"F");
	text_vec.push_back(L"Fe");
	text_vec.push_back(L"Fev");
	text_vec.push_back(L"Feve");
	text_vec.push_back(L"Fever");
	text_vec.push_back(L"Fever ");
	text_vec.push_back(L"Fever p");
	text_vec.push_back(L"Fever po");
	text_vec.push_back(L"Fever poi");
	text_vec.push_back(L"Fever poin");
	text_vec.push_back(L"Fever point");
	text_vec.push_back(L"Fever point ");

	TextAnimation* feverpoint_text = new TextAnimation
	(text_vec, temp_rect, false, 0.1f, font, fontType::squarefont_24, 0, { 1,1,1,1 }, 1);
	feverpoint_text->isActive = false;
	ui_element.push_back(feverpoint_text);
	end_ui->element.push_back(feverpoint_text);

	//========================================================================
	// �߰����ι�
	//========================================================================
	temp_rect.top = 47.0f / 100.0f;
	temp_rect.bottom = 1.0f / 500.0f;
	temp_rect.left = 7.0f / 20.0f;
	temp_rect.right = 1.0f / 3.0f;

	ImgPanel* horizontalBar = new ImgPanel(horizbarImg, temp_rect, 0);
	//horizontalBar->isActive = false;
	end_ui->element.push_back(horizontalBar);

	//========================================================================
	// 6 ���ھ� �ؽ�Ʈ
	//========================================================================
	temp_rect.top = 40.0f / 100.0f;
	temp_rect.left = 69.0f / 200.0f;

	text_vec.clear();
	text_vec.push_back(L"S");
	text_vec.push_back(L"Sc");
	text_vec.push_back(L"Sco");
	text_vec.push_back(L"Scor");
	text_vec.push_back(L"Score");

	TextAnimation* score_text = new TextAnimation
	(text_vec, temp_rect, false, 0.1f, font, fontType::squarefont_24, 0, { 1,1,1,1 }, 1);
	score_text->isActive = false;
	ui_element.push_back(score_text);
	end_ui->element.push_back(score_text);
}

void Script_InGameUI::UpdateEndUI()
{
	if (ui_element.size() > 1)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= textdelay)
		{
			if (ui_element.size() == 2 && scoreGrade == 4)
			{
				UIelement* temp_element = ui_element.back();
				ui_element.pop_back();

				temp_element->isActive = true;
				
			}
			else if(ui_element.size() == 2)
			{
				SoundManager::GetInstance()->PlaySFT("Rank");
				isEndTextAllout = true;
			}
			else if(ui_element.size() == 3)
			{
				textdelay = 3;
			}

			SoundManager::GetInstance()->PlaySFT("Typing");
			UIelement* temp_element = ui_element.back();
			ui_element.pop_back();

			temp_element->isActive = true;
			timecheck = 0;
		}
	}

	if (isEndTextAllout)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= 3.0f)
		{
			continue_text->isActive = true;
			isEndTextAllout = false;
			isEnd = true;
		}
	}
	
	else if (isEnd)
	{
		if (InputHandler::GetInstance()->InputKey("Shoot"))
		{
			//exit(0);

			InputHandler::GetInstance()->MousePosClippingActive(true);
			InputHandler::GetInstance()->InfiniteMouseActive(false);
			InputHandler::GetInstance()->MouseCursorActive(true);
			ShowCursor(true);
			RenderEngine::Get()->GetCamera()->transform.parent = nullptr;

			SceneManager::Get()->ChangeScene(new MainMenuScene());
		}
	}
}

void Script_InGameUI::UpdateDeadUI()
{
	timecheck += Time::Get()->Delta();
	if (timecheck >= 5)
	{
		InputHandler::GetInstance()->MousePosClippingActive(true);
		InputHandler::GetInstance()->MouseCursorActive(true);
		ShowCursor(true);
		InputHandler::GetInstance()->InfiniteMouseActive(false);
		RenderEngine::Get()->GetCamera()->transform.parent = nullptr;

		SceneManager::Get()->ChangeScene(new MainMenuScene());
	}
}

void Script_InGameUI::AddText()
{
	TextAnimation* temp_textanim;
	Text* temp_text;

	temp_textanim = static_cast<TextAnimation*>(ui_element[2]);
	temp_textanim->content_vec.push_back
	(L"Final Score " + to_wstring(score));

	temp_textanim = static_cast<TextAnimation*>(ui_element[3]);
	temp_textanim->content_vec.push_back
	(L"Kill Nier X " + to_wstring(killCount));

	temp_textanim = static_cast<TextAnimation*>(ui_element[4]);
	temp_textanim->content_vec.push_back
	(L"Perfect Shot x " + to_wstring(perfectshot));

	temp_textanim = static_cast<TextAnimation*>(ui_element[5]);
	temp_textanim->content_vec.push_back
	(L"Fever point " + to_wstring(accumulateExp));

	temp_text = static_cast<Text*>(ui_element[1]);

	if (score < SCOREGRADE_NORMAL)
	{
		scoreGrade = 1;
		temp_text->textContent = L"�����Ѵٰ� ���Ұ� ���� �ʾƿ�";
	}
	else if (score < SCOREGRADE_HIGH)
	{
		scoreGrade = 2;
		temp_text->textContent = L"����� ������ ����ϼ̳׿�";
	}
	else if (score < SCOREGRADE_METRONOME)
	{
		scoreGrade = 3;
		temp_text->textContent = L"��� ������ Ÿ�̱���?";
	}
	else
	{
		scoreGrade = 4;
	}
}
