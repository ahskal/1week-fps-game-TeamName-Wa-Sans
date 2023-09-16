#include "stdafx.h"

Player::Player()
{
	// 플레이어 본체
	player = Actor::Create();
	player->LoadFile("Player.xml");


	gun = new Gun();
	gun->GetGun()->rotation.x = 90.0f * ToRadian;


	shotGun = new ShotGun();

	unitType = UnitType::PLAYER;

	hp = 100.0f;
	damage = 10.0f;
	speed = 5.0f;
}

Player::~Player()
{
}

void Player::Init()
{
	player->root->SetWorldPosY(0.1f);

	waitHandDir = 150.0f;
	walkHandDir = 100.0f;
	walkLegDir = 100.0f;
}

void Player::RenderHierarchy()
{
	player->RenderHierarchy();
	gun->RenderHierarchy();
	shotGun->RenderHierarchy();
}

void Player::Update()
{
	ImGui::Text("Player HP : %d", (int)hp);
	ImGui::SameLine();
	ImGui::Text("\tPlayer Damage : %d", (int)damage);


	gun->GetGun()->SetWorldPos(player->Find("RightHandMesh")->GetWorldPos());
	shotGun->GetShotGun()->SetWorldPos(player->Find("RightHandMesh")->GetWorldPos());

	lastPos = player->GetWorldPos();

	/**
	1번 권총, 2번 샷건, 3번 근접무기, 4번 투척무기, 
	5번 붕대, 6번 에너지드링크, 7번 진통제*/
	if (INPUT->KeyDown('1'))
	{
		gunType = GunType::None;
		playerType = PlayerType::None;
	}
	if (INPUT->KeyDown('2'))
	{
		gunType = GunType::Gun;
		playerType = PlayerType::None;
	}
	if (INPUT->KeyDown('3'))
	{
		gunType = GunType::ShotGun;
		playerType = PlayerType::None;
	}

	switch (gunType)
	{
	case GunType::None:
		gun->GetGun()->visible = false;
		shotGun->GetShotGun()->visible = false;
		break;
	case GunType::Gun:
		gun->GetGun()->visible = true;
		shotGun->GetShotGun()->visible = false;
		break;
	case GunType::ShotGun:
		gun->GetGun()->visible = false;
		shotGun->GetShotGun()->visible = true;
		break;
	default:
		break;
	}

	player->Update();
	gun->Update();
	shotGun->Update();
}

void Player::Render()
{
	player->Render();
	gun->Render();
	shotGun->Render();
}

void Player::ResizeScreen()
{
}

