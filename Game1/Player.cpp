#include "stdafx.h"

Player::Player()
{
	// �÷��̾� ��ü
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

	// �ǰݽ� �����ð�
	if (TIMER->GetTick(attackedCoolTime, 1.0f))
	{
		isAttacked = false;
	}


	gun->GetGun()->SetWorldPos(player->Find("RightHandMesh")->GetWorldPos());
	shotGun->GetShotGun()->SetWorldPos(player->Find("RightHandMesh")->GetWorldPos());

	lastPos = player->GetWorldPos();

	/**
	1�� ����, 2�� ����, 3�� ��������, 4�� ��ô����, 
	5�� �ش�, 6�� �������帵ũ, 7�� ������*/
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
	// ������ ������ 
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
	// �ɾ�ٴҶ� (����x / ���� / ���� / ... )
	else if (playerType == PlayerType::Walk)
	{
		// �ӵ� ����
		speed = 5.0f;

		// �÷��̾� �̵� �����Լ� (���� ����ִ��� �ƴ����� ���� ��� ����)
		MotionPlayerWalk(gunType);

		// �ȱ� -> �ٱ�(����Ʈ �����鼭 S�� ������ �ʾ��� ��)
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

		// �ȱ� -> ���ֱ� (����Ű�� ������ �ʾ�����)
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
	// �پ�ٴҶ� (����x / ���� / ���� / ... )
	else if (playerType == PlayerType::Run)
	{
		// �ӵ� ����
		speed = 10.0f;

		// �÷��̾� �̵� �����Լ� (���� ����ִ��� �ƴ����� ���� ��� ����)
		MotionPlayerRun(gunType);

		// �ٱ� -> �ȱ� (����Ʈ�� ���� ��)
		if (INPUT->KeyUp(VK_SHIFT))
		{
			if (gunType == GunType::None or gunType == GunType::Gun)
			{
				player->Find("RightArmPoint")->rotation.x = 0.0f;
				player->Find("LeftArmPoint")->rotation.x = 0.0f;
				gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�

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

		// �ٱ� -> ������ �ֱ� (����Ű�� ������ �ʾ�����)
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



	/** ���� ���°�(�Ѿ� �߻� �Լ�) */
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
	/** ���� ���°�*/

	/** ����*/
	if (isGridCollide)
	{
		gravity = 0.0f;
		jumpCount = 0; // �ѹ��� ������ �����ϰ� �ϱ�����
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
	/** ����*/
}

void Player::PlayerRotationY(Vector3 Rot)
{
	// �¿� ȸ��
	player->rotation.y = Rot.y;

	// ���� ȸ��
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
					cout << "������ ��������" << endl;
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
					cout << "������ �հŸ����� Gun ����" << endl;
				}
				else if (moveToPlayerLength <= 70.0f)
				{
					Attack(monster, 10.0f);
					cout << "������ Gun ����" << endl;
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
					cout << "������ �ٷξտ��� ShotGun ����" << endl;
				}
				else if (moveToPlayerLength < 25.0f)
				{
					Attack(monster, 60.0f);
					cout << "������ ����� �Ÿ����� ShotGun ����" << endl;
				}
				else if (moveToPlayerLength < 40.0f)
				{
					Attack(monster, 30.0f);
					cout << "������ �� �� �Ÿ����� ShotGun ����" << endl;
				}
				else if (moveToPlayerLength < 55.0f)
				{
					Attack(monster, 10.0f);
					cout << "������ �հŸ����� ShotGun ����" << endl;
				}
				else if (moveToPlayerLength > 55.0f and moveToPlayerLength < 70.0f)
				{
					Attack(monster, 1.0f);
					cout << "������ ��û�հŸ����� ShotGun ����" << endl;
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
		// ��ü ���󺹱��ϱ�
		player->Find("UpPoint")->rotation.y = 0.0f * ToRadian;

		//���� ������ ���󺹱��ϱ�
		player->Find("RightArmPoint")->rotation.x = 0.0f;			// ������ �ʱ�ȭ
		player->Find("LeftArmPoint")->rotation.x = 0.0f;			// ���� �ʱ�ȭ
		gun->GetGun()->rotation.x = 90.0f * ToRadian;				// ���� ���� ����ٴϰԲ�
		
		player->Find("RightShoulderPoint")->rotation.z = 0.0f;		// ������ ��� �ʱ�ȭ
		player->Find("RightShoulderPoint")->rotation.y = 0.0f;		// ������ ��� �ʱ�ȭ
		player->Find("RightShoulderPoint")->rotation.x = 0.0f;		// ������ ��� �ʱ�ȭ
		
		player->Find("LeftShoulderPoint")->rotation.x = 0.0f;		// ���� ��� �ʱ�ȭ
		player->Find("LeftShoulderPoint")->rotation.y = 0.0f;		// ���� ��� �ʱ�ȭ
		walkHandDir = 100.0f;
		// ���� ������ ���󺹱��ϱ�

		// ���� ���� ���󺹱�
		player->Find("RightKneePoint")->rotation.x = 0.0f;
		player->Find("LeftKneePoint")->rotation.x = 0.0f;

		player->Find("RightLegPoint")->rotation.x = 0.0f;
		player->Find("LeftLegPoint")->rotation.x = 0.0f;
		walkLegDir = 100.0f;
		// ���� ���� ���󺹱�
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

	// �� ���
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

		// �� ���
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
				gun->GetGun()->rotation.x = 45.0f; // ���� ���� ����ٴϰԲ�
			}
			
		}
		else if (gunType == GunType::None)
		{
			// �Ȳ�ġ ������ -40�� �ؼ� ���� ���� ���·� �����
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // ���� ���� ����ٴϰԲ�

			// ��� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
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

		// �� ���
		{
			// ���� ������ 25�� �ؼ� ���� ���� ���·� �����
			player->Find("RightKneePoint")->rotation.x = 25.0f * ToRadian;
			player->Find("LeftKneePoint")->rotation.x = 25.0f * ToRadian;

			// �� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
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
			//gun->GetGun()->rotation.x = 0.0f; // ���� ���� ����ٴϰԲ�
		}
		else if (gunType == GunType::None)
		{
			// �Ȳ�ġ ������ -40�� �ؼ� ���� ���� ���·� �����
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // ���� ���� ����ٴϰԲ�

			// ��� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
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


		// ���� ������ 25�� �ؼ� ���� ���� ���·� �����
		player->Find("RightKneePoint")->rotation.x = 25.0f * ToRadian;
		player->Find("LeftKneePoint")->rotation.x = 25.0f * ToRadian;

		// �� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
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
				gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�
			}
		}
		else if (gunType == GunType::None)
		{
			// �Ȳ�ġ ������ -40�� �ؼ� ���� ���� ���·� �����
			player->Find("RightArmPoint")->rotation.x = -40.0f * ToRadian;
			player->Find("LeftArmPoint")->rotation.x = -40.0f * ToRadian;
			gun->GetGun()->rotation.x = (90.0f - 40.0f) * ToRadian; // ���� ���� ����ٴϰԲ�

			// ��� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
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




		// ���� ������ 25�� �ؼ� ���� ���� ���·� �����
		player->Find("RightKneePoint")->rotation.x = 30.0f * ToRadian;
		player->Find("LeftKneePoint")->rotation.x = 30.0f * ToRadian;

		// �� x������ -20 ~ 20���� ������ ������ ���� �����̰� ���̱�
		player->Find("RightLegPoint")->rotation.x -= walkLegDir * ToRadian * DELTA;
		player->Find("LeftLegPoint")->rotation.x += walkLegDir * ToRadian * DELTA;
		if (player->Find("RightLegPoint")->rotation.x < -25.0f * ToRadian or
			player->Find("RightLegPoint")->rotation.x > 25.0f * ToRadian)
		{
			walkLegDir = -walkLegDir;
		}
	}
	// �ٱ� -> �ȱ� (������Ʋ ������ �ִ� �����϶� S�� ������ ��)
	if (INPUT->KeyPress(VK_SHIFT) and INPUT->KeyPress('S'))
	{
		if (gunType == GunType::None or gunType == GunType::Gun)
		{
			player->Find("RightArmPoint")->rotation.x = 0.0f;
			player->Find("LeftArmPoint")->rotation.x = 0.0f;
			gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�

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


