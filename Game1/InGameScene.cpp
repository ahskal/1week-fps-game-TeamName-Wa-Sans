#include "stdafx.h"
#include "InGameScene.h"
#include "LobbyScene.h"
//���� �۾� ���� header ���� �߰����ֽø�˴ϴ�!



InGameScene::InGameScene()
{
	PlayerCam = Camera::Create();


	MainCam = Camera::Create();
	MainCam->LoadFile("MainCam.xml");
	Map = VillageMap::Create();
	player = new Player();



	playerAim = UI::Create("playeraim");
	playerAim->LoadFile("playerAim.xml");

	optionUI = UI::Create("option");
	optionUI->LoadFile("optionUI.xml");

	soundUI = UI::Create("sound");
	soundUI->LoadFile("soundUI.xml");

	sensitivityUI = UI::Create("sensitivity");
	sensitivityUI->LoadFile("sensitivityUI.xml");

	Ingamethema = new Sound();
	Ingamethema->AddSound("mainthema.mp3", "mainthema", true);
	Ingamethema->SetVolume("mainthema", 0.2f);

	missionUI = UI::Create("mission");
	//missionUI->LoadFile("missionUI.xml");

	gameoverUI = UI::Create("gameover");
	gameoverUI->LoadFile("gameoverUI.xml");

	playerAim->visible = true;
	optionUI->visible = false;
	soundUI->visible = false;
	sensitivityUI->visible = false;
	missionUI->visible = true;
	gameoverUI->visible = false;
	optionOpen = false;
	soundOn = true;

	CurrentTime = 0.0f;
	zombieSpwanTime = 10.0f;
	mouseSpeed = 0.002f;

	/** ī�޶� �ʱ� ����*/
	MainCam->mainCamSpeed = 30.0f;
	MainCam->viewport.x = 0.0f;
	MainCam->viewport.y = 0.0f;
	MainCam->viewport.width = App.GetWidth();
	MainCam->viewport.height = App.GetHeight();
	MainCam->width = App.GetWidth();
	MainCam->height = App.GetHeight();

	PlayerCam->mainCamSpeed = 30.0f;
	PlayerCam->viewport.x = 0.0f;
	PlayerCam->viewport.y = 0.0f;
	PlayerCam->viewport.width = App.GetWidth();
	PlayerCam->viewport.height = App.GetHeight();
	PlayerCam->width = App.GetWidth();
	PlayerCam->height = App.GetHeight();
	Camera::main = MainCam;
	/** ī�޶� �ʱ� ����*/

	//����
}

InGameScene::~InGameScene()
{
	Map->Release();
}

void InGameScene::Init()
{
	Map->Init();
	player->Init();
	Ingamethema->SetVolume("mainthema", 0.2f);
	Ingamethema->Play("mainthema");

	Monster* mob = new Monster();
	Vector3 randomSpwan = Vector3(RANDOM->Float(-150, 150), 0, RANDOM->Float(-150, 150));
	mob->Init(randomSpwan);
	monster.push_back(mob);
}

void InGameScene::Release()
{

}

