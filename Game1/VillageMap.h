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
    float Timer = 0.0f;

    class House* house2;

    class Camera* cam;

    vector<Actor*> Item;

    float timer = 0.0f;
    float nameIdx = 0;

public:

private:
    VillageMap();
    virtual ~VillageMap();

    void HouseCollision();

public:
    //기존 Actor 변수를 오버라이드 하는코드
    void    Release()override;
    void	Update() override;
    void    Render() override;

    //새로 생성한 맵에 
    void    Init();
    void	LateUpdate();
    void    Hierarchy();
    void    ResizeScreen();

    Camera* GetCam() const { return cam; }

    bool WallCollision(UNIT* unit);
    bool ItemCollision(Actor* player);
    bool HouseToMonsterCollision(Actor* actor);



};



