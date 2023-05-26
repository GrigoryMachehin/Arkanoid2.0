#include "Arkanoid.h"

void newLevel()
{
    newLvl = Mix_LoadWAV("newLvl.wav");
    Mix_PlayChannel(-1, newLvl, 0);
}

void getBonus()
{
    soundOfBonuse = Mix_LoadWAV("bonus.mp3");
    Mix_PlayChannel(-1, soundOfBonuse, 0);
}

void kwa()
{
    minusHeart = Mix_LoadWAV("kwa.wav");
    Mix_PlayChannel(-1, minusHeart, 0);
}

void lose()
{
    defeat = Mix_LoadWAV("proigrish.wav");
    Mix_PlayChannel(-1, defeat, 0);
}

void win()
{
    victory = Mix_LoadWAV("pobeda.wav");
    Mix_PlayChannel(-1, victory, 0);
}

void brokeRect()
{
    broke = Mix_LoadWAV("broke.mp3");
    Mix_PlayChannel(-1, broke, 0);
}

void bounceBall()
{
    bounce = Mix_LoadWAV("bounce.wav");
    Mix_PlayChannel(-1, bounce, 0);
}

void klick()
{
    button = Mix_LoadWAV("button.wav");
    Mix_PlayChannel(-1, button, 0);
}

void backMenu()
{
    fonMenu = Mix_LoadMUS("fonMenu.mp3");
    Mix_PlayMusic(fonMenu, -1);
}

void backGame()
{
    fonGame = Mix_LoadMUS("fonGame.mp3");
    Mix_PlayMusic(fonGame, -1);
}

SDL_Texture* loadTexture(const char* fileName, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int h, int w)
{
    SDL_Rect rect = { x, y, h, w };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void renderText(SDL_Renderer* renderer, const char* message, int x, int y)
{
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, message, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &rect);
    SDL_FreeSurface(surface);
}

void saveRecord(const string& filename, const Record& new_record) {
    Record records[MAX_RECORDS + 1];
    int record_count = 0;

    ifstream file(filename);
    while (file >> records[record_count].nickname >> records[record_count].score&& record_count < MAX_RECORDS) {
        ++record_count;
    }
    file.close();

    records[record_count] = new_record;
    ++record_count;
    sort(records, records + record_count, [](const Record& a, const Record& b) { return a.score > b.score; });

    if (record_count > MAX_RECORDS) {
        --record_count;
    }

    ofstream output_file(filename);
    for (int i = 0; i < record_count; ++i) {
        output_file << records[i].nickname << " " << records[i].score << endl;
    }
    output_file.close();
}

void loadRecords(const std::string& filename, Record records[], int& record_count) {
    ifstream file(filename);
    record_count = 0;
    while (file >> records[record_count].nickname >> records[record_count].score && record_count < MAX_RECORDS) {
        ++record_count;
    }
    file.close();
}

int newRecord()
{
    SDL_Event event;
    int quit = 0;

    char inputText[20] = " ";
    int inputIndex = 0;
    int maxIndex = 0, end = 10;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = 1;
                    return -1;
                case SDLK_RETURN:
                    quit = 1;
                    break;
                    break;
                case SDLK_BACKSPACE:
                    if (inputIndex > 1) {
                        inputText[0] = ' ';
                        inputText[inputIndex - 1] = '\0';
                        inputIndex--;
                    }
                    break;
                default:
                    if ((event.key.keysym.sym >= 1 && event.key.keysym.sym <= 254 && event.key.keysym.sym != 32 && inputIndex < end && inputIndex < 10) and (strlen(inputText) <= 10))
                    {
                        if (inputText[inputIndex + 1] != '\0')
                        {
                            for (int i = end; i > inputIndex; i--)
                            {
                                inputText[i] = inputText[i - 1];
                            }
                            inputText[inputIndex] = (char)event.key.keysym.sym;
                            inputIndex++;
                            maxIndex++;
                        }
                        else
                        {
                            inputText[inputIndex] = (char)event.key.keysym.sym;
                            inputIndex++;
                            maxIndex++;
                        }
                    }
                    break;
                }
                break;
            }
        }
        SDL_RenderClear(renderer);
        renderTexture(recordsBackTexture, renderer, 0, 0, 480, 720);
        renderText(renderer, u8"Введи имя, герой:", 100, 360);
        renderText(renderer, inputText, 150, 400);
        renderText(renderer, u8"(Латиница)", 150, 600);
        renderText(renderer, u8"(Скип - ввод пустой строки)", 15, 640);

        SDL_RenderPresent(renderer);
    }
    Record new_record = { inputText, circle.score };
    saveRecord("records.txt", new_record);

    return 0;
}

