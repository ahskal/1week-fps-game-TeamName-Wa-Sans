#pragma once
enum class GunType
{
	None,
	Gun,
	ShotGun,
};

enum class PlayerType
{
	None,
	Walk,
	Run,
};

class Player : public UNIT
{
private:
	Actor* player;

	Gun* gun;
	ShotGun* shotGun;

	PlayerType playerType = PlayerType::None;
	GunType gunType = GunType::None;

	Vector3 lastPos;				// �浹���� �÷��̾��� ��ǥ��


	bool isJump = false;			// ���� �ߴ��� ���ߴ��� �Ǵ��ϴ� bool��
	bool isGridCollide = false;		// �׸���� �浹�ߴ��� �Ǵ��ϴ� bool��
	bool isAttacked = false;		// �ǰݴ��ߴ��� �Ǵ��ϴ� bool��
	float attackedCoolTime;			// �ǰݽ� ��������

	int jumpCount = 0;				// ���� ī��Ʈ (�ѹ��� �����ϰ� �ϱ� ���� ��)
	float gravity;					// �÷��̾��� �߷°�
	//float speed;					// �÷��̾��� �̵��ӵ�



	float waitHandDir;				// �������� �� ������
	float walkHandDir;				// �������� �� ������
	float walkLegDir;				// �������� �ٸ� ������
public:
	Player();
	~Player();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
	void ResizeScreen();


	void PlayerControl();
	void PlayerRotationY(Vector3 Rot);
	void CollidePlayerToFloor(class VillageMap* map); 
	void CollidePlayerToWall(bool isCollide);
	void CollidePlayerToZombie(class Monster* monster);

	/** ��� �Լ�*/
	// ������������
	void MotionPlayerWait(GunType type);
	// ������
	void MotionPlayerWalk(GunType type);
	// �۶�
	void MotionPlayerRun(GunType type);
	// 
	/** ��� �Լ�*/
	

	/** Get�Լ�*/
	Actor* GetPlayerActor() const { return player; }
	
};

