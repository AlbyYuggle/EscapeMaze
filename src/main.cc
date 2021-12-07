#include "game.hpp"
#include "maze.hpp"

int main()
{

    vector<pair<int, int>> ai_loc_4;
    for(int i = 0; i < 18; i++){
        ai_loc_4.push_back({i,18 - i});
    }

    vector<pair<int, int>> ai_loc_5;
    for(int i = 20; i < 25; i++){
        for(int j = 20; j < 25; j++){
            ai_loc_5.push_back({i,j});
        }
    }
    //Player speed, num_ai, ai_coords, ai_speed, maze width, maze height, player start coords, win tile coords, regen time, num_items
    Level level0{0.001F, 1, {{4, 4}}, 0.0001F, 5, 5, {0, 0}, {4, 4}, 10.0F, 1};
    Level level1{0.001F, 1, {{4, 4}}, 0.0005F, 6, 6, {0,0}, {5, 5}, 5.0F, 3};
    Level level2{0.001F, 3, {{8, 7}, {7, 8}, {8, 8}}, 0.0005F, 10, 10, {0,0}, {5, 5}, 5.0F, 8};
    Level level3{0.0005F, 5, {{12, 12}, {13, 13}, {14, 14}, {12, 13}, {13, 12}}, 0.0002F, 15, 15, {0,0}, {11, 11}, 5.0F, 16};
    Level level4{0.001F, 18, ai_loc_4, 0.0001F, 20, 20, {0,0}, {10, 10}, 1.0F, 20};
    Level level5{0.001F, 25, ai_loc_5, 0.0005F, 25, 25, {0, 0}, {24, 24}, 4.0F, 40};
    Level level6{0.001F, 1, {{29, 29}}, 0.001F, 30, 30, {0, 0}, {29, 29}, 1.0F, 500};
    Level level7{0.001F, 1, {{29, 29}}, 0.002F, 30, 30, {0, 0}, {29, 29}, 0.5F, 1};
    Level level8{0.0015F, 8, {{0,0},{31,31},{0,15},{15,0},{0,31},{31,0},{31,15},{15,31}}, 0.00035F, 32, 32, {15, 15}, {31, 31}, 3.0F, 64};
    Level level9{0.0015F, 4, {{0,0},{49,49},{0,49},{49,0}}, 0.00075F, 50, 50, {25, 25}, {47, 16}, 7.0F, 50};
    Game game;
    game.AddLevel(level0);
    game.AddLevel(level1);
    game.AddLevel(level2);
    game.AddLevel(level3);
    game.AddLevel(level4);
    game.AddLevel(level5);
    game.AddLevel(level6);
    game.AddLevel(level7);
    game.AddLevel(level8);
    game.AddLevel(level9);
    game.Init();
    game.GenerateNextLevel();
    return 0;
}
