#pragma once

class prefab_EnemyTrigger;
class Prefab_EnemySpawner;

class Script_Enemy : public MonoBehavior
{
public:
	Script_Enemy(GameObject* gameObject);
	~Script_Enemy();

	// MonoBehavior��(��) ���� ��ӵ�
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

	// �� ������ 
	Prefab_EnemySpawner* enemySpawner;

	float damage;
	float health;
	bool isActive = false;
	bool isDeath = false;
	bool isCurrentAttacking = false;
	bool isAttacked = false;

	float timecheck = 0;

	// MonoBehavior��(��) ���� ��ӵ�
	virtual void OnCollisionEnter(Collider* other) override;
	virtual void OnCollisionPersist(Collider* other) override;
	virtual void OnCollisionExit(Collider* other) override;
	virtual void OnTriggerEnter(Collider* other) override;
	virtual void OnTriggerPersist(Collider* other) override;
	virtual void OnTriggerExit(Collider* other) override;
};

