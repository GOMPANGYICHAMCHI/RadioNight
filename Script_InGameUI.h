#pragma once
#include "Script_SingleUI.h"

enum class GameUIState
{
	ui_ingame,
	ui_dead,
	ui_end
};

class Script_InGameUI : public MonoBehavior
{
public:
	Script_InGameUI(GameObject* gameObject);
	~Script_InGameUI();

	void SetTextAmmo(int input_ammo);
	void SetHealth(int input_health);
	void SetFeverExp(int input_exp, int input_maxexp, int input_feverLevel);
	void SetScore(int input_score);

	void SetGameEnd
	(
		int input_accumulateExp, 
		int input_killcount, 
		int input_perfectshot,
		int input_score
	);

	void Dead();

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

	void GenerateInGameUI();

	void GenerateDeadUI();

	void GenerateEndUI();
	void UpdateEndUI();
	void UpdateDeadUI();

	float timecheck;
	float endTimeCheck;

	GameUIState state;

	string btn_recorder;

	Script_SingleUI* current_ui;

	Script_SingleUI* ingame_ui;
	Script_SingleUI* dead_ui;
	Script_SingleUI* end_ui;

	Text* score_text;
	Text* ammo_text;
	Text* health_text;
	Text* combomul_text;

	AnimationImg* combostate_img;
	AnimationImg* skill_img;

	Gauge_MultiImage* health_gauge;
	Gauge_SingleImage* combo_gauge;

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

	bool isEndTextAllout;
	bool isEnd;
	Text* continue_text;

	float textdelay;				// 타자 딜레이

	void AddText();					// 텍스트 추가 함수

	//=======================================================
};

