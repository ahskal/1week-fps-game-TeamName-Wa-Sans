#pragma once

class Main : public Scene
{
private:
	Grid* grid;
	Actor* root;
	//Actor* testCol;	//�÷��̾� ���� ����

	class Monster* monster;
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
