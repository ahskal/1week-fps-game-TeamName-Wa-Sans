#include "stdafx.h"
#include "Main.h"
#include "InGameScene.h"
#include "LobbyScene.h"

Main::Main()
{

}

Main::~Main()
{

}

void Main::Init()
{
    //로비씬 나중에 중간중간 완성되가면서 체크합니다
    //SCENE->AddScene("Lobby", new LobbyScene);

    //씬 추가 현재는 임의로 인게임 작업을위해 인게임씬을 먼저둡니다 추후 수정할수있음
    SCENE->AddScene("InGame", new InGameScene);

    //add 씬에서 정해둔 키값으로 변환해서 보여줄씬 정합니다
    SCENE->ChangeScene("InGame");
}

void Main::Release()
{
}

void Main::Update()
{
    SCENE->Update();

}

void Main::LateUpdate()
{
    SCENE->LateUpdate();
}

void Main::PreRender()
{

}

void Main::Render()
{
    SCENE->Render();
}

void Main::ResizeScreen()
{
    SCENE->ResizeScreen();
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    App.SetAppName(L"Game1");
    App.SetInstance(instance);
    WIN->Create();
    D3D->Create();
    Main* main = new Main();
    main->Init();

    int wParam = (int)WIN->Run(main);


    main->Release();
    SafeDelete(main);
    D3D->DeleteSingleton();
    WIN->DeleteSingleton();

    return wParam;
}