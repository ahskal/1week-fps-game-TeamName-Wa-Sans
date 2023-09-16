#pragma once
#define HouseCount 50
#define Range 180
#define RangeLimit 250 


class VillageMap : public Actor
{
public:
    static VillageMap* Create(string name = "VillageMap");
private:
    class House* house[HouseCount];
    bool HouseRender;
    bool HouseLateUpdate;
    
    class House* house2;

    class Camera* cam;

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

    bool    WallCollision(Actor* player);



};



