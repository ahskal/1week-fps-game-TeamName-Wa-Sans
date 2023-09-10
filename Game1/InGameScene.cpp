#include "stdafx.h"
#include "InGameScene.h"
//각자 작업 영역 header 파일 추가해주시면됩니다!


InGameScene::InGameScene()
{
    grid = Grid::Create();

    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");
    Camera::main = cam1;

    cam1->viewport.x = 0.0f;
    cam1->viewport.y = 0.0f;
    cam1->viewport.width = App.GetWidth();
    cam1->viewport.height = App.GetHeight();
    cam1->width = App.GetWidth();
    cam1->height = App.GetHeight();
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
    cam1->RenderHierarchy();
    ImGui::End();

    //배경 카메라 업데이트
    grid->Update();
    cam1->Update();
}

void InGameScene::LateUpdate()
{
    //마우스 위치로 UI 커서 이동시키는 내용입니다 EX ) 총기에임
    Ray Up;
    Up = Utility::MouseToRay();
    Vector3 ndcMouse = Utility::MouseToNDC();
    //target->SetWorldPos(ndcMouse);
}

void InGameScene::PreRender()
{
}

void InGameScene::Render()
{
    //객체들 랜더 모아주세요 
    cam1->Set();
    grid->Render();
}

void InGameScene::ResizeScreen()
{
    //카메라 화면비 조정 
    cam1->viewport.x = 0.0f;
    cam1->viewport.y = 0.0f;
    cam1->viewport.width = App.GetWidth();
    cam1->viewport.height = App.GetHeight();
    cam1->width = App.GetWidth();
    cam1->height = App.GetHeight();
}
