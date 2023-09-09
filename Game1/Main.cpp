#include "stdafx.h"
#include "CityMap.h"
#include "Main.h"

Main::Main()
{
    map = CityMap::Create();
    map->LoadFile("CityMap.xml");
    Camera::main = map->GetCam();
}

Main::~Main()
{
    map->Release();
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
    map->Hierarchy();
    ImGui::End();

    map->Update();
}

void Main::LateUpdate()
{
   
}
void Main::PreRender()
{
}

void Main::Render()
{
    map->Render();
}

void Main::ResizeScreen()
{
    map->ResizeScreen();
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