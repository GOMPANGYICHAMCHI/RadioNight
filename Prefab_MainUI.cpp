#include "framework.h"
#include "Prefab_MainUI.h"
#include "SceneManager.h"
#include "LoadingScene.h"
#include "EnemyScene.h"

#include "Script_MainUI.h"

Prefab_MainUI::Prefab_MainUI()
{
	AddScript<Script_MainUI>();
}

Prefab_MainUI::~Prefab_MainUI()
{
}
