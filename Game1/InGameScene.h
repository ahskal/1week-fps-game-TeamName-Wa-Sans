#pragma once
//인게임에서 작동하고 메인에서 실행할 게임 전체흐름입니다 인게임씬
class InGameScene : public Scene
{
	//프라이빗 함수생성
private:
	//인게임 객체들 
	Camera* MainCam;
	
	class VillageMap* Map;
	class Player* player;
private:
	//인게임 UI
	UI* playerAim;
	UI* optionUI;


	
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
	
	//객체들 상호작용함수



	//프라이빗 변수생성
private:


	//퍼블릭 변수생성
public:

};

