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

	void Init(float initialSpeed, float initialHP, float initialDamage, Vector3& initialSpawnPos);
	
	virtual void TakeDamage(float enemyDamage) { hp -= enemyDamage; }
	virtual void GethealHp(float heal) { hp += heal; }

	//몬스터나 플레이어 죽었을때

};

