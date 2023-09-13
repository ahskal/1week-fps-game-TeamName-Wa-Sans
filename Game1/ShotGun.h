#pragma once
class ShotGun
{
private:
	Actor* shotGun;


	int		bulletCount;	// 총알 갯수

	bool	isfire = false;	// 발사속도가 되면 true, 아니면 false (false동안에는 발사 못함)
	bool	isReady = false;	// 장전했는지
	bool	isLoad = false;
	float	fireTime;		// 발사속도(쏘는 속도)
	float	TriggerTime;	// 발사속도(쏘는 속도)
	float	TriggerSpeed;	// 장전하는놈이 움직이는 속도값
public:
	ShotGun();
	~ShotGun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();


	void ShotGunControl();
	Actor* GetShotGun() { return shotGun; }
};

