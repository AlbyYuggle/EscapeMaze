#ifndef PLAYER_HPP
#define PLAYER_HPP


#include<utility>

using namespace std;
class Player {


private:
    float coord_x_ = 0; //default x coordinate is 0
    float coord_y_ = 0; //default y coordinate is 0
    float coord_z_ = 0; //default z coordinate is 0
    float speed_ = 0; //default speed is 0
    float size_x_ = 0; //default x size is 0
    float size_y_ = 0; //default y size is 0
    float raw_speed_ = 0; //default raw speed is 0
public:
    Player();
    void SetAttributes(pair<float, float> coords, float speed, float size_);
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    float GetSpeed();
    float GetSizeX();
    float GetSizeY();
    float GetCoordsX();
    float GetCoordsY();
    float GetCoordsZ();
    float* GetHitbox();
    pair<float, float> GetCenter();
    void UpdateSpeed(float refresh_rate_);
    void SetSpeed(float speed);
    void SetXCoord(float coord_x);
    void SetYCoord(float coord_y);
    void MoveRight(float dx);
    void MoveUp(float dy);
};

#endif