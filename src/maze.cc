#include "maze.hpp"
#include <utility>
#include <math.h>   
#include <stack>
#include <iostream>
Maze::Maze(int width, int height) {
    width_ = width;
    height_ = height;
}

int Maze::GetWidth() {
    return width_;
}

int Maze::GetHeight() {
    return height_;
}

Locinfo Maze::GetLocinfo(int y, int x){
    return mazedata_[y][x];
}

std::string Maze::RandomNeighbor(int y, int x) {
    // check bound 
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
    if(y == 0) {
        up = true;
    } else {
        up = mazedata_[y-1][x].visited;
    }
    if(y == height_-1) {
        down = true;
    } else {
        down = mazedata_[y+1][x].visited;
    }
    if(x == 0) {
        left = true;
    } else {
        left = mazedata_[y][x-1].visited;
    }
    if(x == width_-1) {
        right = true;
    } else {
        right = mazedata_[y][x+1].visited;
    }
    int num = 0;
    std::vector<std::string> choices;
    if (left == false)  {
        num = num + 1;
        choices.push_back("left");
    }
    if (right == false)  {
        num = num + 1;
        choices.push_back("right");
    }
    if (up == false)  {
        num = num + 1;
        choices.push_back("up");
    }
    if (down == false)  {
        num = num + 1;
        choices.push_back("down");
    }
    if(num == 0) {
        return "no";
    } else {
        int c = rand()%num;
        return choices[c];
    }
}

void Maze::GenerateMaze(int width, int height){
    Locinfo l = {false,false,false,false, false};
    std::vector<Locinfo> currow(width,l);
    mazedata_.assign(height_, currow);
    // TODO: now the start point is set at the top left corner. May modify later
    std::stack<std::pair<int,int>> trace;
    // Choose the initial cell, mark it as visited and push it to the stack
    std::pair <int,int> startpoint (width/2,height/2);  
    mazedata_[width/2][height/2].visited = true;
    trace.push(startpoint);
    std::pair<int,int> cur;
    // While the stack is not empty
    while(!trace.empty()) {
        // Pop a cell from the stack and make it a current cell
        cur = trace.top();
        trace.pop();
        // mark the current cell as visited
        mazedata_[cur.first][cur.second].visited = true;
        // If the current cell has any neighbours which have not been visited
        Locinfo* curinfo = &mazedata_[cur.first][cur.second];
        std::string nei = RandomNeighbor(cur.first, cur.second);
        if(nei == "no") {
            continue;
        }
        // Push the current cell to the stack
        trace.push(cur);
        std::pair<int,int> chosen_loc;
        Locinfo* chosen_n;
        // Choose one of the unvisited neighbours  Remove the wall between the current cell and the chosen cell
        if(nei == "left") {
            curinfo->left = true;
            chosen_n = &mazedata_[cur.first][cur.second-1];
            chosen_n->right = true;
            chosen_loc = std::make_pair(cur.first,cur.second-1);
        }
        if(nei == "right") {
            curinfo->right = true;
            chosen_n = &mazedata_[cur.first][cur.second+1];
            chosen_n->left = true;
            chosen_loc = std::make_pair(cur.first,cur.second+1);
        }
        if(nei == "up") {
            curinfo->up = true;
            chosen_n = &mazedata_[cur.first-1][cur.second];
            chosen_n->down = true;
            chosen_loc = std::make_pair(cur.first-1,cur.second);
        }
        if(nei == "down") {
            curinfo->down = true;
            chosen_n = &mazedata_[cur.first+1][cur.second];
            chosen_n->up = true;
            chosen_loc = std::make_pair(cur.first+1,cur.second);
        }
         // Mark the chosen cell as visited and push it to the stack
        //chosen_n.visited = true;
        trace.push(chosen_loc);
    }
}

void Maze::MazeOut() {
    for(int i = 0; i < width_; ++i){
        std::cout << " _";
    }
    std::cout << std::endl;
    for (int i = 0; i < width_; ++i){
        for (int j = 0; j < width_; ++j){
            Locinfo c = mazedata_[i][j];
            if (c.left)
                std::cout << " ";
            else std::cout << "|";
            std::cout << (c.down ? " " : "_");
            
        }
        std::cout << "|";
        std::cout << std::endl;
    }
}


