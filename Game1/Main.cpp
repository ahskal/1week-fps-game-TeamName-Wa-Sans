#include "stdafx.h"
#include "VillageMap.h"
#include "Main.h"
#include "InGameScene.h"
#include "LobbyScene.h"

Main::Main()
{
    Map = VillageMap::Create();
    Camera::main = Map->GetCam();

    Camera::main->mainCamSpeed = 100;
    //1234
}

Main::~Main()
{
    Map->Release();
}

void Main::Init()
{
    //�κ�� ���߿� �߰��߰� �ϼ��ǰ��鼭 üũ�մϴ�
    //SCENE->AddScene("Lobby", new LobbyScene);

    //�� �߰� ����� ���Ƿ� �ΰ��� �۾������� �ΰ��Ӿ��� �����Ӵϴ� ���� �����Ҽ�����
    SCENE->AddScene("InGame", new InGameScene);

    //add ������ ���ص� Ű������ ��ȯ�ؼ� �����پ� ���մϴ�
    SCENE->ChangeScene("InGame");
    Map->Init();
}

void Main::Release()
{
}

void Main::Update()
{
    SCENE->Update();
    Camera::ControlMainCam();
    ImGui::Begin("Hierarchy");
    Map->Hierarchy();
    ImGui::End();

    Map->Update();
    Camera::main->Update();
}

void Main::LateUpdate()
{
    SCENE->LateUpdate();
}

    Map->LateUpdate();
}
void Main::PreRender()
{

}

void Main::Render()
{
    SCENE->Render();
    Camera::main->Set();

    Map->Render();
}

void Main::ResizeScreen()
{
    SCENE->ResizeScreen();
    Map->ResizeScreen();
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