void printRecord()
{
    SDL_RenderClear(renderer);
    renderTexture(recordsBackTexture, renderer, 0, 0, 480, 720);
    Record records[MAX_RECORDS];
    int record_count;
    loadRecords("records.txt", records, record_count);
    char* strRecord;
    char* argv = new char[255];
    for (int i = 0; i < record_count; ++i) {
        char* argv = new char[255];
        renderText(renderer, records[i].nickname.c_str(), 50, 150 + 40 * i);
        renderText(renderer, _itoa(records[i].score, argv, 10), 300, 150 + 40*i);
    }
    renderText(renderer, u8"САМЫЕ КРУТЫЕ", 120, 100);
    SDL_RenderPresent(renderer);
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    klick();
                    quit = true;

                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
    }
}

void save(int lvl) {
    ofstream file("save.txt");
    file << circle.score << " " << lvl << " " << circle.lives << " " << circle.speedMode <<  " " << platform.widthMode << endl;
        for (int i = 0; i < 49; i++)
        {
            file << rectangles[i].is_visible << " ";
        }
    file.close();
}

int loadGame(int lvl) {
    ifstream file("save.txt");
    file >> circle.score >> lvl >> circle.lives >> circle.speedMode >> platform.widthMode;
    for (int i = 0; i < 49; i++)
    {
        file >> rectangles[i].is_visible;
    }
    file.close();
    return lvl;
}

void close() {
    SDL_DestroyTexture(fonInGame);
    SDL_DestroyTexture(pepe);
    SDL_DestroyTexture(coolCat);
    SDL_DestroyTexture(trollface);
    SDL_DestroyTexture(heart);
    menu();
}

void settings()
{
    int musVolume = 0;
    int effectsVolume = 0;

    ifstream file("volume.txt");
    file >> musVolume >> effectsVolume;
    file.close();    
    SDL_RenderClear(renderer);
    renderTexture(settingsBackTexture, renderer, 0, 0, 480, 720);
    int velocity = 0;

    int yMusik = 75 + musVolume;
    int ySound = 75 + effectsVolume;
    bool musikOrSound = 0;
    SDL_Rect musik = { yMusik, 85, 30, PLATFORM_HEIGHT };
    SDL_Rect sound = { ySound, 147, 30, PLATFORM_HEIGHT };
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (musikOrSound)
                {
                case 0:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        klick();
                        quit = true;
                        break;
                    case SDLK_DOWN: {musikOrSound = 1; break; }
                    case SDLK_LEFT:
                        if (yMusik > 75)
                        {
                            musik = { yMusik -= 5, 85, 30, PLATFORM_HEIGHT };
                            musVolume -= 5; 
                            Mix_VolumeMusic(musVolume);
                        }
                        break;
                    case SDLK_RIGHT:
                        if (yMusik < 380)
                        {
                            musik = { yMusik += 5, 85, 30, PLATFORM_HEIGHT };
                            musVolume += 5; 
                            Mix_VolumeMusic(musVolume);
                        }
                        break;
                    case SDLK_a:
                        if (yMusik > 75)
                        {
                            musik = { yMusik -= 5, 85, 30, PLATFORM_HEIGHT };
                            musVolume -= 5;
                            Mix_VolumeMusic(musVolume);
                        }
                        break;

                    case SDLK_d:
                        if (yMusik < 380)
                        {
                            musik = { yMusik += 5, 85, 30, PLATFORM_HEIGHT };
                            musVolume += 5;
                            Mix_VolumeMusic(musVolume);
                        }
                        break;
                    }
                    break;
                }
                case 1:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        klick();
                        quit = true;
                        break;
                    case SDLK_UP: {musikOrSound = 0; break; }
                    case SDLK_LEFT:
                        if (ySound > 75)
                        {
                            sound = { ySound -= 5, 147, 30, PLATFORM_HEIGHT };
                            effectsVolume -= 5;
                            Mix_Volume(-1, effectsVolume);
                            kwa();
                        }
                        break;
                    case SDLK_RIGHT:
                        if (ySound < 380)
                        {
                            sound = { ySound += 5, 147, 30, PLATFORM_HEIGHT };
                            effectsVolume += 5;
                            Mix_Volume(-1, effectsVolume);
                            kwa();
                        }
                        break;
                    case SDLK_a:
                        if (ySound > 75)
                        {
                            sound = { ySound -= 5, 147, 30, PLATFORM_HEIGHT };
                            effectsVolume -= 5;
                            Mix_Volume(-1, effectsVolume);
                            kwa();
                        }
                        break;

                    case SDLK_d:
                        if (ySound < 380)
                        {
                            sound = { ySound += 5, 147, 30, PLATFORM_HEIGHT };
                            effectsVolume += 5;
                            Mix_Volume(-1, effectsVolume);
                            kwa();
                        }
                        break;
                    }

                }
                break;

                }

            }
            SDL_RenderClear(renderer);
            renderTexture(settingsBackTexture, renderer, 0, 0, 480, 720);
            if (musikOrSound == 0)
            {
                SDL_SetRenderDrawColor(renderer, 10, 255, 10, 255);
                SDL_RenderFillRect(renderer, &musik);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &sound);
            }
            if (musikOrSound == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &musik);
                SDL_SetRenderDrawColor(renderer, 10, 255, 10, 255);
                SDL_RenderFillRect(renderer, &sound);
            }
            SDL_RenderPresent(renderer);
        }
        ofstream fileV("volume.txt");
        fileV << musVolume << " " << effectsVolume;
        fileV.close();
    }
}

