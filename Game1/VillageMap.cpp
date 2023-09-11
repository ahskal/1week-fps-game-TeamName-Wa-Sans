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

	//for (int i = 0; i < 4; i++) {
	//	house[i] = House::Create();
	//}

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
	for (int i = 0; i < HouseCount; i++) {
		house[i] = House::Create();
		house[i]->LoadFile("VillageHouse.xml");
		
		//switch (RANDOM->Int(0, 3)) {
		//case 0:  break;
		//case 1: house[i]->LoadFile("VillageHouse2.xml"); break;
		//case 2: house[i]->LoadFile("VillageHouse3.xml"); break;
		//case 3: house[i]->LoadFile("VillageHouse4.xml"); break;
		//}
		house[i]->SetWorldPosX(RANDOM->Int(-100, 100));
		house[i]->SetWorldPosZ(RANDOM->Int(-100, 100));
		house[i]->rotation.y = RANDOM->Float(-PI, PI);
	}
	//for (int i = 0; i < HouseCount; i++) {
	//	house[i]->collider->visible = false;
	//}

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
	for (int i = 0; i < HouseCount - 1; i++)
	{
		for (int j = i + 1; j < HouseCount; j++)
		{
			// 몬스터가 다른 몬스터와 충돌하면 밀어냄
			if (house[i]->Intersect(house[j]))
			{
				Vector3 enemyDir = house[i]->GetWorldPos() - house[j]->GetWorldPos();
				Vector3 OtherEnemyDir = house[j]->GetWorldPos() - house[i]->GetWorldPos();

				enemyDir.Normalize();
				OtherEnemyDir.Normalize();
				cout << " 충돌 " << endl;

				if (house[i]->GetWorldPos().x > -230 && house[i]->GetWorldPos().x < 230) {
					house[i]->MoveWorldPos(enemyDir * 5000 * DELTA);
				}
				
				if (house[j]->GetWorldPos().x > -230 && house[j]->GetWorldPos().x < 230) {
					house[j]->MoveWorldPos(OtherEnemyDir * 5000 * DELTA);
				}
			}
		}
	}

	for (int i = 0; i < HouseCount; i++)
	{
		if (house[i]->GetWorldPos().x > 200 or house[i]->GetWorldPos().x < -200) {
			house[i]->SetWorldPosX(RANDOM->Int(-100, 100));
		}
		if (house[i]->GetWorldPos().z > 200 or house[i]->GetWorldPos().z < -200) {
			house[i]->SetWorldPosZ(RANDOM->Int(-100,100));
		}
	}

}

void VillageMap::Render()
{
	Actor::Render();
	for (int i = 0; i < HouseCount; i++) {
		house[i]->Render();
	}


	if (!HouseCreateChack) { collisionDetected = true; }

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
