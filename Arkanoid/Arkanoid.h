#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ctime>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

#include <fstream>
#include <algorithm>


using namespace std;

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 720;
const int PLATFORM_WIDTH = 120;
const int PLATFORM_HEIGHT = 20;
const int RECT_COUNT = 49;
const int RECT_WIDTH = 60;
const int RECT_HEIGHT = 20;
const int CIRCLE_RADIUS = 10;
const int MAX_RECORDS = 10;
const int BONUSE_CHANCE = 5;

Mix_Chunk* soundOfBonuse = NULL;
Mix_Chunk* newLvl = NULL;
Mix_Chunk* broke = NULL;
Mix_Chunk* bounce = NULL;
Mix_Chunk* victory = NULL;
Mix_Chunk* minusHeart = NULL;
Mix_Chunk* defeat = NULL;
Mix_Chunk* button = NULL;
Mix_Music* fonMenu = NULL;
Mix_Music* fonGame = NULL;

SDL_Texture* textTexture;

SDL_Texture* markerTexture;
SDL_Texture* continueTexture;
SDL_Texture* newGameTexture;
SDL_Texture* recordsTexture;
SDL_Texture* settingsTexture;
SDL_Texture* saveAndQuitTexture;
SDL_Texture* recordsBackTexture;
SDL_Texture* quitTexture;
SDL_Texture* selectedTexture;
SDL_Texture* arkanoidTexture;
SDL_Texture* menuBackTexture;
SDL_Texture* settingsBackTexture;
SDL_Texture* fonInGame;
SDL_Texture* pepe;
SDL_Texture* trollface;
SDL_Texture* coolCat;
SDL_Texture* heart;
SDL_Texture* speedPlusTexture;
SDL_Texture* speedMinusTexture;
SDL_Texture* widthPlusTexture;
SDL_Texture* widthMinusTexture;

int selectedButton = 0;

struct Record {
    string nickname;
    int score;
};

struct Platform {
    int x, y;
    int velocity_x;
    int widthMode;
};

struct Rect {
    int x, y;
    int strength;
    bool is_visible;
    int bonuse;
};

struct Circle {
    int x, y;
    int velocity_x, velocity_y;
    SDL_Color color;
    int lives;
    int score;
    int speedMode;
};

struct Bonuse {
    int x, y;
    int type;
    SDL_Texture* bonuceTexture;
    bool isVisible;
};

TTF_Font* font = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool quit = false;
bool nextLvl = false;

Platform platform;
Rect rectangles[RECT_COUNT];
Bonuse bonuse;
Circle circle;



#include "Arkanoid.h"

void getBonus();
void newLevel();
void kwa();
void lose();
void win();
void brokeRect();
void bounceBall();
void klick();

void backMenu();
void backGame();


SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* renderer);

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int h, int w);

void renderText(SDL_Renderer* renderer, const char* message, int x, int y);

void saveRecord(const string& filename, const Record& new_record);

void loadRecords(const std::string& filename, Record records[], int& record_count);

int newRecord();

void printRecord();

void save(int lvl);

int loadGame(int lvl);

void close();

void settings();

void pauseMenu(int lvl);

void handleEvents(int lvl);

int update(int lvl);

void draw(int lvl);

bool initialize(int lvl, bool loaded);

void gameLoop(bool loaded);

void menu();

int main(int argc, char* argv[]);