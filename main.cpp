// ENGINE
#include "../GameEngine/engine.hpp"
#include <stdlib.h>
#include <iostream>

#define WINDOW_SIZE 630
#define TILE_WIDTH (2.0f / WINDOW_SIZE * 70.0f)
#define TILE_HEIGHT TILE_WIDTH

class Sudoku
{
private:
    GLuint _VBO, _VAO;

    GLfloat _vertices[3] = {
        // Positions
        0.0f, 0.0f, 0.0f
    };
    void BufferVertexData()
    {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);

        // bind the VAO first, then bind and set vertex buffer(s)
        // and attribute pointer(s)
        glBindVertexArray(_VAO);

        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // good practice to unbind the VAO to prevent strange bugs
        // do NOT unbind the EBO, keep it bound to this VAO
        glBindVertexArray(0);
    }
    typedef struct {
        unsigned int cnt;
        bool vals[9];
        unsigned int val;
    } _field_t;

    _field_t* _board;
    unsigned int _selection;

    unsigned int getFieldVal(unsigned int i)
    {
        if(_board[i].cnt > 1) { return 0;}

        for(unsigned int j = 0; j < 9; j++)
        {
            if(_board[i].vals[j] == true) {
                return j+1;
            }
        }
        return 0;
    }
    bool trivialCheckFieldHelp(unsigned int i)
    {
        unsigned int row = i / 9;
        unsigned int col = i % 9;

        // check the row
        for(unsigned int j = 0; j < 9; j++)
        {
            if(j == col) continue;

            unsigned int idx = row*9 + j;
            if(_board[idx].cnt == 1 && _board[i].vals[_board[idx].val - 1] == true) {
                _board[i].vals[_board[idx].val - 1] = false;
                _board[i].cnt--;
            }
        }
        if(_board[i].cnt == 1)
        {
            _board[i].val = getFieldVal(i);
            printf("ROW insert val %i in (%i,%i)\n", getFieldVal(i), row, col);
            return true;
        }

        // check the column
        for(unsigned int j = 0; j < 9; j++)
        {
            if(j == row) continue;

            unsigned int idx = j*9 + col;
            if(_board[idx].cnt == 1 && _board[i].vals[_board[idx].val - 1] == true) {
                _board[i].vals[_board[idx].val - 1] = false;
                _board[i].cnt--;
            }
        }
        if(_board[i].cnt == 1)
        {
            _board[i].val = getFieldVal(i);
            printf("COL insert val %i in (%i,%i)\n", getFieldVal(i), row, col);
            return true;
        }

        // check the region
        unsigned int reg = ((row / 3) * 3) + (col / 3);
        unsigned int start = (reg / 3) * 27 + ((col / 3) * 3);
        for(unsigned int r = 0; r < 3; r++)
        {
            for(unsigned int c = 0; c < 3; c++)
            {
                unsigned int idx = start + 9*r + c;
                if(idx == i) continue;

                if(_board[idx].cnt == 1 && _board[i].vals[_board[idx].val - 1] == true) {
                    _board[i].vals[_board[idx].val - 1] = false;
                    _board[i].cnt--;
                }
            }
        }
        if(_board[i].cnt == 1)
        {
            _board[i].val = getFieldVal(i);
            printf("REG insert val %i in (%i,%i)\n", getFieldVal(i), row, col);
            return true;
        }

        return false;
    }

    void trivialFindHelp()
    {
        for(unsigned int i = 0; i < 81; i++)
        {
            if(_board[i].cnt > 1 && trivialCheckFieldHelp(i))
            {
                _board[i].cnt = 1;
                return;
            }
        }
        printf("found no field\n");
    }

    void advancedCheckRegion(unsigned int reg)
    {
        unsigned int start = (reg / 3) * 27 + ((reg % 3) * 3);
        bool vals[9] = {true,true,true,true,true,true,true,true,true};

        // check for each field in the region if it has a number, is so remove it from list
        for(unsigned int r = 0; r < 3; r++)
        {
            for(unsigned int c = 0; c < 3; c++)
            {
                unsigned int idx = start + 9*r + c;

                // if field has number, then remove this number from the list
                if(_board[idx].cnt == 1) {
                    vals[_board[idx].val - 1] = false;
                }
            }
        }

        // check for each number if it fits into the region
        for(unsigned int num = 1; num <= 9; num++)
        {
            //if(vals[num-1] == false) continue;


            // search for a fitting
        }
    }
    void advancedCheckRegions()
    {
        for(unsigned int i = 0; i < 9; i++)
        {
            advancedCheckRegion(i);
        }
    }
