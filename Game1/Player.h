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

	Vector3 lastPos;				// 충돌전의 플레이어의 좌표값


	bool isJump = false;			// 점프 했는지 안했는지 판단하는 bool값
	bool isGridCollide = false;		// 그리드와 충돌했는지 판단하는 bool값
	bool isAttacked = false;		// 피격당했는지 판단하는 bool값
	float attackedCoolTime;			// 피격시 무적상태

	int jumpCount = 0;				// 점프 카운트 (한번만 점프하게 하기 위한 값)
	float gravity;					// 플레이어의 중력값
	//float speed;					// 플레이어의 이동속도



	float waitHandDir;				// 서있을때 손 각도값
	float walkHandDir;				// 걸을때의 손 각도값
	float walkLegDir;				// 걸을때의 다리 각도값
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

	/** 모션 함수*/
	// 가만히있을때
	void MotionPlayerWait(GunType type);
	// 걸을때
	void MotionPlayerWalk(GunType type);
	// 뛸때
	void MotionPlayerRun(GunType type);
	// 
	/** 모션 함수*/
	

	/** Get함수*/
	Actor* GetPlayerActor() const { return player; }
	
};

