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

	// 현재 스테이지 태그에 맞게 전환
	void SetStage();
	// 현재 태그 다음 태그로 전환
	void SetNextTag();

	// 적 사망시
	void EnemyDead();
	// 첫번째 웨이브 활성화
	void FirstWaveOn();
	// 두번째 웨이브 생성및 활성화
	void SecondWaveOn();
	// 스테이지 클리어
	void StageClear();

	// 게임 종료 여부
	bool isGameEnd;

	// 게임 전체 클리어
	void GameClear();

	// 현재 적 카운트
	int currentenemycount;
	// 현재 스테이지
	string currenttag;

	// 전체 스테이지 트리거 벡터
	vector<prefab_EnemyTrigger*> stagetrigger;

	// 현재 스테이지 트리거
	prefab_EnemyTrigger* currentStagetrigger;

	// 현재 스테이트
	StageState currentSate;

	// 태그 순서
	vector<string> tag;
};