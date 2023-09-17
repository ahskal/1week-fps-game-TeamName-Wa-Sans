#include "stdafx.h"
#include "UNIT.h"

UNIT::UNIT()
{
	spawnPos = Vector3();
}

UNIT::~UNIT()
{

}

void UNIT::Attack(UNIT* unit, float dam)
{
	damage = dam;
	unit->hp -= damage;
}

bool UNIT::Die()
{
	if (hp <= 0) return true;

	return false;
}












