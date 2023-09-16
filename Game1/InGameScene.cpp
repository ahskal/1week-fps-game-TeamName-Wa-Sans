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
    MainCam->LoadFile("MainCam.xml");
    
    Map = VillageMap::Create();

    player = new Player();

    //playerAim = UI::Create();
    optionUI = UI::Create();

    //���� xml �߰������� �̹��� �۾��� xml ������ ���忹��
    //playerAim->LoadFile("playerAim.xml");
    //optionUI->LoadFile("optionUI.xml");
    optionUI->visible = false;


    CurrentTime = 0.0f;
    zombieSpwanTime = 10.0f;

    Monster* mob = new Monster();
    Vector3 randomSpwan = Vector3(RANDOM->Float(-100, 100), 0, RANDOM->Float(-100, 100));
    mob->Init(randomSpwan);
    monster.push_back(mob);



    MainCam->mainCamSpeed = 30.0f;
    MainCam->viewport.x = 0.0f;
    MainCam->viewport.y = 0.0f;
    MainCam->viewport.width = App.GetWidth();
    MainCam->viewport.height = App.GetHeight();
    MainCam->width = App.GetWidth();
    MainCam->height = App.GetHeight();
    Camera::main = MainCam;

    //����
    
}

InGameScene::~InGameScene()
{
    MainCam->SaveFile("MainCam.xml");
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
    //Map->Hierarchy();
    player->RenderHierarchy();
    for (auto monsterPtr : monster)
    {
        monsterPtr->RenderHierarchy();
    }
    optionUI->RenderHierarchy();
    ImGui::End();

    //�ð����� �������
    /*if (TIMER->GetTick(CurrentTime, zombieSpwanTime))
    {
        for (int i = 0; i < 10; i++)
        {
        Zombie* zombie = new Zombie();
        Vector3 randomSpwan = Vector3(RANDOM->Float(-100, 100), 0, RANDOM->Float(-100, 100));
        zombie->Init(randomSpwan);
        zombies.push_back(zombie);
        }
    }*/


    //��� ī�޶� ������Ʈ
    player->PlayerControl();
   

    Map->Update();
    player->Update();
    for (auto monsterPtr : monster)
    {
        monsterPtr->Update();
    }
    Camera::main->Update();
    optionUI->Update();
}

void InGameScene::LateUpdate()
{
    Map->LateUpdate();
    player->CollidePlayerToFloor(Map); 
    Map->WallCollision(player->GetActor());
}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
    //��ü�� ���� ����ּ��� 
    Camera::main->Set();
    //MainCam->Set();
    Map->Render();
    player->Render();
    for (auto monsterPtr : monster)
    {
        monsterPtr->Render();
    }
    optionUI->Render();
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
