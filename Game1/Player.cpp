#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	// 플레이어 본체
	player = Actor::Create();
	player->LoadFile("Player.xml");
}

Player::~Player()
{
}

void Player::Init()
{
	speed = 3.0f;
}

void Player::RenderHierarchy()
{
	player->RenderHierarchy();
}

void Player::Update()
{
	if (player->GetWorldPos().y < 0)
	{
		player->SetWorldPosY(0);
		gravity = 0.0f;
	}


	gravity -= 5.0f;
	player->MoveWorldPos(player->GetUp()* gravity* DELTA);

	player->Update();
}

void Player::Render()
{
	player->Render();
}

void Player::PlayerControl()
{
	//cam->ControlMainCam();


	if (INPUT->KeyPress('W'))
	{
		player->MoveWorldPos(player->GetForward() * 3 * DELTA);
	}
	if (INPUT->KeyPress('S'))
	{
		player->MoveWorldPos(-player->GetForward() * 3 * DELTA);
	}
	if (INPUT->KeyPress('A'))
	{
		player->MoveWorldPos(-player->GetRight() * 3 * DELTA);
	}
	if (INPUT->KeyPress('D'))
	{
		player->MoveWorldPos(player->GetRight() * 3 * DELTA);
	}
	//
	//if (INPUT->KeyDown(VK_SPACE))
	//{
	//	isJump = true;
	//	gravity = 10.0f;
	//}
}
