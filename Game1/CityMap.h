#pragma once
class CityMap : public Actor
{
public:
	static CityMap* Create(string name = "CityMap");

private:
	Camera* cam;
	Grid* grid;


public:
	CityMap();
	~CityMap();

	void Init();
	void Release();
	void Update();
	void LateUpdate();
	void Render();
	void ResizeScreen();
	void Hierarchy();

	Camera* GetCam() const { return cam; }
	

};

