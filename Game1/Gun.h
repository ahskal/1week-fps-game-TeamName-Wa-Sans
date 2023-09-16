#pragma once
class Gun
{
private:
	Actor* gun;


	int		bulletCount;	// �Ѿ� ����
	int		maxBullet;		// �ִ� �Ѿ� ����
	int		magazineCount;

	bool	b_fire;			// �߻�ӵ��� �Ǹ� true, �ƴϸ� false (false���ȿ��� �߻� ����)

	float	fireTime;			// �߻�ӵ�
public:
	Gun();
	~Gun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();


	void GunControl();
	Actor* GetGun() { return gun; }
};

