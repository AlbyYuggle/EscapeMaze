#include "game.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

void Game::Init(){
    // initialize random seed and start time
    srand((unsigned int)time(NULL));
    InitializeWindow();
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        exit(EXIT_FAILURE);
    }

   
    programs_ = BuildShaders(kVertexShaderSource, kFragmentSources, kNames);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    player_ = new Player();

    element_buffer_objects_ = new unsigned int[kNumObjects];
    vertex_array_objects_  = new unsigned int[kNumObjects];
    vertex_buffer_objects_  = new unsigned int[kNumObjects];
}

void Game::InitializeWindow(){
    game_creation_time_ = chrono::system_clock::now();
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "EscapeMaze", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            window = window;
            glViewport(0, 0, width, height);
        });
    game_window_ = window;
}

void Game::GenerateNextLevel(){
    level_over = false;
    if(cur_level_ >= levels_.size()){
        auto timediff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - game_creation_time_).count();
        cout << "Congratulations - You Win!" << "\nTime Taken: " << timediff << " seconds \nScore: " << 1000000.0f/timediff << endl;
        exit(EXIT_SUCCESS);
    }   

    Level next_level = levels_.at(cur_level_++);
    num_items_ = next_level.num_items_; 
    items_.clear();
    set<pair<int, int>> existing_items;
    existing_items.insert({next_level.start_coord_.first, next_level.start_coord_.second});
    existing_items.insert({next_level.win_coord_.first, next_level.win_coord_.second});

    while(items_.size() < num_items_){
        Item new_item;
        new_item.SetRandomAttributes(next_level.maze_height_);
        if(existing_items.count(CastCoorFloatToGrid(new_item.GetCenter().first, new_item.GetCenter().second, next_level.maze_height_)) == 0){   
            items_.push_back(new_item);
            existing_items.insert(CastCoorFloatToGrid(new_item.GetCenter().first, new_item.GetCenter().second, next_level.maze_height_));
        }
    }
    existing_items.clear();
    player_->SetAttributes(CastCoorGridToFloat(next_level.start_coord_.first, next_level.start_coord_.second, next_level.maze_height_), next_level.player_speed_, (next_level.maze_height_-3)*(0.5F/next_level.maze_height_)/next_level.maze_height_);
    ai_.clear();
    if(next_level.num_ai_ > 0){    
        for(int i = 0; i < next_level.num_ai_; i++){
            Ai* ai = new Ai();
            ai->SetAttributes(CastCoorGridToFloat(next_level.ai_start_coords_[i].first, next_level.ai_start_coords_[i].second, next_level.maze_height_), next_level.ai_speed_, (next_level.maze_height_-3)*(0.5F/next_level.maze_height_)/next_level.maze_height_);
            ai_.push_back(ai);
        }
    }
    Maze maze(next_level.maze_width_, next_level.maze_height_);

    start_time_ = chrono::system_clock::now();
    Play(next_level, maze);
}

