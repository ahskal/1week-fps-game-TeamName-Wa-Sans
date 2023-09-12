#include "stdafx.h"
#include "house.h"
#include "VillageMap.h"

VillageMap* VillageMap::Create(string name)
{
	VillageMap* temp = new VillageMap();
	temp->LoadFile("VillageMap.xml");
	temp->type = ObType::Actor;
	temp->name = name;
	return temp;
}

VillageMap::VillageMap()
{
	cam = Camera::Create();
	cam->LoadFile("VillageCam.xml");

	// �׸��� ���� ���ϴ� ����
	HouseRender = true;
	if (not HouseRender) HouseLateUpdate = false;

	// ���̶�� ���� �����ض�
	if (HouseRender) {
		for (int i = 0; i < HouseCount; i++) {
			house[i] = House::Create();
		}
	}
	house2 = House::Create();
	house2->LoadFile("Hospital.xml");
}

VillageMap::~VillageMap()
{

}

void VillageMap::Release()
{
	cam->SaveFile("VillageCam.xml");
}

void VillageMap::Init()
{
	// ���̶�� ���� ��ǥ�� �������
	if (HouseRender) {
		HouseLateUpdate = true;
		for (int i = 0; i < HouseCount; i++) {
			if (i < 1) {
				house[i]->LoadFile("GunShop.xml");
			}
			else if (i < 2) {
				house[i]->LoadFile("Hospital.xml");
			}
			else {
				switch (RANDOM->Int(0, 5)) {
				case 0: house[i]->LoadFile("VillageHouse.xml");  break;
				case 1: house[i]->LoadFile("VillageHouse2.xml"); break;
				case 2: house[i]->LoadFile("VillageHouse3.xml"); break;
				case 3: house[i]->LoadFile("VillageHouse4.xml"); break;
				case 4: house[i]->LoadFile("Hospital.xml");      break;
				case 5: house[i]->LoadFile("GunShop.xml");		 break;
				}
				house[i]->SetWorldPosX(RANDOM->Int(-180, 180));
				house[i]->SetWorldPosZ(RANDOM->Int(-180, 180));
			}
			house[i]->rotation.y = RANDOM->Float(-PI, PI);
		}
	}

}

void VillageMap::Update()
{
	Actor::Update();

	if (INPUT->KeyDown('P')) {
		Init();
	}
	if (HouseRender)
		for (int i = 0; i < HouseCount; i++) {
			house[i]->Update();
		}
	house2->Update();

}

void VillageMap::LateUpdate()
{
	// �� �浹ó�� ������ ���Ǿ� �ڵ尡 ��Ȱ��ȭ.
	{
		if (HouseLateUpdate) {
			// ���� ���� ����������� �о�� �ڵ�
			for (int i = 0; i < HouseCount - 1; i++)
			{
				for (int j = 0; j < HouseCount; j++)
				{
					if (house[i] == house[j]) continue;
					// ���� ���� �浹������� �о��.
					if (house[i]->Intersect(house[j]))
					{
						// �浹�� �������� ������� ���ϱ�
						Vector3 enemyDir = house[i]->GetWorldPos() - house[j]->GetWorldPos();
						Vector3 OtherEnemyDir = house[j]->GetWorldPos() - house[i]->GetWorldPos();

						// ������� ��Į�� ����
						enemyDir.Normalize();
						OtherEnemyDir.Normalize();

						// ���� ��ǥ�� ���� �о��ֱ�
						if (house[i]->GetWorldPos().x > -250 && house[i]->GetWorldPos().x < 250) {
							house[i]->MoveWorldPos(enemyDir * 10000 * DELTA);
						}
						else if (house[j]->GetWorldPos().x > -250 && house[j]->GetWorldPos().x < 250) {
							house[j]->MoveWorldPos(OtherEnemyDir * 10000 * DELTA);
						}
					}
				}
			}
			// ���� ���� �������� ��������� ��ġ ������		
			for (int i = 0; i < HouseCount; i++)
			{
				if (house[i]->GetWorldPos().x > 250 or house[i]->GetWorldPos().x < -250) {
					house[i]->SetWorldPosX(RANDOM->Int(-150, 150));
				}
				else if (house[i]->GetWorldPos().z > 250 or house[i]->GetWorldPos().z < -250) {
					house[i]->SetWorldPosZ(RANDOM->Int(-150, 150));
				}
			}
		}
	}
}

void VillageMap::Render()
{
	Actor::Render();
	// ���� �׸���
	if (HouseRender)
		for (int i = 0; i < HouseCount; i++) {
			house[i]->Render();
		}
	//house2->Render();

}



void VillageMap::Hierarchy()
{
	cam->RenderHierarchy();
	RenderHierarchy();
}

void VillageMap::ResizeScreen()
{
	cam->width = App.GetWidth();;
	cam->height = App.GetHeight();
	cam->viewport.x = 0;
	cam->viewport.y = 0;
	cam->viewport.width = App.GetWidth();
	cam->viewport.height = App.GetHeight();
}
