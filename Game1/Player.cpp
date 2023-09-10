#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	// �÷��̾� ��ü
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
