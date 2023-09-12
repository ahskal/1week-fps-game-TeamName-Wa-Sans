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

	// 참이라면 집을 생성해라
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
	// 참이라면 집의 좌표를 정해줘라
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
	// 집 충돌처리 생성후 계산되어 코드가 비활성화.
	{
		// 만약 집의 충돌처리가 참이라면 연산 계산
		bool disableHouseLateUpdate = true;
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
			// 집이 만약 범위내에 나갔을경우 위치 재정의		
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
	// 집을 그릴때
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
