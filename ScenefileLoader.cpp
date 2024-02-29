#include "framework.h"
#include "ScenefileLoader.h"
#include "Prefab_Genji.h"
#include "Prefab_Box.h"
#include "prefab_BoxColldier.h"
#include "prefab_TestMap.h"
#include "prefab_EnemyTrigger.h"
#include "prefab_Enemy.h"
#include "prefab_IngameMainFlow.h"
#include "Scene.h"

ScenefileLoader* ScenefileLoader::instance = nullptr;

ScenefileLoader::ScenefileLoader()
{
	LoadData();
}

ScenefileLoader::~ScenefileLoader()
{
	for (auto s : sceneDatas)
	{
		for (auto data : s.second->objectdata)
		{
			SAFE_DELETE(data);
		}
		for (auto data : s.second->stagedata)
		{
			SAFE_DELETE(data);
		}
		SAFE_DELETE(s.second);
	}
}

void ScenefileLoader::LoadStageFromFile(string filepath)
{
	if (sceneDatas.find(filepath) == sceneDatas.end())//찾지 못한다면(첫 로드 시)
		LoadFile(filepath.c_str());//파일을 로드한다.

	SceneData* scene = sceneDatas.at(filepath);

	LoadStage(scene);
}

void ScenefileLoader::CreateObjectfromData(string input_scenename, Scene* current_scene, Vector3 adjustamount)
{
	// 현재 생성중인 오브젝트
	GameObject* temp_init;
	// 불러올 씬 데이터
	SceneData* temp_scene = sceneDatas[input_scenename];

	if (temp_scene != nullptr)
	{
		// 오브젝트 전체 순회
		for (SceneObject* temp_object : temp_scene->objectdata)
		{
			// 맵 콜라이더
			if (temp_object->name == " BoxBox")
			{
				// 박스 콜라이더 게임오브젝트 생성
				temp_init = new prefab_BoxColldier();
				// 트랜스폼 삽입
				InsertTransform(temp_init, temp_object, adjustamount);
				// 현재 씬 오브젝트 에 삽입
				current_scene->AddGameObject(temp_init);
				// 트랜스폼 재삽입 
				// ( 이유는 알수 없지만 이렇게 하지않으면 배치되지 않는다 )
				InsertTransform(temp_init, temp_object, adjustamount);
			}
		}

		// 메인 플로우 생성
		prefab_IngameMainFlow* mainflow = new prefab_IngameMainFlow();

		// 스테이지 데이터 전체 순회
		for (auto temp_stage : temp_scene->stagedata)
		{
#pragma region 트리거 생성
			// 트리거 오브젝트 생성
			prefab_EnemyTrigger* temp_trigger = new prefab_EnemyTrigger();
			InsertTransform(temp_trigger, temp_stage->enemy_trigger, adjustamount);

			// 트리거 생성
			temp_trigger->InitTrigger();
			// 현재씬 할당
			temp_trigger->currentScene = current_scene;
			// 태그 전달
			temp_trigger->tag = temp_stage->tag;
			// 메인플로우 전달
			temp_trigger->MainFlowSet(mainflow);

			// 현재 씬 오브젝트 에 삽입
			current_scene->AddGameObject(temp_trigger);
			// 트랜스폼 재삽입 
			InsertTransform(temp_trigger, temp_stage->enemy_trigger, adjustamount);

			// 메인 플로우 스테이지 트리거에 삽입
			mainflow->stagetrigger.push_back(temp_trigger);
#pragma endregion
#pragma region 1차 적 생성
			for (auto temp_enemy : temp_stage->enemy)
			{
				// 위치 정보 생성 및 삽입
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_enemy, adjustamount);
				temp_trigger->enemypos.push_back(temp_object);
			}
#pragma endregion
#pragma region 2차 적 생성
			for (auto temp_enemyfalse : temp_stage->enemy_falsed)
			{
				// 위치 정보 생성 및 삽입
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_enemyfalse, adjustamount);
				temp_trigger->enemyfalsepos.push_back(temp_object);
			}
#pragma endregion
#pragma region 스테이지 알파벽 생성
			for (auto temp_alphawall : temp_stage->stage_block)
			{
				// 위치 정보 생성 및 삽입
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_alphawall, adjustamount);
				temp_trigger->roadblock.push_back(temp_object);
			}
