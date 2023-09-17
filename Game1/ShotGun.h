#pragma once
class ShotGun
{
private:
	Actor* shotGun;


	int		bulletCount;	// �Ѿ� ����
	int		maxBullet;		// �ִ� �Ѿ� ����
	int		magazineCount;	// źâ ��(��ź�̳� �̷��� ����Ÿ� ����ؼ�)


	bool	isfire = false;	// �߻�ӵ��� �Ǹ� true, �ƴϸ� false (false���ȿ��� �߻� ����)
	bool	isLoad = false;		// ����������
	bool	isrecoil = false;	// �ݵ��� �ִ���


	float	fireTime;		// �߻�ӵ�(��� �ӵ�)
	float	TriggerTime;	// �߻�ӵ�(��� �ӵ�)
	float	TriggerSpeed;	// �����ϴ³��� �����̴� �ӵ���

	float	recoilDir;		// ���� �ݵ� ������
public:
	ShotGun();
	~ShotGun();

	void Init();
	void RenderHierarchy();
	void Update();
	void Render();


	void ShotGunControl();
	void ShotCunAim(class Player* player);


	/** Get�Լ� */
	Actor* GetShotGun() { return shotGun; }
	int GetBulletCount() const { return bulletCount; }
	const bool GetIsLoad() { return isLoad; }
	const bool GetIsRecoil() { return isrecoil; }
};

