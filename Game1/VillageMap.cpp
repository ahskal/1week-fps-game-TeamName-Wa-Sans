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

	// ���̶�� ���� �����ض�
	if (HouseRender) {
		for (int i = 0; i < HouseCount; i++) {
			house[i] = House::Create();
		}
	}


}

VillageMap::~VillageMap()
{

}

void VillageMap::Release()
{
	cam->SaveFile("VillageCam.xml");
	SaveFile("VillageMap.xml");
}

void VillageMap::Init()
{
	// ���̶�� ���� ��ǥ�� �������
	if (HouseRender)
		HouseLateUpdate = true;
	for (int i = 0; i < HouseCount; i++) {
		switch (RANDOM->Int(0, 3)) {
		case 0: house[i]->LoadFile("VillageHouse.xml"); break;
		case 1: house[i]->LoadFile("VillageHouse2.xml"); break;
		case 2: house[i]->LoadFile("VillageHouse3.xml"); break;
		case 3: house[i]->LoadFile("VillageHouse4.xml"); break;
		}
		house[i]->SetWorldPosX(RANDOM->Int(-100, 100));
		house[i]->SetWorldPosZ(RANDOM->Int(-100, 100));
		house[i]->rotation.y = RANDOM->Float(-PI, PI);
	}

}

void VillageMap::Update()
{
	Actor::Update();

	if (INPUT->KeyDown('P')) {
		Init();
	}

	for (int i = 0; i < HouseCount; i++) {
		house[i]->Update();
	}
}

void VillageMap::LateUpdate()
{
	// �� �浹ó�� ������ ���Ǿ� �ڵ尡 ��Ȱ��ȭ.
	{
		// ���� ���� �浹ó���� ���̶�� ���� ���
		bool disableHouseLateUpdate = true;
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
						if (house[i]->GetWorldPos().x > -230 && house[i]->GetWorldPos().x < 230) {
							house[i]->MoveWorldPos(enemyDir * 5000 * DELTA);
						}
						else if (house[j]->GetWorldPos().x > -230 && house[j]->GetWorldPos().x < 230) {
							house[j]->MoveWorldPos(OtherEnemyDir * 5000 * DELTA);
						}
						disableHouseLateUpdate = false;
					}
				}
			}
			// ���� ���� �������� ��������� ��ġ ������		
			for (int i = 0; i < HouseCount; i++)
			{
				if (house[i]->GetWorldPos().x > 200 or house[i]->GetWorldPos().x < -200) {
					house[i]->SetWorldPosX(RANDOM->Int(-100, 100));
				}
				else if (house[i]->GetWorldPos().z > 200 or house[i]->GetWorldPos().z < -200) {
					house[i]->SetWorldPosZ(RANDOM->Int(-100, 100));
				}
			}
		}
		if (disableHouseLateUpdate) {
			HouseLateUpdate = false;
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
