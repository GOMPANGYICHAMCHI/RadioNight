#pragma once
#include "Script_SingleUI.h"

class Script_ScoreUI : public MonoBehavior
{
public:
	Script_ScoreUI(GameObject* gameObject);
	~Script_ScoreUI();

private:
	// MonoBehavior을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void DrawUI() override;

	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;

	void GenerateScroeScene();
	void UpdateScoreScene();

	float timecheck = 0;
	bool isactive = false;

	string btn_recorder;
	Script_SingleUI* current_ui;


	DXTKFont* font;

	//=======================================================
	// End SceneUI
	//=======================================================

	int accumulateExp;				// 누적 EXP
	int killCount;					// 킬수
	int perfectshot;				// 퍼펙트 샷
	int score;						// 점수

	int scoreGrade;					// 점수 등급

	vector<UIelement*> ui_element;	// 순차적 UI 엘레멘트

	float textdelay = 1.5f;			// 타자 딜레이

	void AddText();

	//float timecheck;

};

