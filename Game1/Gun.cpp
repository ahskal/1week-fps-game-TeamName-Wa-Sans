#include "stdafx.h"

Gun::Gun()
{
	gun = Actor::Create();
	gun->LoadFile("beretta92.xml");
}

Gun::~Gun()
{
}

void Gun::Init()
{
}

void Gun::RenderHierarchy()
{
	gun->RenderHierarchy();
}

void Gun::Update()
{
	gun->Update();
}

void Gun::Render()
{
	gun->Render();
}
