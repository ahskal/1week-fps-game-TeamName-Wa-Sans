#pragma once
//인게임에서 작동하고 메인에서 실행할 게임 전체흐름입니다 인게임씬
class InGameScene : public Scene
{
	//프라이빗 함수생성
private:
	//인게임 객체들 
	Camera* MainCam;
	Camera* PlayerCam;

	class VillageMap* Map;
	class Player* player;
	vector<class Monster*> monster;

	//인게임 스폰시간변수
	float CurrentTime;
	float zombieSpwanTime;

	

private:
	//인게임 UI
	UI* playerAim;
	UI* optionUI;
	UI* soundUI;
	UI* sensitivityUI;

	//미션 ui 추가
	UI* killmissionUI;
	UI* killcountUI1;
	UI* killcountUI2;

	//게임오버 ui추가
	UI* gameoverUI;
	UI* gamewinUI;

	Sound* Ingamethema;
	//설정창 켜져있는지 불리언
	bool optionOpen = false;
	//사운드 켜져있는지 불리언
	bool soundOn = false;
	//마우스 감도변수
	float mouseSpeed;

	
	
    //몬스터 킬숫자
	int killCount;
	//죽여야할 미션몬스터숫자
	int missionKill;
	int uikillcount;

public:
	//기본기능함수들
	InGameScene();
	~InGameScene();
	//초기화
	virtual void Init();
	//해제
	virtual void Release();
	//Loop
	virtual void Update();
	virtual void LateUpdate();
	virtual void PreRender();
	virtual void Render();
	//Window Resize
	virtual void ResizeScreen();






	//프라이빗 변수생성
private:


	//퍼블릭 변수생성
public:

};

