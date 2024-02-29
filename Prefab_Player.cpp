#include "framework.h"
#include "Script_Player.h"
#include "Prefab_Player.h"
#include "Script_PlayerRhythm.h"

#include "CapsuleCollider.h"


Prefab_Player::Prefab_Player()
{
	// 플레이어 오브젝트 명칭
	name = "Player";

	//SetGeometryInfo("Character/Gun&Arm_Final_End.ase", { 0,0,0 }, { 0,3.14,0 });

	AddScript<Script_PlayerRhythm>();
	AddScript<Script_Player>();

	RenderEngine::Get()->factory->CreateGeo("Character/Gun&Arm_Final_End.ase", GetScript< Script_Player>()->cameratransform, { 0,-3.95,-2.2f }, { 0,3.14f / 2,0 });

	RenderEngine::Get()->factory->SetCurrentAnimation
	("Character/Gun&Arm_Final_End.ase", GetScript< Script_Player>()->cameratransform, "Idle1");
}

Prefab_Player::~Prefab_Player()
{
	// 모델 렌더에서 삭제
	RenderEngine::Get()->factory->DeleteGeo("Character/Gun&Arm_Final_End.ase", GetScript< Script_Player>()->cameratransform);
}

void Prefab_Player::InitCollider()
{
	collider = new CapsuleCollider(this, Vector3(0.0f, 0.0f, 0.0f), 1.25f, 2.5f);
	collider->SetPhysicsType(PhysicsEngine::ePhysicsType::eDYNAMIC);
	collider->SetAxisLock(PhysicsEngine::PhysicsAixsLock(false, false, false, true, true, true));
	collider->Init();
	collider->SetMass(1.0f);
	collider->SetLinearDrag(10);
}