public:
    Sudoku()
    {
        _board = new _field_t[81];
        _selection = 0;

        for(unsigned int i = 0; i < 81; i++)
        {
            _board[i].cnt = 9;
            _board[i].val = 0;
            for(unsigned int j = 0; j < 9; j++)
            {
                _board[i].vals[j] = true;
            }
        }
        BufferVertexData();
    }
    ~Sudoku()
    {
        delete _board;
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
    }

    void KeyInput(int key)
    {
        switch(key) {
        case GLFW_KEY_1:
        case GLFW_KEY_KP_1:
            _board[_selection].cnt = 1;
            _board[_selection].val = 1;
            break;
        case GLFW_KEY_2:
        case GLFW_KEY_KP_2:
            _board[_selection].cnt = 1;
            _board[_selection].val = 2;
            break;
        case GLFW_KEY_3:
        case GLFW_KEY_KP_3:
            _board[_selection].cnt = 1;
            _board[_selection].val = 3;
            break;
        case GLFW_KEY_4:
        case GLFW_KEY_KP_4:
            _board[_selection].cnt = 1;
            _board[_selection].val = 4;
            break;
        case GLFW_KEY_5:
        case GLFW_KEY_KP_5:
            _board[_selection].cnt = 1;
            _board[_selection].val = 5;
            break;
        case GLFW_KEY_6:
        case GLFW_KEY_KP_6:
            _board[_selection].cnt = 1;
            _board[_selection].val = 6;
            break;
        case GLFW_KEY_7:
        case GLFW_KEY_KP_7:
            _board[_selection].cnt = 1;
            _board[_selection].val = 7;
            break;
        case GLFW_KEY_8:
        case GLFW_KEY_KP_8:
            _board[_selection].cnt = 1;
            _board[_selection].val = 8;
            break;
        case GLFW_KEY_9:
        case GLFW_KEY_KP_9:
            _board[_selection].cnt = 1;
            _board[_selection].val = 9;
            break;

        case GLFW_KEY_BACKSPACE:
        case GLFW_KEY_DELETE:
        case GLFW_KEY_D:
            _board[_selection].cnt = 9;
            _board[_selection].val = 0;
            for(unsigned int i = 0; i < 9; i++) {
                _board[_selection].vals[i] = true;
            }
            break;

        case GLFW_KEY_UP:
            _selection -= (_selection >= 9) ? 9 : 0;
            break;
        case GLFW_KEY_DOWN:
            _selection += (_selection < 72) ? 9 : 0;
            break;
        case GLFW_KEY_LEFT:
            _selection -= (_selection % 9 > 0) ? 1 : 0;
            break;
        case GLFW_KEY_RIGHT:
            _selection += (_selection % 9 < 8) ? 1 : 0;
            break;

        case GLFW_KEY_H:
            trivialFindHelp();
            break;
        case GLFW_KEY_J:
            advancedCheckRegions();
            break;

        default:
            break;
        }
    }
    void DrawBoard(Shaders::ShaderWrapper* shd, unsigned int size, Textures::Texture2D** tex)
    {

        glBindVertexArray(_VAO);
        for(unsigned int i = 0; i < 81; i++)
        {
            shd->SetUniform("position", glm::vec2(2.0f / 9.0f * (float)(i % 9) - 1.0f,
                                                  -2.0f / 9.0f * (float)(i / 9) + 1.0f));
            shd->SetUniform("width", TILE_WIDTH);
            shd->SetUniform("height", TILE_HEIGHT);
            shd->SetUniform("selection", (i == _selection));
            shd->SetUniform("windowSize", WINDOW_SIZE);

            // several possibilities for a field
            if(_board[i].cnt > 1) {
                tex[0]->Bind();
            }
            // only one possibility for a field
            else if(_board[i].cnt == 1) {
                tex[_board[i].val]->Bind();
            }

            shd->SetUniformTexture("tex", GL_TEXTURE0);

            glDrawArrays(GL_POINTS, 0, 1);
        }
        glBindVertexArray(0);
    }
    void InsertGameBoard(unsigned int* board)
    {
        for(unsigned int i = 0; i < 81; i++)
        {
            // insert empty field
            if(board[i] == 0) {
                _board[i].cnt = 9;
                _board[i].val = 0;
            }
            // insert a number
            else {
                _board[i].cnt = 1;
                _board[i].val = board[i];
            }
        }
    }
};

