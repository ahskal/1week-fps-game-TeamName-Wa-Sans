#pragma once
class Monster : public UNIT
{
private:
	Actor* monster;
public:
	Monster();
	~Monster();

	void Init(Vector3 spwan);
	void RenderHierarchy();
	void Update();
	void Render();
};

