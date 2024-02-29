#pragma once

enum class Direction
{
	forward,
	backward,
	left,
	right,
	none
};

enum class AnimState
{
	anim_Reload1,
	anim_Reload2,
	anim_Shot,
	anim_Walk1,
	anim_Walk2,
};

enum class PlayerState
{
	player_Idle,
	player_Shot,
	player_Reload1,
	player_Reload2,
	player_Dead,
	player_End
};

enum class SyncType
{
	type_Perfect,
	type_Good,
	type_Fail
};

class Script_PlayerRhythm;
class Script_InGameUI;
class Script_Player : public MonoBehavior
{
public:
	Script_Player(GameObject* gameObject);
	~Script_Player();

	// MonoBehavior��(��) ���� ��ӵ�
	virtual void Start() override;
	virtual void Update() override;

	//=================================================
	virtual void DrawUI() override;			// ����׿�
	DXTKFont* font;							//
	//=================================================

	// �ܺο��� ȣ�� �ϴ� ������ �ο� �Լ�
	void GiveDamage(int Damage);	// ������ �ο�
	void SetEnd();					// ���� Ŭ����

private:
	Script_InGameUI* playerUI;		// �÷��̾� �ΰ��� UI
	Script_PlayerRhythm* rhythm;	// �÷��̾� ����

public:
	Transform* cameratransform;		// ī�޶� Ʈ������
private:
	Transform* camHorTrans;			// ī�޶� ������ ȸ���� Ʈ������

	PlayerState state;				// ���� �÷��̾� ����
	float timecheck;				// �ð�üũ�� ����
	float timecheck_fever;			// �ǹ� �ð�üũ�� ����
	float currentratio;				// ���� ����

	SyncType current_sync;			// ���� ��ũ
	int current_exp;				// ���� exp
	int current_feverlevel;			// ���� �ǹ� ����
	int old_feverlevel;
	int current_health;				// ���� ü��
	int current_ammo;				// ���� ��ź
	int current_score;				// ���� ����

	void Move();					// �̵�
	void Shoot();					// ���
	void Reload();					// ������
	void Dead();					// ���
	void UpdateFever();				// �ǹ� ������Ʈ
	void CheckEnd();				// ���� üũ
	void CheckDead();				// ���� üũ

	void SetFever();				// �ǹ� �Ǻ�
	void SetSync();					// ��ũ �Ǻ�
	void AddExp();					// EXP �߰�
	void AddScore(bool iskill);		// �����߰�

	int accumulateExp;				// ���� EXP
	int killCount;					// ų��
	int perfectshot;				// ����Ʈ ��

	bool isNowAnim;
	bool isWalking;
	bool isEnd;
	bool isReload1;
	bool isMoving;
	bool isDash;

	bool isSuperMode;
	bool isMulDamageMode;
	bool isInfiniteAmmoMode;

	float dashcheck;
	Direction dir_state;

	float MouseSens_x;
	float MouseSens_y;

	void UpdateMouseSensitivity();

	// MonoBehavior��(��) ���� ��ӵ�
	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;
};