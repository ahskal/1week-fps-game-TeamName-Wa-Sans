#include "stdafx.h"
#include "InGameScene.h"
#include "LobbyScene.h"
//���� �۾� ���� header ���� �߰����ֽø�˴ϴ�!
#include "UNIT.h"
#include "VillageMap.h"
#include "house.h"
#include "Player.h"
#include "Weapon.h"
#include "Gun.h"

InGameScene::InGameScene()
{
    MainCam = Camera::Create();
    //grid = Grid::Create();
    Map = VillageMap::Create();

    player = new Player();

    MainCam->LoadFile("Cam1.xml");
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
    MainCam->SaveFile("Cam1.xml");
    Map->Release();
}

void InGameScene::Init()
{
    Map->Init();
    player->Init();
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
    MainCam->RenderHierarchy();
    Map->Hierarchy();
    player->RenderHierarchy();
    ImGui::End();

    //��� ī�޶� ������Ʈ
    player->PlayerControl();
   

    Map->Update();
    player->Update();
    MainCam->Update();
}

void InGameScene::LateUpdate()
{
    Map->LateUpdate();
    player->CollidePlayerToFloor(Map); 
}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
    //��ü�� ���� ����ּ��� 
    MainCam->Set();
    Map->Render();
    player->Render();
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
