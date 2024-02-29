#include "framework.h"
#include "Prefab_InGameUI.h"
#include "Script_InGameUI.h"

Prefab_InGameUI::Prefab_InGameUI()
{
	name = "InGameUI";

	AddScript< Script_InGameUI>();
}

Prefab_InGameUI::~Prefab_InGameUI()
{
}