void InGameScene::Update()
{
	//�ӽ� ���ӿ��� Ȯ�ο� �÷��̾� �����
	{
		// �÷��̾� ����� GameOverUI true
		if (player->Die())
		{
			player->GetPlayerActor()->visible = false;
			gameoverUI->visible = true;
		}

		// ���� hp 0�϶� ���Ͱ�ü ����
		monster.erase(std::remove_if(monster.begin(), monster.end(),
			[](Monster* m)
			{
				if (m->Die())
				{
					delete m;
					return true;
				}
				else return false;
			}), monster.end());
	}

	// �÷��̾� ����� �ʱ�ȭ
	if (gameoverUI->visible and INPUT->KeyDown('R'))
	{
		//�÷��̾� �ʱ�ȭ
		{
			player->Init();
		}
		//���� �ʱ�ȭ
		{
			monster.clear();
		}
		//�� �ʱ�ȭ
		{
			Map->Init();
		}
		//���ӻ��� �ʱ�ȭ
		{
			gameoverUI->visible = false;
		}
		//Ÿ�̸� �ʱ�ȭ
		{
			CurrentTime = 0.0f;
			zombieSpwanTime = 10.0f;
		}
	}


	//�۾��� ī�޶� �÷��̾� 3��Īķ ��ȯ
	PlayerCam->SetWorldPos(player->GetPlayerActor()->Find("HeadMesh")->GetWorldPos()); // �÷��̾� ķ�� �÷��̾��� �Ӹ���ǥ�� ��� �޾ƿ����ϱ�
	{
		//�۾���
		if (INPUT->KeyDown(VK_F5))
		{
			Camera::main = MainCam;
		}
		//�÷��̾� 1��Ī����
		if (INPUT->KeyDown(VK_F6))
		{
			Camera::main = PlayerCam;
		}
	}

	//�ɼ�â ui ����ݱ�
	{
		//ui �ɼ�â �������� �����̰��ص׽��ϴ�
		if (INPUT->KeyDown(VK_ESCAPE))
		{
			optionOpen = !optionOpen;
			optionUI->visible = !optionUI->visible;
			soundUI->visible = !soundUI->visible;
			sensitivityUI->visible = !sensitivityUI->visible;
		}
	}

	//�ɼ�â �������� �ΰ��Ӿ� ������Ʈ
	{
		if (!optionOpen)
		{
			Map->Update();
			playerAim->Update();
			player->PlayerControl();
			player->Update();
			for (auto monsterPtr : monster)
			{
				monsterPtr->Update();
			}
			Camera::main->Update();
			//���� ī�޶� ��Ʈ��
			Camera::ControlMainCam();

			//�ð����� �������
			//if (TIMER->GetTick(CurrentTime, zombieSpwanTime))
			//{
			//	for (int i = 0; i < 5; i++)
			//	{
			//		Monster* mob = new Monster();
			//		Vector3 randomSpwan = Vector3(RANDOM->Float(-150, 150), 0, RANDOM->Float(-150, 150));
			//		mob->Init(randomSpwan);
			//		monster.push_back(mob);
			//	}
			//}
		}
	}

	//���콺 ���� ����
	{
		if (INPUT->KeyPress(VK_F1))
		{
			//�߾Ӱ�
			POINT ptMouse;
			ptMouse.x = App.GetHalfWidth();
			ptMouse.y = App.GetHalfHeight();
			Vector3 Rot;
			Rot.x = (INPUT->position.y - ptMouse.y) * mouseSpeed;
			Rot.y = (INPUT->position.x - ptMouse.x) * mouseSpeed;
			Camera::main->rotation += Rot;
			player->PlayerRotationY(Camera::main->rotation);
			ClientToScreen(App.GetHandle(), &ptMouse);
			SetCursorPos(ptMouse.x, ptMouse.y);
		}
	}

	//���̾����Ű
	{
		ImGui::Begin("Hierarchy");
		MainCam->RenderHierarchy();
		PlayerCam->RenderHierarchy();
		//Map->Hierarchy();
		player->RenderHierarchy();
		for (auto monsterPtr : monster)
		{
			monsterPtr->RenderHierarchy();
		}
		playerAim->RenderHierarchy();
		optionUI->RenderHierarchy();
		soundUI->RenderHierarchy();
		sensitivityUI->RenderHierarchy();
		gameoverUI->RenderHierarchy();
		missionUI->RenderHierarchy();
		ImGui::End();
	}







	playerAim->Update();
	optionUI->Update();
	soundUI->Update();
	sensitivityUI->Update();
	missionUI->Update();
	gameoverUI->Update();
}

void InGameScene::LateUpdate()
{
	//Map->LateUpdate();
	player->CollidePlayerToFloor(Map);											// �÷��̾� - �ٴ� �浹�Լ�
	player->CollidePlayerToWall(Map->WallCollision(player->GetPlayerActor()));	// �÷��̾� - �� �浹�Լ�
	for (auto iter = monster.begin(); iter != monster.end(); iter++)
	{
		player->CollidePlayerToZombie((*iter));										// �÷��̾� - ���� �浹�Լ�
	}

	//���� �÷��̾� ����
	for (auto monsterPtr : monster)
	{
		monsterPtr->Chase(player);
	}

	//���� ui ����
	{
		if ((soundUI->visible and soundUI->MouseOver() and soundOn))
		{
			if (INPUT->KeyDown(VK_LBUTTON))
			{
				soundOn = false;
				soundUI->texture = RESOURCE->textures.Load("soundoff.png");
				cout << "���岨��" << endl;
				Ingamethema->Pause("mainthema");
			}
		}
		else if ((soundUI->visible and soundUI->MouseOver() and !soundOn))
		{
			if (INPUT->KeyDown(VK_LBUTTON))
			{
				soundOn = true;
				soundUI->texture = RESOURCE->textures.Load("soundon.png");
				cout << "��������" << endl;
				Ingamethema->Resume("mainthema");
			}
		}
	}

	//���콺 ���� ����
	{
		if ((sensitivityUI->visible and sensitivityUI->MouseOver()))
		{
			if (INPUT->KeyDown(VK_F2))
			{
				mouseSpeed += 0.001f;
				cout << "���� ����" << mouseSpeed << endl;
			}
			if (INPUT->KeyDown(VK_F3))
			{
				mouseSpeed -= 0.001f;
				cout << "���� ����" << mouseSpeed << endl;
			}
		}
	}

	//���콺���� ����ġ ����
	if (mouseSpeed < 0.001) mouseSpeed = 0.001f;
}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
	//��ü�� ���� ����ּ��� 
	Camera::main->Set();
	Map->Render();
	player->Render();
	for (auto monsterPtr : monster)
	{
		monsterPtr->Render();
	}
	playerAim->Render();
	optionUI->Render();
	soundUI->Render();
	sensitivityUI->Render();
	missionUI->Render();
	gameoverUI->Render();
}

void InGameScene::ResizeScreen()
{
	//ī�޶� ȭ��� ���� 
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();

	player->ResizeScreen();
	Map->ResizeScreen();
}

