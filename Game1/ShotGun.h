#pragma once
class ShotGun
{
private:
	Actor* shotGun;


	int		bulletCount;	// �Ѿ� ����

	bool	isfire = false;	// �߻�ӵ��� �Ǹ� true, �ƴϸ� false (false���ȿ��� �߻� ����)
	bool	isReady = false;	// �����ߴ���
	bool	isLoad = false;
	float	fireTime;		// �߻�ӵ�(��� �ӵ�)
	float	TriggerTime;	// �߻�ӵ�(��� �ӵ�)
	float	TriggerSpeed;	// �����ϴ³��� �����̴� �ӵ���
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

