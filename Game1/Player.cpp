#include "stdafx.h"

Player::Player()
{
	// �÷��̾� ��ü
	player = Actor::Create();
	player->LoadFile("Player.xml");


	gun = new Gun();
	gun->GetGun()->rotation.x = 90.0f * ToRadian;


	shotGun = new ShotGun();
}

Player::~Player()
{
}

void Player::Init()
{
	speed = 3.0f;
	player->root->SetWorldPosY(0.1f);

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
	ImGui::Text("gravity : %.2f", gravity);
	ImGui::Text("playerType : %d", (int)playerType);
	ImGui::Text("isGridCollide : %d", (int)isGridCollide);
	//ImGui::Text("walkDir : %.2f", walkDir);
	gun->GetGun()->SetWorldPos(player->Find("RightHandMesh")->GetWorldPos());



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

void Player::PlayerControl()
{
	// ������ ������ 
	if (playerType == PlayerType::None)
	{
		//���� ������ ���󺹱��ϱ�
		player->Find("RightArmPoint")->rotation.x = 0.0f;
		player->Find("LeftArmPoint")->rotation.x = 0.0f;
		gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�

		player->Find("RightShoulderPoint")->rotation.z = 0.0f;
		player->Find("RightShoulderPoint")->rotation.x = 0.0f;
		player->Find("LeftShoulderPoint")->rotation.x = 0.0f;

		player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
		player->Find("RightShoulderPoint")->rotation.y = 0.0f;
		walkHandDir = 100.0f;
		// ���� ������ ���󺹱��ϱ�

		// ���� ���� ���󺹱�
		player->Find("RightKneePoint")->rotation.x = 0.0f;
		player->Find("LeftKneePoint")->rotation.x = 0.0f;

		player->Find("RightLegPoint")->rotation.x = 0.0f;
		player->Find("LeftLegPoint")->rotation.x = 0.0f;
		walkLegDir = 100.0f;
		// ���� ���� ���󺹱�


		if (INPUT->KeyPress('W') or INPUT->KeyPress('S') or INPUT->KeyPress('A') or INPUT->KeyPress('D'))
		{
			playerType = PlayerType::Walk;
		}
	}
	// �ɾ�ٴҶ� (����x / ���� / ���� / ... )
	else if (playerType == PlayerType::Walk)
	{
		speed = 1.0f;
		// �ȱ� -> �ٱ�(����Ʈ �����鼭 S�� ������ �ʾ��� ��)
		if (INPUT->KeyPress(VK_SHIFT) and not INPUT->KeyPress('S'))
		{
			playerType = PlayerType::Run;
			walkLegDir = 150.0f;
			walkHandDir = 150.0f;
			player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
			player->Find("RightShoulderPoint")->rotation.y = 0.0f;

			player->Find("LeftShoulderPoint")->rotation.x = 0.0f;
			player->Find("RightShoulderPoint")->rotation.x = 0.0f;
			return;
		}

		// �÷��̾� �̵� ���� (���� ����ִ��� �ƴ����� ���� ��� ����)
		{
			if (INPUT->KeyPress('W'))
			{
				player->MoveWorldPos(player->GetForward() * speed * DELTA);

				if (gunType == GunType::Gun)
				{
					player->Find("RightArmPoint")->rotation.x = 0.0f;
					player->Find("LeftArmPoint")->rotation.x = 0.0f;

					player->Find("LeftShoulderPoint")->rotation.x = -75.0f * ToRadian;
					player->Find("RightShoulderPoint")->rotation.x = -75.0f * ToRadian;

					player->Find("LeftShoulderPoint")->rotation.y = 25.0f * ToRadian;
					player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
					gun->GetGun()->rotation.x = 0.0f; // ���� ���� ����ٴϰԲ�
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

				}

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
			if (INPUT->KeyPress('S'))
			{
				player->MoveWorldPos(-player->GetForward() * speed * DELTA);

				if (gunType == GunType::Gun)
				{
					player->Find("LeftShoulderPoint")->rotation.x = -75.0f * ToRadian;
					player->Find("RightShoulderPoint")->rotation.x = -75.0f * ToRadian;

					player->Find("LeftShoulderPoint")->rotation.y = 25.0f * ToRadian;
					player->Find("RightShoulderPoint")->rotation.y = -25.0f * ToRadian;
					gun->GetGun()->rotation.x = 0.0f; // ���� ���� ����ٴϰԲ�
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
			if (INPUT->KeyPress('D'))
			{
				player->MoveWorldPos(player->GetRight() * speed * DELTA);
			}
		}
		// �ȱ� -> ���ֱ� (����Ű�� ������ �ʾ�����)
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or 
			INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}
	// �پ�ٴҶ� (����x / ���� / ���� / ... )
	else if (playerType == PlayerType::Run)
	{
		speed = 2.0f;
		// �ٱ� -> �ȱ� (����Ʈ�� ���� ��)
		if (INPUT->KeyUp(VK_SHIFT))
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

			playerType = PlayerType::Walk;
			walkLegDir = 100.0f;
			walkHandDir = 100.0f;

			return;
		}
		

		// �÷��̾� �̵� ���� (���� ����ִ��� �ƴ����� ���� ��� ����)
		{
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
					gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�

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
				player->Find("RightArmPoint")->rotation.x = 0.0f;
				player->Find("LeftArmPoint")->rotation.x = 0.0f;
				gun->GetGun()->rotation.x = 90.0f * ToRadian;		// ���� ���� ����ٴϰԲ�

				player->Find("RightShoulderPoint")->rotation.z = 0.0f;
				player->Find("RightShoulderPoint")->rotation.x = 0.0f;
				player->Find("LeftShoulderPoint")->rotation.x = 0.0f;

				player->Find("LeftShoulderPoint")->rotation.y = 0.0f;
				player->Find("RightShoulderPoint")->rotation.y = 0.0f;
				walkHandDir = 100.0f;

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
		// �ٱ� -> ������ �ֱ� (����Ű�� ������ �ʾ�����)
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}

	/** ���� ���°�*/
	if (gunType == GunType::Gun)
	{
		//gun->GunControl();
	}
	else if (gunType == GunType::ShotGun)
	{
		shotGun->ShotGunControl();
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

void Player::CollidePlayerToFloor(Grid* grid)
{
	// ���⼭ �Ű����� grid�� �ٴ��̶�� �����ϸ� �ɵ�
	// �ٴڰ� �ε������� isGridCollide = true;
	if (player->Intersect(grid->Find("floor")))
		isGridCollide = true;
	else
		isGridCollide = false;
}
