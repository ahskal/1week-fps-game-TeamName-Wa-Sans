#include "stdafx.h"

ShotGun::ShotGun()
{
	shotGun = Actor::Create();
	shotGun->LoadFile("Remington.xml");

    TriggerSpeed = 10.0f;
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
	shotGun->Update();
}

void ShotGun::Render()
{
	shotGun->Render();
}

void ShotGun::ShotGunControl()
{
    if (bulletCount == 8) //8�� �߻� ��� �� ���������
    {
        shotGun->Find("Trigger")->SetLocalPosZ(1.451f);
        if (INPUT->KeyUp('R'))
        {
            bulletCount = 0;
            shotGun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
        }

        if (INPUT->KeyDown(VK_LBUTTON)) // �Ѿ��� ���� ���¿��� �ݹ�������
        {
            SOUND->Stop("NoBullet");
            SOUND->Play("NoBullet");
        }
    }
    else
    {
        fireTime += DELTA;
        /**
        1.5�ʵǸ� �߻�
        0.4�ʵǸ� �������
        �߻�Ǹ� �ٽ� 0
        */
        if (fireTime >= 0.4f and fireTime < 1.0f)
        {
            if (shotGun->Find("Trigger")->GetLocalPos().z <= 0.85f)
            {
                TriggerSpeed = -TriggerSpeed;
            }

            if (shotGun->Find("Trigger")->GetLocalPos().z > 1.45f)
            {
                shotGun->Find("Trigger")->SetLocalPosZ(1.451f);
            }
            else
                shotGun->Find("Trigger")->MoveLocalPos(-shotGun->Find("Trigger")->GetForward() * TriggerSpeed * DELTA);
        }
        else if (fireTime >= 1.5f)
        {
            if (INPUT->KeyDown(VK_LBUTTON))
            {
                shotGun->Find("TriggerPoint")->rotation.x = 2 * ToRadian;
                shotGun->Find("Trigger")->SetLocalPosZ(1.450f);
                TriggerSpeed = -TriggerSpeed;
                isfire = true;
                isReady = false;
                fireTime = 0.0f;
                bulletCount++;
            }
        }

        if (INPUT->KeyUp(VK_LBUTTON))
        {
            shotGun->Find("TriggerPoint")->rotation.x = -2 * ToRadian;
        }
    }
}
