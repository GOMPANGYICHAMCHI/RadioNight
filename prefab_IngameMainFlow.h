#pragma once
#include "prefab_EnemyTrigger.h"
#include "DXTKFont.h"

enum class StageState
{
	Disabled,
	FirstWave,
	SecondWave
};

class prefab_IngameMainFlow : public GameObject
{
public:
	prefab_IngameMainFlow();
	~prefab_IngameMainFlow();
	void GenerateTag();
	void Start();

	DXTKFont* font;
	virtual void DrawUI() override;

	// ���� �������� �±׿� �°� ��ȯ
	void SetStage();
	// ���� �±� ���� �±׷� ��ȯ
	void SetNextTag();

	// �� �����
	void EnemyDead();
	// ù��° ���̺� Ȱ��ȭ
	void FirstWaveOn();
	// �ι�° ���̺� ������ Ȱ��ȭ
	void SecondWaveOn();
	// �������� Ŭ����
	void StageClear();

	// ���� ���� ����
	bool isGameEnd;

	// ���� ��ü Ŭ����
	void GameClear();

	// ���� �� ī��Ʈ
	int currentenemycount;
	// ���� ��������
	string currenttag;

	// ��ü �������� Ʈ���� ����
	vector<prefab_EnemyTrigger*> stagetrigger;

	// ���� �������� Ʈ����
	prefab_EnemyTrigger* currentStagetrigger;

	// ���� ������Ʈ
	StageState currentSate;

	// �±� ����
	vector<string> tag;
};