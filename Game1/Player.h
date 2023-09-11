#pragma once
class Player
{
private:
	Actor* player;



	bool isJump = false;

	float gravity;
	float speed;
public:
	Player();
	~Player();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();

	void PlayerControl();

	//Actor* GetPlayer() { return player; }
};

