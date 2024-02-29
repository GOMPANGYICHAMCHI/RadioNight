#pragma once
#include "Script_SingleUI.h"

class Script_ScoreUI : public MonoBehavior
{
public:
	Script_ScoreUI(GameObject* gameObject);
	~Script_ScoreUI();

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

	int accumulateExp;				// ���� EXP
	int killCount;					// ų��
	int perfectshot;				// ����Ʈ ��
	int score;						// ����

	int scoreGrade;					// ���� ���

	vector<UIelement*> ui_element;	// ������ UI ������Ʈ

	float textdelay = 1.5f;			// Ÿ�� ������

	void AddText();

	//float timecheck;

};

