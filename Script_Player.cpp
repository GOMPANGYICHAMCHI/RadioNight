#include "framework.h"
#include "Script_Player.h"
#include "PlayerStat.h"
#include "Script_Enemy.h"
#include "Script_InGameUI.h"
#include "Script_PlayerRhythm.h"

#include "Clamp.h"

#include "PikcingManager.h"


#include "MainMenuScene.h"

Script_Player::Script_Player(GameObject* gameObject)
	:MonoBehavior(gameObject), state(PlayerState::player_Idle), current_health(PLAYER_MAXHEALTH),
	timecheck(0), timecheck_fever(0), current_ammo(6), current_exp(0), current_feverlevel(0), current_score(0),
	accumulateExp(0), killCount(0), perfectshot(0), dashcheck(0), old_feverlevel(0),
	isEnd(false), isWalking(false), isReload1(false), isNowAnim(false), isDash(false), 
	isSuperMode(false), isMulDamageMode(false), isInfiniteAmmoMode(false),
	MouseSens_x(MOUSE_SENSITIVE_X), MouseSens_y(MOUSE_SENSITIVE_Y)
{
	//===============================================
	font = RenderEngine::Get()->GetFont();	// 디버그용
	//===============================================

	cameratransform = &RenderEngine::Get()->GetCamera()->transform;
	camHorTrans = new Transform();

	// 카메라 가로축 제어를 위한 빈 트랜스폼
	camHorTrans->relateS = false;
	camHorTrans->parent = &gameObject->transform;
	camHorTrans->pos = Vector3(0, 2, 0);
	camHorTrans->rot = { 0,3.14f / 2.0f,0 };

	// 카메라
	Camera* temp_cam = RenderEngine::Get()->GetCamera();
	temp_cam->transform.relateS = false;
	temp_cam->transform.rot = gameObject->transform.rot;
	temp_cam->transform.pos = Vector3(0, 2, 0);
	temp_cam->transform.parent = camHorTrans;

	gameObject->transform.pos = { 22,gameObject->transform.pos.y,52 };
	gameObject->transform.UpdateWorld();

	// 인게임 UI 로드
	playerUI = SceneManager::Get()->FindGameObject("InGameUI")->GetScript< Script_InGameUI>();
	// 플레이어 리듬 로드
	rhythm = gameObject->GetScript<Script_PlayerRhythm>();

	// 현재 남은 총알 UI 설정
	playerUI->SetTextAmmo(current_ammo);
	// 현재 남은 체력 UI 설정
	playerUI->SetHealth(current_health);
	// 피버 , 콤보 UI 설정
	playerUI->SetFeverExp(current_exp, FEVER_5_EXP, current_feverlevel);
	// 점수 UI 설정
	playerUI->SetScore(current_score);
}

Script_Player::~Script_Player()
{
}

void Script_Player::Start()
{
}

void Script_Player::Update()
{
	if (!isEnd)
	{
		if (InputHandler::GetInstance()->InputKeyDown("Press2"))
		{
			isSuperMode = !isSuperMode;
		}
		if (InputHandler::GetInstance()->InputKeyDown("Press3"))
		{
			isMulDamageMode = !isMulDamageMode;
		}
		if (InputHandler::GetInstance()->InputKeyDown("Press4"))
		{
			isInfiniteAmmoMode = !isInfiniteAmmoMode;

			if (isInfiniteAmmoMode)
			{
				current_ammo = 6;
				// 현재 잔탄수 UI로 표시
				playerUI->SetTextAmmo(current_ammo);

				timecheck = 0;
				state = PlayerState::player_Idle;
				isReload1 = false;

				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle1");
			}
		}

		Move();
		Shoot();
		Reload();
		UpdateFever();
		CheckEnd();
		UpdateMouseSensitivity();
	}
}

