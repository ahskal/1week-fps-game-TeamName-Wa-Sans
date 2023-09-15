#include "stdafx.h"
#include "UNIT.h"

UNIT::UNIT()
{
	speed = 0.0f;
	hp = 100.0f;
	damage = 0.0f;

	spawnPos = Vector3();
	unitType = UnitType::PLAYER;
}

UNIT::~UNIT()
{

}




