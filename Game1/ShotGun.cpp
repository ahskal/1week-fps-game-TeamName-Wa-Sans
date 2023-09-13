#include "stdafx.h"

ShotGun::ShotGun()
{
	shotGun = Actor::Create();
	shotGun->LoadFile("Remington.xml");

    TriggerSpeed = 3.0f;
    recoilDir = 200.0f;
    fireTime = 0.4f;

    SOUND->AddSound("ShotgunFire.wav", "ShotgunFire");
    SOUND->AddSound("ShotgunReload.wav", "ShotgunReload");

    SOUND->SetVolume("ShotgunFire", 0.7f);
    SOUND->SetVolume("ShotgunReload", 0.7f);
}

ShotGun::~ShotGun()
{
}

void ShotGun::Init()
{
}

void ShotGun::RenderHierarchy()
{
	shotGun->RenderHierarchy();
}

void ShotGun::Update()
{
    ImGui::Text("bulletCount : %d", bulletCount);
    ImGui::Text("isLoad : %d", (int)isLoad);
	shotGun->Update();
}

void ShotGun::Render()
{
	shotGun->Render();
}

void ShotGun::ShotGunControl()
{
    if (bulletCount == 8) //8발 발사 모두 다 사용했을때
    {
        shotGun->Find("Trigger")->SetLocalPosZ(0.725f);
        if (INPUT->KeyUp('R'))
        {
            bulletCount = 0;
            fireTime = 0.4f;
            shotGun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
        }

        if (INPUT->KeyDown(VK_LBUTTON)) // 총알이 없는 상태에서 격발했을때
        {
            SOUND->Stop("NoBullet");
            SOUND->Play("NoBullet");
        }
    }
    else
    {
        fireTime += DELTA;
        /**
        1.5초되면 발사
        0.4초되면 장전모션
        발사되면 다시 0
        */
        if (fireTime < 0.4f)
        {
            if (shotGun->rotation.x > 0.0f)
            {
                shotGun->rotation.x = 0.0f;
            }
            else
            {
                shotGun->rotation.x -= recoilDir * ToRadian * DELTA;
            }
            if (shotGun->rotation.x < -10.0f * ToRadian)
            {
                recoilDir = -recoilDir;
            }
        }
        else if (fireTime >= 0.4f and fireTime < 1.0f)
        {
            isLoad = true;
            isrecoil = false;
            shotGun->rotation.x = 0.0f;
            recoilDir = 200.0f;
            SOUND->Play("ShotgunReload");
            if (shotGun->Find("Trigger")->GetLocalPos().z <= 0.425f)
            {
                TriggerSpeed = -TriggerSpeed;
            }

            if (shotGun->Find("Trigger")->GetLocalPos().z > 0.725f)
            {
                shotGun->Find("Trigger")->SetLocalPosZ(0.7251f);
            }
            else
                shotGun->Find("Trigger")->MoveLocalPos(-shotGun->Find("Trigger")->GetForward() * TriggerSpeed * DELTA);
        }
        else if (fireTime >= 1.5f)
        {
            if (INPUT->KeyDown(VK_LBUTTON))
            {
                shotGun->Find("TriggerPoint")->rotation.x = 2 * ToRadian;
                shotGun->Find("Trigger")->SetLocalPosZ(0.725f);
                TriggerSpeed = -TriggerSpeed;
                isLoad = false;
                isrecoil = true;
                fireTime = 0.0f;
                bulletCount++;
                SOUND->Stop("ShotgunFire");
                SOUND->Play("ShotgunFire");
            }
        }

        if (INPUT->KeyUp(VK_LBUTTON))
        {
            shotGun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
        }
    }
}