void Player::PlayerControl()
{
	// 가만히 있을때 
	if (playerType == PlayerType::None)
	{
		MotionPlayerWait(gunType);

		if (INPUT->KeyPress('W') or INPUT->KeyPress('S') or INPUT->KeyPress('A') or INPUT->KeyPress('D'))
		{
			player->Find("UpPoint")->rotation.y = 0.0f * ToRadian;
			if (gunType == GunType::ShotGun)
			{
				shotGun->GetShotGun()->rotation.y = -80.0f * ToRadian;

				player->Find("RightShoulderPoint")->rotation.x = 10.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.z = 0.0f * ToRadian;

				player->Find("RightArmPoint")->rotation.x = -108.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("LeftShoulderPoint")->rotation.x = -7.0f * ToRadian;
				player->Find("LeftArmPoint")->rotation.x = -100.0f * ToRadian;
			}


			playerType = PlayerType::Walk;
		}
	}
	// 걸어다닐때 (무기x / 권총 / 샷건 / ... )
	else if (playerType == PlayerType::Walk)
	{
		// 속도 제어
		speed = 5.0f;

		// 플레이어 이동 구현함수 (총을 들고있는지 아닌지에 따라 모션 변경)
		MotionPlayerWalk(gunType);

		// 걷기 -> 뛰기(쉬프트 누르면서 S를 누르지 않았을 때)
		if (INPUT->KeyPress(VK_SHIFT) and not INPUT->KeyPress('S'))
		{
			playerType = PlayerType::Run;
			walkLegDir = 150.0f;
			walkHandDir = 150.0f;

			if (gunType == GunType::None or gunType == GunType::Gun)
			{
				player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
				player->Find("RightShoulderPoint")->rotation.y = 0.0f;

				player->Find("LeftShoulderPoint")->rotation.x = 0.0f;
				player->Find("RightShoulderPoint")->rotation.x = 0.0f;
			}
			return;
		}

		// 걷기 -> 서있기 (방향키를 누르지 않았을때)
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or 
			INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}
	// 뛰어다닐때 (무기x / 권총 / 샷건 / ... )
	else if (playerType == PlayerType::Run)
	{
		// 속도 제어
		speed = 10.0f;

		// 플레이어 이동 구현함수 (총을 들고있는지 아닌지에 따라 모션 변경)
		MotionPlayerRun(gunType);

		// 뛰기 -> 걷기 (쉬프트를 뗐을 때)
		if (INPUT->KeyUp(VK_SHIFT))
		{
			if (gunType == GunType::None or gunType == GunType::Gun)
			{
				player->Find("RightArmPoint")->rotation.x = 0.0f;
				player->Find("LeftArmPoint")->rotation.x = 0.0f;
				gun->GetGun()->rotation.x = 90.0f * ToRadian;		// 총이 손을 따라다니게끔

				player->Find("RightShoulderPoint")->rotation.z = 0.0f;
				player->Find("RightShoulderPoint")->rotation.x = 0.0f;
				player->Find("LeftShoulderPoint")->rotation.x = 0.0f;

				player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
				player->Find("RightShoulderPoint")->rotation.y = 0.0f;
				walkHandDir = 100.0f;
			}

			playerType = PlayerType::Walk;
			walkLegDir = 100.0f;
			walkHandDir = 100.0f;

			return;
		}

		// 뛰기 -> 가만히 있기 (방향키를 누르지 않았을때)
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}



	/** 무기 상태값(총알 발사 함수) */
	if (gunType == GunType::Gun)
	{
		gun->GunControl();
	}
	else if (gunType == GunType::ShotGun)
	{
		shotGun->ShotGunControl();
	}
	/** 무기 상태값*/

	/** 점프*/
	if (isGridCollide)
	{
		gravity = 0.0f;
		jumpCount = 0; // 한번의 점프만 가능하게 하기위한
	}
	else
	{
		player->MoveWorldPos(player->GetUp()* gravity* DELTA);
		gravity -= 20.0f * DELTA;
	}

	if (INPUT->KeyDown(VK_SPACE) and jumpCount == 0)
	{
		gravity = 10.0f;
		player->MoveWorldPos(player->GetUp() * gravity * DELTA);
		player->Update();
		jumpCount++;
	}
	/** 점프*/
}

void Player::PlayerRotationY(Vector3 Rot)
{
	player->rotation.y = Rot.y;
	player->Find("UpPoint")->rotation.x = Rot.x;
}

void Player::CollidePlayerToFloor(VillageMap* map)
{
	if (player->Intersect(map))
		isGridCollide = true;
	else
		isGridCollide = false;
}

void Player::CollidePlayerToWall(bool isCollide)
{
	if (isCollide)
	{
		player->SetWorldPosX(lastPos.x);
		player->SetWorldPosZ(lastPos.z);
		player->Update();
	}
}

void Player::CollidePlayerToZombie(Monster* monster)
{
	Ray Up;
	Up = Utility::MouseToRay();
	Vector3 Hit;
	if (monster->GetMonsterActor()->Intersect(Up, Hit))
	{
		if (INPUT->KeyDown(VK_LBUTTON))
		{ 
			Attack(monster);
			cout << "적에게 총알 발사" << endl;
		}
	}
}

