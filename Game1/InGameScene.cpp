#include "stdafx.h"
#include "InGameScene.h"
#include "LobbyScene.h"
//각자 작업 영역 header 파일 추가해주시면됩니다!
#include "UNIT.h"
#include "VillageMap.h"
#include "house.h"

InGameScene::InGameScene()
{
    MainCam = Camera::Create();
    //grid = Grid::Create();
    Map = VillageMap::Create();

    MainCam->LoadFile("Cam.xml");
   

    Camera::main = MainCam;
    MainCam->mainCamSpeed = 30.0f;

    MainCam->viewport.x = 0.0f;
    MainCam->viewport.y = 0.0f;
    MainCam->viewport.width = App.GetWidth();
    MainCam->viewport.height = App.GetHeight();
    MainCam->width = App.GetWidth();
    MainCam->height = App.GetHeight();
}

InGameScene::~InGameScene()
{
    MainCam->SaveFile("Cam.xml");
    Map->Release();
}

void InGameScene::Init()
{
    Map->Init();
}

void InGameScene::Release()
{

}

void InGameScene::Update()
{
    //메인 카메라 컨트롤
    Camera::ControlMainCam();

    //gui 내부제작 위치 크기 등등
    ImGui::Begin("Hierarchy");
    //grid->RenderHierarchy();
    MainCam->RenderHierarchy();
    Map->Hierarchy();
    ImGui::End();

    //배경 카메라 업데이트
    //grid->Update();
    Map->Update();
    MainCam->Update();
}

void InGameScene::LateUpdate()
{
    Map->LateUpdate();

}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
    //객체들 랜더 모아주세요 
    MainCam->Set();
    //grid->Render();
    Map->Render();
}

void InGameScene::ResizeScreen()
{
    //카메라 화면비 조정 
    MainCam->viewport.x = 0.0f;
    MainCam->viewport.y = 0.0f;
    MainCam->viewport.width = App.GetWidth();
    MainCam->viewport.height = App.GetHeight();
    MainCam->width = App.GetWidth();
    MainCam->height = App.GetHeight();

    Map->ResizeScreen();
}