void Game::Play(Level &level, Maze &maze){
    maze.GenerateMaze(maze.GetWidth(), maze.GetHeight());

    glGenBuffers(kNumObjects, element_buffer_objects_);
    glGenVertexArrays(kNumObjects, vertex_array_objects_);
    glGenBuffers(kNumObjects, vertex_buffer_objects_);
    map<string, vector<pair<int, const void*>>> name_to_size_data;
    for (string name : kNames) {
        name_to_size_data_[name] = vector<pair<int, const void*>>();
    }
    float * fetched_player_hitbox = player_->GetHitbox();
    float * fetched_win_tile_hitbox = GetHitbox(CastCoorGridToFloat(level.win_coord_.first, level.win_coord_.second, level.maze_height_), player_->GetSizeX(), player_->GetSizeY());
    float player_hitbox[12];
    float win_tile_hitbox[12];
    for(int i = 0; i < 12; i++){
        player_hitbox[i] = fetched_player_hitbox[i];
        win_tile_hitbox[i] = fetched_win_tile_hitbox[i];
    }
    float *items_array_ = new float[num_items_*12];
    for (int x = 0; x < num_items_; x++) {
        Item i = items_[x];
        float* cur = i.GetHitbox();
        int pos = x*12;
        items_array_[pos]=cur[0];
        items_array_[pos+1]=cur[1];
        items_array_[pos+2]=cur[2];
        items_array_[pos+3]=cur[3];
        items_array_[pos+4]=cur[4];
        items_array_[pos+5]=cur[5];
        items_array_[pos+6]=cur[6];
        items_array_[pos+7]=cur[7];
        items_array_[pos+8]=cur[8];
        items_array_[pos+9]=cur[9];
        items_array_[pos+10]=cur[10];
        items_array_[pos+11]=cur[11];
    }
    timerhitbox[0] = -0.875f;
    timerhitbox[1] = 0.8f;
    timerhitbox[2] = 0.0f;
    timerhitbox[3] = -0.25f;
    timerhitbox[4] = 0.8f;
    timerhitbox[5] = 0.0f;
    timerhitbox[6] = -0.25f;
    timerhitbox[7] = 0.875;
    timerhitbox[8] = 0.0f;
    timerhitbox[9] = -0.875f;
    timerhitbox[10] = 0.875f;
    timerhitbox[11] = 0.0f;
    name_to_size_data_["player"] = {{sizeof(player_hitbox), player_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    name_to_size_data_["win_tile"] = {{sizeof(win_tile_hitbox), win_tile_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    name_to_size_data_["walls"] = maze.GetSizeData();
    name_to_size_data_["ai"] = GetAiSizeData();
    name_to_size_data_["items"] = {{num_items_*48, items_array_},{num_items_*24, maze.WallCoorIndex(num_items_)}, {num_items_*6, (void*) 0}};
    name_to_size_data_["timer"] = {{sizeof(timerhitbox), timerhitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    for (string name : kNames) {
        BindElement(name);
    }
    int frame_counter = 0;
    chrono::system_clock::time_point start_time = chrono::system_clock::now();
    chrono::system_clock::time_point speed_start_time = chrono::system_clock::now() ;
    chrono::system_clock::time_point invincible_start_time = chrono::system_clock::now();
    while(!glfwWindowShouldClose(game_window_)){
        ProcessInputAndRegenerate(level, maze);
        ProcessItems(maze, speed_start_time, invincible_start_time);
        chrono::duration<double, std::milli> telapsed = chrono::system_clock::now() - start_time;
        if(frame_counter <= 100){
            refresh_rate_ = static_cast<float>((1.0f*frame_counter)/(telapsed.count()));
            player_->UpdateSpeed(refresh_rate_);
            for(Ai* ai : ai_){
                ai->UpdateSpeed(refresh_rate_);
            }
            frame_counter++;
        }
    }
    glDeleteVertexArrays(kNumObjects, vertex_array_objects_);
    glDeleteBuffers(kNumObjects, vertex_buffer_objects_);
    glDeleteBuffers(kNumObjects, element_buffer_objects_);
    for(const auto& name_program_pair : programs_){    
        glDeleteProgram(name_program_pair.second);
    }
    glfwTerminate();
}

vector<pair<int, const void*>> Game::GetAiSizeData(){
    vector<float*> ai_coords_v;
    for(int i = 0; i < ai_.size(); i++){
        ai_coords_v.push_back(ai_[i]->GetHitbox());
    }
    return {{ai_coords_v.size()*48, CoorArray(ai_coords_v)},{ai_coords_v.size()*24, CoorIndex(ai_coords_v.size())}, {ai_coords_v.size()*6, (void*) 0}};
}

void Game::Draw(string object_name){
    glUseProgram(programs_.at(object_name));
    glBindVertexArray(vertex_array_objects_[kIndexMap.at(object_name)]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, name_to_size_data_[object_name][2].first, GL_UNSIGNED_INT, 0);
}

void Game::ProcessInput(Level &level, Maze &maze){
   

    float inc = player_->GetSpeed();

    float* player_hitbox = player_->GetHitbox();
    vector<float> player_current_coords;
    player_current_coords.push_back(player_hitbox[0]);
    player_current_coords.push_back(player_hitbox[3]);
    player_current_coords.push_back(player_hitbox[1]);
    player_current_coords.push_back(player_hitbox[10]);

    vector<vector<float>> ai_coords;
    for(Ai* ai : ai_){
        ai_coords.push_back(ai->GetCorners());
    }
    if(!invincible && !teleport_colors && CollideAi(player_current_coords, ai_coords, 0, 0)){
        if(!game_over)
            std::cout << "Game Over - You Lose.\nPress Enter to retry level.\nPress Space to restart game." << std::endl;
        game_over = true;
        glClearColor(0.3F, 0.0F, 0.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glfwSetWindowShouldClose(game_window_, true);
        // exit(EXIT_SUCCESS);
    }

    set<vector<float>> walls;
    walls = maze.GetWallCoor();
    float* win_tile_hitbox = GetHitbox(CastCoorGridToFloat(level.win_coord_.first, level.win_coord_.second, level.maze_height_), player_->GetSizeX(), player_->GetSizeY());
    vector<float> win_tile_coords;
    win_tile_coords.push_back(win_tile_hitbox[0]);
    win_tile_coords.push_back(win_tile_hitbox[3]);
    win_tile_coords.push_back(win_tile_hitbox[1]);
    win_tile_coords.push_back(win_tile_hitbox[10]);

    if(spedUp) inc*=2;
    inc = std::min(inc, 0.0095f);
    if(CollideOnMove(player_current_coords, win_tile_coords, 0, 0)){
        level_over = true;
    }

    if (glfwGetKey(game_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(game_window_, true);
        exit(EXIT_SUCCESS);
    }

    if (game_over && glfwGetKey(game_window_, GLFW_KEY_SPACE) == GLFW_PRESS){
        cur_level_ = 0;
        game_over = false;
        glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GenerateNextLevel();
    }

    if (game_over && glfwGetKey(game_window_, GLFW_KEY_ENTER) == GLFW_PRESS){
        cur_level_--;
        game_over = false;
        glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GenerateNextLevel();
    }

    if(!level_over && !game_over){
        const int shiftcount_max = 7;
        if (glfwGetKey(game_window_, GLFW_KEY_RIGHT) == GLFW_PRESS){
            if(player_current_coords[3]+inc <= 1 && (!CollideWalls(player_current_coords, walls, inc, 0))){
                    player_->MoveRight(inc);
            }
            else{
                int shiftcount = 0;
                float temp_inc = inc;
                while(shiftcount++ < shiftcount_max && !(player_current_coords[3]+inc <= 1 && (!CollideWalls(player_current_coords, walls, inc, 0)))){
                    inc *= 0.5F;
                }
                if(shiftcount < shiftcount_max){
                    player_->MoveRight(inc);
                }
                inc = temp_inc;
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_LEFT) == GLFW_PRESS){
            if(player_current_coords[0]-inc>=-1 && (!CollideWalls(player_current_coords, walls, -inc, 0))){
                player_->MoveRight(-1*inc);
            }
            else{
                int shiftcount = 0;
                float temp_inc = inc;
                while(shiftcount++ < shiftcount_max && !(player_current_coords[0]-inc>=-1 && (!CollideWalls(player_current_coords, walls, -inc, 0)))){
                    inc *= 0.5F;
                }
                if(shiftcount < shiftcount_max){
                    player_->MoveRight(-1*inc);
                }
                inc = temp_inc;
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_DOWN) == GLFW_PRESS){
            if(player_current_coords[1]-inc >= -1 && (!CollideWalls(player_current_coords, walls, 0, -inc))){
                    player_->MoveUp(-1*inc);
            }
            else{
                int shiftcount = 0;
                float temp_inc = inc;
                while(shiftcount++ < shiftcount_max && !(player_current_coords[1]-inc >= -1 && (!CollideWalls(player_current_coords, walls, 0, -inc)))){
                    inc *= 0.5F;
                }
                if(shiftcount < shiftcount_max){
                    player_->MoveUp(-1*inc);
                }
                inc = temp_inc;
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_UP) == GLFW_PRESS){
            if(player_current_coords[7]+inc <= 1 && (!CollideWalls(player_current_coords, walls, 0, inc))){
                player_->MoveUp(inc);
            }
            else{
                int shiftcount = 0;
                float temp_inc = inc;
                while(shiftcount++ < shiftcount_max && !(player_current_coords[7]+inc <= 1 && (!CollideWalls(player_current_coords, walls, 0, inc)))){
                    inc *= 0.5F;
                }
                if(shiftcount < shiftcount_max){
                    player_->MoveUp(inc);
                }
                inc = temp_inc;
            }
        }
    }
}

void Game::ProcessItems(Maze &maze, chrono::system_clock::time_point &invincible_start_time_, chrono::system_clock::time_point &speed_start_time_){
    float* player_hitbox = player_->GetHitbox();
    vector<float> player_current_coords;
    player_current_coords.push_back(player_hitbox[0]);
    player_current_coords.push_back(player_hitbox[3]);
    player_current_coords.push_back(player_hitbox[1]);
    player_current_coords.push_back(player_hitbox[10]);

    if(tp_color_change == 20){
        tp_color_change=0;
        teleport_colors = false;
        if(!invincible && !spedUp){
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        if(spedUp){
            glClearColor(0.0f, 0.5f, 0.0f, 0.4f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }else if(teleport_colors){
        tp_color_change++;
    }

    for(int i = 0; i<num_items_; i++){
        if(CollideOnMove(player_current_coords, items_[i].GetCorners(), 0, 0)){
            if(items_[i].GetTeleport()){
                glClearColor(0.7f, 0.0f, 0.7f, 0.5f);
                glClear(GL_COLOR_BUFFER_BIT);
                tp_color_change = 0;
                teleport_colors = true;
                float coord_x_ = -0.7f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.4f)));
                float coord_y_ = -0.7f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.4f)));
                float tx = CastToCenter(coord_x_, coord_y_, maze.GetHeight()).first;
                float ty = CastToCenter(coord_x_, coord_y_, maze.GetHeight()).second;
                player_->SetXCoord(tx);
                player_->SetYCoord(ty);
            }
            if(items_[i].GetInvincible()){
                invincible = true;
                invincible_start_time_ = chrono::system_clock::now();
            }
            if(items_[i].GetSpeedBoost()){
                if(!spedUp) player_->SetSpeed(player_->GetSpeed()*2);
                spedUp = true;
                speed_start_time_ = chrono::system_clock::now();
                glClearColor(0.0f, 0.5f, 0.0f, 0.4f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            items_.erase(items_.begin() + i);
            i--;
            num_items_--;
        }
    }
    chrono::system_clock::time_point cur_time = chrono::system_clock::now();
    if(invincible){
        chrono::duration<double, std::milli> diff = chrono::system_clock::now() - invincible_start_time_;
        if((diff.count()/500) - (int)(diff.count()/500) < 0.1 && !jchanged){
            float c1 = (rand()%10)/20.0f;
            float c2 = (rand()%10)/20.0f;
            float c3 = (rand()%10)/20.0f;
            glClearColor(c1, c2, c3, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            jchanged = true;
        }else{
            jchanged = false;
        }
        if((diff).count() > 5*1000){
            invincible = false;
            if(!teleport_colors && !spedUp){
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }
    }

    if(spedUp){
        chrono::duration<double, std::milli> diff = chrono::system_clock::now() - speed_start_time_;
        if((diff).count() > 10*1000){
            if(!invincible && !teleport_colors){
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            player_->SetSpeed(player_->GetSpeed()/2);
            spedUp = false;
        }
    }
}

void Game::ProcessInputAndRegenerate(Level &level, Maze &maze){    // render
    // ------
    glClear(GL_COLOR_BUFFER_BIT);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    //Redo map
    if(level_over){
        start_time_ = chrono::system_clock::now();
        GenerateNextLevel();
    }
    else{
        chrono::system_clock::time_point cur_time = chrono::system_clock::now();
        chrono::duration<double> elapsed = cur_time - start_time_;
        timerhitbox[3] = -0.25f - (elapsed.count()/level.regen_time_interval * 0.625f);
        timerhitbox[6] = -0.25f - (elapsed.count()/level.regen_time_interval * 0.625f);
        if(elapsed.count() > level.regen_time_interval){
            maze.GenerateMaze(maze.GetWidth(), maze.GetHeight());
            start_time_ = chrono::system_clock::now();
            name_to_size_data_["walls"] = maze.GetSizeData();
            BindElement("walls");
        }
        
        name_to_size_data_["ai"] = GetAiSizeData();
        BindElement("ai");
        CheckOverlap(maze);

        float * fetched_player_hitbox = player_->GetHitbox();
        float player_hitbox[12];
        for(int i = 0; i < 12; i++){
            player_hitbox[i] = fetched_player_hitbox[i];
        }
        name_to_size_data_["player"] = {{sizeof(player_hitbox), player_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
        BindElement("player");
        float* items_array_ = new float[num_items_*12];
        int pos = 0;
    
    for (Item i : items_) {
        float* cur = i.GetHitbox();
        items_array_[pos]=cur[0];
        items_array_[pos+1]=cur[1];
        items_array_[pos+2]=cur[2];
        items_array_[pos+3]=cur[3];
        items_array_[pos+4]=cur[4];
        items_array_[pos+5]=cur[5];
        items_array_[pos+6]=cur[6];
        items_array_[pos+7]=cur[7];
        items_array_[pos+8]=cur[8];
        items_array_[pos+9]=cur[9];
        items_array_[pos+10]=cur[10];
        items_array_[pos+11]=cur[11];
        pos+=12;
    }
    name_to_size_data_["items"] = {{num_items_*48, items_array_},{num_items_*24, maze.WallCoorIndex(num_items_)}, {num_items_*6, (void*) 0}};
    BindElement("items");
        for(string name : kNames){
            Draw(name);
        }
    }
    
    name_to_size_data_["timer"] = {{sizeof(timerhitbox), timerhitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    BindElement("timer");

    ProcessInput(level, maze);
    for(Ai* ai : ai_){
        ai->Seek(player_->GetCenter(), level.maze_height_, maze);
    }
    glfwSwapBuffers(game_window_);
    glfwPollEvents();
}

map<string, unsigned int> Game::BuildShaders(const char* vertex_source, vector<const char*> fragment_sources, vector<string> fragment_names){
    // build and compile our shader program
    map<string, unsigned int> shader_programs;
    unsigned int vertex_shader = CreateVertexShader(vertex_source);
    for(unsigned int i = 0; i < fragment_sources.size(); i++){
        unsigned int fragment = CreateFragmentShader(fragment_sources[i]);
        unsigned int shader = LinkShader(vertex_shader, fragment);
        glDeleteShader(fragment);
        shader_programs[fragment_names[i]] = shader;
    }
    return shader_programs;
}

unsigned int Game::CreateVertexShader(const char* source){
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &source, NULL);
    glCompileShader(vertex_shader);

    CheckShaderCompile(vertex_shader);
    return vertex_shader;
}

unsigned int Game::CreateFragmentShader(const char* source){
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &source, NULL);
    glCompileShader(fragment_shader);

    CheckShaderCompile(fragment_shader);
    return fragment_shader;
}

unsigned int Game::LinkShader(unsigned int &vertex_shader, unsigned int& fragment_shader){
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    CheckLinkStatus(shader_program);
    return shader_program;
}

// check for shader compile errors
void Game::CheckShaderCompile(unsigned int &shader){
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << endl;
        exit(EXIT_FAILURE);
    }
}

// check for linking errors
void Game::CheckLinkStatus(unsigned int &program){
    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << endl;
    }
}

void Game::BindElement(string object_name){
    int index = kIndexMap.at(object_name);
    glBindVertexArray(vertex_array_objects_[index]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects_[index]);
    glBufferData(GL_ARRAY_BUFFER, name_to_size_data_[object_name][0].first, name_to_size_data_[object_name][0].second, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_objects_[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, name_to_size_data_[object_name][1].first, name_to_size_data_[object_name][1].second, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Game::AddLevel(Level level){
    levels_.push_back(level);
}


void Game::CheckOverlap(Maze &maze) {
    float* player_hitbox = player_->GetHitbox();
    vector<float> player_current_coords;
    player_current_coords.push_back(player_hitbox[0]);
    player_current_coords.push_back(player_hitbox[3]);
    player_current_coords.push_back(player_hitbox[1]);
    player_current_coords.push_back(player_hitbox[10]);
    set<vector<float>> walls;
    walls = maze.GetWallCoor();
    // if collide with the wall
    while(CollideWalls(player_current_coords, walls, 0, 0)) {
        std::pair<float,float> a = CastToCenter(player_hitbox[0],player_hitbox[1],maze.GetHeight());
        float dx = a.first - player_hitbox[0];
        float dy = a.second - player_hitbox[1];
        if(dy > 0) {
            player_->MoveUp(dy/10.0f);
        } else {
            player_->MoveUp(dy/10.0f);
        }
        if(dx > 0) {
            player_->MoveRight(dx/10.0f);
        } else {
            player_->MoveRight(dx/10.0f);
        }
        // std::cout<<a.first<<std::endl;
        // std::cout<<a.second<<std::endl;
        float* t_player_hitbox = player_->GetHitbox();
        player_current_coords[0] = t_player_hitbox[0];
        player_current_coords[1] = t_player_hitbox[3];
        player_current_coords[2] = t_player_hitbox[1];
        player_current_coords[3] = t_player_hitbox[10];
    }
}
