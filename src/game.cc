#include "game.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <iostream>
void Game::Config(){
}

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
            glViewport(0, 0, width, height);
        });
    game_window_ = window;
}

void Game::GenerateNextLevel(){
    level_over = false;
    if(cur_level_ >= levels_.size()){
        //TODO: Endgame handling
        cout << "End of Game" << endl;
        cout << refresh_rate_ << endl;
        exit(EXIT_SUCCESS);
    }   
    level_over = false;
    Level next_level = levels_.at(cur_level_++);
    player_->SetAttributes(next_level.start_coord_.first, next_level.start_coord_.second, next_level.player_speed_, next_level.player_width_, next_level.player_height_);
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
    float * fetched_win_tile_hitbox = GetHitbox(level.win_coord_, player_->GetSizeX(), player_->GetSizeY());
    float player_hitbox[12];
    float win_tile_hitbox[12];
    for(int i = 0; i < 12; i++){
        player_hitbox[i] = fetched_player_hitbox[i];
        win_tile_hitbox[i] = fetched_win_tile_hitbox[i];
    }
    name_to_size_data_["player"] = {{sizeof(player_hitbox), player_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    name_to_size_data_["win_tile"] = {{sizeof(win_tile_hitbox), win_tile_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
    name_to_size_data_["walls"] = maze.GetSizeData();
    for (string name : kNames) {
        BindElement(name);
    }
    
    
    int frame_counter = 0;
    chrono::system_clock::time_point start_time = chrono::system_clock::now();
    while(!glfwWindowShouldClose(game_window_)){
        ProcessInputAndRegenerate(level, maze);
        frame_counter++;
        if(frame_counter == 100){
            refresh_rate_ = 100.0f/((chrono::system_clock::now() - start_time).count());
        }else if(frame_counter < 100){
            refresh_rate_ = (1.0f*frame_counter)/((chrono::system_clock::now() - start_time).count());
        }
        player_->UpdateSpeed(refresh_rate_);
    }
    glDeleteVertexArrays(kNumObjects, vertex_array_objects_);
    glDeleteBuffers(kNumObjects, vertex_buffer_objects_);
    glDeleteBuffers(kNumObjects, element_buffer_objects_);
    for(const auto& name_program_pair : programs_){    
        glDeleteProgram(name_program_pair.second);
    }

    glfwTerminate();
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

    set<vector<float>> walls;
    walls = maze.GetWallCoor();
    float* win_tile_hitbox = GetHitbox(level.win_coord_, player_->GetSizeX(), player_->GetSizeY());
    vector<float> win_tile_coords;
    win_tile_coords.push_back(win_tile_hitbox[0]);
    win_tile_coords.push_back(win_tile_hitbox[3]);
    win_tile_coords.push_back(win_tile_hitbox[1]);
    win_tile_coords.push_back(win_tile_hitbox[10]);

    if(CollideOnMove(player_current_coords, win_tile_coords, 0, 0)){
        float c1 = (rand()%10)/10.0f;
        float c2 = (rand()%10)/10.0f;
        float c3 = (rand()%10)/10.0f;
        glClearColor(c1, c2, c3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        level_over = true;
    }

    if (glfwGetKey(game_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(game_window_, true);
    }

    if(!level_over){
        if (glfwGetKey(game_window_, GLFW_KEY_RIGHT) == GLFW_PRESS){
            if(player_current_coords[3]+inc <= 1 && !CollideWalls(player_current_coords, walls, inc, 0)){
                player_->MoveRight();
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_LEFT) == GLFW_PRESS){
            if(player_current_coords[0]-inc>=-1 && !CollideWalls(player_current_coords, walls, -inc, 0)){
                player_->MoveLeft();
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_DOWN) == GLFW_PRESS){
            if(player_current_coords[1]-inc >= -1 && !CollideWalls(player_current_coords, walls, 0, -inc)){
                player_->MoveDown();
            }
        }
        if (glfwGetKey(game_window_, GLFW_KEY_UP) == GLFW_PRESS){
            if(player_current_coords[7]+inc <= 1 && !CollideWalls(player_current_coords, walls, 0, inc)){
                player_->MoveUp();
            }
        }
    }
}

void Game::ProcessInputAndRegenerate(Level &level, Maze &maze){
    
    // render
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
        if(elapsed.count() > level.regen_time_interval){
            maze.GenerateMaze(maze.GetWidth(), maze.GetHeight());
            start_time_ = chrono::system_clock::now();
            name_to_size_data_["walls"] = maze.GetSizeData();
            BindElement("walls");
        }
        // to do
        float * fetched_player_hitbox = player_->GetHitbox();
        float player_hitbox[12];
        for(int i = 0; i < 12; i++){
            player_hitbox[i] = fetched_player_hitbox[i];
        }
        name_to_size_data_["player"] = {{sizeof(player_hitbox), player_hitbox}, {sizeof(rectangle_ind), rectangle_ind}, {6, (void*) 0}};
        BindElement("player");
        
        for(string name : kNames){
            Draw(name);
        }
    }
    ProcessInput(level, maze);
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