void pauseMenu(int lvl)
{
    SDL_RenderClear(renderer);

    menuBackTexture = fonInGame;
    arkanoidTexture = loadTexture("Arkanoid.png", renderer);
    recordsBackTexture = loadTexture("recordsBack.png", renderer);
    markerTexture = loadTexture("marker.png", renderer);
    continueTexture = loadTexture("continue.png", renderer);
    recordsTexture = loadTexture("records.png", renderer);
    settingsTexture = loadTexture("settings.png", renderer);
    saveAndQuitTexture = loadTexture("saveAndQuit.png", renderer);

    selectedTexture = continueTexture;

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                case SDLK_UP:
                    if (selectedButton > 0)
                    {
                        klick();
                        selectedButton--;
                    }
                    break;
                case SDLK_DOWN:
                    if (selectedButton < 3)
                    {
                        klick();
                        selectedButton++;
                    }
                    break;
                case SDLK_RETURN:
                    switch (selectedButton)
                    {
                    case 0:
                        klick();
                        return;
                        break;
                    case 1:
                        klick();
                        printRecord();
                        break;
                    case 2:
                        klick();
                        settings();
                        break;
                    case 3:
                        klick();
                        save(lvl);
                        quit = true;
                        break;
                    }
                    break;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 380 && mouseY <= 440)
                {
                    selectedButton = 0;
                    klick();
                    return;
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 440 && mouseY <= 500)
                {
                    selectedButton = 1;
                    klick();
                    printRecord();
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 500 && mouseY <= 560)
                {
                    selectedButton = 2;
                    klick();
                    settings();
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 560 && mouseY <= 620)
                {
                    selectedButton = 3;
                    klick();
                    quit = true;
                    break;
                }
                break;
            }
        }

        SDL_RenderClear(renderer);
        renderTexture(menuBackTexture, renderer, 0, 0, 480, 720);
        renderTexture(arkanoidTexture, renderer, 0, 80, 500, 200);
        renderTexture(markerTexture, renderer, 110, 379 + selectedButton * 60, 240, 60);
        renderTexture(selectedTexture, renderer, 120, 380, 240, 60);
        renderTexture(recordsTexture, renderer, 120, 440, 240, 60);
        renderTexture(settingsTexture, renderer, 120, 500, 240, 60);
        renderTexture(saveAndQuitTexture, renderer, 120, 560, 240, 60);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(continueTexture);

    SDL_DestroyTexture(markerTexture);
    SDL_DestroyTexture(selectedTexture);
    SDL_DestroyTexture(menuBackTexture);
    SDL_DestroyTexture(arkanoidTexture);
    SDL_DestroyTexture(recordsTexture);
    SDL_DestroyTexture(settingsTexture);
    SDL_DestroyTexture(saveAndQuitTexture);
    close();
}

void handleEvents(int lvl) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            //close();
            pauseMenu(lvl);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                pauseMenu(lvl);
                //close();
                break;
            case SDLK_p:
                pauseMenu(lvl);
                break;
            case SDLK_LEFT:
                platform.velocity_x = -15;
                break;
            case SDLK_RIGHT:
                platform.velocity_x = 15;
                break;
            case SDLK_a:
                platform.velocity_x = -15;
                break;
            case SDLK_d:
                platform.velocity_x = 15;
                break;
            case SDLK_g:
                nextLvl = true;
                break;
            case SDLK_h:
                SDL_RenderClear(renderer);
                bonuse.isVisible = false;
                lose();
                renderTexture(trollface, renderer, 50, 200, 400, 400);
                SDL_RenderPresent(renderer);
                SDL_Delay(2000);
                circle.lives = 0;
                close();
                break;
            case SDLK_SPACE:
                if (circle.velocity_x == 0 && circle.velocity_y == 0) {
                    int angle = (rand() % 20) + 70;
                    if (rand() % 2 == 0) {
                        angle *= -1;
                    }
                    circle.velocity_x = int((double)CIRCLE_RADIUS / tan(angle * 3.1415926535 / 180));
                    circle.velocity_y = -CIRCLE_RADIUS;
                }
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_a:
            case SDLK_d:
            case SDLK_LEFT:
            case SDLK_RIGHT:
                platform.velocity_x = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

