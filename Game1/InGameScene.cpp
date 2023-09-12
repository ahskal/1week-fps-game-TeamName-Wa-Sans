#include "stdafx.h"
#include "InGameScene.h"
#include "LobbyScene.h"
//���� �۾� ���� header ���� �߰����ֽø�˴ϴ�!
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
    //���� ī�޶� ��Ʈ��
    Camera::ControlMainCam();

    //gui �������� ��ġ ũ�� ���
    ImGui::Begin("Hierarchy");
    //grid->RenderHierarchy();
    MainCam->RenderHierarchy();
    Map->Hierarchy();
    ImGui::End();

    //��� ī�޶� ������Ʈ
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
    //��ü�� ���� ����ּ��� 
    MainCam->Set();
    //grid->Render();
    Map->Render();
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

    Map->ResizeScreen();
}
