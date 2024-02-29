#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

class Scene;

#define _CRT_SECURE_NO_WARNINGS

// ���� ������Ʈ ����ü
struct SceneObject
{
	string name;
	string tag;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 localeuler;

	bool isrotin = false;
	bool isposin = false;
	bool issclin = false;
	bool iseuleranglein = false;
};

// �������� ����
// �� , 2�� ���� �� , �� Ȱ��ȭ Ʈ���� , �������� ���ĺ�
struct StageData
{
	string tag;

	SceneObject* gate;						// ��
	SceneObject* enemy_trigger;				// �� Ȱ��ȭ Ʈ����
	vector<SceneObject*> enemy;				// ��
	vector<SceneObject*> enemy_falsed;		// 2�� ���� ��
	vector<SceneObject*> stage_block;		// �������� ���ĺ�
};

// �� ������
struct SceneData
{
	// ��ü ������Ʈ
	vector<SceneObject*> objectdata;
	// �������� ������
	vector<StageData*> stagedata;
};

//
//	2022 / 08 / 02 _ GOMP
//	�ؽ�Ʈ �ε� ��� �̱��� ����Ƽ �� �δ�
//
class ScenefileLoader
{
public :
	static ScenefileLoader* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ScenefileLoader();
		}
		return instance;
	}

	static void Delete()
	{
		SAFE_DELETE(instance);
	}

	ScenefileLoader();
	~ScenefileLoader();

	// �Էµ� ������ �� ����ִ� ������Ʈ�� �����Ͽ� ���� �ִ´�
	void CreateObjectfromData(string input_scenename, Scene* current_scene,Vector3 adjustamount = (0,0,0));
	
private:
	// �̱��� �ν��Ͻ�
	static ScenefileLoader* instance;

private:
	// Ʈ������ �Է� �Լ�
	void InsertTransform(GameObject* input_object,SceneObject* input_data,Vector3 adjustamount);
	void InsertTransform(GameObject* dest,SceneObject* src);

	// Ʈ������ �Է� �Լ�
	void InsertTransform(Transform* input_transform, SceneObject* input_data, Vector3 adjustamount);

	// ���Ϸε� ( �Է°� : ���ϰ��)
	vector<SceneObject*> LoadFile(const char* filepath);
	void LoadFileAJW(string filepath);

	// ������ �ε� ( �ʱ�ȭ �� ȣ�� )
	void LoadData();

	// �ε�� ���������� ���� �������� ���� �ε�
	void LoadStage(SceneData* scene);
	void LoadStageAJW(SceneData* scene);

	// �� ������
	unordered_map<string, SceneData*> sceneDatas;
public:
	void LoadStageFromFile(string filepath);
};