#pragma endregion
#pragma region 문 생성
			
			// 위치 정보 생성 및 삽입
			Transform* temp_object = new Transform();
			InsertTransform(temp_object, temp_stage->gate, adjustamount);
			temp_trigger->gatetrans = temp_object;
			temp_trigger->InitGate();
#pragma endregion
		}

		// 현재 씬 오브젝트 에 삽입
		current_scene->AddGameObject(mainflow);
		// 메인 플로우 시작
		mainflow->Start();
	}
}

void ScenefileLoader::LoadStage(SceneData* scene)
{
	// 현재 스테이지
	StageData* temp_stage;

	for (auto temp_object : scene->objectdata)
	{
		// 태그가 존재 할경우, ( 유일한 Unstagged 이외의 태그인 MainCamera도 제외 )
		if (temp_object->tag != " Untagged" && temp_object->tag != " MainCamera")
		{
			// 동일 태그의 스테이지 데이터 존재 유무 확인
			temp_stage = nullptr;
			for (auto findstage : scene->stagedata)
			{
				if (findstage->tag == temp_object->tag)
				{
					temp_stage = findstage;
					break;
				}
			}

			// 스테이지 데이터가 없을 경우,
			if (temp_stage == nullptr)
			{
				// 스테이지 데이터 생성
				temp_stage = new StageData();
				temp_stage->tag = temp_object->tag;

				// 씬 스테이지 데이터 벡터에 추가
				scene->stagedata.push_back(temp_stage);
			}

			// 몬스터
			if (temp_object->name == " Enemy")
			{
				temp_stage->enemy.push_back(temp_object);
			}
			// 2차 생성 몬스터
			else if (temp_object->name == " Enemy_False")
			{
				temp_stage->enemy_falsed.push_back(temp_object);
			}
			// 스테이지 트리거
			else if (temp_object->name == " Stage_Trigger")
			{
				temp_stage->enemy_trigger = temp_object;
			}
			// 스테이지 블락
			else if (temp_object->name == " Road_Block")
			{
				temp_stage->stage_block.push_back(temp_object);
			}
			// 게이트
			else if (temp_object->name == " Gate")
			{
				temp_stage->gate = temp_object;
			}
		}
	}
}

void ScenefileLoader::InsertTransform(GameObject* input_object, SceneObject* input_data, Vector3 adjustamount)
{
	input_object->transform.pos = (input_data->position + adjustamount);
	input_object->transform.rot = input_data->rotation;
	input_object->transform.scale = input_data->scale;
}

void ScenefileLoader::InsertTransform(GameObject* dest, SceneObject* src)
{
	dest->transform.pos = src->position;
	dest->transform.rot = src->rotation;
	dest->transform.scale = src->scale;
}

void ScenefileLoader::InsertTransform(Transform* input_transform, SceneObject* input_data, Vector3 adjustamount)
{
	input_transform->pos = (input_data->position + adjustamount);
	input_transform->rot = input_data->rotation;
	input_transform->scale = input_data->scale;
}

