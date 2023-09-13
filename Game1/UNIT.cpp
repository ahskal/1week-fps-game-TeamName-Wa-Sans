#include "stdafx.h"
#include "UNIT.h"

UNIT::UNIT()
{
	speed = 0.0f;
	hp = 0.0f;
	damage = 0.0f;

	spawnPos = Vector3();
	unitType = UnitType::PLAYER;
}

UNIT::~UNIT()
{

}


void UNIT::Init(float initialSpeed, float initialHP, float initialDamage, Vector3& initialSpawnPos)
{

	speed = initialSpeed;
	hp = initialHP;
	damage = initialDamage;
	spawnPos = initialSpawnPos;
	if (unitType == UnitType::PLAYER)
	{
		//나중에 플레이어 스폰시 능력치조절
	}
	else if (unitType == UnitType::PLAYER)
	{
		//나중에 몬스터 스폰시 능력치조절
	}

}


