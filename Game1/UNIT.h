#pragma once
enum class UnitType
{
	PLAYER,
	ZOMBIE
};

class UNIT 
{
protected:

	float     speed;
	float     hp;
	float     damage;

	UnitType  unitType;
	Vector3   spawnPos;

public:


public:
	UNIT();
	virtual ~UNIT();

	
	
	virtual void TakeDamage(float enemyDamage) { hp -= enemyDamage; }
	virtual void GethealHp(float heal) { hp += heal; }
	//void CollidePlayerToFloor(class VillageMap* map);

	//몬스터나 플레이어 죽었을때

};

