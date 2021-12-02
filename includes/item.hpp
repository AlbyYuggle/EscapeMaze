#ifndef ITEM_HPP
#define ITEM_HPP

#include "player.hpp"
#include <string>
#include <vector>

using namespace std;
class Item {

private:
    float coord_x_ = 0.0F;
    float coord_y_ = 0.0F;
    float coord_z_ = 0.0F;
    float size_x_ = 0.02F;
    float size_y_ = 0.02F;
    int mSize = 1;
    string item_name_;
    float speed_multiplier_;
    bool invincible_;
    vector<string> possible_items_ = {"Monster" , "Coffee" , "Water"};

public:
    
    float GetSizeX();
    float GetSizeY();
    float* GetHitbox();
    void SetRandomAttributes(int h);
    float GetSpeedMultiplier();
    bool GetInvincible();
    
    
};




#endif