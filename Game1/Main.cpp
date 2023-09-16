#include "stdafx.h"
#include "Monster.h"        //몬스터의 내용을 사용하기 위해 헤더 파일 추가
#include "Main.h"

Main::Main()
{
    root = Actor::Create();
    root->LoadFile("Monster.xml"); 

    /*testCol = Actor::Create();
    testCol->LoadFile("testCol.xml");*/ //플레이어 추적 실험

    grid = Grid::Create();

    monster = new Monster();

    Camera::main = (Camera*)root->Find("BodyCam");
}

Main::~Main()
{
    Camera::main->SaveFile("Cam.xml");
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
    //testCol->RenderHierarchy();
    //root->RenderHierarchy();
    ImGui::End();


    Camera::main->Update();
    grid->Update();
    //root->Update();
    monster->Update();
    //testCol->Update();
}

void Main::LateUpdate()
{
    //monster->Collide(testCol); //몬스터 충돌 함수
    //monster->Chase(testCol, testCol->GetWorldPos());    //플레이어 추적 함수
}
void Main::PreRender()
{
}

void Main::Render()
{
    Camera::main->Set();
    grid->Render();
    monster->Render();
    //testCol->Render();
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