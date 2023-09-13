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
	bool isJump = false;			// 점프 했는지 안했는지 판단하는 bool값
	bool isGridCollide = false;		// 그리드와 충돌했는지 판단하는 bool값

	int jumpCount = 0;				// 점프 카운트 (한번만 점프하게 하기 위한 값)
	float gravity;					// 플레이어의 중력값
	float speed;					// 플레이어의 이동속도



	float waitHandDir;				// 걸을때의 손 각도값
	float walkHandDir;				// 걸을때의 손 각도값
	float walkLegDir;				// 걸을때의 다리 각도값
public:
	Player();
	~Player();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();

	void PlayerControl();
	void CollidePlayerToFloor(class Grid* grid);
};