void Player::MotionPlayerWait(GunType type)
{
	if (type == GunType::None or type == GunType::Gun)
	{
		// 상체 원상복구하기
		player->Find("UpPoint")->rotation.y = 0.0f * ToRadian;

		//팔의 각도를 원상복구하기
		player->Find("RightArmPoint")->rotation.x = 0.0f;			// 오른팔 초기화
		player->Find("LeftArmPoint")->rotation.x = 0.0f;			// 왼팔 초기화
		gun->GetGun()->rotation.x = 90.0f * ToRadian;				// 총이 손을 따라다니게끔
		
		player->Find("RightShoulderPoint")->rotation.z = 0.0f;		// 오른쪽 어깨 초기화
		player->Find("RightShoulderPoint")->rotation.y = 0.0f;		// 오른쪽 어깨 초기화
		player->Find("RightShoulderPoint")->rotation.x = 0.0f;		// 오른쪽 어깨 초기화
		
		player->Find("LeftShoulderPoint")->rotation.x = 0.0f;		// 왼쪽 어깨 초기화
		player->Find("LeftShoulderPoint")->rotation.y = 0.0f;		// 왼쪽 어깨 초기화
		walkHandDir = 100.0f;
		// 팔의 각도를 원상복구하기

		// 발의 각도 원상복구
		player->Find("RightKneePoint")->rotation.x = 0.0f;
		player->Find("LeftKneePoint")->rotation.x = 0.0f;

		player->Find("RightLegPoint")->rotation.x = 0.0f;
		player->Find("LeftLegPoint")->rotation.x = 0.0f;
		walkLegDir = 100.0f;
		// 발의 각도 원상복구
	}
	else if (type == GunType::ShotGun)
	{
		shotGun->GetShotGun()->rotation.y = 0.0f * ToRadian;

		player->Find("UpPoint")->rotation.y = 36.0f * ToRadian;

		player->Find("RightShoulderPoint")->rotation.x = 0.0f * ToRadian;
		player->Find("RightShoulderPoint")->rotation.y = -61.0f * ToRadian;
		player->Find("RightShoulderPoint")->rotation.z = 50.0f * ToRadian;

		player->Find("RightArmPoint")->rotation.x = -123.0f * ToRadian;

		if (shotGun->GetIsLoad())
		{
			if (player->Find("LeftArmPoint")->rotation.z < 0.0f * ToRadian)
			{
				player->Find("LeftArmPoint")->rotation.z = -0.1f * ToRadian;
			}
			else
			{
				player->Find("LeftArmPoint")->rotation.z += waitHandDir * ToRadian * DELTA;
			}
			if (player->Find("LeftArmPoint")->rotation.z > 15.0f * ToRadian)
			{
				waitHandDir = -waitHandDir;
			}
			//if (INPUT->KeyDown(VK_LBUTTON))
			//{
			//	waitHandDir = 150.0f;
			//}
		}
		else
		{
			waitHandDir = 150.0f;
			player->Find("LeftArmPoint")->rotation.z = 0.001f;

		}

		player->Find("LeftShoulderPoint")->rotation.x = -90.0f * ToRadian;
		player->Find("LeftShoulderPoint")->rotation.y = 0.0f * ToRadian;

		player->Find("LeftArmPoint")->rotation.x = 0.0f * ToRadian;
	}
}

