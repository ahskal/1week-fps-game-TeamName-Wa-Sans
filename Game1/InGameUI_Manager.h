#pragma once
class InGameUI_Manager : public UI
{
public:
	UI* playerAim;
	UI* optionUI;

public:
	InGameUI_Manager();
	~InGameUI_Manager();

	void  	Update();
	void	Render();
};