int update(int lvl) {
    platform.x += platform.velocity_x;
    if (circle.velocity_x != 0 || circle.velocity_y != 0) {
        circle.x += circle.velocity_x;
        circle.y += circle.velocity_y;

        if (circle.x <= CIRCLE_RADIUS || circle.x >= SCREEN_WIDTH - 2 - CIRCLE_RADIUS) {
            bounceBall();
            circle.velocity_x *= -1;
        }
        if (circle.y <= CIRCLE_RADIUS) {
            bounceBall();
            circle.velocity_y *= -1;
        }
        if (circle.y >= SCREEN_HEIGHT - PLATFORM_HEIGHT - CIRCLE_RADIUS * 2 + 10) {
            if (circle.x > platform.x - 10 - platform.widthMode && circle.x < platform.x + PLATFORM_WIDTH + 10 + platform.widthMode*2) {
                bounceBall();
                circle.velocity_y *= -1;

                int diff = circle.x - (platform.x + PLATFORM_WIDTH / 2+(rand()%2));
                if (diff > 0) {
                    circle.velocity_x = int((double)CIRCLE_RADIUS / tan((80 + diff) * 3.1415926535 / 180));
                }
                else if (diff < 0) {
                    circle.velocity_x = int((double)CIRCLE_RADIUS / tan((100 + diff) * 3.1415926535 / 180)) * -1;
                }
            }
            else {
                circle.lives--;
                bonuse.isVisible = false;
                kwa();
                renderTexture(pepe, renderer, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 200);
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
                if (circle.lives > 0) {
                    circle.x = SCREEN_WIDTH / 2;
                    circle.y = SCREEN_HEIGHT - PLATFORM_HEIGHT - CIRCLE_RADIUS * 2 - 20;
                    circle.velocity_x = 0;
                    circle.velocity_y = 0;
                }
            }
        }

        for (int i = 0; i < RECT_COUNT; i++) {
            if (rectangles[i].is_visible) {
                if (circle.x >= rectangles[i].x -10 && circle.x <= rectangles[i].x + 10 + RECT_WIDTH &&
                    circle.y - CIRCLE_RADIUS <= rectangles[i].y + RECT_HEIGHT && circle.y + CIRCLE_RADIUS >= rectangles[i].y) {
                    rectangles[i].strength--;
                    if (rectangles[i].strength == 0) {
                        brokeRect();
                        if (rectangles[i].bonuse == 1 && !bonuse.isVisible && circle.speedMode > -1)
                        {
                            bonuse.isVisible = true;
                            bonuse.type = 1;
                            bonuse.x = rectangles[i].x;
                            bonuse.y = rectangles[i].y;
                            bonuse.bonuceTexture = speedPlusTexture;
                        }
                        if (rectangles[i].bonuse == 2 && !bonuse.isVisible && circle.speedMode < 1)
                        {
                            bonuse.isVisible = true;
                            bonuse.type = 2;
                            bonuse.x = rectangles[i].x;
                            bonuse.y = rectangles[i].y;
                            bonuse.bonuceTexture = speedMinusTexture;
                        }
                        if (rectangles[i].bonuse == 3 && !bonuse.isVisible)
                        {
                            bonuse.isVisible = true;
                            bonuse.type = 3;
                            bonuse.x = rectangles[i].x;
                            bonuse.y = rectangles[i].y;
                            bonuse.bonuceTexture = widthPlusTexture;
                        }
                        if (rectangles[i].bonuse == 4 && !bonuse.isVisible)
                        {
                            bonuse.isVisible = true;
                            bonuse.type = 4;
                            bonuse.x = rectangles[i].x;
                            bonuse.y = rectangles[i].y;
                            bonuse.bonuceTexture = widthMinusTexture;
                        }
                        rectangles[i].is_visible = false;
                    }
                    bounceBall();
                    circle.velocity_y *= -1;

                    if (circle.x <= rectangles[i].x || circle.x >= rectangles[i].x + RECT_WIDTH) {
                        circle.velocity_x *= -1;
                    }
                    break;
                }
            }
        }
        if (bonuse.isVisible) { bonuse.y += 5; }
        if (bonuse.y >= SCREEN_HEIGHT - PLATFORM_HEIGHT - CIRCLE_RADIUS * 2 + 20 && bonuse.isVisible) {
            if (bonuse.x > platform.x - 10 - platform.widthMode && bonuse.x < platform.x + PLATFORM_WIDTH + 10 + platform.widthMode * 2)
            {
                getBonus();
                if (bonuse.type == 1) { circle.speedMode = -10; bonuse.isVisible = false; }
                if (bonuse.type == 2) { circle.speedMode = 7; bonuse.isVisible = false; }
                if (bonuse.type == 3) { platform.widthMode = 10; bonuse.isVisible = false; }
                if (bonuse.type == 4) { platform.widthMode = -10; bonuse.isVisible = false; }
            }
            else
            {
                bonuse.isVisible = false;
            }
        }

        bool all_rectangles_invisible = true;
        if (circle.lives <= 0) {
            SDL_RenderClear(renderer);
            lose();
            renderTexture(trollface, renderer, 50, 200, 400, 400);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000); close(); }
        for (int i = 0; i < RECT_COUNT; i++) {
            if (rectangles[i].is_visible) {
                all_rectangles_invisible = false;
                break;
            }
        }
        if (all_rectangles_invisible) {
            nextLvl = true;
        }
    }

    if (platform.x < 0) {
        platform.x = 0;
    }
    else if (platform.x > SCREEN_WIDTH - PLATFORM_WIDTH) {
        platform.x = SCREEN_WIDTH - PLATFORM_WIDTH;
    }
    return 0;
}

