#include "stdafx.h"
#include "InGameScene.h"
#include "LobbyScene.h"
//각자 작업 영역 header 파일 추가해주시면됩니다!



InGameScene::InGameScene()
{
	PlayerCam = Camera::Create();


	MainCam = Camera::Create();
	MainCam->LoadFile("MainCam.xml");
	Map = VillageMap::Create();
	player = new Player();



	//playerAim = UI::Create();
	optionUI = UI::Create("option");
	optionUI->LoadFile("optionUI.xml");

	soundUI = UI::Create("sound");
	soundUI->LoadFile("soundUI.xml");

	sensitivityUI = UI::Create("sensitivity");
	sensitivityUI->LoadFile("sensitivityUI.xml");

	Ingamethema = new Sound();
	Ingamethema->AddSound("mainthema.mp3","mainthema", true);
	Ingamethema->SetVolume("mainthema",0.2f);
	
	//아직 xml 추가안했음 이미지 작업후 xml 유아이 저장예정
	//playerAim->LoadFile("playerAim.xml");
	
	optionUI->visible = false;
	soundUI->visible = false;
	sensitivityUI->visible = false;
	optionOpen = false;
	soundOn = true;

	CurrentTime = 0.0f;
	zombieSpwanTime = 10.0f;
	mouseSpeed = 0.002f;

	Monster* mob = new Monster();
	Vector3 randomSpwan = Vector3(RANDOM->Float(-100, 100), 0, RANDOM->Float(-100, 100));
	mob->Init(randomSpwan);
	monster.push_back(mob);


	/** 카메라 초기 세팅*/
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
	/** 카메라 초기 세팅*/

	//사운드
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
}

void InGameScene::Release()
{

}

void InGameScene::Update()
{
	//작업용 카메라 플레이어 3인칭캠 변환
	PlayerCam->SetWorldPos(player->GetPlayerActor()->Find("HeadMesh")->GetWorldPos()); // 플레이어 캠이 플레이어의 머리좌표를 계속 받아오게하기
	{
		//작업용
		if (INPUT->KeyDown(VK_F5))
		{
			Camera::main = MainCam;
		}
		//플레이어 1인칭시점
		if (INPUT->KeyDown(VK_F6))
		{
			Camera::main = PlayerCam;
		}
	}

	//옵션창 ui 열기닫기
	{
		//ui 옵션창 열였을때 만보이게해뒀습니다
		if (INPUT->KeyDown(VK_ESCAPE))
		{
			optionOpen = !optionOpen;
			optionUI->visible = !optionUI->visible;
			soundUI->visible = !soundUI->visible;
			sensitivityUI->visible = !sensitivityUI->visible;
		}
	}

	//옵셩창 안켰을때 인게임씬 업데이트
	{
		if (!optionOpen)
		{
			Map->Update();
			player->PlayerControl();
			player->Update();
			for (auto monsterPtr : monster)
			{
				monsterPtr->Update();
			}
			Camera::main->Update();
			//메인 카메라 컨트롤
			Camera::ControlMainCam();
		}

		//마우스 감도 조절
		{
			if (INPUT->KeyPress(VK_F1))
			{
				//중앙값
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
	}

	//하이어라이키
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
		optionUI->RenderHierarchy();
		soundUI->RenderHierarchy();
		sensitivityUI->RenderHierarchy();
		ImGui::End();
	}


	//시간으로 좀비생성
	/*if (TIMER->GetTick(CurrentTime, zombieSpwanTime))
	{
		for (int i = 0; i < 10; i++)
		{
		Zombie* zombie = new Zombie();
		Vector3 randomSpwan = Vector3(RANDOM->Float(-100, 100), 0, RANDOM->Float(-100, 100));
		zombie->Init(randomSpwan);
		zombies.push_back(zombie);
		}
	}*/





	optionUI->Update();
	soundUI->Update();
	sensitivityUI->Update();
}

void InGameScene::LateUpdate()
{
	Map->LateUpdate();
	player->CollidePlayerToFloor(Map);											// 플레이어 - 바닥 충돌함수
	player->CollidePlayerToWall(Map->WallCollision(player->GetPlayerActor()));	// 플레이어 - 벽 충돌함수
	player->CollidePlayerToZombie(false);										// 플레이어 - 좀비 충돌함수

	
	
	//사운드 ui 설정
	{
		if ((soundUI->visible and soundUI->MouseOver() and soundOn))
		{
			if (INPUT->KeyDown(VK_LBUTTON))
			{
				soundOn = false;
				soundUI->texture = RESOURCE->textures.Load("soundoff.png");
				cout << "사운드꺼짐" << endl;
				Ingamethema->Pause("mainthema");
			}
		}
		else if ((soundUI->visible and soundUI->MouseOver() and !soundOn))
		{
			if (INPUT->KeyDown(VK_LBUTTON))
			{
				soundOn = true;
				soundUI->texture = RESOURCE->textures.Load("soundon.png");
				cout << "사운드켜짐" << endl;
				Ingamethema->Resume("mainthema");
			}
		}
	}

	//마우스 감도 설정
	{
		if ((sensitivityUI->visible and sensitivityUI->MouseOver()))
		{
			if (INPUT->KeyDown(VK_F2))
			{
				mouseSpeed += 0.001f;
				cout << "감도 증가" << mouseSpeed << endl;
			}
			if (INPUT->KeyDown(VK_F3))
			{
				mouseSpeed -= 0.001f;
				cout << "감도 감소" << mouseSpeed << endl;
			}
		}
	}

	//마우스감도 최저치 설정
    if (mouseSpeed < 0.001) mouseSpeed = 0.001f;


}

void InGameScene::PreRender()
{

}

void InGameScene::Render()
{
	//객체들 랜더 모아주세요 
	Camera::main->Set();
	Map->Render();
	player->Render();
	for (auto monsterPtr : monster)
	{
		monsterPtr->Render();
	}
	optionUI->Render();
	soundUI->Render();
	sensitivityUI->Render();
}

void InGameScene::ResizeScreen()
{
	//카메라 화면비 조정 
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();

	player->ResizeScreen();
	Map->ResizeScreen();
}
