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
	Actor* col;

	MonType monType;
	float LegDir = 1.0f;	//�ٸ��� ����

	bool isCollide;		//������Ʈ �浹 ����
	bool isChase;
public:
	Monster();
	~Monster();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
	
	void Collide(GameObject* ob);	//������Ʈ �浹 �Լ� *** �浹�� ��ü�� ���� Ÿ���� ��ȯ�ϼ��� ***
	void Chase(GameObject* player, Vector3 playerPos);	//�÷��̾� ���� �Լ� *** �÷��̾� Ÿ������ ��ȯ�ϼ��� ***
};