unsigned int game_board[81] = {
    7,0,0, 5,6,1, 0,0,8,
    5,2,0, 0,0,0, 0,0,0,
    0,0,8, 0,0,2, 5,7,0,

    0,1,5, 0,2,3, 0,8,0,
    2,0,0, 0,5,0, 0,0,4,
    0,7,0, 1,9,0, 3,5,2,

    6,5,4, 2,0,9, 8,0,0,
    0,0,0, 0,0,0, 0,6,5,
    0,0,0, 6,8,5, 0,0,9
};

Windows::WindowedWindow* win;
Sudoku* sud;
Shaders::ShaderWrapper* shd;
Textures::Texture2D* tex_tiles[10];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS) { sud->KeyInput(key); }
}
int main()
{
    // WINDOW
    win = new Windows::WindowedWindow("Sudoku", WINDOW_SIZE, Windows::ASPECT_RATIO_1_1);
    win->SetKeyCallback(key_callback);
    glfwSetInputMode(win->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // SHADER
    shd = new Shaders::ShaderWrapper("shaders|tile", Shaders::SHADERS_VGF);
    shd->Activate();

    // TEXTURES
    unsigned long tex_options = TEX_GENERATE_MIPMAP | TEX_MIXED_FILTER;
    tex_tiles[0] = new Textures::Texture2D("images|number|tile_blank.png", tex_options);
    tex_tiles[1] = new Textures::Texture2D("images|number|tile_1.png", tex_options);
    tex_tiles[2] = new Textures::Texture2D("images|number|tile_2.png", tex_options);
    tex_tiles[3] = new Textures::Texture2D("images|number|tile_3.png", tex_options);
    tex_tiles[4] = new Textures::Texture2D("images|number|tile_4.png", tex_options);
    tex_tiles[5] = new Textures::Texture2D("images|number|tile_5.png", tex_options);
    tex_tiles[6] = new Textures::Texture2D("images|number|tile_6.png", tex_options);
    tex_tiles[7] = new Textures::Texture2D("images|number|tile_7.png", tex_options);
    tex_tiles[8] = new Textures::Texture2D("images|number|tile_8.png", tex_options);
    tex_tiles[9] = new Textures::Texture2D("images|number|tile_9.png", tex_options);

    // SUDOKU
    sud = new Sudoku();
    sud->InsertGameBoard(game_board);

    // GAME LOOP
    while(!win->ShouldClose())
    {
        win->WaitEvents();

        if(glfwGetKey(win->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            win->CloseWindow();
            continue;
        }

        win->ClearScreen();
        sud->DrawBoard(shd, 10, tex_tiles);
        win->SwapBuffers();
    }

    delete tex_tiles[0];
    delete tex_tiles[1];
    delete tex_tiles[2];
    delete tex_tiles[3];
    delete tex_tiles[4];
    delete tex_tiles[5];
    delete tex_tiles[6];
    delete tex_tiles[7];
    delete tex_tiles[8];
    delete tex_tiles[9];
    return 0;
}
