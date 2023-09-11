#pragma once
enum class GunType
{
	None,
	Gun,
	ShotGun,
};

class Player
{
private:
	Actor* player;

	Gun* gun;


	GunType gunType = GunType::None;
	bool isJump = false;			// ���� �ߴ��� ���ߴ��� �Ǵ��ϴ� bool��


	int jumpCount = 0;				// ���� ī��Ʈ (�ѹ��� �����ϰ� �ϱ� ���� ��)
	float gravity;					// �÷��̾��� �߷°�
	float speed;					// �÷��̾��� �̵��ӵ�



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
	void PlayerCollide(class Grid* grid);
	//Actor* GetPlayer() { return player; }
};

