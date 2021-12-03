#ifndef GAME_HPP
#define GAME_HPP

#include "utils.hpp"
#include "maze.hpp"
#include "player.hpp"


#include <iostream>
#include <chrono>
#include <utility>
#include <map>
using namespace std;
struct Level{
    const float player_speed_;
    const float player_width_;
    const float player_height_;
    const int num_ai_;
    const float ai_speed_;
    const int maze_height_;
    const int maze_width_;
    const pair<float, float> start_coord_;
    const pair<float, float> win_coord_; 
    const float regen_time_interval;
};
class Game { 
    

    private:
        int cur_level_ = 0;
        vector<Level> levels_;
        Player* player_;
        GLFWwindow* game_window_;
        unsigned int * element_buffer_objects_;
        unsigned int * vertex_array_objects_;
        unsigned int * vertex_buffer_objects_;
        map<string, vector<pair<int, const void*>>> name_to_size_data_;
        chrono::system_clock::time_point start_time_;
        map<string, unsigned int> programs_;

        void Config();
        void InitializeWindow();
        void ProcessInput(Level &level, Maze &maze);
        map<string, unsigned int> BuildShaders(const char* vertex_source, vector<const char*> fragment_sources, vector<string> fragment_names);
        unsigned int CreateVertexShader(const char* source);
        unsigned int CreateFragmentShader(const char* source);
        void CheckShaderCompile(unsigned int &shader);
        void CheckLinkStatus(unsigned int &shader);
        unsigned int LinkShader(unsigned int &vertex_shader, unsigned int& fragment_shader);
        void Draw(string object_name);
        void BindElement(string object_name);
        void Play(Level &level, Maze &maze);
        void ProcessInputAndRegenerate(Level &level, Maze &maze);
        //todo
        void Moveitem(Maze &maze);
    public:
        void Init();
        void GenerateNextLevel();
        void AddLevel(Level level);
        float refresh_rate_;

};





#endif