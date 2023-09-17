#include "stdafx.h"
#include "VillageMap.h"
#include "Main.h"

Main::Main()
{
    Map = VillageMap::Create();
    Map->ResizeScreen();
    Camera::main = Map->GetCam();
    Camera::main->mainCamSpeed = 100;

    // 화면가릴 이미지
    ui = UI::Create();
    ui->LoadFile("LoaddingImage.xml");

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
    Map->Hierarchy();

    ui->RenderHierarchy();

    ImGui::End();

    ui->Update();
    Map->Update();
    Camera::main->Update();
}

void Main::LateUpdate()
{
    Map->LateUpdate();
    //맵 불러오는 도중에는 화면을 가림
    if (Map->IsSetHouseDone()) {
        ui->visible = true;
    }
    else {
        ui->visible = false;
    }
}
void Main::PreRender()
{
}

void Main::Render()
{
    Camera::main->Set();
    Map->Render();
    ui->Render();
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