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

	
	SOUND->AddSound("playerNoneAttack.wav", "playerNoneAttack");
	SOUND->AddSound("PlayerWalk.wav", "PlayerWalk");
	SOUND->AddSound("PlayerRun.wav", "PlayerRun");

	SOUND->SetVolume("PlayerWalk", 0.5f);
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

	hp = 100.0f;
	damage = 10.0f;
	speed = 5.0f;
}

void Player::RenderHierarchy()
{
	player->RenderHierarchy();
	gun->RenderHierarchy();
	shotGun->RenderHierarchy();
}

void Player::Update()
{
	if (Die())
	{
		return;
	}


	ImGui::Text("Player HP : %d", (int)hp);
	ImGui::SameLine();
	ImGui::Text("\tPlayer Damage : %d", (int)damage);

	// 피격시 무적시간
	if (TIMER->GetTick(attackedCoolTime, 1.0f))
	{
		isAttacked = false;
	}


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
		if (playerType == PlayerType::None)
			shotGun->GetShotGun()->visible = true;
		else
		{
			if (INPUT->KeyPress(VK_RBUTTON))
				shotGun->GetShotGun()->visible = true;
			else shotGun->GetShotGun()->visible = false;
		}
		break;
	default:
		break;
	}

	switch (playerType)
	{
	case PlayerType::None:
		SOUND->Stop("PlayerWalk");
		SOUND->Stop("PlayerRun");
		break;
	case PlayerType::Walk:
		SOUND->Play("PlayerWalk");
		SOUND->Stop("PlayerRun");
		break;
	case PlayerType::Run:
		SOUND->Stop("PlayerWalk");
		SOUND->Play("PlayerRun");
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
	if (Die())
	{
		return;
	}

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
		if (not(INPUT->KeyPress('W') or INPUT->KeyPress('S') or INPUT->KeyPress('A') or INPUT->KeyPress('D')))
		{
			SOUND->Stop("PlayerWalk");
			SOUND->Stop("PlayerRun");
			playerType = PlayerType::None;
			gun->GetGun()->rotation.x = 0.0;
			gun->GetGun()->rotation.y = 0.0;
			shotGun->GetShotGun()->rotation.x = 0.0;
			shotGun->GetShotGun()->rotation.y = 0.0;
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
		if (INPUT->KeyUp('W') and INPUT->KeyUp('S') and INPUT->KeyUp('A') and INPUT->KeyUp('D'))
		{
			SOUND->Stop("PlayerWalk");
			SOUND->Stop("PlayerRun");
			playerType = PlayerType::None;
			gun->GetGun()->rotation.x = 0.0;
			gun->GetGun()->rotation.y = 0.0;
			shotGun->GetShotGun()->rotation.x = 0.0;
			shotGun->GetShotGun()->rotation.y = 0.0;
		}
	}



	/** 무기 상태값(총알 발사 함수) */
	if (gunType == GunType::None)
	{
		if (INPUT->KeyDown(VK_LBUTTON))
		{
			SOUND->Stop("playerNoneAttack");
			SOUND->Play("playerNoneAttack");
		}
	}
	else if (gunType == GunType::Gun)
	{
		if (INPUT->KeyPress(VK_RBUTTON))
		{
			gun->GunControl();
		}
	}
	else if (gunType == GunType::ShotGun)
	{
		if (INPUT->KeyPress(VK_RBUTTON))
		{
			shotGun->ShotGunControl();
		}
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
	// 좌우 회전
	player->rotation.y = Rot.y;

	// 상하 회전
	player->Find("UpPoint")->rotation.x = Rot.x;

	gun->GetGun()->rotation.y = player->rotation.y;
	gun->GetGun()->rotation.x = player->Find("UpPoint")->rotation.x;

	shotGun->GetShotGun()->rotation.y = player->rotation.y;
	shotGun->GetShotGun()->rotation.x = player->Find("UpPoint")->rotation.x;

	if (playerType == PlayerType::None)
	{
		
		//shotGun->GetShotGun()->rotation.y = player->rotation.y;
		//
		//
		//shotGun->GetShotGun()->rotation.x = player->Find("UpPoint")->rotation.x;
	}
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
		float moveToPlayerLength = (player->GetWorldPos() - monster->GetMonsterActor()->GetWorldPos()).Length();
		ImGui::Text("moveToPlayerLength : %.2f", moveToPlayerLength);
		if (gunType == GunType::None)
		{
			if (INPUT->KeyDown(VK_LBUTTON))
			{
				if (moveToPlayerLength < 10.0f)
				{
					Attack(monster, 5.0f);
					cout << "적에게 근접공격" << endl;
				}
			}
		}
		else if (gunType == GunType::Gun)
		{
			if (INPUT->KeyPress(VK_RBUTTON) and INPUT->KeyDown(VK_LBUTTON) and gun->GetBulletCount() != 0)
			{
				if (moveToPlayerLength > 70.0f and moveToPlayerLength <= 120.0f)
				{
					Attack(monster, 5.0f);
					cout << "적에게 먼거리에서 Gun 공격" << endl;
				}
				else if (moveToPlayerLength <= 70.0f)
				{
					Attack(monster, 10.0f);
					cout << "적에게 Gun 공격" << endl;
				}
			}
		}
		else if (gunType == GunType::ShotGun)
		{
			if (INPUT->KeyPress(VK_RBUTTON) and INPUT->KeyDown(VK_LBUTTON) and shotGun->GetBulletCount() != 0)
			{
				if (moveToPlayerLength < 10.0f)
				{
					Attack(monster, 100.0f);
					cout << "적에게 바로앞에서 ShotGun 공격" << endl;
				}
				else if (moveToPlayerLength < 25.0f)
				{
					Attack(monster, 60.0f);
					cout << "적에게 가까운 거리에서 ShotGun 공격" << endl;
				}
				else if (moveToPlayerLength < 40.0f)
				{
					Attack(monster, 30.0f);
					cout << "적에게 좀 먼 거리에서 ShotGun 공격" << endl;
				}
				else if (moveToPlayerLength < 55.0f)
				{
					Attack(monster, 10.0f);
					cout << "적에게 먼거리에서 ShotGun 공격" << endl;
				}
				else if (moveToPlayerLength > 55.0f and moveToPlayerLength < 70.0f)
				{
					Attack(monster, 1.0f);
					cout << "적에게 엄청먼거리에서 ShotGun 공격" << endl;
				}
			}
		}
		
	}

	if (monster->GetMonsterActor()->Intersect(player))
	{
		if (not isAttacked)
		{
			monster->Attack(this, 15.0f);
			isAttacked = true;
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
		//shotGun->GetShotGun()->rotation.y = 0.0f * ToRadian;
		shotGun->ShotCunAim(this);
		//player->Find("UpPoint")->rotation.y = 36.0f * ToRadian;
		//
		//player->Find("RightShoulderPoint")->rotation.x = 0.0f * ToRadian;
		//player->Find("RightShoulderPoint")->rotation.y = -61.0f * ToRadian;
		//player->Find("RightShoulderPoint")->rotation.z = 50.0f * ToRadian;
		//
		//player->Find("RightArmPoint")->rotation.x = -123.0f * ToRadian;

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

		//player->Find("LeftShoulderPoint")->rotation.x = -90.0f * ToRadian;
		//player->Find("LeftShoulderPoint")->rotation.y = 0.0f * ToRadian;
		//
		//player->Find("LeftArmPoint")->rotation.x = 0.0f * ToRadian;
	}

	// 발 모션
	{
		player->Find("RightKneePoint")->rotation.x = 0;
		player->Find("LeftKneePoint")->rotation.x = 0;

		// 
		player->Find("RightLegPoint")->rotation.x = 0;
		player->Find("LeftLegPoint")->rotation.x = 0;
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
			if (INPUT->KeyPress(VK_RBUTTON))
			{
				gun->GunAim(this);
			}
			else
			{
				player->Find("RightArmPoint")->rotation.x = -45.0f * ToRadian;
				player->Find("LeftArmPoint")->rotation.x = 0.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.x = -45.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.x = -14.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y = 47.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
				gun->GetGun()->rotation.x = 45.0f; // 총이 손을 따라다니게끔
			}
			
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
			if (INPUT->KeyPress(VK_RBUTTON))
			{
				shotGun->ShotCunAim(this);
			}
			else
			{
				shotGun->GetShotGun()->rotation.y = -80.0f * ToRadian;

				player->Find("UpPoint")->rotation.y = 0.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.x = 10.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.z = 0.0f * ToRadian;

				player->Find("RightArmPoint")->rotation.x = -108.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("LeftShoulderPoint")->rotation.x = -7.0f * ToRadian;
				player->Find("LeftArmPoint")->rotation.x = -100.0f * ToRadian;

				//shotGun->GetShotGun()->rotation.x = 0.0f;
				shotGun->GetShotGun()->rotation.y = -90.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y += walkHandDir * 0.3f * ToRadian * DELTA;
				player->Find("RightShoulderPoint")->rotation.z += walkHandDir * 0.3f * ToRadian * DELTA;
				if (player->Find("RightShoulderPoint")->rotation.z < 0.0f * ToRadian or
					player->Find("RightShoulderPoint")->rotation.z > 10.0f * ToRadian)
				{
					walkHandDir = -walkHandDir;
				}
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
			gun->GunAim(this);
			//player->Find("LeftShoulderPoint")->rotation.x = -75.0f * ToRadian;
			//player->Find("RightShoulderPoint")->rotation.x = -75.0f * ToRadian;
			//
			//player->Find("LeftShoulderPoint")->rotation.y = 25.0f * ToRadian;
			//player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
			//gun->GetGun()->rotation.x = 0.0f; // 총이 손을 따라다니게끔
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
			if (INPUT->KeyPress(VK_RBUTTON))
			{
				gun->GunAim(this);
			}
			else if (INPUT->KeyUp(VK_RBUTTON))
			{
				player->Find("LeftShoulderPoint")->rotation.x = 0 * ToRadian;
				player->Find("RightShoulderPoint")->rotation.x = 0 * ToRadian;
			}
			else
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
			if (INPUT->KeyPress(VK_RBUTTON))
			{
				shotGun->ShotCunAim(this);
			}
			else
			{
				shotGun->GetShotGun()->rotation.y = -80.0f * ToRadian;

				player->Find("UpPoint")->rotation.y = 0.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.x = 10.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("RightShoulderPoint")->rotation.z = 0.0f * ToRadian;

				player->Find("RightArmPoint")->rotation.x = -108.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y = -25.0f * ToRadian;
				player->Find("LeftShoulderPoint")->rotation.x = -7.0f * ToRadian;
				player->Find("LeftArmPoint")->rotation.x = -100.0f * ToRadian;

				//shotGun->GetShotGun()->rotation.x = 0.0f;
				shotGun->GetShotGun()->rotation.y = -90.0f * ToRadian;

				player->Find("LeftShoulderPoint")->rotation.y += walkHandDir * 0.5f * ToRadian * DELTA;
				player->Find("RightShoulderPoint")->rotation.z += walkHandDir * 0.5f * ToRadian * DELTA;
				if (player->Find("RightShoulderPoint")->rotation.z < 0.0f * ToRadian or
					player->Find("RightShoulderPoint")->rotation.z > 15.0f * ToRadian)
				{
					walkHandDir = -walkHandDir;
				}
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


