#pragma once
//게임시작전 옵션선택및 게임시작 종료 를 담당할 씬입니다
class LobbyScene : public Scene
{
	//프라이빗 함수생성공간
private:

	//퍼블릭 함수생성공간
public:
	LobbyScene();
	~LobbyScene();
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

