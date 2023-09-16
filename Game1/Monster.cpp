#include "stdafx.h"

Monster::Monster()
{
    monster = Actor::Create();
    monster->LoadFile("Monster.xml");
}

Monster::~Monster()
{

}

void Monster::Init()
{

}

void Monster::RenderHierarchy()
{
    monster->RenderHierarchy();
}

void Monster::Update()
{
    //���� ������ �ִ� ����
    if (monType == MonType::IDLE)
    {
        monster->Find("LeftThigh")->rotation.x = 0;
        monster->Find("RightThigh")->rotation.x = 0;
    }
    lastPos = monster->GetWorldPos();   //�ǽð� ��ġ

    //���Ͱ� ������ �����δٸ�...
    if (INPUT->KeyPress('W'))
    {
        monType = MonType::WALK;
        monster->MoveWorldPos(monster->GetForward() * 3 * DELTA);

        monster->Find("LeftThigh")->rotation.x += LegDir * DELTA;
        monster->Find("RightThigh")->rotation.x -= LegDir * DELTA;
    }
    if (INPUT->KeyUp('W'))
    {
        monType = MonType::IDLE;
    }

    //���Ͱ� �ڷ� �����δٸ�...
    if (INPUT->KeyPress('S'))
    {
        monster->MoveWorldPos(-monster->GetForward() * 3 * DELTA);
    }

    //���Ͱ� ���� �����ٸ�...
    if (INPUT->KeyPress('A'))
    {
        monster->rotation.y -= DELTA;
    }
    if (INPUT->KeyPress('D'))
    {
        monster->rotation.y += DELTA;
    }

    //���� �� �ٸ��� ���� ���� ������ ��(ex : ���� 10��)
    if (monster->Find("LeftThigh")->rotation.x / ToRadian > 10 or monster->Find("LeftThigh")->rotation.x / ToRadian < -10)
    {
        LegDir = -LegDir;
    }

    //�浹 �� ������
    if (isCollide)
    {
        monster->SetWorldPos(lastPos);
    }

    monster->Update();
}

void Monster::Render()
{
    monster->Render();
}

//������Ʈ �浹 �Լ� 
void Monster::Collide(GameObject* ob)
{
    if (monster->Intersect(ob))
        isCollide = true;
    else
        isCollide = false;
}