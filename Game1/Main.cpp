#include "stdafx.h"
#include "VillageMap.h"
#include "Main.h"

Main::Main()
{
    Map = VillageMap::Create();
    Camera::main = Map->GetCam();

    Camera::main->mainCamSpeed = 100;
    
    root = Actor::Create();
    root->LoadFile("Robot.xml");
    //1234
}

Main::~Main()
{
    Map->Release();
}

void Main::Init()
{
    Map->Init();
}

void Main::Release()
{
}

void Main::Update()
{
    Camera::ControlMainCam();
    ImGui::Begin("Hierarchy");

    root->RenderHierarchy();
    Map->Hierarchy();
    ImGui::End();

    LastPos = root->GetWorldPos();
    if (INPUT->KeyPress(VK_LEFT) ) {
        root->MoveWorldPos(-root->GetRight() * DELTA* 10);
    }
    if (INPUT->KeyPress(VK_RIGHT)) {
        root->MoveWorldPos(root->GetRight() * DELTA * 10);
    }
    if (INPUT->KeyPress(VK_UP)) {
        root->MoveWorldPos(root->GetForward() * DELTA * 10);
    }
    if (INPUT->KeyPress(VK_DOWN)) {
        root->MoveWorldPos(-root->GetForward() * DELTA * 10);
    }






    root->Update();
    Map->Update();
    Camera::main->Update();
}

void Main::LateUpdate()
{
    Map->LateUpdate();


    if (Map->WallCollision(root)) {
        root->SetWorldPos(LastPos);
        root->Update();
    }
    if (Map->ItemCollision(root)) {

    }


}
void Main::PreRender()
{
}

void Main::Render()
{
    Camera::main->Set();

    root->Render();
    Map->Render();
}

void Main::ResizeScreen()
{
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