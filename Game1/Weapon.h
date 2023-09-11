#pragma once
class Weapon
{
private:

public:
	Weapon();
	~Weapon();

	virtual void Init();
	virtual void RenderHierarchy();
	virtual void Update();
	virtual void Render();
};