void Script_Player::DrawUI()
{
	if (!isEnd)
	{
		if (isSuperMode)
		{
			wstring temp_str = L"무적 모드";

			font->DrawTextColor(50, 50, { 1,1,1,1 }, fontType::esamanru_Light_30, (TCHAR*)temp_str.c_str(), 1);
		}

		if (isMulDamageMode)
		{
			wstring temp_str = L"데미지 배율";

			font->DrawTextColor(50, 100, { 1,1,1,1 }, fontType::esamanru_Light_30, (TCHAR*)temp_str.c_str(), 1);
		}

		if (isInfiniteAmmoMode)
		{
			wstring temp_str = L"무한 탄창";

			font->DrawTextColor(50, 150, { 1,1,1,1 }, fontType::esamanru_Light_30, (TCHAR*)temp_str.c_str(), 1);
		}
	}
	//wstring temp_str = to_wstring(current_exp);
	//
	//font->DrawTextColor(100, 500, { 0,1,0,1 }, (TCHAR*)temp_str.c_str(), 1);
	//
	//temp_str = to_wstring(current_feverlevel);
	//
	//font->DrawTextColor(100, 600, { 0,1,0,1 }, (TCHAR*)temp_str.c_str(), 1);
}

void Script_Player::GiveDamage(int Damage)
{
	if (state != PlayerState::player_Dead && !isSuperMode)
	{
		// 현재 체력 감소
		current_health -= Damage;

		// 현재 남은 체력 UI 설정
		playerUI->SetHealth(current_health);

		rhythm->SetRatio_Screen(1.0f);

		// 체력이 0 이하 일 경우,
		if (current_health <= 0)
		{
			// 사망처리
			current_health = 0;
			Dead();
		}
	}
}

void Script_Player::Move()
{
	if (state != PlayerState::player_Dead && state != PlayerState::player_End)
	{
		if (cameratransform != NULL)
		{
			camHorTrans->rot.y += InputHandler::GetInstance()->MousePosDelta.x * MouseSens_x;

			cameratransform->rot.x += InputHandler::GetInstance()->MousePosDelta.y * MouseSens_y;
			cameratransform->rot.x = Clamp(cameratransform->rot.x, -1.5f, 1.5f);
		}

		isMoving = false;
		dir_state = Direction::none;

		if (InputHandler::GetInstance()->InputKey("MoveForward"))
		{
			Vector3 temp_vec = camHorTrans->Forward() * PLAYER_MOVESPEED;
			gameObject->collider->AddForce({ temp_vec.x, 0, temp_vec.z });

			isMoving = true;
			dir_state = Direction::forward;
			//SoundManager::GetInstance()->RePlayMusic();
		}

		if (InputHandler::GetInstance()->InputKey("MoveBackward"))
		{
			Vector3 temp_vec = camHorTrans->Forward() * PLAYER_MOVESPEED;
			gameObject->collider->AddForce({ -temp_vec.x, 0, -temp_vec.z });

			isMoving = true;
			dir_state = Direction::backward;
			//SoundManager::GetInstance()->PauseMusic();
		}

		if (InputHandler::GetInstance()->InputKey("MoveRight"))
		{
			Vector3 temp_vec = camHorTrans->Right() * PLAYER_MOVESPEED;
			gameObject->collider->AddForce({ temp_vec.x, 0, temp_vec.z });

			isMoving = true;
			dir_state = Direction::right;
		}

		if (InputHandler::GetInstance()->InputKey("MoveLeft"))
		{
			Vector3 temp_vec = camHorTrans->Right() * PLAYER_MOVESPEED;
			gameObject->collider->AddForce({ -temp_vec.x, 0, -temp_vec.z });

			isMoving = true;
			dir_state = Direction::left;
		}

		gameObject->transform.pos.y = 2;

		if (!isDash && isMoving && dir_state != Direction::none && state == PlayerState::player_Idle)
		{
			if (InputHandler::GetInstance()->InputKey("Dash"))
			{
				isDash = true;

				SoundManager::GetInstance()->PlaySFT("Dodge");

				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Dash");

				switch (dir_state)
				{
				case Direction::backward:
					{
						Vector3 temp_vec = camHorTrans->Forward() * PLAYER_DASHSPEED;
						gameObject->collider->AddForce({ -temp_vec.x,0,-temp_vec.z });
					}break;
				case Direction::forward:
					{
						Vector3 temp_vec = camHorTrans->Forward() * PLAYER_DASHSPEED;
						gameObject->collider->AddForce({ temp_vec.x,0,temp_vec.z });
					}break;
				case Direction::right:
					{
						Vector3 temp_vec = camHorTrans->Right() * PLAYER_DASHSPEED;
						gameObject->collider->AddForce({ temp_vec.x ,0,temp_vec.z });
					}break;
				case Direction::left:
					{
						Vector3 temp_vec = camHorTrans->Right() * PLAYER_DASHSPEED;
						gameObject->collider->AddForce({ -temp_vec.x,0,-temp_vec.z });
					}break;
				}

			}
		}


		if 
		(
			InputHandler::GetInstance()->InputKeyDown("MoveForward")||
			InputHandler::GetInstance()->InputKeyDown("MoveBackward")||
			InputHandler::GetInstance()->InputKeyDown("MoveRight")||
			InputHandler::GetInstance()->InputKeyDown("MoveLeft")
		)
		{
			if (!isNowAnim && !isDash)
			{
				if (state == PlayerState::player_Reload2)
				{
					// 애니메이션 전환
					RenderEngine::Get()->factory->SetCurrentAnimation
					("Character/Gun&Arm_Final_End.ase", cameratransform, "Walk2");
				}
				else
				{
					// 애니메이션 전환
					RenderEngine::Get()->factory->SetCurrentAnimation
					("Character/Gun&Arm_Final_End.ase", cameratransform, "Walk1");
				}
			}
		}
		if 
		(
			InputHandler::GetInstance()->InputKeyUp("MoveForward") ||
			InputHandler::GetInstance()->InputKeyUp("MoveBackward") ||
			InputHandler::GetInstance()->InputKeyUp("MoveRight") ||
			InputHandler::GetInstance()->InputKeyUp("MoveLeft")
		)
		{
			if (!isNowAnim && !isMoving && !isDash)
			{
				if (state == PlayerState::player_Reload2)
				{
					// 애니메이션 전환
					RenderEngine::Get()->factory->SetCurrentAnimation
					("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle2");
				}
				else
				{
					// 애니메이션 전환
					RenderEngine::Get()->factory->SetCurrentAnimation
					("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle1");
				}
			}
		}
	}

	if (isDash)
	{
		dashcheck += Time::Get()->Delta();
		if (dashcheck >= 0.5f)
		{
			if (state == PlayerState::player_Reload2 && !isNowAnim)
			{
				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle2");
			}
			else if(!isNowAnim)
			{
				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle1");
			}
		}
		if (dashcheck >= PLAYER_DASHDELAY)
		{
			isDash = false;
			dashcheck = 0;
		}
	}
}