void Player::MotionPlayerWalk(GunType type)
{
	//MotionInit();

	if (INPUT->KeyPress('W'))
	{
		player->MoveWorldPos(player->GetForward() * speed * DELTA);

		// 팔 모션
		if (gunType == GunType::Gun)
		{
			player->Find("RightArmPoint")->rotation.x = -45.0f;
			player->Find("LeftArmPoint")->rotation.x = 0.0f;

			player->Find("LeftShoulderPoint")->rotation.x = -45.0f * ToRadian;
			player->Find("RightShoulderPoint")->rotation.x = -14.0f * ToRadian;

			player->Find("LeftShoulderPoint")->rotation.y = 47.0f * ToRadian;
			player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
			gun->GetGun()->rotation.x = 45.0f; // 총이 손을 따라다니게끔
		}
		else if (gunType == GunType::None)
		{
			// 팔꿈치 각도를 -40도 해서 팔이 접힌 상태로 만들기
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // 총이 손을 따라다니게끔

			// 어깨 x각도를 -20 ~ 20도로 돌려서 걸을때 팔이 움직이게 보이기
			player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA;
			player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
			if (player->Find("RightShoulderPoint")->rotation.x < -20.0f * ToRadian or
				player->Find("RightShoulderPoint")->rotation.x > 20.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}
		else if (gunType == GunType::ShotGun)
		{
			player->Find("LeftShoulderPoint")->rotation.y += walkHandDir * 0.3f * ToRadian * DELTA;
			player->Find("RightShoulderPoint")->rotation.z += walkHandDir * 0.3f * ToRadian * DELTA;
			if (player->Find("RightShoulderPoint")->rotation.z < 0.0f * ToRadian or
				player->Find("RightShoulderPoint")->rotation.z > 10.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}

		// 발 모션
		{
			// 무릎 각도를 25도 해서 팔이 접힌 상태로 만들기
			player->Find("RightKneePoint")->rotation.x = 25.0f * ToRadian;
			player->Find("LeftKneePoint")->rotation.x = 25.0f * ToRadian;

			// 발 x각도를 -20 ~ 20도로 돌려서 걸을때 발이 움직이게 보이기
			player->Find("RightLegPoint")->rotation.x -= walkLegDir * ToRadian * DELTA;
			player->Find("LeftLegPoint")->rotation.x += walkLegDir * ToRadian * DELTA;
			if (player->Find("RightLegPoint")->rotation.x < -20.0f * ToRadian or
				player->Find("RightLegPoint")->rotation.x > 20.0f * ToRadian)
			{
				walkLegDir = -walkLegDir;
			}
		}
	}
	else if (INPUT->KeyPress('S'))
	{
		player->MoveWorldPos(-player->GetForward() * speed * DELTA);

		if (gunType == GunType::Gun)
		{
			player->Find("LeftShoulderPoint")->rotation.x = -75.0f * ToRadian;
			player->Find("RightShoulderPoint")->rotation.x = -75.0f * ToRadian;

			player->Find("LeftShoulderPoint")->rotation.y = 25.0f * ToRadian;
			player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
			gun->GetGun()->rotation.x = 0.0f; // 총이 손을 따라다니게끔
		}
		else if (gunType == GunType::None)
		{
			// 팔꿈치 각도를 -40도 해서 팔이 접힌 상태로 만들기
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // 총이 손을 따라다니게끔

			// 어깨 x각도를 -20 ~ 20도로 돌려서 걸을때 팔이 움직이게 보이기
			player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
			player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA;
			if (player->Find("LeftShoulderPoint")->rotation.x < -20.0f * ToRadian or
				player->Find("LeftShoulderPoint")->rotation.x > 20.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}
		else if (gunType == GunType::ShotGun)
		{
			player->Find("LeftShoulderPoint")->rotation.y += walkHandDir * 0.3f * ToRadian * DELTA;
			player->Find("RightShoulderPoint")->rotation.z += walkHandDir * 0.3f * ToRadian * DELTA;
			if (player->Find("RightShoulderPoint")->rotation.z < 0.0f * ToRadian or
				player->Find("RightShoulderPoint")->rotation.z > 10.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}


		// 무릎 각도를 25도 해서 팔이 접힌 상태로 만들기
		player->Find("RightKneePoint")->rotation.x = 25.0f * ToRadian;
		player->Find("LeftKneePoint")->rotation.x = 25.0f * ToRadian;

		// 발 x각도를 -20 ~ 20도로 돌려서 걸을때 발이 움직이게 보이기
		player->Find("RightLegPoint")->rotation.x += walkLegDir * ToRadian * DELTA;
		player->Find("LeftLegPoint")->rotation.x -= walkLegDir * ToRadian * DELTA;
		if (player->Find("RightLegPoint")->rotation.x < -20.0f * ToRadian or
			player->Find("RightLegPoint")->rotation.x > 20.0f * ToRadian)
		{
			walkLegDir = -walkLegDir;
		}
	}
	if (INPUT->KeyPress('A'))
	{
		player->MoveWorldPos(-player->GetRight() * speed * DELTA);
	}
	else if (INPUT->KeyPress('D'))
	{
		player->MoveWorldPos(player->GetRight() * speed * DELTA);
	}
}

void Player::MotionPlayerRun(GunType type)
{
	//MotionInit();

	if (INPUT->KeyPress('W'))
	{
		player->MoveWorldPos(player->GetForward() * speed * DELTA);

		if (gunType == GunType::Gun)
		{
			player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
			player->Find("RightShoulderPoint")->rotation.y = 0.0f;
			player->Find("RightShoulderPoint")->rotation.z = 7.0f * ToRadian;

			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
			if (player->Find("LeftShoulderPoint")->rotation.x < -25.0f * ToRadian or
				player->Find("LeftShoulderPoint")->rotation.x > 25.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}

			player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA * 0.2f;



			player->Find("RightArmPoint")->rotation.x = 0.0f;
			gun->GetGun()->rotation.x = 90.0f * ToRadian;		// 총이 손을 따라다니게끔

		}
		else if (gunType == GunType::None)
		{
			// 팔꿈치 각도를 -40도 해서 팔이 접힌 상태로 만들기
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // 총이 손을 따라다니게끔

			// 어깨 x각도를 -20 ~ 20도로 돌려서 걸을때 팔이 움직이게 보이기
			player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA;
			player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
			if (player->Find("RightShoulderPoint")->rotation.x < -25.0f * ToRadian or
				player->Find("RightShoulderPoint")->rotation.x > 25.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}
		else if (gunType == GunType::ShotGun)
		{
			player->Find("LeftShoulderPoint")->rotation.y += walkHandDir * 0.5f * ToRadian * DELTA;
			player->Find("RightShoulderPoint")->rotation.z += walkHandDir * 0.5f * ToRadian * DELTA;
			if (player->Find("RightShoulderPoint")->rotation.z < 0.0f * ToRadian or
				player->Find("RightShoulderPoint")->rotation.z > 15.0f * ToRadian)
			{
				walkHandDir = -walkHandDir;
			}
		}




		// 무릎 각도를 25도 해서 팔이 접힌 상태로 만들기
		player->Find("RightKneePoint")->rotation.x = 30.0f * ToRadian;
		player->Find("LeftKneePoint")->rotation.x = 30.0f * ToRadian;

		// 발 x각도를 -20 ~ 20도로 돌려서 걸을때 발이 움직이게 보이기
		player->Find("RightLegPoint")->rotation.x -= walkLegDir * ToRadian * DELTA;
		player->Find("LeftLegPoint")->rotation.x += walkLegDir * ToRadian * DELTA;
		if (player->Find("RightLegPoint")->rotation.x < -25.0f * ToRadian or
			player->Find("RightLegPoint")->rotation.x > 25.0f * ToRadian)
		{
			walkLegDir = -walkLegDir;
		}
	}
	// 뛰기 -> 걷기 (쉬프르틀 누르고 있는 상태일때 S를 눌렀을 때)
	if (INPUT->KeyPress(VK_SHIFT) and INPUT->KeyPress('S'))
	{
		if (gunType == GunType::None or gunType == GunType::Gun)
		{
			player->Find("RightArmPoint")->rotation.x = 0.0f;
			player->Find("LeftArmPoint")->rotation.x = 0.0f;
			gun->GetGun()->rotation.x = 90.0f * ToRadian;		// 총이 손을 따라다니게끔

			player->Find("RightShoulderPoint")->rotation.z = 0.0f;
			player->Find("RightShoulderPoint")->rotation.x = 0.0f;
			player->Find("LeftShoulderPoint")->rotation.x = 0.0f;

			player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
			player->Find("RightShoulderPoint")->rotation.y = 0.0f;
			walkHandDir = 100.0f;
		}

		playerType = PlayerType::Walk;
		walkLegDir = 100.0f;
		walkHandDir = 100.0f;
		return;
	}
	if (INPUT->KeyPress('A'))
	{
		player->MoveWorldPos(-player->GetRight() * speed * DELTA);
	}
	if (INPUT->KeyPress('D'))
	{
		player->MoveWorldPos(player->GetRight() * speed * DELTA);
	}
}
