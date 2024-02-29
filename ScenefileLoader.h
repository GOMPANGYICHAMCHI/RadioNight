#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

class Scene;

#define _CRT_SECURE_NO_WARNINGS

// 게임 오브젝트 구조체
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

// 스테이지 묶음
// 적 , 2차 생성 적 , 적 활성화 트리거 , 스테이지 알파벽
struct StageData
{
	string tag;

	SceneObject* gate;						// 문
	SceneObject* enemy_trigger;				// 적 활성화 트리거
	vector<SceneObject*> enemy;				// 적
	vector<SceneObject*> enemy_falsed;		// 2차 생성 적
	vector<SceneObject*> stage_block;		// 스테이지 알파벽
};

// 씬 데이터
struct SceneData
{
	// 전체 오브젝트
	vector<SceneObject*> objectdata;
	// 스테이지 데이터
	vector<StageData*> stagedata;
};

//
//	2022 / 08 / 02 _ GOMP
//	텍스트 로드 기반 싱글톤 유니티 씬 로더
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

	// 입력된 씬정보 에 들어있는 오브젝트를 생성하여 씬에 넣는다
	void CreateObjectfromData(string input_scenename, Scene* current_scene,Vector3 adjustamount = (0,0,0));
	
private:
	// 싱글톤 인스턴스
	static ScenefileLoader* instance;

private:
	// 트랜스폼 입력 함수
	void InsertTransform(GameObject* input_object,SceneObject* input_data,Vector3 adjustamount);
	void InsertTransform(GameObject* dest,SceneObject* src);

	// 트랜스폼 입력 함수
	void InsertTransform(Transform* input_transform, SceneObject* input_data, Vector3 adjustamount);

	// 파일로드 ( 입력값 : 파일경로)
	vector<SceneObject*> LoadFile(const char* filepath);
	void LoadFileAJW(string filepath);

	// 데이터 로드 ( 초기화 시 호출 )
	void LoadData();

	// 로드된 파일정보로 부터 스테이지 정보 로드
	void LoadStage(SceneData* scene);
	void LoadStageAJW(SceneData* scene);

	// 씬 데이터
	unordered_map<string, SceneData*> sceneDatas;
public:
	void LoadStageFromFile(string filepath);
};