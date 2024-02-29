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

	// MonoBehavior을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;

	//=================================================
	virtual void DrawUI() override;			// 디버그용
	DXTKFont* font;							//
	//=================================================

	// 외부에서 호출 하는 데미지 부여 함수
	void GiveDamage(int Damage);	// 데미지 부여
	void SetEnd();					// 게임 클리어

private:
	Script_InGameUI* playerUI;		// 플레이어 인게임 UI
	Script_PlayerRhythm* rhythm;	// 플레이어 리듬

public:
	Transform* cameratransform;		// 카메라 트랜스폼
private:
	Transform* camHorTrans;			// 카메라 가로축 회전용 트랜스폼

	PlayerState state;				// 현재 플레이어 상태
	float timecheck;				// 시간체크용 변수
	float timecheck_fever;			// 피버 시간체크용 변수
	float currentratio;				// 현재 리듬

	SyncType current_sync;			// 현재 싱크
	int current_exp;				// 현재 exp
	int current_feverlevel;			// 현재 피버 레벨
	int old_feverlevel;
	int current_health;				// 현재 체력
	int current_ammo;				// 현재 잔탄
	int current_score;				// 현재 점수

	void Move();					// 이동
	void Shoot();					// 사격
	void Reload();					// 재장전
	void Dead();					// 사망
	void UpdateFever();				// 피버 업데이트
	void CheckEnd();				// 종료 체크
	void CheckDead();				// 죽음 체크

	void SetFever();				// 피버 판별
	void SetSync();					// 싱크 판별
	void AddExp();					// EXP 추가
	void AddScore(bool iskill);		// 점수추가

	int accumulateExp;				// 누적 EXP
	int killCount;					// 킬수
	int perfectshot;				// 퍼펙트 샷

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

	// MonoBehavior을(를) 통해 상속됨
	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;
};