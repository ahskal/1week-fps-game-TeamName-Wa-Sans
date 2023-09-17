#pragma once
class Gun
{
private:
	Actor* gun;


	int		bulletCount;	// 총알 갯수
	int		maxBullet;		// 최대 총알 갯수
	int		magazineCount;

	bool	b_fire;			// 발사속도가 되면 true, 아니면 false (false동안에는 발사 못함)

	float	fireTime;			// 발사속도
public:
	Gun();
	~Gun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();

	void GunControl();
	void GunAim(class Player* player);
	/**Get함수*/
	Actor* GetGun() { return gun; }
	int GetBulletCount() const { return bulletCount; }
};

