#include "stdafx.h"
#include "Player.h"
#include "Main.h"

Main::Main()
{
    grid = Grid::Create();

    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");
    Camera::main = cam1;

    player = new Player();

    wall = Actor::Create();
    wall->LoadFile("Wall.xml");
}

Main::~Main()
{

}

void Main::Init()
{
    player->Init();
}

void Main::Release()
{
}

void Main::Update()
{
    Camera::ControlMainCam();

    ImGui::Begin("Hierarchy");
    grid->RenderHierarchy();
    wall->RenderHierarchy();
    player->RenderHierarchy();
    cam1->RenderHierarchy();
    ImGui::End();

    //플레이어 컨트롤 함수
    player->PlayerControl();
    

    Camera::main->Update();
    grid->Update();
    wall->Update();
    player->Update();
}

void Main::LateUpdate()
{
    // 플레이어 - 바닥 충돌판정 함수
    player->CollidePlayerToFloor(grid);

    // 플레이어 - 벽 충돌함수
    player->CollidePlayerToWall(wall);
}
void Main::PreRender()
{
}

void Main::Render()
{
    Camera::main->Set();
    grid->Render();
    wall->Render();
    player->Render();
}

void Main::ResizeScreen()
{
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