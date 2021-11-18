#include "item.hpp"



void Item::SetRandomAttributes() { //add energy to the user
    int idx = rand()%possible_items_.size(); //random idx number generator from 0 to 2
    item_name_ = possible_items_.at(idx);
    
    coord_x_ = -0.7f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.4f)));
    coord_y_ = -0.7f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.4f)));

    if (item_name_ == "Monster") {  ///Monster Drink
        speed_multiplier_ = 2.5f;
        size_x_ = 0.025f;
        size_y_ = 0.025f;
    } else if (item_name_ == "Coffee") {  ///Coffee Drink
        speed_multiplier_ += 2.0f;
        size_x_ = 0.015f;
        size_y_ = 0.015f;
    } else if (item_name_ == "Water") {  ///Coffee Drink
        speed_multiplier_ += 1.5f;
        size_x_ = 0.01f;
        size_y_ = 0.01f;
    }
    
}

float Item::GetSizeX(){
    return size_x_;
}
float Item::GetSizeY(){
    return size_y_;
}


float * Item::GetHitbox(){
    float x1 = coord_x_ - size_x_/2.0F;
    float x2 = coord_x_ + size_x_/2.0F;
    float y1 = coord_y_ - size_y_/2.0F;
    float y2 = coord_y_ + size_y_/2.0F;
    //TODO: fix this
    float * hitbox = new float[12];
    hitbox[0] = x1;
    hitbox[1] = y1;
    hitbox[2] = 0.0F;
    hitbox[3] = x2;
    hitbox[4] = y1;
    hitbox[5] = 0.0F;
    hitbox[6] = x2;
    hitbox[7] = y2;
    hitbox[8] = 0.0F;
    hitbox[9] = x1;
    hitbox[10] = y2;
    hitbox[11] = 0.0F;
    return hitbox;
}

float Item::GetSpeedMultiplier() {
    return speed_multiplier_;
}
    
bool Item::GetInvincible() {
    return invincible_;
}




