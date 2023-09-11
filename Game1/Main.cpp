#include "stdafx.h"
#include "VillageMap.h"
#include "Main.h"

Main::Main()
{
    Map = VillageMap::Create();
    Camera::main = Map->GetCam();
    //1234
}

Main::~Main()
{
    Map->Release();
}

void Main::Init()
{
	
}

void Main::Release()
{
}

void Main::Update()
{
    Camera::ControlMainCam();
    ImGui::Begin("Hierarchy");
    Map->Hierarchy();
    ImGui::End();

    Map->Update();
}

void Main::LateUpdate()
{
   
}
void Main::PreRender()
{
}

void Main::Render()
{
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