#include "framework.h"
#include "prefab_IngameMainFlow.h"
#include "Script_Player.h"
#include "SceneManager.h"

prefab_IngameMainFlow::prefab_IngameMainFlow():
	isGameEnd(false)
{
	// 오브젝트 명칭
	name = "MainFlow";

	// 태그 생성
	GenerateTag();

	font = RenderEngine::Get()->GetFont();
}

prefab_IngameMainFlow::~prefab_IngameMainFlow()
{
	for (auto temp_trigger : stagetrigger)
	{
		//SceneManager::Get()->SubGameObject(temp_trigger);
	}
}

void prefab_IngameMainFlow::GenerateTag()
{
	string temp_tag1 = " enemyholder_1";
	string temp_tag2 = " enemyholder_2";
	string temp_tag3 = " enemyholder_3";
	string temp_tag4 = " enemyholder_4";

	tag.push_back(temp_tag4);
	tag.push_back(temp_tag3);
	tag.push_back(temp_tag2);
	tag.push_back(temp_tag1);

	currenttag = tag.back();
}

void prefab_IngameMainFlow::Start()
{
	// 스테이지 스테이트 설정
	currentSate = StageState::Disabled;
	// 스테이지 설정
	SetStage();

	// 스테이지 첫번째 적 생성
	currentenemycount = currentStagetrigger->GenerateFirstEnemy();
}

void prefab_IngameMainFlow::DrawUI()
{
	//if(isGameEnd)
	//{ 
	//	wstring fps;
	//	string a;
	//	
	//	fps = L"GAME CLEAR";
	//	font->DrawTextColor(800, 560, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//}

	//wstring fps;
	//string a;
	//
	//fps = L"Stage Tag : ";
	//font->DrawTextColor(50, 560, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//fps = L"EnemyCount : ";
	//font->DrawTextColor(50, 590, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//fps = L"State : ";
	//font->DrawTextColor(50, 620, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//
	//fps.assign(currenttag.begin(), currenttag.end());
	//font->DrawTextColor(150, 560, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//fps = to_wstring(currentenemycount);
	//font->DrawTextColor(150, 590, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
	//fps = to_wstring((int)currentSate);
	//font->DrawTextColor(150, 620, XMFLOAT4(0, 1, 0, 1), (TCHAR*)fps.c_str());
}

void prefab_IngameMainFlow::SetStage()
{
	for (auto temp_stage : stagetrigger)
	{
		if (temp_stage->tag == currenttag)
		{
			currentStagetrigger = temp_stage;
			return;
		}
	}
}

void prefab_IngameMainFlow::SetNextTag()
{
	tag.pop_back();

	if (tag.size() == 0)
	{
		GameClear();
	}
	else
	{
		currenttag = tag.back();
	}
}

void prefab_IngameMainFlow::EnemyDead()
{
	// 전체 적 카운트 -1
	currentenemycount -= 1;
	cout << "EnemtRemain : " << currentenemycount << endl;

	// 만약 현재 스테이지에 있는 전체 적이 없을경우,
	if (currentenemycount <= 0)
	{
		// 현재 상태가 첫번째 웨이브 였을 경우
		if (currentSate == StageState::FirstWave)
		{
			// 2차 웨이브 생성
			SecondWaveOn();
		}
		// 현재 상태가 두번째 웨이브 였을경우,
		else if (currentSate == StageState::SecondWave)
		{
			// 스테이지 클리어
			StageClear();
		}
	}
}

void prefab_IngameMainFlow::FirstWaveOn()
{
	// 현재 스테이지 상태 전환
	currentSate = StageState::FirstWave;

	// 현재 스테이지 내에서 블락
	currentStagetrigger->BlockStage();

	// 현재 스테이지 내에 있는 적 활성화
	currentStagetrigger->AwakeFirstEnemy();
}

void prefab_IngameMainFlow::SecondWaveOn()
{
	cout << "SecondWave on\n";

	// 현재 스테이지 상태 전환
	currentSate = StageState::SecondWave;

	// 두번째 적 생성
	currentenemycount = currentStagetrigger->GenerateSecondEnemy();

	// 현재 스테이지 내에 있는 적 활성화
	currentStagetrigger->AwakeFirstEnemy();
}

void prefab_IngameMainFlow::StageClear()
{
	// 현재 스테이지 상태 전환
	currentSate = StageState::Disabled;

	// 현재 스테이지 블락 비활성화
	if (currentStagetrigger != nullptr)
	{
		currentStagetrigger->UnBlockStage();
	}

	// 다음 스테이지로 전환
	SetNextTag();

	if (!isGameEnd)
	{
		SetStage();
		
		// 다음 스테이지 첫번째 적 생성
		currentenemycount = currentStagetrigger->GenerateFirstEnemy();
	}
}

void prefab_IngameMainFlow::GameClear()
{
	isGameEnd = true;

	Script_Player* player_script = SceneManager::Get()->FindGameObject("Player")->GetScript< Script_Player>();
	player_script->SetEnd();
}
