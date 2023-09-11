#pragma once
class Player
{
private:
	Actor* player;



	bool isJump = false;

	int jumpCount = 0;
	float gravity;
	float speed;



	float walkHandDir;
	float walkLegDir;
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

