#include "stdafx.h"
#include "House.h"

House* House::Create(string name)
{
	House* temp = new House();
	temp->LoadFile("VillageHouse.xml");
	temp->type = ObType::Actor;

	return temp;
}

House::House()
{
}

House::~House()
{
}

void House::Update()
{
	Actor::Update();
}
