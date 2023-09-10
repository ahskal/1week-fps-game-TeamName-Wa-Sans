#pragma once
class Player
{
private:
	Actor* player;


public:
	Player();
	~Player();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();


	Actor* GetPlayer() { return player; }
};

