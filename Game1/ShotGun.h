#pragma once
class ShotGun
{
private:
	Actor* shotGun;


	int		bulletCount;	// 총알 갯수

	bool	isfire = false;	// 발사속도가 되면 true, 아니면 false (false동안에는 발사 못함)
	bool	isLoad = false;		// 장전중인지
	bool	isrecoil = false;	// 반동이 있는지


	float	fireTime;		// 발사속도(쏘는 속도)
	float	TriggerTime;	// 발사속도(쏘는 속도)
	float	TriggerSpeed;	// 장전하는놈이 움직이는 속도값

	float	recoilDir;		// 샷건 반동 각도값
public:
	ShotGun();
	~ShotGun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();


	void ShotGunControl();
	Actor* GetShotGun() { return shotGun; }
	const bool GetIsLoad() { return isLoad; }
	const bool GetIsRecoil() { return isrecoil; }
};

