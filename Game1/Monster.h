#pragma once
enum class MonType
{
	IDLE, 
	WALK
};	//현재 정의만 해 놓은 상태(2023/09/16 16:30)

class Monster
{
private:
	Actor* monster;
	Vector3 lastPos;
	Actor* col;

	MonType monType;
	float LegDir = 1.0f;	//다리의 각도

	bool isCollide;		//오브젝트 충돌 여부
	bool isChase;
public:
	Monster();
	~Monster();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
	
	void Collide(GameObject* ob);	//오브젝트 충돌 함수 *** 충돌할 물체에 따라 타입을 변환하세요 ***
	void Chase(GameObject* player, Vector3 playerPos);	//플레이어 추적 함수 *** 플레이어 타입으로 변환하세요 ***
};

