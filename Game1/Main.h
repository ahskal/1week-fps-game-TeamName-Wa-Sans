#pragma once

class Main : public Scene
{
private:
	Camera* cam1;
	Camera* cam_first;
	Grid* grid;


	/** �÷��̾� */
	class Player* player;


	/** �ӽ� ��*/
	Actor* wall;
public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
