#ifndef CONFIG_HPP
#define CONFIG_HPP

#include<map>
#include<string>
#include<vector>
#include<utility>

using namespace std;

const unsigned int kScreenWidth = 800;
const unsigned int kScreenHeight = 800;

const char *kVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n" 
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *kFragmentShaderSourcePlayer = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *kFragmentShaderSourceWalls = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.2f, 0.6f, 0.6f, 1.0f);\n"
    "}\n\0";

const vector<const char*> kFragmentSources = {kFragmentShaderSourceWalls, kFragmentShaderSourceWalls, kFragmentShaderSourcePlayer};

const vector<string> kNames = {"walls", "win_tile", "player"};

const map<string, unsigned int> kIndexMap = {{"walls", 0}, {"win_tile", 1}, {"player", 2}};

bool game_over = false;
bool level_over = false;

constexpr unsigned int kNumObjects = 3;

unsigned int rectangle_ind[] = {
    0, 1, 3,
    1, 2, 3,
};


#endif