#include "stdafx.h"
#include "InGameUI_Manager.h"



InGameUI_Manager::InGameUI_Manager()
{
	//플레이어 크로스헤어
	playerAim = UI::Create();
	//인게임 환경설정 UI
	optionUI  = UI::Create();

	//아직 xml 추가안했음 이미지 작업후 xml 유아이 저장예정
	//playerAim->LoadFile("playerAim.xml");
	//optionUI->LoadFile("optionUI.xml");
	
}

InGameUI_Manager::~InGameUI_Manager()
{


}

void InGameUI_Manager::Update()
{
	playerAim->Update();
	optionUI->Update();
}

void InGameUI_Manager::Render()
{
	playerAim->Render();
	optionUI->Render();
}
