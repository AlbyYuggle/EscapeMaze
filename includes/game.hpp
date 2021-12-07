#ifndef GAME_HPP
#define GAME_HPP

#include "utils.hpp"
#include "player.hpp"
#include "ai.hpp"
#include "item.hpp"

#include <iostream>
#include <chrono>
#include <utility>
#include <map>
struct Level{
    const float player_speed_; //speed of the player
    const int num_ai_;//number of AI
    const vector<pair<int, int>> ai_start_coords_;
    const float ai_speed_; //speed of the AI
    const int maze_height_;//height of the maze
    const int maze_width_;//width of the maze
    const pair<int, int> start_coord_; //starting coordinate
    const pair<int, int> win_coord_; //winning coordinate
    const float regen_time_interval; //time interval of the maze regenerating
    const int num_items_;//number of items in the game

};
class Game { //Game class
    private:
        size_t cur_level_ = 0;
        int num_items_ = 0;
        vector<Level> levels_;
        vector<Ai*> ai_;
        Player* player_;
        GLFWwindow* game_window_;
        bool invincible = false;
        bool spedUp = false;
        bool jchanged;
        bool teleport_colors = false;
        unsigned int tp_color_change = 0;
        unsigned int * element_buffer_objects_;
        unsigned int * vertex_array_objects_;
        unsigned int * vertex_buffer_objects_;
        map<string, vector<pair<int, const void*>>> name_to_size_data_;
        chrono::system_clock::time_point start_time_;
        chrono::system_clock::time_point game_creation_time_;
        map<string, unsigned int> programs_;
        float timerhitbox[12];

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
        std::vector<std::pair<int, const void*>> GetAiSizeData();
        void ProcessItems(Maze &maze, chrono::system_clock::time_point &invincible_start_time_, chrono::system_clock::time_point &speed_start_time_);
        void CheckOverlap(Maze &maze);

    public:
        void Init(); //initialize random seed and the start time is also randomized
        void GenerateNextLevel(); //function to generate the next level
        void AddLevel(Level level); //add the level onto each other
        float refresh_rate_; //the rate of the refreshing the game
        std::vector<Item> items_; //vector of Item objects

};

#endif
