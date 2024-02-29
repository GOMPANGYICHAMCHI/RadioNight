#pragma once

enum UI_State
{
	Main,
	Settings,
	Loading
};

class Script_SceneUI : public MonoBehavior
{
public:
	Script_SceneUI(GameObject* gameObject);
	~Script_SceneUI();

	// MonoBehavior을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;
	virtual void DrawUI() override;
private:
	// MonoBehavior을(를) 통해 상속됨
	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;

private:
	void Generate_MainMenu();
	void Update_MainMenu();
	void Generate_Credit();
	void Update_Credit();
	void Generate_Loading();

	UI_State currentstate;

	string btn_recorder;

	unordered_map<string, Script_SingleUI*> uiflow;
	Script_SingleUI* current_ui;

	bool iscanbtnpress = true;
};