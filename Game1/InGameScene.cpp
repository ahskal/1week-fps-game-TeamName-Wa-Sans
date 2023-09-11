#include "stdafx.h"
#include "InGameScene.h"
//���� �۾� ���� header ���� �߰����ֽø�˴ϴ�!
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
    //���� ī�޶� ��Ʈ��
    Camera::ControlMainCam();

    //gui �������� ��ġ ũ�� ���
    ImGui::Begin("Hierarchy");
    grid->RenderHierarchy();
    MainCam->RenderHierarchy();
    PlayerAimUI->RenderHierarchy();
    ImGui::End();

    //��� ī�޶� ������Ʈ
    grid->Update();
    MainCam->Update();
    PlayerAimUI->Update();
}

void InGameScene::LateUpdate()
{
    //�÷��̾� AimUI ũ�ν����
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
    //��ü�� ���� ����ּ��� 
    MainCam->Set();
    grid->Render();
    PlayerAimUI->Render();
}

void InGameScene::ResizeScreen()
{
    //ī�޶� ȭ��� ���� 
    MainCam->viewport.x = 0.0f;
    MainCam->viewport.y = 0.0f;
    MainCam->viewport.width = App.GetWidth();
    MainCam->viewport.height = App.GetHeight();
    MainCam->width = App.GetWidth();
    MainCam->height = App.GetHeight();
}
