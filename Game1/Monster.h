#pragma once
enum class MonType
{
	IDLE, 
	WALK
};	//���� ���Ǹ� �� ���� ����(2023/09/16 16:30)

class Monster
{
private:
	Actor* monster;
	Vector3 lastPos;

	MonType monType;
	float LegDir = 1.0f;	//�ٸ��� ����

	bool isCollide;		//������Ʈ �浹 ����
public:
	Monster();
	~Monster();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
	
	void Collide(GameObject* ob);	//������Ʈ �浹 �Լ� 
};

