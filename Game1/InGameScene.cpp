#include "stdafx.h"
#include "InGameScene.h"
//각자 작업 영역 header 파일 추가해주시면됩니다!
#include "UNIT.h"

InGameScene::InGameScene()
{
    MainCam = Camera::Create();
    grid = Grid::Create();
    PlayerAimUI = Actor::Create();

    MainCam->LoadFile("Cam.xml");
    PlayerAimUI->LoadFile("PlayerAimUI.xml");

    Camera::main = MainCam;

    MainCam->viewport.x = 0.0f;
    MainCam->viewport.y = 0.0f;
    MainCam->viewport.width = App.GetWidth();
    MainCam->viewport.height = App.GetHeight();
    MainCam->width = App.GetWidth();
    MainCam->height = App.GetHeight();
}

InGameScene::~InGameScene()
{

}

void InGameScene::Init()
{

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
    grid->RenderHierarchy();
    MainCam->RenderHierarchy();
    PlayerAimUI->RenderHierarchy();
    ImGui::End();

    //배경 카메라 업데이트
    grid->Update();
    MainCam->Update();
    PlayerAimUI->Update();
}

void InGameScene::LateUpdate()
{
    //플레이어 AimUI 크로스헤어
    {
        Ray Up;
        Up = Utility::MouseToRay();
        Vector3 ndcMouse = Utility::MouseToNDC();
        PlayerAimUI->SetWorldPos(ndcMouse);
    }
}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
    //객체들 랜더 모아주세요 
    MainCam->Set();
    grid->Render();
    PlayerAimUI->Render();
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
}
