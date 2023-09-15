#pragma once
class Monster
{
private:
	Actor* monster;
public:
	Monster();
	~Monster();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();
};

