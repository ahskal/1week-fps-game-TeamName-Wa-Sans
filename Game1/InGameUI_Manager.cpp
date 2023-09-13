#include "stdafx.h"
#include "InGameUI_Manager.h"



InGameUI_Manager::InGameUI_Manager()
{
	//�÷��̾� ũ�ν����
	playerAim = UI::Create();
	//�ΰ��� ȯ�漳�� UI
	optionUI  = UI::Create();

	//���� xml �߰������� �̹��� �۾��� xml ������ ���忹��
	//playerAim->LoadFile("playerAim.xml");
	//optionUI->LoadFile("optionUI.xml");
	
}

InGameUI_Manager::~InGameUI_Manager()
{


}

void InGameUI_Manager::Update()
{
	playerAim->Update();
	optionUI->Update();
}

void InGameUI_Manager::Render()
{
	playerAim->Render();
	optionUI->Render();
}