void draw(int lvl) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderTexture(fonInGame, renderer, 0, 0, 480, 720);


    for (int i = 0; i < RECT_COUNT; i++) {
        if ((rectangles[i].is_visible) && (rectangles[i].strength == 1)){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect rect = { rectangles[i].x, rectangles[i].y, RECT_WIDTH, RECT_HEIGHT };
            SDL_RenderFillRect(renderer, &rect);
        }
        if ((rectangles[i].is_visible) && (rectangles[i].strength == 2)) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_Rect rect = { rectangles[i].x, rectangles[i].y, RECT_WIDTH, RECT_HEIGHT };
            SDL_RenderFillRect(renderer, &rect);
        }
        if ((rectangles[i].is_visible) && (rectangles[i].strength == 3)) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect = { rectangles[i].x, rectangles[i].y, RECT_WIDTH, RECT_HEIGHT };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    if (circle.speedMode > 0)
    {
        renderTexture(speedMinusTexture, renderer, 420, 640, 40, 40);
    }
    if (circle.speedMode < 0)
    {
        renderTexture(speedPlusTexture, renderer, 420, 640, 40, 40);
    }
    if (platform.widthMode > 0)
    {
        renderTexture(widthPlusTexture, renderer, 420, 600, 40, 40);
    }
    if (platform.widthMode < 0)
    {
        renderTexture(widthMinusTexture, renderer, 420, 600, 40, 40);
    }
    if (bonuse.isVisible)
    {
        renderTexture(bonuse.bonuceTexture, renderer, bonuse.x, bonuse.y, 50, 50);
    }
    for (int i = 0; i < circle.lives; i++) renderTexture(heart, renderer, 100 + i * 25, 5, 40, 40);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = { platform.x - platform.widthMode, SCREEN_HEIGHT - PLATFORM_HEIGHT, PLATFORM_WIDTH  + platform.widthMode*2, PLATFORM_HEIGHT };
    SDL_RenderFillRect(renderer, &rect);
    if (circle.velocity_x == 0 && circle.velocity_y == 0)
    {
        renderText(renderer, u8"Нажмите Space", 120, 640);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (int w = 0; w < CIRCLE_RADIUS * 2; w++) {
        for (int h = 0; h < CIRCLE_RADIUS * 2; h++) {
            int dx = CIRCLE_RADIUS - w;
            int dy = CIRCLE_RADIUS - h;
            if (sqrt(dx * dx + dy * dy) < CIRCLE_RADIUS) {
                SDL_RenderDrawPoint(renderer, circle.x + dx, circle.y + dy);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    char* argv = new char[255];
    renderText(renderer, _itoa(circle.score, argv, 10), 10, 5);
    renderText(renderer, "lvl", 425, 25);
    renderText(renderer, _itoa(lvl, argv, 10), 440, 5);

    delete argv;

    SDL_Delay(10 + circle.speedMode);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

bool initialize(int lvl, bool loaded) {
    srand(time(0));

    pepe = loadTexture("pepe.png", renderer);
    trollface = loadTexture("trollface.png", renderer);
    coolCat = loadTexture("coolCat.png", renderer);
    heart = loadTexture("heart.png", renderer);
    if (lvl == 1 || lvl == 2) fonInGame = loadTexture("fon0.png", renderer);
    if (lvl == 3 || lvl == 4) fonInGame = loadTexture("fon1.png", renderer);
    if (lvl == 5 || lvl == 6) fonInGame = loadTexture("fon2.png", renderer);
    if (lvl == 7 || lvl == 8) fonInGame = loadTexture("fon3.png", renderer);
    if (lvl == 9 || lvl == 10) fonInGame = loadTexture("fon4.png", renderer);

    circle.x = SCREEN_WIDTH / 2;
    circle.y = SCREEN_HEIGHT - PLATFORM_HEIGHT - CIRCLE_RADIUS * 2;
    circle.velocity_x = 0;
    circle.velocity_y = 0;
    circle.color = { 255, 255, 255, 255 };

    platform.x = (SCREEN_WIDTH - PLATFORM_WIDTH) / 2;
    platform.y = SCREEN_HEIGHT - PLATFORM_HEIGHT;
    platform.velocity_x = 0;

    if (loaded)
    { 
        int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
        int rect_y = 50;
        int i = 0;
        for (; i < RECT_COUNT / 7; i++) {
            rectangles[i].x = rect_x;
            rectangles[i].y = rect_y;
            rectangles[i].strength = (rand() % 3) + 1;
            rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 7].x = rect_x;
            rectangles[i + 7].y = rect_y + 50;
            rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 7].strength = (rand() % 3) + 1;
            rectangles[i + 14].x = rect_x;
            rectangles[i + 14].y = rect_y + 100;
            rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 14].strength = (rand() % 3) + 1;
            rectangles[i + 21].x = rect_x;
            rectangles[i + 21].y = rect_y + 150;
            rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 21].strength = (rand() % 3) + 1;
            rectangles[i + 28].x = rect_x;
            rectangles[i + 28].y = rect_y + 200;
            rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 28].strength = (rand() % 3) + 1;
            rectangles[i + 35].x = rect_x;
            rectangles[i + 35].y = rect_y + 250;
            rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 35].strength = (rand() % 3) + 1;
            rectangles[i + 42].x = rect_x;
            rectangles[i + 42].y = rect_y + 300;
            rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
            rectangles[i + 42].strength = (rand() % 3) + 1;

            rect_x += RECT_WIDTH + 10;
        }
    }
    else
    {
        if (lvl == 1)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i+7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = false;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = false;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i+42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rect_x += RECT_WIDTH + 10;
            }
        }
        if (lvl == 2)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = true;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = true;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = true;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[0].is_visible = false; rectangles[6].is_visible = false; rectangles[3].is_visible = false;
            rectangles[21].is_visible = false; rectangles[27].is_visible = false; rectangles[28].is_visible = false;
            rectangles[29].is_visible = false; rectangles[34].is_visible = false; rectangles[33].is_visible = false;
            rectangles[35].is_visible = false; rectangles[36].is_visible = false; rectangles[37].is_visible = false;
            rectangles[39].is_visible = false; rectangles[40].is_visible = false; rectangles[41].is_visible = false;
        }
        if (lvl == 3)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = false;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[7].is_visible = false; rectangles[8].is_visible = false; rectangles[12].is_visible = false; rectangles[13].is_visible = false;
        }
        if (lvl == 4)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = true;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = true;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[0].is_visible = false; rectangles[7].is_visible = false; rectangles[14].is_visible = false;
            rectangles[21].is_visible = false; rectangles[28].is_visible = false; rectangles[2].is_visible = false;
            rectangles[9].is_visible = false; rectangles[23].is_visible = false; rectangles[30].is_visible = false;
            rectangles[0].is_visible = false; rectangles[4].is_visible = false; rectangles[11].is_visible = false;
            rectangles[18].is_visible = false; rectangles[25].is_visible = false; rectangles[32].is_visible = false;
            rectangles[12].is_visible = false; rectangles[6].is_visible = false; rectangles[13].is_visible = false;
            rectangles[20].is_visible = false; rectangles[27].is_visible = false; rectangles[34].is_visible = false;
        }
        if (lvl == 5)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[1].is_visible = false; rectangles[3].is_visible = false; rectangles[5].is_visible = false;
            rectangles[7].is_visible = false; rectangles[9].is_visible = false; rectangles[11].is_visible = false;
            rectangles[13].is_visible = false; rectangles[15].is_visible = false; rectangles[17].is_visible = false;
            rectangles[19].is_visible = false;
        }
        if (lvl == 6)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = true;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[0].is_visible = false; rectangles[1].is_visible = false; rectangles[3].is_visible = false;
            rectangles[5].is_visible = false; rectangles[6].is_visible = false; rectangles[7].is_visible = false;
            rectangles[14].is_visible = false; rectangles[15].is_visible = false; rectangles[13].is_visible = false;
            rectangles[9].is_visible = false; rectangles[11].is_visible = false; rectangles[9].is_visible = false;
            rectangles[20].is_visible = false; rectangles[19].is_visible = false; rectangles[21].is_visible = false;
            rectangles[22].is_visible = false; rectangles[23].is_visible = false; rectangles[25].is_visible = false;
            rectangles[27].is_visible = false; rectangles[26].is_visible = false;
        }
        if (lvl == 7)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = true;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = true;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = true;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = true;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = true;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[9].is_visible = false; rectangles[11].is_visible = false; rectangles[17].is_visible = false;
            rectangles[23].is_visible = false; rectangles[25].is_visible = false; rectangles[30].is_visible = false;
            rectangles[32].is_visible = false;
        }
        if (lvl == 8)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 350;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = true;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = false;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = false;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = false;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
        }
        if (lvl == 9)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = false;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = false;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = false;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = false;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = true;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rectangles[i].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 7].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 14].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 21].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 28].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 35].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rectangles[i + 42].bonuse = (rand() % BONUSE_CHANCE) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[37].is_visible = true; rectangles[38].is_visible = true; rectangles[39].is_visible = true;
            rectangles[17].is_visible = true; rectangles[10].is_visible = true; rectangles[3].is_visible = true;
            rectangles[31].is_visible = true; rectangles[21].is_visible = true; rectangles[29].is_visible = true;
            rectangles[27].is_visible = true; rectangles[33].is_visible = true;
        }
        if (lvl == 10)
        {
            int rect_x = (SCREEN_WIDTH - (RECT_COUNT / 7 * (RECT_WIDTH + 10))) / 2;
            int rect_y = 50;
            int i = 0;
            for (; i < RECT_COUNT / 7; i++) {
                rectangles[i].x = rect_x;
                rectangles[i].y = rect_y;
                rectangles[i].is_visible = false;
                rectangles[i].strength = (rand() % 3) + 1;
                rectangles[i + 7].x = rect_x;
                rectangles[i + 7].y = rect_y + 50;
                rectangles[i + 7].is_visible = false;
                rectangles[i + 7].strength = (rand() % 3) + 1;
                rectangles[i + 14].x = rect_x;
                rectangles[i + 14].y = rect_y + 100;
                rectangles[i + 14].is_visible = false;
                rectangles[i + 14].strength = (rand() % 3) + 1;
                rectangles[i + 21].x = rect_x;
                rectangles[i + 21].y = rect_y + 150;
                rectangles[i + 21].is_visible = false;
                rectangles[i + 21].strength = (rand() % 3) + 1;
                rectangles[i + 28].x = rect_x;
                rectangles[i + 28].y = rect_y + 200;
                rectangles[i + 28].is_visible = false;
                rectangles[i + 28].strength = (rand() % 3) + 1;
                rectangles[i + 35].x = rect_x;
                rectangles[i + 35].y = rect_y + 250;
                rectangles[i + 35].is_visible = false;
                rectangles[i + 35].strength = (rand() % 3) + 1;
                rectangles[i + 42].x = rect_x;
                rectangles[i + 42].y = rect_y + 300;
                rectangles[i + 42].is_visible = false;
                rectangles[i + 42].strength = (rand() % 3) + 1;
                rect_x += RECT_WIDTH + 10;
            }
            rectangles[23].is_visible = true; rectangles[24].is_visible = true; rectangles[25].is_visible = true;
            rectangles[17].is_visible = true; rectangles[30].is_visible = true; rectangles[10].is_visible = true;
            rectangles[31].is_visible = true; rectangles[21].is_visible = true; rectangles[36].is_visible = true;
            rectangles[27].is_visible = true; rectangles[40].is_visible = true; rectangles[38].is_visible = true;
            rectangles[32].is_visible = true; rectangles[45].is_visible = true; rectangles[42].is_visible = true;
            rectangles[48].is_visible = true; rectangles[15].is_visible = true; rectangles[19].is_visible = true;
            rectangles[7].is_visible = true; rectangles[13].is_visible = true;
        }
    }

    loaded = false;
    return loaded;
}

