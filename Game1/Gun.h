#pragma once
class Gun
{
private:
	Actor* gun;


	int		bulletCount;	// �Ѿ� ����
	int		maxBullet;		// �ִ� �Ѿ� ����
	int		magazineCount;

	bool	b_fire;			// �߻�ӵ��� �Ǹ� true, �ƴϸ� false (false���ȿ��� �߻� ����)
	bool	isAttack = false;
	float	attackTime;			
	float	fireTime;			// �߻�ӵ�
public:
	Gun();
	~Gun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();

	void GunControl();
	void GunAim(class Player* player);
	/**Get�Լ�*/
	Actor* GetGun() { return gun; }
	int GetBulletCount() const { return bulletCount; }
	bool GetIsAttack() const { return isAttack; }
};