void ScenefileLoader::LoadFile(string filepath)
{
	string line;
	string buffer;
	ifstream file(filepath.c_str());
	vector<string> temp_vec1;
	vector<string> temp_vec2;

	// 씬 오브젝트 저장 벡터 ( 함수 종료시 반환된다 )
	vector<SceneObject*> temp_objvec = {};
	// 현재 저장중인 오브젝트
	SceneObject* temp_obj = nullptr;
	// 파일을 열어 이름과 태그 , 트랜스폼 등을 불러온다 
	// 트랜스폼은 쓰레기값이 들어오는것을 방지 하기 위해 한번씩만 불러들인다
	if (file.is_open())
	{
		while (getline(file, line))
		{
			temp_vec1.clear();

			istringstream iss(line);

			while (getline(iss, buffer, ':'))
			{
				temp_vec1.push_back(buffer);
			}

			if (temp_vec1[0] == "GameObject")
			{
				temp_obj = new SceneObject();
				temp_objvec.push_back(temp_obj);
			}

			else if (temp_vec1[0] == "  m_TagString")
			{
				temp_obj->tag = temp_vec1[1];
			}

			else if (temp_vec1[0] == "  m_Name")
			{
				temp_obj->name = temp_vec1[1];
			}
#pragma region 로테이션 값
			else if (temp_vec1[0] == "  m_LocalRotation")
			{
				if (!temp_obj->isrotin)
				{
					temp_obj->isrotin = true;

					temp_vec2.clear();

					for (int i = 2; i < 6; i++)
					{
						istringstream iss(temp_vec1[i]);

						if (i == 5)
						{
							while (getline(iss, buffer, '}'))
							{
								temp_vec2.push_back(buffer);
							}
						}
						else
						{
							while (getline(iss, buffer, ','))
							{
								temp_vec2.push_back(buffer);
							}
						}
					}

					stringstream temp_str1(temp_vec2[0]);
					temp_str1 >> temp_obj->rotation.x;

					stringstream temp_str2(temp_vec2[2]);
					temp_str2 >> temp_obj->rotation.y;

					stringstream temp_str3(temp_vec2[4]);
					temp_str3 >> temp_obj->rotation.z;

					stringstream temp_str4(temp_vec2[temp_vec2.size() - 1]);
					temp_str4 >> temp_obj->rotation.w;
				}
			}
#pragma endregion
#pragma region 포지션 값
			else if (temp_vec1[0] == "  m_LocalPosition")
			{
				if (!temp_obj->isposin)
				{
					temp_obj->isposin = true;
					temp_vec2.clear();

					for (int i = 1; i < 5; i++)
					{
						istringstream iss(temp_vec1[i]);

						if (i == 4)
						{
							while (getline(iss, buffer, '}'))
							{
								temp_vec2.push_back(buffer);
							}
						}
						else
						{
							while (getline(iss, buffer, ','))
							{
								temp_vec2.push_back(buffer);
							}
						}
					}

					stringstream temp_str1(temp_vec2[1]);
					temp_str1 >> temp_obj->position.x;

					stringstream temp_str2(temp_vec2[3]);
					temp_str2 >> temp_obj->position.y;

					stringstream temp_str3(temp_vec2[5]);
					temp_str3 >> temp_obj->position.z;
				}
			}
#pragma endregion
#pragma region 스케일 값
			else if (temp_vec1[0] == "  m_LocalScale")
			{
				if (!temp_obj->issclin)
				{
					temp_obj->issclin = true;

					temp_vec2.clear();

					for (int i = 1; i < 5; i++)
					{
						istringstream iss(temp_vec1[i]);

						if (i == 4)
						{
							while (getline(iss, buffer, '}'))
							{
								temp_vec2.push_back(buffer);
							}
						}
						else
						{
							while (getline(iss, buffer, ','))
							{
								temp_vec2.push_back(buffer);
							}
						}
					}

					stringstream temp_str1(temp_vec2[1]);
					temp_str1 >> temp_obj->scale.x;

					stringstream temp_str2(temp_vec2[3]);
					temp_str2 >> temp_obj->scale.y;

					stringstream temp_str3(temp_vec2[5]);
					temp_str3 >> temp_obj->scale.z;
				}
			}
#pragma endregion
#pragma region 오일러 앵글 힌트
			else if (temp_vec1[0] == "  m_LocalEulerAnglesHint")
			{
				if (!temp_obj->iseuleranglein)
				{
					temp_obj->iseuleranglein = true;

					temp_vec2.clear();

					for (int i = 1; i < 5; i++)
					{
						istringstream iss(temp_vec1[i]);

						if (i == 4)
						{
							while (getline(iss, buffer, '}'))
							{
								temp_vec2.push_back(buffer);
							}
						}
						else
						{
							while (getline(iss, buffer, ','))
							{
								temp_vec2.push_back(buffer);
							}
						}
					}

					stringstream temp_str1(temp_vec2[1]);
					temp_str1 >> temp_obj->localeuler.x;

					stringstream temp_str2(temp_vec2[3]);
					temp_str2 >> temp_obj->localeuler.y;

					stringstream temp_str3(temp_vec2[5]);
					temp_str3 >> temp_obj->localeuler.z;
				}
			}
#pragma endregion
		}

		file.close();

		SceneData* sceneData = new SceneData();
		sceneData->objectdata = temp_objvec;
		sceneDatas.insert(make_pair(filepath, sceneData));
	}
}

void ScenefileLoader::LoadData()
{
	SceneData* temp_scene;

	temp_scene = new SceneData();
	temp_scene->objectdata = LoadFile("4_Resources/SceneData/EnemyScene.txt");
	sceneDatas.insert(make_pair("MainGameScene", temp_scene));
	LoadStage(temp_scene);
}
