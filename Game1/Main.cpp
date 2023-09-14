#include "stdafx.h"
#include "Main.h"


Main::Main()
{
    

}

Main::~Main()
{
    

}

void Main::Init()
{
    

    //�� �߰� ����� ���Ƿ� �ΰ��� �۾������� �ΰ��Ӿ��� �����Ӵϴ� ���� �����Ҽ�����
    SCENE->AddScene("InGame", new InGameScene);
    SCENE->AddScene("LobbyScene", new LobbyScene);

    //add ������ ���ص� Ű������ ��ȯ�ؼ� �����پ� ���մϴ�
    SCENE->ChangeScene("LobbyScene");
    
 
}

void Main::Release()
{
}

void Main::Update()
{





    if (INPUT->KeyDown(VK_F1))
    {
        
        SCENE->ChangeScene("InGame");
    }
    SCENE->Update();


}

void Main::LateUpdate()
{
    SCENE->LateUpdate();
}

   

    

void Main::PreRender()
{

}

void Main::Render()
{
    SCENE->Render();
    
}

void Main::ResizeScreen()
{
    SCENE->ResizeScreen();
    
    
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    App.SetAppName(L"Game1");
    App.SetInstance(instance);
    WIN->Create();
    D3D->Create();
    Main* main = new Main();
    main->Init();

    int wParam = (int)WIN->Run(main);


    main->Release();
    SafeDelete(main);
    D3D->DeleteSingleton();
    WIN->DeleteSingleton();

    return wParam;
}