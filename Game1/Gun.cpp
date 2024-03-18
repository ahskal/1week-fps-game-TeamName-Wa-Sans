#include "stdafx.h"

Gun::Gun()
{
	gun = Actor::Create();
	gun->LoadFile("beretta92.xml");

    SOUND->AddSound("beretta92Sound.wav", "beretta92");
    SOUND->AddSound("NoBullet.wav", "NoBullet");
    SOUND->SetVolume("beretta92", 0.5f);
    SOUND->SetVolume("NoBullet", 0.5f);
    bulletCount = 15;
    maxBullet = 45;
    magazineCount = 15;
}

Gun::~Gun()
{
}

void Gun::Init()
{
}

void Gun::RenderHierarchy()
{
	gun->RenderHierarchy();
}

void Gun::Update()
{
    ImGui::Text("\n\nGun Bullet");
    ImGui::Text("bullet : %d", bulletCount);
    ImGui::SameLine();
    ImGui::Text("  /  Max : %d", maxBullet);


	gun->Update();
}

void Gun::Render()
{
	gun->Render();
}

void Gun::GunControl()
{
    if (bulletCount == 0)
    {
        gun->Find("BarrelPoint")->SetLocalPosZ(-0.35f);
        if (INPUT->KeyUp('R'))
        {
            if (maxBullet < magazineCount)
            {
                bulletCount = maxBullet;
                maxBullet = 0;
            }
            else
            {
                bulletCount = magazineCount;
                maxBullet -= bulletCount;
            }
            gun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
            gun->Find("BarrelPoint")->SetLocalPosZ(0.0f);
        }

        if (INPUT->KeyDown(VK_LBUTTON))
        {
            SOUND->Stop("NoBullet");
            SOUND->Play("NoBullet");
        }
    }
    else
    {
        // 총알을 다 쓰지 않았을때 재장전
        if (INPUT->KeyUp('R'))
        {// bulletCount = 5라고 가정
            //          += (8 - 5  = 3)
            if (maxBullet < (magazineCount - bulletCount))
            {
                bulletCount += maxBullet;
                maxBullet = 0;
            }
            else
            {
                maxBullet -= (magazineCount - bulletCount);
                bulletCount += (magazineCount - bulletCount);
            }

            fireTime = 0.4f;
            gun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
        }

        // 공격시
        if (b_fire)
        {
            if (INPUT->KeyDown(VK_LBUTTON))
            {
                gun->Find("TriggerPoint")->rotation.x = 2 * ToRadian;
                gun->Find("BarrelPoint")->SetLocalPosZ(-0.01f);
                SOUND->Stop("beretta92");
                SOUND->Play("beretta92");
                isAttack = true;
                b_fire = false;
                fireTime = 0.0f;
                bulletCount--;
            }

        }
        else
        {
            if (TIMER->GetTick(attackTime, 0.1f))
            {
                isAttack = false;
            }
            if (TIMER->GetTick(fireTime, 0.5f))
            {
                b_fire = true;
            }
        }
        if (INPUT->KeyUp(VK_LBUTTON))
        {
            gun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
            gun->Find("BarrelPoint")->SetLocalPosZ(0.0f);
        }
    }
}

void Gun::GunAim(Player* player)
{
    player->GetPlayerActor()->Find("RightArmPoint")->rotation.x = 0.0f;
    player->GetPlayerActor()->Find("LeftArmPoint")->rotation.x = 0.0f;

    player->GetPlayerActor()->Find("LeftShoulderPoint")->rotation.x = -75.0f * ToRadian;
    player->GetPlayerActor()->Find("RightShoulderPoint")->rotation.x = -75.0f * ToRadian;

    player->GetPlayerActor()->Find("LeftShoulderPoint")->rotation.y = 25.0f * ToRadian;
    player->GetPlayerActor()->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
    gun->rotation.x = 0.0f; // 총이 손을 따라다니게끔
}
