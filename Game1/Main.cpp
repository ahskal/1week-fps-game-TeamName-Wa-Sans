#include "stdafx.h"
#include "Monster.h"        //������ ������ ����ϱ� ���� ��� ���� �߰�
#include "Main.h"

Main::Main()
{
    root = Actor::Create();
    root->LoadFile("Monster.xml");
    grid = Grid::Create();

    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");

    monster = new Monster();

    Camera::main = (Camera*)root->Find("BodyCam");

    
}

Main::~Main()
{
    cam1->SaveFile("Cam.xml");
    root->Release();
}

void Main::Init()
{
	
}

void Main::Release()
{
}

void Main::Update()
{
    //Camera::ControlMainCam();
    ImGui::Begin("Hierarchy");
    grid->RenderHierarchy();
    monster->RenderHierarchy();
    cam1->RenderHierarchy();
    //root->RenderHierarchy();
    ImGui::End();


    Camera::main->Update();
    grid->Update();
    //root->Update();
    monster->Update();
}

void Main::LateUpdate()
{
   
}
void Main::PreRender()
{
}

void Main::Render()
{
    Camera::main->Set();
    grid->Render();
    //root->Render();
    monster->Render();
}

void Main::ResizeScreen()
{

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