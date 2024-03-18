#include "stdafx.h"

Monster::Monster()
{
    monster = Actor::Create();
    monster->LoadFile("Monster.xml");
    SOUND->AddSound("ZombieAttack.wav", "ZombieAttack");
    SOUND->AddSound("ZombieIdle.wav", "ZombieIdle");
    
    //SOUND->SetVolume("ZombieAttack", 0.2f);
    SOUND->Play("ZombieIdle");
}

Monster::~Monster()
{
    
}

void Monster::Init(Vector3 spwan)
{
    
    this->GetMonsterActor()->SetWorldPos(spwan);
    hp = 100.0f;
    damage = 15.0f;
    speed = 5.0f;
}

void Monster::RenderHierarchy()
{
    monster->RenderHierarchy();
    //col->RenderHierarchy();
}

void Monster::Update()
{
    /** 디버그모드*/
    if (INPUT->KeyDown(VK_F2))
    {
        debug = !debug;
    }
    if (debug)
        monster->collider->visible = true;
    else monster->collider->visible = false;
    /** 디버그모드*/


    ImGui::Text("Monster HP : %d", (int)hp);
    ImGui::SameLine();
    ImGui::Text("\tMonster Damage : %d", (int)damage);


    //몬스터 가만히 있는 상태
    if (monType == MonType::IDLE)
    {
        SOUND->Play("ZombieIdle");

        monster->Find("LeftThigh")->rotation.x = 0;
        monster->Find("RightThigh")->rotation.x = 0;
    }
    lastPos = monster->GetWorldPos();   //실시간 위치

    //보행 중 다리를 일정 각도 돌렸을 때(ex : 현재 10도)
    if (monster->Find("LeftThigh")->rotation.x / ToRadian > 10 or monster->Find("LeftThigh")->rotation.x / ToRadian < -10)
    {
        LegDir = -LegDir;
    }

    monster->Update();
}

void Monster::Render()
{
    monster->Render();
}

void Monster::CollidePlayer(Player* player)
{
    if (monster->Intersect(player->GetPlayerActor()))
    {
        monster->SetWorldPos(lastPos);
        monster->Update();
    }

    float moveToPlayerLength = (player->GetPlayerActor()->GetWorldPos() - monster->GetWorldPos()).Length(); // 100일때 0.1 10일때 1
    if (moveToPlayerLength > 100.0f)
    {
        SOUND->SetVolume("ZombieIdle", 0.05f);
    }
    else if (moveToPlayerLength > 10.0f and moveToPlayerLength <= 100.0f)
    {
        SOUND->SetVolume("ZombieIdle", 1.0f / moveToPlayerLength * 10.0f);
    }
    else if (moveToPlayerLength <= 10.0f)
    {
        SOUND->SetVolume("ZombieIdle", 1.0f);
    }
}

//플레이어 추적 함수
void Monster::Chase(Player* player)
{
    CollidePlayer(player);

    if (monster->Find("MonSight")->Intersect(player->GetPlayerActor()) or monster->Find("MonBackHead")->Intersect(player->GetPlayerActor()))
        isChase = true;
    else
        isChase = false;

    if (isChase)
    {
        monType = MonType::WALK;
        Vector3 moveToPlayer = player->GetPlayerActor()->GetWorldPos() - monster->GetWorldPos();
        monster->rotation.y = atan2f(moveToPlayer.x, moveToPlayer.z);
        //cout << moveToPlayer.x << " " << moveToPlayer.Length() << " " << acos(moveToPlayer.x / moveToPlayer.Length())/ ToRadian << endl;
        monster->MoveWorldPos(monster->GetForward() * DELTA * 7.0f);
        monster->Find("LeftThigh")->rotation.x += LegDir * DELTA;
        monster->Find("RightThigh")->rotation.x -= LegDir * DELTA;
    }
}