void Script_Player::Shoot()
{
	if (state == PlayerState::player_Idle && current_ammo != 0)
	{
		if (InputHandler::GetInstance()->InputKeyDown("Shoot"))
		{
			// 싱크판별
			SetSync();

			// 싱크가 실패가 아닐때
			if (current_sync != SyncType::type_Fail)
			{
				// 현재 상태 사격으로 전환
				state = PlayerState::player_Shot;

				SoundManager::GetInstance()->PlaySFT("Gunfire");

				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Fire");

				isNowAnim = true;

				// 피킹
				Collider* other = PickingManager::Get()->pickedObject();
				if (other != nullptr)
				{
					// 적을 맞췄을때
					if (other->gameObject->name == "Enemy")
					{
						Script_Enemy* temp_enemy = other->gameObject->GetScript<Script_Enemy>();

						SoundManager::GetInstance()->PlaySFT("Hit");

						// 적이 죽지 않았을때
						if (!temp_enemy->isDeath)
						{
							bool isKill;
							if (isMulDamageMode)
							{
								isKill = temp_enemy->GiveDamage(PLAYER_DAMAGE * 10);
							}
							else
							{
								isKill = temp_enemy->GiveDamage(PLAYER_DAMAGE);
							}

							// EXP 추가
							AddExp();
							// 피버 설정
							SetFever();
							// 피버 , 콤보 UI 설정
							playerUI->SetFeverExp(current_exp, FEVER_5_EXP, current_feverlevel);

							// 점수추가
							AddScore(isKill);
							// 점수 UI 설정
							playerUI->SetScore(current_score);

							if (isKill)
							{
								killCount += 1;
							}
						}

						// 적이 죽었을때
						else
						{
							
						}
					}
					// 맞추지 못했을때
					else
					{
						current_exp -= FEVER_DOWN;

						// 피버 설정
						SetFever();
						// 피버 , 콤보 UI 설정
						playerUI->SetFeverExp(current_exp, FEVER_5_EXP, current_feverlevel);
					}
				}

				if (isInfiniteAmmoMode)
				{
					current_ammo = 6;
					// 현재 잔탄수 UI로 표시
					playerUI->SetTextAmmo(current_ammo);
				}
				else
				{
					// 잔탄수 -1
					current_ammo -= 1;
					// 현재 잔탄수 UI로 표시
					playerUI->SetTextAmmo(current_ammo);
				}
			}

			else if (current_sync == SyncType::type_Fail)
			{
				SoundManager::GetInstance()->PlaySFT("Fail");
			}
		}
	}
	else if (state == PlayerState::player_Shot)
	{
		// 샷 딜레이 대기 후 상태 변경
		timecheck += Time::Get()->Delta();
		if (timecheck >= PLAYER_SHOTDELAY)
		{
			timecheck = 0;
			// 현재 상태 설정
			state = PlayerState::player_Idle;

			isNowAnim = false;

			// 애니메이션 전환
			RenderEngine::Get()->factory->SetCurrentAnimation
			("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle1");
		}
	}

	if (InputHandler::GetInstance()->InputKeyDown("Shoot") && current_ammo == 0)
	{
		SoundManager::GetInstance()->PlaySFT("Empty");
	}
}

