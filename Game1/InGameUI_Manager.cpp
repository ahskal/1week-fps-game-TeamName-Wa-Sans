#include "stdafx.h"
#include "InGameUI_Manager.h"



InGameUI_Manager::InGameUI_Manager()
{

	if (lobbyscene)
	{
		lobbythema = UI::Create();
		//lobbytema->LoadFile("lobbythema.xml");
		gamestart = UI::Create();
		//gamestart->LoadFile("gamestart.xml");
		exit = UI::Create();
		//exit->LoadFile("exit.xml");

	}

	if (inGamescene)
	{
		//playerAim = UI::Create();
		optionUI = UI::Create();

	   //아직 xml 추가안했음 이미지 작업후 xml 유아이 저장예정
	   //playerAim->LoadFile("playerAim.xml");
	   //optionUI->LoadFile("optionUI.xml");
	}




}

InGameUI_Manager::~InGameUI_Manager()
{


}

void InGameUI_Manager::RenderDetail()
{

}

void InGameUI_Manager::Update()
{
	if (lobbyscene)
	{
		lobbythema->RenderHierarchy();
		gamestart->RenderHierarchy();
		exit->RenderHierarchy();
	}
	lobbythema->Update();
	gamestart->Update();
	exit->Update();
	//lobby
	if (inGamescene)
	{
		playerAim->RenderHierarchy();
		optionUI->RenderHierarchy();
	}
	//ingame
	playerAim->Update();
	optionUI->Update();
}

void InGameUI_Manager::Render()
{
	if (lobbyscene)
	{
		lobbythema->Update();
		gamestart->Update();
		exit->Update();




	}


	if (inGamescene)
	{
		playerAim->Render();
		optionUI->Render();




	}
	
}
