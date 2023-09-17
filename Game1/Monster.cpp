#include "stdafx.h"

Monster::Monster()
{
    monster = Actor::Create();
    monster->LoadFile("Monster.xml");
 
    
    unitType = UnitType::ZOMBIE;
    hp = 100.0f;
    damage = 15.0f;
    speed = 5.0f;
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
}

void Monster::Update()
{
    ImGui::Text("Zombie HP : %d", (int)hp);
    ImGui::SameLine();
    ImGui::Text("\tZombie Damage : %d", (int)damage);

    //if (INPUT->KeyPress('W'))
    //{
    //    monster->MoveWorldPos(monster->GetForward()*3 * DELTA);
    //}
    //if (INPUT->KeyPress('S'))
    //{
    //    monster->MoveWorldPos(-monster->GetForward() * 3 * DELTA);
    //}
    //
    //
    //if (INPUT->KeyPress('A'))
    //{
    //    monster->rotation.y -= DELTA;
    //}
    //if (INPUT->KeyPress('D'))
    //{
    //    monster->rotation.y += DELTA;
    //}

    monster->Update();
}

void Monster::Render()
{
    monster->Render();
}