void Script_Player::Reload()
{
	if (InputHandler::GetInstance()->InputKeyDown("ReLoad") && current_ammo != 6)
	{
		if (state == PlayerState::player_Idle)
		{
			isReload1 = true;

			SoundManager::GetInstance()->PlaySFT("Reload1");

			// 잔탄수 최대로
			current_ammo = 0;
			// 현재 잔탄수 UI로 표시
			playerUI->SetTextAmmo(current_ammo);

			// 현재 상태 설정
			state = PlayerState::player_Reload1;

			// 애니메이션 전환
			RenderEngine::Get()->factory->SetCurrentAnimation
			("Character/Gun&Arm_Final_End.ase", cameratransform, "Reload1");

			isNowAnim = true;
		}
		else if (state == PlayerState::player_Reload2 && isReload1)
		{
			SetSync();

			if (current_sync != SyncType::type_Fail)
			{
				SoundManager::GetInstance()->PlaySFT("Reload2");

				// 애니메이션 전환
				RenderEngine::Get()->factory->SetCurrentAnimation
				("Character/Gun&Arm_Final_End.ase", cameratransform, "Reload2");

				isNowAnim = true;
				isReload1 = false;
			}
			else if (current_sync == SyncType::type_Fail)
			{
				SoundManager::GetInstance()->PlaySFT("Fail");
			}
		}
	}

	else if (state == PlayerState::player_Reload1)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= PLAYER_RELOADTIME)
		{
			timecheck = 0;
			state = PlayerState::player_Reload2;

			// 애니메이션 전환
			RenderEngine::Get()->factory->SetCurrentAnimation
			("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle2");

			isNowAnim = false;
		}
	}
	else if (state == PlayerState::player_Reload2 && !isReload1)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= PLAYER_RELOADTIME)
		{
			timecheck = 0;
			state = PlayerState::player_Idle;

			// 잔탄수 최대로
			current_ammo = 6;
			// 현재 잔탄수 UI로 표시
			playerUI->SetTextAmmo(current_ammo);

			// 애니메이션 전환
			RenderEngine::Get()->factory->SetCurrentAnimation
			("Character/Gun&Arm_Final_End.ase", cameratransform, "Idle1");

			isNowAnim = false;
		}
	}
}

void Script_Player::Dead()
{
	// 현재 상태 설정
	state = PlayerState::player_Dead;

	gameObject->GetScript<Script_PlayerRhythm>()->isON = false;
	SoundManager::GetInstance()->StopAllSound();

	SoundManager::GetInstance()->PlayBGM("Dead");

	playerUI->Dead();
	timecheck = 0;

	isEnd = true;
}

void Script_Player::UpdateFever()
{
	if (state != PlayerState::player_Dead && state != PlayerState::player_End && current_exp != 0)
	{
		timecheck_fever += Time::Get()->Delta();
		if (timecheck_fever >= EXP_DECREASE_DELAY)
		{
			// 시간 초기화
			timecheck_fever = 0;
			// EXP 감소
			current_exp -= EXP_DECREASE_AMOUNT;
			if (current_exp <= 0)
			{
				current_exp = 0;
			}
			// 피버 설정
			SetFever();
			// 피버 , 콤보 UI 설정
			playerUI->SetFeverExp(current_exp, FEVER_5_EXP, current_feverlevel);
		}
	}
}

void Script_Player::CheckEnd()
{
	if (state == PlayerState::player_End)
	{
		timecheck += Time::Get()->Delta();
		if (timecheck >= DELAY_GAMEEND)
		{
			playerUI->SetGameEnd(accumulateExp, killCount, perfectshot, current_score);
			isEnd = true;
		}

	}
}

