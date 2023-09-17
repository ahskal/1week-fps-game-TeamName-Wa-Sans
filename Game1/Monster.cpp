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
    //cout << monster->rotation.y << endl;
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



//�÷��̾� ���� �Լ�
void Monster::Chase(Player* player)
{
    if (monster->Find("MonSight")->Intersect(player->GetPlayerActor()) or monster->Find("MonBackHead")->Intersect(player->GetPlayerActor()))
        isChase = true;
    else
        isChase = false;

    if (isChase)
    {
        if (!isCollide)
        {
            
            monType = MonType::WALK;
            Vector3 moveToPlayer = player->GetPlayerActor()->GetWorldPos() - monster->GetWorldPos();
            monster->rotation.y = atan2f(moveToPlayer.z, moveToPlayer.x);
            //cout << moveToPlayer.x << " " << moveToPlayer.Length() << " " << acos(moveToPlayer.x / moveToPlayer.Length())/ ToRadian << endl;
            monster->MoveWorldPos(moveToPlayer * DELTA * 0.4f);
            /*monster->rotation.y = �տ��� �ν��ϵ� �ڿ��� �ν��ϵ� ĳ���� ������ �ٶ���� �ϴµ� �ʹ� ��ƽ��ϴ�.���� �տ��� ���� �� ������ϱ�
                                    �� ���ڿ������ �ڿ� ������ �ν��ϰ��� �ް������� �ٰ����� ���Ŀ� �����ؾ� �ϴ� �κ����� �����մϴ�.*/
            monster->Find("LeftThigh")->rotation.x += LegDir * DELTA;
            monster->Find("RightThigh")->rotation.x -= LegDir * DELTA;
        }
    }
}