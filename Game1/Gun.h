#pragma once
class Gun
{
private:
	Actor* gun;

public:
	Gun();
	~Gun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();

	Actor* GetGun() { return gun; }
};