void gameLoop(bool loaded)
{
    speedPlusTexture = loadTexture("plusSpeed.png", renderer);
    speedMinusTexture = loadTexture("minspeed.png", renderer);
    widthPlusTexture = loadTexture("plusWidth.png", renderer);
    widthMinusTexture = loadTexture("minWidth.png", renderer);
    backGame();
    int lvl = 1;
    if (loaded)
    {
        bonuse.isVisible = false;
        lvl = loadGame(lvl);
    }
    else
    {
        circle.speedMode = 0;
        platform.widthMode = 0;
        circle.score = 0;
        circle.lives = 3;
        int lvl = 1;
        bonuse.isVisible = false;
    }
    for (; lvl < 11;)
    {
        nextLvl = false;
        loaded = initialize(lvl, loaded);
        while (!quit && !nextLvl) {
            handleEvents(lvl);
            update(lvl);
            draw(lvl);
            if (nextLvl == true) {
                lvl++;
                circle.score += circle.lives * 100;
                circle.lives++;
            }
        }
        newLevel();
        circle.speedMode = 0;
        platform.widthMode = 0;
        bonuse.isVisible = false;
    }
    SDL_RenderClear(renderer);
    win();
    renderTexture(coolCat, renderer, 50, 200, 400, 400);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    newRecord();
}

