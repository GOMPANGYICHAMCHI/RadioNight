#include "framework.h"
#include "prefab_IngameMainFlow.h"
#include "Script_Player.h"
#include "SceneManager.h"

prefab_IngameMainFlow::prefab_IngameMainFlow():
	isGameEnd(false)
{
	// ������Ʈ ��Ī
	name = "MainFlow";

	// �±� ����
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
	// �������� ������Ʈ ����
	currentSate = StageState::Disabled;
	// �������� ����
	SetStage();

	// �������� ù��° �� ����
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
	// ��ü �� ī��Ʈ -1
	currentenemycount -= 1;
	cout << "EnemtRemain : " << currentenemycount << endl;

	// ���� ���� ���������� �ִ� ��ü ���� �������,
	if (currentenemycount <= 0)
	{
		// ���� ���°� ù��° ���̺� ���� ���
		if (currentSate == StageState::FirstWave)
		{
			// 2�� ���̺� ����
			SecondWaveOn();
		}
		// ���� ���°� �ι�° ���̺� �������,
		else if (currentSate == StageState::SecondWave)
		{
			// �������� Ŭ����
			StageClear();
		}
	}
}

void prefab_IngameMainFlow::FirstWaveOn()
{
	// ���� �������� ���� ��ȯ
	currentSate = StageState::FirstWave;

	// ���� �������� ������ ���
	currentStagetrigger->BlockStage();

	// ���� �������� ���� �ִ� �� Ȱ��ȭ
	currentStagetrigger->AwakeFirstEnemy();
}

void prefab_IngameMainFlow::SecondWaveOn()
{
	cout << "SecondWave on\n";

	// ���� �������� ���� ��ȯ
	currentSate = StageState::SecondWave;

	// �ι�° �� ����
	currentenemycount = currentStagetrigger->GenerateSecondEnemy();

	// ���� �������� ���� �ִ� �� Ȱ��ȭ
	currentStagetrigger->AwakeFirstEnemy();
}

void prefab_IngameMainFlow::StageClear()
{
	// ���� �������� ���� ��ȯ
	currentSate = StageState::Disabled;

	// ���� �������� ��� ��Ȱ��ȭ
	if (currentStagetrigger != nullptr)
	{
		currentStagetrigger->UnBlockStage();
	}

	// ���� ���������� ��ȯ
	SetNextTag();

	if (!isGameEnd)
	{
		SetStage();
		
		// ���� �������� ù��° �� ����
		currentenemycount = currentStagetrigger->GenerateFirstEnemy();
	}
}

void prefab_IngameMainFlow::GameClear()
{
	isGameEnd = true;

	Script_Player* player_script = SceneManager::Get()->FindGameObject("Player")->GetScript< Script_Player>();
	player_script->SetEnd();
}
