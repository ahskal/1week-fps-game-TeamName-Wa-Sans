#include "stdafx.h"

Gun::Gun()
{
	gun = Actor::Create();
	gun->LoadFile("beretta92.xml");

    SOUND->AddSound("beretta92Sound.wav", "beretta92");
    SOUND->AddSound("NoBullet.wav", "NoBullet");
    SOUND->SetVolume("beretta92", 0.1f);
    SOUND->SetVolume("NoBullet", 0.1f);
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
	gun->Update();
}

void Gun::Render()
{
	gun->Render();
}

void Gun::GunControl()
{
    if (bulletCount == 8)
    {
        gun->Find("BarrelPoint")->SetLocalPosZ(-0.35f);
        if (INPUT->KeyUp('R'))
        {
            bulletCount = 0;
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
        if (TIMER->GetTick(fireTime, 1.0f))
        {
            b_fire = true;
        }
        // °ø°Ý½Ã
        if (b_fire)
        {
            if (INPUT->KeyDown(VK_LBUTTON))
            {
                gun->Find("TriggerPoint")->rotation.x = 2 * ToRadian;
                gun->Find("BarrelPoint")->SetLocalPosZ(-0.01f);
                SOUND->Stop("beretta92");
                SOUND->Play("beretta92");
                b_fire = false;
                fireTime = 0.0f;
                bulletCount++;
            }

        }
        if (INPUT->KeyUp(VK_LBUTTON))
        {
            gun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
            gun->Find("BarrelPoint")->SetLocalPosZ(0.0f);
        }
    }
}
