#include "stdafx.h"
#include "Player.h"
#include "Main.h"
#include "InGameScene.h"
#include "LobbyScene.h"

Main::Main()
{
    
    

    
    grid = Grid::Create();

    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");
    Camera::main = cam1;

    player = new Player();
}

Main::~Main()
{
    
}

void Main::Init()
{
    //�κ�� ���߿� �߰��߰� �ϼ��ǰ��鼭 üũ�մϴ�
    //SCENE->AddScene("Lobby", new LobbyScene);

    //�� �߰� ����� ���Ƿ� �ΰ��� �۾������� �ΰ��Ӿ��� �����Ӵϴ� ���� �����Ҽ�����
    SCENE->AddScene("InGame", new InGameScene);

    //add ������ ���ص� Ű������ ��ȯ�ؼ� �����پ� ���մϴ�
    SCENE->ChangeScene("InGame");
    
    player->Init();
}

void Main::Release()
{
}

void Main::Update()
{
    SCENE->Update();
    Camera::ControlMainCam();
    ImGui::Begin("Hierarchy");
    ImGui::End();
    grid->RenderHierarchy();
    player->RenderHierarchy();
    cam1->RenderHierarchy();
    ImGui::End();

    
    Camera::main->Update();

    player->PlayerControl();
    

    Camera::main->Update();
    grid->Update();
    player->Update();
}

void Main::LateUpdate()
{
    SCENE->LateUpdate();
}

   

    player->CollidePlayerToFloor(grid);
}
void Main::PreRender()
{

}

void Main::Render()
{
    SCENE->Render();
    Camera::main->Set();

    
    Camera::main->Set();
    grid->Render();
    player->Render();
}

void Main::ResizeScreen()
{
    SCENE->ResizeScreen();
    
    Camera::main->viewport.x = 0.0f;
    Camera::main->viewport.y = 0.0f;
    Camera::main->viewport.width = App.GetWidth();
    Camera::main->viewport.height = App.GetHeight();

    Camera::main->width = App.GetWidth();
    Camera::main->height = App.GetHeight();
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