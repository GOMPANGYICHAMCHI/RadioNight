#include "framework.h"
#include "Script_Enemy.h"
#include "SceneManager.h"
#include "prefab_EnemyTrigger.h"
#include "Prefab_EnemySpawner.h"
#include "Script_Player.h"
#include "PlayerStat.h"

Script_Enemy::Script_Enemy(GameObject* gameObject)
	:MonoBehavior(gameObject), health(MONSTER_HEALTH), damage(MONSTER_DAMAGE)
{
}

Script_Enemy::~Script_Enemy()
{
}

void Script_Enemy::Start()
{
}

void Script_Enemy::Update()
{
	EnemyAI();
}

void Script_Enemy::EnemyAI()
{
	if (isCurrentAttacking && !isDeath)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= 3)
		{
			isCurrentAttacking = false;
			timecheck = 0;

			RenderEngine::Get()->factory->SetCurrentAnimation
			("Character/Monster_Final1.ase", &gameObject->transform, "Walk");
		}
		if (timecheck >= MONSTER_ATTACKDELAY && !isAttacked)
		{
			isAttacked = true;
			if ((gameObject->transform.pos - playerobject->transform.pos).Length() < MONSTER_REACH)
			{
				playerobject->GetScript< Script_Player>()->GiveDamage(damage);
			}
		}
	}

	//if (!isDeath)
	if (isActive && !isDeath)
	{
		if (playerobject != nullptr)
		{
			if ((gameObject->transform.pos - playerobject->transform.pos).Length() < MONSTER_ATTACKRANGE && isCurrentAttacking == false)
			{
				isCurrentAttacking = true;
				isAttacked = false;

				SoundManager::GetInstance()->PlaySFT("Attack1");

				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Monster_Final1.ase", &gameObject->transform, "Attack");

				gameObject->transform.LookAtOnlyY(playerobject->transform.pos);
			}
			else if (isCurrentAttacking == false)
			{
				gameObject->transform.LookAtOnlyY(playerobject->transform.pos);
				//gameObject->transform.pos += gameObject->transform.Forward() * 10 * TIME->Delta();
				Vector3 temp_vec = gameObject->transform.Forward() * MONSTER_MOVESPEED;
				gameObject->collider->AddForce({ temp_vec.x , 0 , temp_vec.z });
			}
		}
	}

	// À¸¾Ó Áê±Ý
	if (isDeath)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= 3)
		{
			DeadActual();
		}
	}
}

void Script_Enemy::SetActive()
{
	isActive = true;
	RenderEngine::Get()->factory->SetCurrentAnimation
	("Character/Monster_Final1.ase", &gameObject->transform, "Walk");
}

void Script_Enemy::SetPlayerObject(GameObject* input_player)
{
	playerobject = input_player;
}

bool Script_Enemy::GiveDamage(float damage)
{
	if (!isDeath)
	{
		health -= damage;

		if (health <= 0)
		{
			health = 0;
			Dead();
			return true;
		}
	}
	return false;
}

void Script_Enemy::Dead()
{
	SAFE_DELETE(gameObject->collider);
	SoundManager::GetInstance()->PlaySFT("Monster_dead");

	timecheck = 0;
	isDeath = true;
	enemyTrigger->EnemyDead();
	RenderEngine::Get()->factory->SetCurrentAnimation
	("Character/Monster_Final1.ase", &gameObject->transform, "Dead");

	//enemySpawner->Spawn();
}

void Script_Enemy::DeadActual()
{
	SceneManager::Get()->SubGameObject(gameObject);
}

void Script_Enemy::OnCollisionEnter(Collider* other)
{
}

void Script_Enemy::OnCollisionPersist(Collider* other)
{
}

void Script_Enemy::OnCollisionExit(Collider* other)
{
}

void Script_Enemy::OnTriggerEnter(Collider* other)
{
}

void Script_Enemy::OnTriggerPersist(Collider* other)
{
}

void Script_Enemy::OnTriggerExit(Collider* other)
{
}
