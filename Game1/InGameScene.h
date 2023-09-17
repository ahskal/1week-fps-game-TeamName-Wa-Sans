#pragma once
//�ΰ��ӿ��� �۵��ϰ� ���ο��� ������ ���� ��ü�帧�Դϴ� �ΰ��Ӿ�
class InGameScene : public Scene
{
	//�����̺� �Լ�����
private:
	//�ΰ��� ��ü�� 
	Camera* MainCam;
	Camera* PlayerCam;

	class VillageMap* Map;
	class Player* player;
	vector<class Monster*> monster;

	//�ΰ��� �����ð�����
	float CurrentTime;
	float zombieSpwanTime;

	

private:
	//�ΰ��� UI
	UI* playerAim;
	UI* optionUI;
	UI* soundUI;
	UI* sensitivityUI;

	//�̼� ui �߰�
	UI* killmissionUI;
	UI* killcountUI1;
	UI* killcountUI2;

	//���ӿ��� ui�߰�
	UI* gameoverUI;
	UI* gamewinUI;

	Sound* Ingamethema;
	//����â �����ִ��� �Ҹ���
	bool optionOpen = false;
	//���� �����ִ��� �Ҹ���
	bool soundOn = false;
	//���콺 ��������
	float mouseSpeed;

	
	
    //���� ų����
	int killCount;
	//�׿����� �̼Ǹ��ͼ���
	int missionKill;
	int uikillcount;

public:
	//�⺻����Լ���
	InGameScene();
	~InGameScene();
	//�ʱ�ȭ
	virtual void Init();
	//����
	virtual void Release();
	//Loop
	virtual void Update();
	virtual void LateUpdate();
	virtual void PreRender();
	virtual void Render();
	//Window Resize
	virtual void ResizeScreen();






	//�����̺� ��������
private:


	//�ۺ� ��������
public:

};

