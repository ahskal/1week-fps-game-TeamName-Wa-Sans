#include "stdafx.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
	lobbycam = Camera::Create();
	//lobbycam->LoadFile("lobbycam.xml");
	Camera::main = lobbycam;

	mouse = UI::Create("mouse");
	mouse->LoadFile("mouse.xml");

	lobbythema = UI::Create("lobbythema");
	lobbythema->LoadFile("lobbythema.xml");
	gamestart = UI::Create("gamestart");
	gamestart->LoadFile("gamestart.xml");
	exit = UI::Create("exit");
	exit->LoadFile("exit.xml");

	themaback = UI::Create("themaback");
	themaback->LoadFile("themaback.xml");

	lobbycam->viewport.x = 0.0f;
	lobbycam->viewport.y = 0.0f;
	lobbycam->viewport.width = App.GetWidth();
	lobbycam->viewport.height = App.GetHeight();
	lobbycam->width = App.GetWidth();
	lobbycam->height = App.GetHeight();


	bg = new Sound();
	bg->AddSound("LobbyBGM.mp3","Lobby",true);
}

LobbyScene::~LobbyScene()
{
	
}

void LobbyScene::Init()
{
	bg->Play("Lobby");
	bg->SetVolume("Lobby", 0.2f);

	gamestart->visible = false;
	exit->visible = false;
}

void LobbyScene::Release()
{

}

void LobbyScene::Update()
{
	

	ImGui::Begin("Hierarchy");
	mouse->RenderHierarchy();
	lobbythema->RenderHierarchy();
	gamestart->RenderHierarchy();
	exit->RenderHierarchy();
	themaback->RenderHierarchy();
	lobbycam->RenderHierarchy();
	ImGui::End();


	mouse->Update();
	lobbycam->Update();
	lobbythema->Update();
	themaback->Update();
	gamestart->Update();
	exit->Update();

}

void LobbyScene::LateUpdate()
{
	themaback->rotation.z += 1.0f * DELTA;

	//시작버튼 클릭 메인의 업데이트에 씬변경로직있습니다
	if (gamestart->MouseOver() and INPUT->KeyDown(VK_LBUTTON))
	{
		SCENE->AddScene("InGame", new InGameScene);
		SCENE->ChangeScene("InGame");
		bg->Stop("Lobby");
	}
	if (exit->MouseOver() and INPUT->KeyDown(VK_LBUTTON))
	{
		PostQuitMessage(0);
	}

}

void LobbyScene::PreRender()
{
}

void LobbyScene::Render()
{
	//ShowCursor(false);

	lobbycam->Set();


	themaback->Render();
	lobbythema->Render();
	gamestart->Render();
	exit->Render();
	mouse->SetWorldPos(Utility::MouseToNDC());
	mouse->Render();
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