void menu()
{
    backMenu();
    SDL_RenderClear(renderer);
    menuBackTexture = loadTexture("MenuBack.png", renderer);
    settingsBackTexture = loadTexture("settingsBack.png", renderer);
    recordsBackTexture = loadTexture("recordsBack.png", renderer);
    arkanoidTexture = loadTexture("Arkanoid.png", renderer);
    markerTexture = loadTexture("marker.png", renderer);
    continueTexture = loadTexture("continue.png", renderer);
    newGameTexture = loadTexture("new_game.png", renderer);
    recordsTexture = loadTexture("records.png", renderer);
    settingsTexture = loadTexture("settings.png", renderer);
    quitTexture = loadTexture("quit.png", renderer);

    selectedTexture = continueTexture;

    bool loaded;
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                case SDLK_UP:
                    if (selectedButton > 0)
                    {
                        klick();
                        selectedButton--;
                    }
                    break;
                case SDLK_DOWN:
                    if (selectedButton < 4)
                    {
                        klick();
                        selectedButton++;
                    }
                    break;
                case SDLK_RETURN:
                    switch (selectedButton)
                    {
                    case 0:
                        klick();
                        gameLoop(1);
                        break;
                    case 1:
                        klick();
                        gameLoop(0);
                        break;
                    case 2:
                        klick();
                        printRecord();
                        break;
                    case 3:
                        klick();
                        settings();
                        break;
                    case 4:
                        klick();
                        quit = true;
                        break;
                    }
                    break;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 380 && mouseY <= 440)
                {
                    selectedButton = 0;
                    klick();
                    gameLoop(1);
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 440 && mouseY <= 500)
                {
                    selectedButton = 1;
                    klick();
                    gameLoop(0);
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 500 && mouseY <= 560)
                {
                    selectedButton = 2;
                    klick();
                    printRecord();
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 560 && mouseY <= 620)
                {
                    selectedButton = 3;
                    klick();
                    settings();
                    break;
                }
                if (mouseX >= 120 && mouseX <= 360 && mouseY >= 620 && mouseY <= 660)
                {
                    selectedButton = 4;
                    klick();
                    quit = true;
                    break;
                }
                break;
            }
        }

        SDL_RenderClear(renderer);
        renderTexture(menuBackTexture, renderer, 0, 0, 480, 720);
        renderTexture(arkanoidTexture, renderer, 0, 80, 500, 200);
        renderTexture(markerTexture, renderer, 110, 379 + selectedButton * 60, 240, 60);
        renderTexture(selectedTexture, renderer, 120, 380, 240, 60);
        renderTexture(newGameTexture, renderer, 120, 440, 240, 60);
        renderTexture(recordsTexture, renderer, 120, 500, 240, 60);
        renderTexture(settingsTexture, renderer, 120, 560, 240, 60);
        renderTexture(quitTexture, renderer, 120, 620, 240, 60);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(continueTexture);
    SDL_DestroyTexture(settingsBackTexture);
    SDL_DestroyTexture(recordsBackTexture);
    SDL_DestroyTexture(markerTexture);
    SDL_DestroyTexture(menuBackTexture);
    SDL_DestroyTexture(arkanoidTexture);
    SDL_DestroyTexture(newGameTexture);
    SDL_DestroyTexture(recordsTexture);
    SDL_DestroyTexture(settingsTexture);
    SDL_DestroyTexture(quitTexture);
    SDL_DestroyTexture(speedPlusTexture);
    SDL_DestroyTexture(speedMinusTexture);
    SDL_DestroyTexture(widthPlusTexture);
    SDL_DestroyTexture(widthMinusTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_FreeMusic(fonMenu);
    Mix_FreeMusic(fonGame);
    Mix_FreeChunk(defeat);
    Mix_FreeChunk(victory);
    Mix_FreeChunk(minusHeart);
    Mix_FreeChunk(bounce);
    Mix_FreeChunk(broke);
    Mix_FreeChunk(newLvl);
    Mix_FreeChunk(button);
    Mix_FreeChunk(soundOfBonuse);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{

    Mix_Init(0);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512);
    int musVolume = 0;
    int effectsVolume = 0;

    ifstream file("volume.txt");
    file >> musVolume >> effectsVolume;
    file.close();
    Mix_VolumeMusic(musVolume);
    Mix_Volume(-1, effectsVolume);


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("ARKANOID", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "SDL window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "SDL renderer creation failed. SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    if (TTF_Init() < 0) {
        std::cout << "Failed to initialize SDL TTF: " << TTF_GetError() << std::endl;
        exit(1);
    }
    font = TTF_OpenFont("font.ttf", 20);
    if (font == NULL) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        exit(1);
    }

    menu();
    return 0;
}