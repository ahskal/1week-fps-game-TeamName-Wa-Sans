#include "stdafx.h"
#include "VillageMap.h"

Player::Player()
{
	// 플레이어 본체
	player = Actor::Create();
	player->LoadFile("Player.xml");

	player->SetWorldPosY(1);
	
	gun = new Gun();
	gun->GetGun()->rotation.x = 90.0f * ToRadian;
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
}

void Player::Render()
{
	player->Render();
	gun->Render();
}

void Player::PlayerControl()
{

	if (playerType == PlayerType::None)
	{
		//팔의 각도를 원상복구하기
		player->Find("RightArmPoint")->rotation.x = 0.0f;
		player->Find("LeftArmPoint")->rotation.x = 0.0f;
		gun->GetGun()->rotation.x = 90.0f * ToRadian;		// 총이 손을 따라다니게끔

		player->Find("RightShoulderPoint")->rotation.x = 0.0f;
		player->Find("LeftShoulderPoint")->rotation.x = 0.0f;
		walkHandDir = 100.0f;
		// 팔의 각도를 원상복구하기

		// 발의 각도 원상복구
		player->Find("RightKneePoint")->rotation.x = 0.0f;
		player->Find("LeftKneePoint")->rotation.x = 0.0f;

		player->Find("RightLegPoint")->rotation.x = 0.0f;
		player->Find("LeftLegPoint")->rotation.x = 0.0f;
		walkLegDir = 100.0f;
		// 발의 각도 원상복구


		if (INPUT->KeyPress('W') or INPUT->KeyPress('S') or INPUT->KeyPress('A') or INPUT->KeyPress('D'))
		{
			playerType = PlayerType::Walk;
		}
	}
	else if (playerType == PlayerType::Walk)
	{
		speed = 1.0f;
		if (INPUT->KeyPress(VK_SHIFT))
		{
			playerType = PlayerType::Run;
		}

		// 플레이어 이동 구현 (총을 들고있는지 아닌지에 따라 모션 변경)
		{
			if (INPUT->KeyPress('W'))
			{
				player->MoveWorldPos(player->GetForward() * speed * DELTA);

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
					player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA;
					player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
					if (player->Find("RightShoulderPoint")->rotation.x < -20.0f * ToRadian or
						player->Find("RightShoulderPoint")->rotation.x > 20.0f * ToRadian)
					{
						walkHandDir = -walkHandDir;
					}
				}

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
			if (INPUT->KeyPress('S'))
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
			if (INPUT->KeyPress('D'))
			{
				player->MoveWorldPos(player->GetRight() * speed * DELTA);
			}
		}
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}
	else if (playerType == PlayerType::Run)
	{
		speed = 2.0f;

		if (INPUT->KeyUp(VK_SHIFT))
		{
			playerType = PlayerType::Walk;
		}

		// 플레이어 이동 구현 (총을 들고있는지 아닌지에 따라 모션 변경)
		{
			if (INPUT->KeyPress('W'))
			{
				player->MoveWorldPos(player->GetForward() * speed * DELTA);

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
					player->Find("RightShoulderPoint")->rotation.x += walkHandDir * ToRadian * DELTA;
					player->Find("LeftShoulderPoint")->rotation.x -= walkHandDir * ToRadian * DELTA;
					if (player->Find("RightShoulderPoint")->rotation.x < -20.0f * ToRadian or
						player->Find("RightShoulderPoint")->rotation.x > 20.0f * ToRadian)
					{
						walkHandDir = -walkHandDir;
					}
				}



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
			if (INPUT->KeyPress('S'))
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
			if (INPUT->KeyPress('D'))
			{
				player->MoveWorldPos(player->GetRight() * speed * DELTA);
			}
		}
		if (INPUT->KeyUp('W') or INPUT->KeyUp('S') or INPUT->KeyUp('A') or INPUT->KeyUp('D'))
		{
			playerType = PlayerType::None;
		}
	}

	
	

	// 점프 구현
	/*if (not isGridCollide)
	{
		player->MoveWorldPos(player->GetUp() * gravity * DELTA);
	}
	else
	{
		isJump = false;
		jumpCount = 0;
	}
	if (INPUT->KeyDown(VK_SPACE) and jumpCount == 0)
	{
		isJump = true;
		gravity = 10.0f;
		player->MoveWorldPos(player->GetUp() * gravity * DELTA);
		player->Update();
		jumpCount++;
	}
	else
	{
		gravity -= 20.0f * DELTA;
	}*/
	if (isGridCollide)
	{
		gravity = 0.0f;
		jumpCount = 0;
	}
	else
	{
		player->MoveWorldPos(player->GetUp() * gravity * DELTA);
		gravity -= 20.0f * DELTA;
	}

	if (INPUT->KeyDown(VK_SPACE) and jumpCount == 0)
	{
		isJump = true;
		gravity = 10.0f;
		player->MoveWorldPos(player->GetUp() * gravity * DELTA);
		player->Update();
		jumpCount++;
	}

}

void Player::CollidePlayerToFloor(VillageMap* map)
{
	// 여기서 매개변수 grid는 바닥이라고 생각하면 될듯
	// 바닥과 부딪혔을때 isGridCollide = true;
	if (player->Intersect(map))
	{
		isGridCollide = true;
	}
	else
		isGridCollide = false;
}
