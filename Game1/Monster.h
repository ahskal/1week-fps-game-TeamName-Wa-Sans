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

	MonType monType;
	float LegDir = 1.0f;	//다리의 각도

	bool isCollide;		//오브젝트 충돌 여부
public:
	Monster();
	~Monster();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
	
	void Collide(GameObject* ob);	//오브젝트 충돌 함수 
};

