#pragma once
class InGameUI_Manager : public UI
{
public:
	//lobbyui
	UI* lobbythema;
	UI* gamestart;
	UI* exit;

	//ingame ui
	UI* playerAim;
	UI* optionUI;

public:
	InGameUI_Manager();
	~InGameUI_Manager();


	void  	Update();
	void	Render();
};

