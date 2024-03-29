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

	killmissionUI = UI::Create("killmission");
	killmissionUI->LoadFile("killmissionUI.xml");

	killcountUI1 = UI::Create("killcount1");
	killcountUI1->LoadFile("killcount1.xml");
	killcountUI2 = UI::Create("killcount2");
	killcountUI2->LoadFile("killcount2.xml");


	gameoverUI = UI::Create("gameover");
	gameoverUI->LoadFile("gameoverUI.xml");

	gamewinUI = UI::Create("gamewin");
	gamewinUI->LoadFile("gamewinUI.xml");

	loadingUI = UI::Create("loading");
	loadingUI->LoadFile("loadingUI.xml");

	playerAim->visible = true;
	optionUI->visible = false;
	soundUI->visible = false;
	sensitivityUI->visible = false;
	killmissionUI->visible = true;
	killcountUI1->visible = true;
	killcountUI2->visible = true;
	gameoverUI->visible = false;
	gamewinUI->visible = false;
	optionOpen = false;
	soundOn = true;

	

	CurrentTime = 0.0f;
	zombieSpwanTime = 10.0f;
	mouseSpeed = 0.002f;
	killCount = 0;
	missionKill = 1;
	uikillcount = 0;



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
	uikillcount = 10;
	missionKill = 10
	killCount = 0;
	for (int i = 0; i < 5; i++)
	{
		Monster* mob = new Monster();
		Vector3 randomSpwan = Vector3(RANDOM->Float(-150, 150), 0, RANDOM->Float(-150, 150));
		mob->Init(randomSpwan);
		monster.push_back(mob);
	}
}

void InGameScene::Release()
{

}

