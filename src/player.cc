#include "player.hpp"

#include <iostream>

Player::Player(){
    
}

void Player::SetAttributes(float coord_x, float coord_y, float speed, float size_x, float size_y){
    coord_x_ = coord_x;
    coord_y_ = coord_y;
    speed_ = speed;
    raw_speed_ = speed;
    size_x_ = size_x;
    size_y_ = size_y;
}

void Player::SetSpeed(float speed){
    speed_ = speed;
}

void Player::SetXCoord(float coord_x){
    coord_x_ = coord_x;
}

void Player::SetYCoord(float coord_y){
    coord_y_ = coord_y;
}

void Player::UpdateSpeed(float refresh_rate_){
    if(refresh_rate_!=0){
        speed_ = 1.0F/refresh_rate_ * raw_speed_;
        //std::cout << refresh_rate_ << " " << speed_ << std::endl;
    }
}

void Player::MoveUp() {
    coord_y_ += speed_;
}

void Player::MoveDown() {
    coord_y_ -= speed_;
}
    
void Player::MoveLeft() {
    coord_x_ -= speed_;
}
    
void Player::MoveRight() {
    coord_x_ += speed_;
}

float Player::GetSizeX(){
    return size_x_;
}
float Player::GetSizeY(){
    return size_y_;
}

float Player::GetSpeed(){
    return speed_;
}

float * Player::GetHitbox(){
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


