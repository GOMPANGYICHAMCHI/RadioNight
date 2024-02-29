#pragma once

class prefab_EnemyTrigger;
class Prefab_EnemySpawner;

class Script_Enemy : public MonoBehavior
{
public:
	Script_Enemy(GameObject* gameObject);
	~Script_Enemy();

	// MonoBehavior을(를) 통해 상속됨
	virtual void Start() override;
	virtual void Update() override;

	void EnemyAI();
	void SetActive();

	void SetPlayerObject(GameObject* input_player);
	bool GiveDamage(float damage);
	void Dead();
	void DeadActual();

	GameObject* playerobject;

	prefab_EnemyTrigger* enemyTrigger;

	// 적 스포너 
	Prefab_EnemySpawner* enemySpawner;

	float damage;
	float health;
	bool isActive = false;
	bool isDeath = false;
	bool isCurrentAttacking = false;
	bool isAttacked = false;

	float timecheck = 0;

	// MonoBehavior을(를) 통해 상속됨
	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;
};