void InGameScene::Update()
{
	uikillcount = monster.size();
	missionKill = monster.size();
	//임시 게임오버 확인용 플레이어 사망시
	{
		// 플레이어 사망시 GameOverUI true
		if (player->Die())
		{
			player->GetPlayerActor()->visible = false;
			gameoverUI->visible = true;
		}

		// 몬스터 hp 0일때 몬스터개체 삭제
		monster.erase(std::remove_if(monster.begin(), monster.end(),
			[this](Monster* m)
			{
				if (m->Die())
				{
					killCount++;
					delete m;
					return true;
				}
				else return false;
			}), monster.end());
	}

	//killcount
	{
		// cout<< killCount << endl;
		uikillcount = missionKill - killCount;
		uikillcount = max(0, missionKill - killCount);
		int tensDigit = uikillcount / 10;    // 10의 자리 숫자
		int onesDigit = uikillcount % 10;    // 1의 자리 숫자

		killcountUI1->texture = RESOURCE->textures.Load("num" + std::to_string(tensDigit) + ".png");
		killcountUI2->texture = RESOURCE->textures.Load("num" + std::to_string(onesDigit) + ".png");
	}

	// 미션 달성 승리
	if (uikillcount == 0)
	{
		gamewinUI->visible = true;
	}
	if (uikillcount == 0 and INPUT->KeyDown(VK_F11))
	{
		Init();
		//플레이어 초기화
		{
			player->Init();
		}
		//몬스터 초기화
		{
			monster.clear();
		}
		//맵 초기화
		{
			Map->Init();
		}
		//게임상태 초기화
		{
			gameoverUI->visible = false;
			gamewinUI->visible = false;
		}
		//타이머 초기화
		{
			CurrentTime = 0.0f;
			zombieSpwanTime = 5.0f;
		}
		Ingamethema->Stop("mainthema");
		SCENE->ChangeScene("LobbyScene");
		return;
		//PostQuitMessage(0);
	}


	// 플레이어 사망시 초기화
	if (gameoverUI->visible and INPUT->KeyDown('R'))
	{
		//플레이어 초기화
		{
			player->Init();
			killCount = 0;
			uikillcount = 1;
		}
		//몬스터 초기화
		{
			monster.clear();
		}
		//맵 초기화
		{
			Map->Init();
		}
		//게임상태 초기화
		{
			gameoverUI->visible = false;
			gamewinUI->visible = false;
		}
		//타이머 초기화
		{
			CurrentTime = 0.0f;
			zombieSpwanTime = 10.0f;
		}
	}


	//작업용 카메라 플레이어 3인칭캠 변환
	PlayerCam->SetWorldPos(player->GetPlayerActor()->Find("HeadMesh")->GetWorldPos()); // 플레이어 캠이 플레이어의 머리좌표를 계속 받아오게하기
	Camera::main = PlayerCam;
	{
		////작업용
		//if (INPUT->KeyDown(VK_F5))
		//{
		//	Camera::main = MainCam;
		//}
		////플레이어 1인칭시점
		//if (INPUT->KeyDown(VK_F6))
		//{
		//	Camera::main = PlayerCam;
		//}
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

	//옵션창 안켰을때 인게임씬 업데이트
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
			//메인 카메라 컨트롤
			Camera::ControlMainCam();

			//시간으로 좀비생성
			if (TIMER->GetTick(CurrentTime, zombieSpwanTime))
			{
				for (int i = 0; i < 5; i++)
				{
					Monster* mob = new Monster();
					Vector3 randomSpwan = Vector3(RANDOM->Float(-150, 150), 0, RANDOM->Float(-150, 150));
					mob->Init(randomSpwan);
					monster.push_back(mob);
				}
			}
		}
	}

	//마우스 감도 조절
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
		if (!optionOpen)
		{
			ClientToScreen(App.GetHandle(), &ptMouse);
			SetCursorPos(ptMouse.x, ptMouse.y);
			ShowCursor(false);
		}
		if (optionOpen) ShowCursor(true);
		
		//if (INPUT->KeyPress(VK_F1))
		//{
		//	//중앙값
		//	POINT ptMouse;
		//	ptMouse.x = App.GetHalfWidth();
		//	ptMouse.y = App.GetHalfHeight();
		//	Vector3 Rot;
		//	Rot.x = (INPUT->position.y - ptMouse.y) * mouseSpeed;
		//	Rot.y = (INPUT->position.x - ptMouse.x) * mouseSpeed;
		//	Camera::main->rotation += Rot;
		//	player->PlayerRotationY(Camera::main->rotation);
		//	ClientToScreen(App.GetHandle(), &ptMouse);
		//	SetCursorPos(ptMouse.x, ptMouse.y);
		//}
	}

	//하이어라이키
	{
		ImGui::Begin("Hierarchy");
		MainCam->RenderHierarchy();
		PlayerCam->RenderHierarchy();
		//Map->Hierarchy();
		player->RenderHierarchy();
		/*for (auto monsterPtr : monster)
		{
			monsterPtr->RenderHierarchy();
		}*/
		playerAim->RenderHierarchy();
		optionUI->RenderHierarchy();
		soundUI->RenderHierarchy();
		sensitivityUI->RenderHierarchy();
		gameoverUI->RenderHierarchy();
		gamewinUI->RenderHierarchy();
		killmissionUI->RenderHierarchy();
		killcountUI1->RenderHierarchy();
		killcountUI2->RenderHierarchy();
		loadingUI->RenderHierarchy();
		ImGui::End();
	}


	playerAim->Update();
	optionUI->Update();
	soundUI->Update();
	sensitivityUI->Update();
	killmissionUI->Update();
	killcountUI1->Update();
	killcountUI2->Update();
	gameoverUI->Update();
	gamewinUI->Update();
	loadingUI->Update();
}

void InGameScene::LateUpdate()
{

	if (Map->IsSetHouseDone()) {
		loadingUI->visible = true;
	}
	else {
		loadingUI->visible = false;
	}



	Map->LateUpdate();
	player->CollidePlayerToFloor(Map);											// 플레이어 - 바닥 충돌함수
	player->CollidePlayerToWall(Map->WallCollision(player->GetPlayerActor()));	// 플레이어 - 벽 충돌함수
	player->CollidePlayerToItem(Map->ItemCollision(player->GetPlayerActor()));	// 플레이어 - 아이템 충돌함수
	for (auto monsterPtr : monster)
	{
		if (Map->HouseToMonsterCollision(monsterPtr->GetMonsterActor())) {
			monsterPtr->GoBack();
		}
	}
	for (auto monsterPtr : monster)
	{
		player->CollidePlayerToZombie(monsterPtr);								// 플레이어 - 좀비 충돌함수
		monsterPtr->Chase(player);												// 좀비 - 플레이어 추척함수
	}

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
	playerAim->Render();
	optionUI->Render();
	soundUI->Render();
	sensitivityUI->Render();
	killmissionUI->Render();
	killcountUI1->Render();
	killcountUI2->Render();
	gameoverUI->Render();
	gamewinUI->Render();
	loadingUI->Render();
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


