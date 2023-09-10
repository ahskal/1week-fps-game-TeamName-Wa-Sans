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
}

void Player::RenderHierarchy()
{
	player->RenderHierarchy();
}

void Player::Update()
{
	player->Update();
}

void Player::Render()
{
	player->Render();
}