std::set<std::vector<float>> Maze::GetWallCoor() {
    std::set<std::vector<float>> toreturn;
    // -0.7 to 0.7
    float range = 1.4f;
    // the height of every brick
    float b_height = 1.4f/height_;
    float b_width = 0.01f;
    for(int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            std::vector<float> leftcoor;
            if(mazedata_[i][j].left == false) {
                //push the four points of the left wall into coor in format of x1,x2,y1,y2
                // x1
                leftcoor.push_back(-0.7f + (b_height) * j);
                // x2
                leftcoor.push_back(-0.7f +(b_height) * j + b_width);
                // y1
                leftcoor.push_back(0.7f - (b_height) * i);
                // y2
                if(i!=height_-1)
                    leftcoor.push_back(0.7f - (b_height) * (i+1) - b_width);
                else
                    leftcoor.push_back(0.7f - (b_height) * (i+1));
                toreturn.insert(leftcoor);
            }
            std::vector<float> topcoor;
            if(mazedata_[i][j].up == false) {
                // push top wall into coor
                // x1
                topcoor.push_back(-0.7f +(b_height) * j);
                // x2
                if(j!=width_-1)
                    topcoor.push_back(-0.7f +(b_height) * (j+1) + b_width);
                else   
                    topcoor.push_back(-0.7f +(b_height) * (j+1) + b_width);
                // y1
                topcoor.push_back(0.7f - (b_height) * i);
                // y2
                topcoor.push_back(0.7f - (b_height) * i - b_width);
                toreturn.insert(topcoor);
            }
        }
    }
    // add the rightmost wall 
    float x1 = -0.7f + (b_height) * width_;
    float x2 = x1 + b_width;
    for(int i = 0; i < height_; i++) {
        std::vector<float> temp;
        // x1
        temp.push_back(x1);
        // x2
        temp.push_back(x2);
        // y1
        temp.push_back(0.7f - (b_height) * i);
        //y2
        temp.push_back(0.7f - (b_height) * (i+1));
        toreturn.insert(temp);
    }
    //add down wall 
    float y1 = -0.7f;
    float y2 = y1 + b_width;
    for(int i = 0; i < width_; i++) {
        std::vector<float> temp;
        // x1
        temp.push_back(-0.7f +(b_height) * i);
        // x2
        temp.push_back(-0.7f +(b_height) * (i+1));
        // y1
        temp.push_back(y1);
        // y2
        temp.push_back(y2);
        toreturn.insert(temp);
    }
    return toreturn;
} 

float* Maze::WallCoorArray(std::set<std::vector<float>> coor) {
    float *output = new float[coor.size() * 4 * 3];
    std::set<std::vector<float>>::iterator wIt = coor.begin();
    int pos = 0;
    while(wIt != coor.end()) {
        std::vector<float> cur = (*wIt);
        float x1 = cur[0];
        float x2 = cur[1];
        float y1 = cur[2];
        float y2 = cur[3];
        output[pos]=x1;
        output[pos+1]=y1;
        output[pos+2]=0.0f;
        output[pos+3]=x2;
        output[pos+4]=y1;
        output[pos+5]=0.0f;
        output[pos+6]=x2;
        output[pos+7]=y2;
        output[pos+8]=0.0f;
        output[pos+9]=x1;
        output[pos+10]=y2;
        output[pos+11]=0.0f;
        pos+=12;
        wIt++;
    }
    return output;
}

unsigned int* Maze::WallCoorIndex(size_t size){
    unsigned int* output = new unsigned int[size*6];
    int pos = 0;
    for(size_t i = 0; i< size; i++){
        output[pos] = 4*i;
        output[pos+1] = 4*i+1;
        output[pos+2] = 4*i+3;
        output[pos+3] = 4*i+1;
        output[pos+4] = 4*i+2;
        output[pos+5] = 4*i+3;
        pos+=6;
    }
    return output;
}

std::vector<std::pair<int, const void*>> Maze::GetSizeData(){
    return {{GetWallCoor().size()*48, WallCoorArray(GetWallCoor())},{GetWallCoor().size()*24, WallCoorIndex(GetWallCoor().size())}, {GetWallCoor().size()*6, (void*) 0}};
}

std::pair<int,int> Maze::CastCoor_Otom(float y, float x){
    float range = 1.4f;
    y = -y + 0.7;
    x = x + 0.7;
    // the height of every brick 
    float b_height = 1.4f/height_;
    // first check horizontal --x
    int mx = std::floor(x/b_height);
    // in the most right wall
    if(mx == width_) {
        mx = mx - 1;
    }
    // vertical --y
    int my = std::floor(y/b_height);
    //in the most bottom wall
    if(my == height_) {
        my = my -1;
    }
    std::pair<int,int> output = std::make_pair(my,mx);
    return output; 
}

std::pair<float,float> Maze::CastCoor_Mtoo(int y, int x) {
    float range = 1.4f;
    // the height of every brick 
    float b_height = 1.4f/height_;
    // the width of every brick
    float b_width = 0.01f;
    // horizontal --x
    float x_center = (b_height + b_width)/2;
    float output_x = x * b_height + x_center - 0.7f;
    // vertical --y
    float y_center = (b_height + b_width)/2;
    float output_y = y * b_height + y_center - 0.7f;
    output_y = (-1) * output_y;
    std::pair<float,float> output = std::make_pair(output_y,output_x);
    return output; 
}

std::pair<float,float> Maze::CastToCenter(float y, float x){
    std::pair<int,int> cell = CastCoor_Otom(y, x);
    return CastCoor_Mtoo(cell.first, cell.second);
}

bool Maze::CheckWallOverlap(float y, float x) {
    float b_height = 1.4f/height_;
    float b_width = 0.01f;
    // vertical wall
    float mx = std::fmod(x, b_height);
    int cell_x = floor(x/b_height);
    float my = std::fmod(y,b_height);
    int cell_y = floor(y/b_height);
    // to do
    return true;
}