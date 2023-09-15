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
				house[i]->SetWorldPosX(RANDOM->Int(-Range, Range));
				house[i]->SetWorldPosZ(RANDOM->Int(-Range, Range));
			}
			house[i]->rotation.y = RANDOM->Float(-PI, PI);
			house[i]->scale *= 0.5f;
			house[i]->SetWorldPosY(-1);

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

	if (TIMER->GetTick(timer, 1.0f)) {

		Actor* temp = Actor::Create();
		temp->name = "Item" + to_string(nameIdx); nameIdx++;
		temp->mesh = RESOURCE->meshes.Load("6.Cube.mesh");
		temp->shader = RESOURCE->shaders.Load("6.Cube.hlsl");
		temp->texture = RESOURCE->textures.Load("/Map/white.png");
		temp->color = Color(1, 1, 0);
		temp->scale *= 2;
		temp->SetWorldPosX(RANDOM->Int(-Range, Range));
		temp->SetWorldPosZ(RANDOM->Int(-Range, Range));

		Item.push_back(temp);
	}


	for (auto ptr : Item) {
		ptr->Update();
	}



}

void VillageMap::LateUpdate()
{
	// �� �浹ó�� ������ ���Ǿ� �ڵ尡 ��Ȱ��ȭ.
	HouseCollision();
}

void VillageMap::HouseCollision()
{
	if (!HouseLateUpdate) return;

	// ���� ���� �������� ��������� ��ġ ������		
	for (int i = 0; i < HouseCount; i++)
	{
		if (house[i]->GetWorldPos().x > RangeLimit or house[i]->GetWorldPos().x < -RangeLimit) {
			house[i]->SetWorldPosX(RANDOM->Int(-Range, Range));
		}
		else if (house[i]->GetWorldPos().z > RangeLimit or house[i]->GetWorldPos().z < -RangeLimit) {
			house[i]->SetWorldPosZ(RANDOM->Int(-Range, Range));
		}
	}
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
				if (house[i]->GetWorldPos().x > -RangeLimit && house[i]->GetWorldPos().x < RangeLimit) {
					house[i]->MoveWorldPos(enemyDir * 10000 * DELTA);
				}
				else if (house[j]->GetWorldPos().x > -RangeLimit && house[j]->GetWorldPos().x < RangeLimit) {
					house[j]->MoveWorldPos(OtherEnemyDir * 10000 * DELTA);
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
	for (auto ptr : Item) {
		ptr->Render();
	}
	//house2->Render();

}



void VillageMap::Hierarchy()
{
	cam->RenderHierarchy();
	RenderHierarchy();
	for (int i = 0; i < HouseCount; i++) {
		house[i]->RenderHierarchy();
	}
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

void VillageMap::WallCollision(Actor* player)
{
	for (int i = 0; i < HouseCount; i++) {
		for (int j = 0; j < house[i]->Find("Pillar")->children.size(); j++) {
			string str = "Pillar_Mesh_" + to_string(j + 1);
			//cout << str << " " << "�浹" << endl;

			if (house[i]->Find(str)->Intersect(player))
			{
				cout << str << " " << "�浹" << endl;
			}
		}

		for (int j = 0; j < house[i]->Find("Wall")->children.size(); j++) {
			string str = "Wall_" + to_string(j + 1);
			for (int k = 0; k < house[i]->Find(str)->children.size(); k++) {
				string str2 = str + "_Mesh_" + to_string(k + 1);

				if (house[i]->Find(str2)->Intersect(player))
				{
					cout << str2 << " " << "�浹" << endl;
				}
			}
		}
	}
}


