#pragma once

class Main : public Scene
{
private:
	Camera* cam1;
	Camera* cam_first;
	Grid* grid;


	/** 플레이어 */
	class Player* player;


	/** 임시 벽*/
	Actor* wall;
public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
