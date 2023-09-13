#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{

	lobbycam = Camera::Create();
	//lobbycam->LoadFile("lobbycam.xml");
	Camera::main = lobbycam;


	lobbythema = UI::Create("lobbythema");
	lobbythema->LoadFile("lobbythema.xml");
	gamestart = UI::Create("gamestart");
	//gamestart->LoadFile("gamestart.xml");
	exit = UI::Create("exit");
	//exit->LoadFile("exit.xml");

	







	lobbycam->viewport.x = 0.0f;
	lobbycam->viewport.y = 0.0f;
	lobbycam->viewport.width = App.GetWidth();
	lobbycam->viewport.height = App.GetHeight();
	lobbycam->width = App.GetWidth();
	lobbycam->height = App.GetHeight();
}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::Init()
{

}

void LobbyScene::Release()
{

}

void LobbyScene::Update()
{
	ImGui::Begin("Hierarchy");
	lobbythema->RenderHierarchy();
	gamestart->RenderHierarchy();
	exit->RenderHierarchy();
	lobbycam->RenderHierarchy();
	ImGui::End();

	lobbycam->Update();
	lobbythema->Update();
	gamestart->Update();
	exit->Update();

}

void LobbyScene::LateUpdate()
{

}

void LobbyScene::PreRender()
{

}

void LobbyScene::Render()
{
	lobbycam->Set();
	lobbythema->Render();
	gamestart->Render();
	exit->Render();

}

void LobbyScene::ResizeScreen()
{
	lobbycam->viewport.x = 0.0f;
	lobbycam->viewport.y = 0.0f;
	lobbycam->viewport.width = App.GetWidth();
	lobbycam->viewport.height = App.GetHeight();
	lobbycam->width = App.GetWidth();
	lobbycam->height = App.GetHeight();
}
