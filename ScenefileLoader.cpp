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
	if (sceneDatas.find(filepath) == sceneDatas.end())//ã�� ���Ѵٸ�(ù �ε� ��)
		LoadFile(filepath.c_str());//������ �ε��Ѵ�.

	SceneData* scene = sceneDatas.at(filepath);

	LoadStage(scene);
}

void ScenefileLoader::CreateObjectfromData(string input_scenename, Scene* current_scene, Vector3 adjustamount)
{
	// ���� �������� ������Ʈ
	GameObject* temp_init;
	// �ҷ��� �� ������
	SceneData* temp_scene = sceneDatas[input_scenename];

	if (temp_scene != nullptr)
	{
		// ������Ʈ ��ü ��ȸ
		for (SceneObject* temp_object : temp_scene->objectdata)
		{
			// �� �ݶ��̴�
			if (temp_object->name == " BoxBox")
			{
				// �ڽ� �ݶ��̴� ���ӿ�����Ʈ ����
				temp_init = new prefab_BoxColldier();
				// Ʈ������ ����
				InsertTransform(temp_init, temp_object, adjustamount);
				// ���� �� ������Ʈ �� ����
				current_scene->AddGameObject(temp_init);
				// Ʈ������ ����� 
				// ( ������ �˼� ������ �̷��� ���������� ��ġ���� �ʴ´� )
				InsertTransform(temp_init, temp_object, adjustamount);
			}
		}

		// ���� �÷ο� ����
		prefab_IngameMainFlow* mainflow = new prefab_IngameMainFlow();

		// �������� ������ ��ü ��ȸ
		for (auto temp_stage : temp_scene->stagedata)
		{
#pragma region Ʈ���� ����
			// Ʈ���� ������Ʈ ����
			prefab_EnemyTrigger* temp_trigger = new prefab_EnemyTrigger();
			InsertTransform(temp_trigger, temp_stage->enemy_trigger, adjustamount);

			// Ʈ���� ����
			temp_trigger->InitTrigger();
			// ����� �Ҵ�
			temp_trigger->currentScene = current_scene;
			// �±� ����
			temp_trigger->tag = temp_stage->tag;
			// �����÷ο� ����
			temp_trigger->MainFlowSet(mainflow);

			// ���� �� ������Ʈ �� ����
			current_scene->AddGameObject(temp_trigger);
			// Ʈ������ ����� 
			InsertTransform(temp_trigger, temp_stage->enemy_trigger, adjustamount);

			// ���� �÷ο� �������� Ʈ���ſ� ����
			mainflow->stagetrigger.push_back(temp_trigger);
#pragma endregion
#pragma region 1�� �� ����
			for (auto temp_enemy : temp_stage->enemy)
			{
				// ��ġ ���� ���� �� ����
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_enemy, adjustamount);
				temp_trigger->enemypos.push_back(temp_object);
			}
#pragma endregion
#pragma region 2�� �� ����
			for (auto temp_enemyfalse : temp_stage->enemy_falsed)
			{
				// ��ġ ���� ���� �� ����
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_enemyfalse, adjustamount);
				temp_trigger->enemyfalsepos.push_back(temp_object);
			}
#pragma endregion
#pragma region �������� ���ĺ� ����
			for (auto temp_alphawall : temp_stage->stage_block)
			{
				// ��ġ ���� ���� �� ����
				Transform* temp_object = new Transform();
				InsertTransform(temp_object, temp_alphawall, adjustamount);
				temp_trigger->roadblock.push_back(temp_object);
			}
#pragma endregion
#pragma region �� ����
			
			// ��ġ ���� ���� �� ����
			Transform* temp_object = new Transform();
			InsertTransform(temp_object, temp_stage->gate, adjustamount);
			temp_trigger->gatetrans = temp_object;
			temp_trigger->InitGate();
#pragma endregion
		}

		// ���� �� ������Ʈ �� ����
		current_scene->AddGameObject(mainflow);
		// ���� �÷ο� ����
		mainflow->Start();
	}
}

void ScenefileLoader::LoadStage(SceneData* scene)
{
	// ���� ��������
	StageData* temp_stage;

	for (auto temp_object : scene->objectdata)
	{
		// �±װ� ���� �Ұ��, ( ������ Unstagged �̿��� �±��� MainCamera�� ���� )
		if (temp_object->tag != " Untagged" && temp_object->tag != " MainCamera")
		{
			// ���� �±��� �������� ������ ���� ���� Ȯ��
			temp_stage = nullptr;
			for (auto findstage : scene->stagedata)
			{
				if (findstage->tag == temp_object->tag)
				{
					temp_stage = findstage;
					break;
				}
			}

			// �������� �����Ͱ� ���� ���,
			if (temp_stage == nullptr)
			{
				// �������� ������ ����
				temp_stage = new StageData();
				temp_stage->tag = temp_object->tag;

				// �� �������� ������ ���Ϳ� �߰�
				scene->stagedata.push_back(temp_stage);
			}

			// ����
			if (temp_object->name == " Enemy")
			{
				temp_stage->enemy.push_back(temp_object);
			}
			// 2�� ���� ����
			else if (temp_object->name == " Enemy_False")
			{
				temp_stage->enemy_falsed.push_back(temp_object);
			}
			// �������� Ʈ����
			else if (temp_object->name == " Stage_Trigger")
			{
				temp_stage->enemy_trigger = temp_object;
			}
			// �������� ���
			else if (temp_object->name == " Road_Block")
			{
				temp_stage->stage_block.push_back(temp_object);
			}
			// ����Ʈ
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

	// �� ������Ʈ ���� ���� ( �Լ� ����� ��ȯ�ȴ� )
	vector<SceneObject*> temp_objvec = {};
	// ���� �������� ������Ʈ
	SceneObject* temp_obj = nullptr;
	// ������ ���� �̸��� �±� , Ʈ������ ���� �ҷ��´� 
	// Ʈ�������� �����Ⱚ�� �����°��� ���� �ϱ� ���� �ѹ����� �ҷ����δ�
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
#pragma region �����̼� ��
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
#pragma region ������ ��
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
#pragma region ������ ��
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
#pragma region ���Ϸ� �ޱ� ��Ʈ
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
