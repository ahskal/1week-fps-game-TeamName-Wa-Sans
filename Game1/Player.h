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

class Player
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

	int jumpCount = 0;				// ���� ī��Ʈ (�ѹ��� �����ϰ� �ϱ� ���� ��)
	float gravity;					// �÷��̾��� �߷°�
	float speed;					// �÷��̾��� �̵��ӵ�



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

	void PlayerControl();
	void CollidePlayerToFloor(class Grid* grid);
	void CollidePlayerToWall(class Actor* wall);

	/** ��� �Լ�*/
	// ������������
	void MotionPlayerWait(GunType type);
	// ������
	void MotionPlayerWalk(GunType type);
	// �۶�
	void MotionPlayerRun(GunType type);
	// 
	/** ��� �Լ�*/
};

