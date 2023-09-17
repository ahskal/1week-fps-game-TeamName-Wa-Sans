#pragma once
enum class MonType
{
	IDLE, 
	WALK
};	//���� ���Ǹ� �� ���� ����(2023/09/16 16:30)

class Monster : public UNIT
{
private:
	bool debug = false;

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

	void Init(Vector3 spwan);
	void RenderHierarchy();
	void Update();
	void Render();

	void GoBack() {
		monster->SetWorldPosX(lastPos.x);
		monster->SetWorldPosZ(lastPos.z);
		monster->Update();
	}
	
	Actor* GetMonsterActor() const { return monster; }
	
	void CollidePlayer(Player* player);
	void Chase(Player* player);	//�÷��̾� ���� �Լ� *** �÷��̾� Ÿ������ ��ȯ�ϼ��� ***
};

