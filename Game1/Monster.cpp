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
    //col->RenderHierarchy();
}

void Monster::Update()
{
    cout << monster->rotation.y << endl;
    //몬스터 가만히 있는 상태
    if (monType == MonType::IDLE)
    {
        monster->Find("LeftThigh")->rotation.x = 0;
        monster->Find("RightThigh")->rotation.x = 0;
    }
    lastPos = monster->GetWorldPos();   //실시간 위치

    //몬스터가 앞으로 움직인다면...
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

    //몬스터가 뒤로 움직인다면...
    if (INPUT->KeyPress('S'))
    {
        monster->MoveWorldPos(-monster->GetForward() * 3 * DELTA);
    }

    //몬스터가 몸을 돌린다면...
    if (INPUT->KeyPress('A'))
    {
        monster->rotation.y -= DELTA;
    }
    if (INPUT->KeyPress('D'))
    {
        monster->rotation.y += DELTA;
    }

    //보행 중 다리를 일정 각도 돌렸을 때(ex : 현재 10도)
    if (monster->Find("LeftThigh")->rotation.x / ToRadian > 10 or monster->Find("LeftThigh")->rotation.x / ToRadian < -10)
    {
        LegDir = -LegDir;
    }

    //충돌 시 막히기
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

//오브젝트 충돌 함수 
void Monster::Collide(GameObject* ob)
{
    if (monster->Intersect(ob))
        isCollide = true;
    else
        isCollide = false;
}

//플레이어 추적 함수
void Monster::Chase(GameObject* player, Vector3 playerPos)
{
    if (monster->Find("MonSight")->Intersect(player) or monster->Find("MonBackHead")->Intersect(player))
        isChase = true;
    else
        isChase = false;

    if (isChase)
    {
        if (!isCollide)
        {
            
            monType = MonType::WALK;
            Vector3 moveToPlayer = playerPos - monster->GetWorldPos();
            cout << moveToPlayer.x << " " << moveToPlayer.Length() << " " << acos(moveToPlayer.x / moveToPlayer.Length())/ ToRadian << endl;
            monster->MoveWorldPos(moveToPlayer * DELTA * 0.4f);
            /*monster->rotation.y = 앞에서 인식하든 뒤에서 인식하든 캐릭터 방향을 바라봐야 하는데 너무 어렵습니다.현재 앞에서 오는 건 따라오니까
                                    덜 부자연스러운데 뒤에 있음을 인식하고서는 뒷걸음으로 다가오니 추후에 변경해야 하는 부분으로 생각합니다.*/
            monster->Find("LeftThigh")->rotation.x += LegDir * DELTA;
            monster->Find("RightThigh")->rotation.x -= LegDir * DELTA;
        }
    }
}