#include "framework.h"
#include "Script_SingleUI.h"

Script_SingleUI::Script_SingleUI()
{

}

Script_SingleUI::~Script_SingleUI()
{
	for (auto temp : element)
	{
		SAFE_DELETE(temp);
	}
}

void Script_SingleUI::Update()
{
	for (auto temp : element)
	{
		temp->Update(Time::Get()->Delta());
	}
}

void Script_SingleUI::Render()
{
	for (auto temp : element)
	{
		temp->Render();
	}
}