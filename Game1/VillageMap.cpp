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

	// 그릴지 말지 정하는 변수
	HouseRender = true;
	if (not HouseRender) HouseLateUpdate = false;

	// 참이라면 집을 생성해라
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
	// 참이라면 집의 좌표를 정해줘라
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
				house[i]->SetWorldPosX(RANDOM->Int(-Range, Range));
				house[i]->SetWorldPosZ(RANDOM->Int(-Range, Range));
			}
			house[i]->rotation.y = RANDOM->Float(-PI, PI);
			house[i]->scale *= 0.5f;
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
	// 집 충돌처리 생성후 계산되어 코드가 비활성화.
	{
		if (HouseLateUpdate) {
			// 집과 집이 겹쳐있을경우 밀어내는 코드
			for (int i = 0; i < HouseCount - 1; i++)
			{
				for (int j = 0; j < HouseCount; j++)
				{
					if (house[i] == house[j]) continue;
					// 집과 집이 충돌했을경우 밀어낸다.
					if (house[i]->Intersect(house[j]))
					{
						// 충돌한 집끼리의 방향백터 구하기
						Vector3 enemyDir = house[i]->GetWorldPos() - house[j]->GetWorldPos();
						Vector3 OtherEnemyDir = house[j]->GetWorldPos() - house[i]->GetWorldPos();

						// 방향백터 스칼라값 빼기
						enemyDir.Normalize();
						OtherEnemyDir.Normalize();

						// 집의 좌표에 따라 밀어주기
						if (house[i]->GetWorldPos().x > -RangeLimit && house[i]->GetWorldPos().x < RangeLimit) {
							house[i]->MoveWorldPos(enemyDir * 10000 * DELTA);
						}
						else if (house[j]->GetWorldPos().x > -RangeLimit && house[j]->GetWorldPos().x < RangeLimit) {
							house[j]->MoveWorldPos(OtherEnemyDir * 10000 * DELTA);
						}
					}
				}
			}
			// 집이 만약 범위내에 나갔을경우 위치 재정의		
			for (int i = 0; i < HouseCount; i++)
			{
				if (house[i]->GetWorldPos().x > RangeLimit or house[i]->GetWorldPos().x < -RangeLimit) {
					house[i]->SetWorldPosX(RANDOM->Int(-Range, Range));
				}
				else if (house[i]->GetWorldPos().z > RangeLimit or house[i]->GetWorldPos().z < -RangeLimit) {
					house[i]->SetWorldPosZ(RANDOM->Int(-Range, Range));
				}
			}
		}
	}
}

void VillageMap::Render()
{
	Actor::Render();
	// 집을 그릴때
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
