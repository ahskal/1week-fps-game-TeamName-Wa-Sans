#pragma once
class House : public Actor
{
public:
    static House* Create(string name = "house");
private:
    House();
    virtual ~House();
public:
    void	Update() override;
};

