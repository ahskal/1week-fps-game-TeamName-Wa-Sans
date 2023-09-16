#include "stdafx.h"

Monster::Monster()
{
    monster = Actor::Create();
    monster->LoadFile("Monster.xml");
 
}

Monster::~Monster()
{

}

void Monster::Init(Vector3 spwan)
{
    
}

void Monster::RenderHierarchy()
{
    monster->RenderHierarchy();
}

void Monster::Update()
{
    if (INPUT->KeyPress('W'))
    {
        monster->MoveWorldPos(monster->GetForward()*3 * DELTA);
    }
    if (INPUT->KeyPress('S'))
    {
        monster->MoveWorldPos(-monster->GetForward() * 3 * DELTA);
    }


    if (INPUT->KeyPress('A'))
    {
        monster->rotation.y -= DELTA;
    }
    if (INPUT->KeyPress('D'))
    {
        monster->rotation.y += DELTA;
    }

    monster->Update();
}

void Monster::Render()
{
    monster->Render();
}