void Script_Player::CheckDead()
{

}

void Script_Player::SetEnd()
{
	state = PlayerState::player_End;

	gameObject->GetScript<Script_PlayerRhythm>()->isON = false;
	SoundManager::GetInstance()->StopAllSound();

	timecheck = 0;
}

void Script_Player::SetFever()
{
	if (current_exp <= FEVER_0_EXP)
	{
		current_feverlevel = 0;
	}
	else if (current_exp <= FEVER_1_EXP)
	{
		current_feverlevel = 1;
	}
	else if (current_exp <= FEVER_2_EXP)
	{
		current_feverlevel = 2;
	}
	else if (current_exp <= FEVER_3_EXP)
	{
		current_feverlevel = 3;
	}
	else if (current_exp <= FEVER_4_EXP)
	{
		current_feverlevel = 4;
	}
	else if (current_exp <= FEVER_5_EXP)
	{
		current_feverlevel = 5;
	}

	if (old_feverlevel < current_feverlevel)
	{
		SoundManager::GetInstance()->PlaySFT("Fever");
	}

	old_feverlevel = current_feverlevel;
}

void Script_Player::SetSync()
{
	// Ratio 획득
	currentratio = rhythm->GetRatio();

	if (currentratio < 0.2f)
	{
		rhythm->SetRatio_perfect(SYNC_RING_TIME);
			current_sync = SyncType::type_Perfect;
	}
	else if (currentratio < 0.4f)
	{
		rhythm->SetRatio_Good(SYNC_RING_TIME);
		current_sync = SyncType::type_Good;
	}
	else if (currentratio < 0.8f)
	{
		rhythm->SetRatio_Bad(SYNC_RING_TIME);
		current_sync = SyncType::type_Fail;
	}
	else
	{
		rhythm->SetRatio_perfect(SYNC_RING_TIME);
		current_sync = SyncType::type_Perfect;
	}
}

void Script_Player::AddExp()
{
	switch (current_sync)
	{
		case SyncType::type_Perfect:
		{
				current_exp += SYNC_PERFECT_EXP;
				accumulateExp += SYNC_PERFECT_EXP;

				perfectshot++;
		}break;

		case SyncType::type_Good:
		{
				current_exp += SYNC_GOOD_EXP;
				accumulateExp += SYNC_GOOD_EXP;
		}break;

		case SyncType::type_Fail:
		{
				current_exp += SYNC_FAIL_EXP;
				accumulateExp += SYNC_FAIL_EXP;
		}break;
	}

	if (current_exp > FEVER_5_EXP)
	{
		current_exp = FEVER_5_EXP;
	}
}

void Script_Player::AddScore(bool iskill)
{
	int score;

	if (iskill)
	{
		score = SCORE_ENEMY_KILL;
	}
	else
	{
		score = SCORE_ENEMY_HIT;
	}

	switch (current_feverlevel)
	{
		case 0:
		{
				current_score += score * FEVER_0_SCOREMUL;
		}break;

		case 1:
		{
				current_score += score * FEVER_1_SCOREMUL;
		}break;

		case 2:
		{
				current_score += score * FEVER_2_SCOREMUL;
		}break;

		case 3:
		{
				current_score += score * FEVER_3_SCOREMUL;
		}break;

		case 4:
		{
				current_score += score * FEVER_4_SCOREMUL;
		}break;

		case 5:
		{
				current_score += score * FEVER_5_SCOREMUL;
		}break;
	}
}

void Script_Player::UpdateMouseSensitivity()
{
	if (InputHandler::GetInstance()->InputKeyDown("MouseUp"))
	{
		MouseSens_x += 0.001f; 
		MouseSens_y += 0.001f;
	}
	else if (InputHandler::GetInstance()->InputKeyDown("MouseDown"))
	{
		MouseSens_x -= 0.001f;
		MouseSens_y -= 0.001f;
	}
}

void Script_Player::OnCollisionEnter(Collider* other)
{
}

void Script_Player::OnCollisionPersist(Collider* other)
{
}

void Script_Player::OnCollisionExit(Collider* other)
{
}

void Script_Player::OnTriggerEnter(Collider* other)
{
}

void Script_Player::OnTriggerPersist(Collider* other)
{
}

void Script_Player::OnTriggerExit(Collider* other)
{
}
