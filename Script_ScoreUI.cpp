#include "framework.h"
#include "Script_ScoreUI.h"
#include "PlayerStat.h"

Script_ScoreUI::Script_ScoreUI(GameObject* gameObject)
	: MonoBehavior(gameObject), timecheck(0)
{
	font = RenderEngine::Get()->GetFont();
	GenerateScroeScene();

	accumulateExp = 260;
	killCount = 23;
	perfectshot = 34;
	score = 7000;

	AddText();
	//isactive = true;
}

Script_ScoreUI::~Script_ScoreUI()
{
	SAFE_DELETE(current_ui);
}

void Script_ScoreUI::Start()
{
}

void Script_ScoreUI::Update()
{
	UpdateScoreScene();
}

void Script_ScoreUI::DrawUI()
{
	current_ui->Render();

	//wstring temp = InputHandler
	//font->DrawTextColor(50, 560, XMFLOAT4(1, 1, 1, 1), (TCHAR*)current_string.c_str());
}

void Script_ScoreUI::OnCollisionEnter(Collider* other)
{
}

void Script_ScoreUI::OnCollisionPersist(Collider* other)
{
}

void Script_ScoreUI::OnCollisionExit(Collider* other)
{
}

void Script_ScoreUI::OnTriggerEnter(Collider* other)
{
}

void Script_ScoreUI::OnTriggerPersist(Collider* other)
{
}

void Script_ScoreUI::OnTriggerExit(Collider* other)
{
}

void Script_ScoreUI::GenerateScroeScene()
{
	current_ui = new Script_SingleUI();

	FloatRect temp_rect;

	vector<wstring> text_vec;

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

	ImgPanel* background = new ImgPanel(background_image, temp_rect, 0);
	current_ui->element.push_back(background);

	//========================================================================
	// 0 마지막 문구 텍스트
	//========================================================================
	temp_rect.top = 77.0f / 100.0f;
	temp_rect.left = 68.0f / 200.0f;

	wstring temp_text = L"                     메트로늄";

	Text* finalscoretext_text = new Text
	(temp_text, temp_rect, font, fontType::esamanru_Light_50, 0, { 1,0,0,1 });
	finalscoretext_text->isActive = false;
	ui_element.push_back(finalscoretext_text);
	current_ui->element.push_back(finalscoretext_text);

	//========================================================================
	// 1 스코어별 문구 텍스트
	//========================================================================
	temp_rect.top = 77.0f / 100.0f;
	temp_rect.left = 68.0f / 200.0f;

	temp_text = L"당신은 휴먼";

	Text* scoretext_text = new Text(temp_text, temp_rect, font, fontType::esamanru_Light_50);
	scoretext_text->isActive = false;
	ui_element.push_back(scoretext_text);
	current_ui->element.push_back(scoretext_text);

	//========================================================================
	// 2 최종 스코어 텍스트
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
	current_ui->element.push_back(finalscore_text);

	//========================================================================
	// 하단가로바
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
	current_ui->element.push_back(horizontalBar_2);

	//========================================================================
	// 3 니어 킬카운트 텍스트
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
	current_ui->element.push_back(killcount_text);

	//========================================================================
	// 4 퍼펙트샷 텍스트
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
	current_ui->element.push_back(perfectshot_text);

	//========================================================================
	// 5 피버포인트 텍스트
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
	current_ui->element.push_back(feverpoint_text);

	//========================================================================
	// 중간가로바
	//========================================================================
	temp_rect.top = 47.0f / 100.0f;
	temp_rect.bottom = 1.0f / 500.0f;
	temp_rect.left = 7.0f / 20.0f;
	temp_rect.right = 1.0f / 3.0f;

	ImgPanel* horizontalBar = new ImgPanel(horizbarImg, temp_rect, 0);
	//horizontalBar->isActive = false;
	current_ui->element.push_back(horizontalBar);

	//========================================================================
	// 6 스코어 텍스트
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
	current_ui->element.push_back(score_text);
}

void Script_ScoreUI::UpdateScoreScene()
{
	if (ui_element.size() >= 1)
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

			UIelement* temp_element = ui_element.back();
			ui_element.pop_back();

			temp_element->isActive = true;
			timecheck = 0;

			if (ui_element.size() == 2)
			{
				textdelay = 3;
			}
		}
	}

	current_ui->Update();
}

void Script_ScoreUI::AddText()
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
		temp_text->textContent = L"연습한다고 잘할것 같진 않아요";
	}
	else if (score < SCOREGRADE_HIGH)
	{
		scoreGrade = 2;
		temp_text->textContent = L"재능은 없지만 노력하셨네요";
	}
	else if (score < SCOREGRADE_METRONOME)
	{
		scoreGrade = 3;
		temp_text->textContent = L"어디서 리듬좀 타셨군요?";
	}
	else
	{
		scoreGrade = 4;
	}
}
