#pragma once

#define HouseCount 30
class VillageMap : public Actor
{
public:
    static VillageMap* Create(string name = "VillageMap");
private:
    class Camera* cam;
    //class House* house[4];

    class House* house[HouseCount];
    bool HouseCreateChack = false;
    bool collisionDetected = false;
    
    

public:

private:
    VillageMap();
    virtual ~VillageMap();
public:
    //���� Actor ������ �������̵� �ϴ��ڵ�
    void    Release()override;
    void	Update() override;
    void    Render() override;

    //���� ������ �ʿ� 
    void    Init();
    void	LateUpdate();
    void    Hierarchy();
    void    ResizeScreen();

    Camera* GetCam() const { return cam; }

};



