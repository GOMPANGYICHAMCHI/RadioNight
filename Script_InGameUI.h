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
	// MonoBehavior��(��) ���� ��ӵ�
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

	int accumulateExp;				// ���� EXP
	int killCount;					// ų��
	int perfectshot;				// ����Ʈ ��
	int score;						// ����

	int scoreGrade;					// ���� ���

	vector<UIelement*> ui_element;	// ������ UI ������Ʈ

	bool isEndTextAllout;
	bool isEnd;
	Text* continue_text;

	float textdelay;				// Ÿ�� ������

	void AddText();					// �ؽ�Ʈ �߰� �Լ�

	//=======================================================
};

