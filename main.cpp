    #include <raylib.h>
    #include <cmath>
    #include <iostream>
    #include <vector>
    #include <cstdlib>
    #include <time.h>
    #include <string>
    #include <chrono>
    #include <thread>
    #include <algorithm>
    
    
    #include "player.h"

    #define MAX_SQUARE_COUNT 9

    Rectangle playerIconSize = (Rectangle){0,0,25,25};

    int currGameState[MAX_SQUARE_COUNT] = {0};

    double getSineValue(double x) {
        int offset = 0;
        double lambdaVal = 0.02;
        return 2 * sin(lambdaVal * x) + offset;
    }

    Vector2 calcScoreTextPosition(int currScore, int fontSize,Rectangle scoreContainer)  {
        Vector2 pos;

        int text_width = MeasureText(std::to_string(currScore).c_str(),fontSize);
        int text_height = fontSize;

        float x = scoreContainer.x + (scoreContainer.width - text_width) / 2 ;
        float y = scoreContainer.y + (scoreContainer.height - text_height) / 2;

        pos.x = x;
        pos.y = y;

        return pos;
    }


    void drawScoreBoard(std::vector<Player> &players, std::vector<Texture2D> playerIcons, int currPlayer) {

        // Setup the background of the scoreboard
        Color scoreBackColor = (Color){0x50, 0xC8, 0x78, 255};
        Rectangle scoreBackSize = (Rectangle){400,0,200,400};
        
        std::vector<std::pair<float,float>> cordPlayerIcon = {{445,40},{445,320}};
        std::vector<std::pair<float,float>> cordPlayerScore = {{505,40},{505,320}};
        
        DrawRectangleRec(scoreBackSize, scoreBackColor);
        DrawRectangleLinesEx(scoreBackSize, 5,(Color){0x4B,0xBC,0x70,255});
        
        for(int i = 0; i < 2; ++i)  {
            Rectangle playerProfileBackgroundSize = (Rectangle){cordPlayerIcon.at(i).first - 10, cordPlayerIcon.at(i).second - 7, 130, 64};
            std::string strPlayerNumber = "Player " + std::to_string(i + 1);
            DrawText(strPlayerNumber.c_str(), cordPlayerIcon.at(i).first + 15,cordPlayerIcon.at(i).second - 35,20, RAYWHITE);
            DrawRectangleRounded(playerProfileBackgroundSize, 0.25, 0, (Color){0x4C, 0xBB, 0x17, 255});
            //Draw the rectangle　behind the playerIcon and playerScore
            DrawRectangleRoundedLinesEx(playerProfileBackgroundSize, 0.25, 0, 2, (Color){0x46, 0xAC, 0x15, 255});
            
            //Draw the current match the player has one in that session.
            Rectangle scoreIconSize = (Rectangle){cordPlayerIcon.at(i).first, cordPlayerIcon.at(i).second,50,50};
            DrawRectangleLinesEx(scoreIconSize, 2 ,RAYWHITE);
            // Vector2 iconPos = (Vector2){cordPlayerIcon.at(i).first + 10, cordPlayerIcon.at(i).second + 5};
            Rectangle iconPos = (Rectangle){cordPlayerIcon.at(i).first + 4, cordPlayerIcon.at(i).second + 5,40,40};
            DrawTexturePro(playerIcons.at(i), playerIconSize, iconPos , (Vector2){0,0}, 0.0f,RAYWHITE);

            //Draws the score of current players
            Rectangle scoreDisplaySize = (Rectangle){cordPlayerScore.at(i).first, cordPlayerScore.at(i).second, 50,50};
            DrawRectangleRec(scoreDisplaySize, RAYWHITE);
            Vector2 relativeTextPosToContainer = calcScoreTextPosition(players.at(i).getPlayerPoint(),40, scoreDisplaySize);
            DrawText(std::to_string(players.at(i).getPlayerPoint()).c_str(),relativeTextPosToContainer.x, relativeTextPosToContainer.y,40,BLACK);
        }

        DrawText("Player's Turn",415, 115, 25, RAYWHITE);

        //Draw the sprite of the current player's turn
        Rectangle currentPlayerTurnBackground = (Rectangle){437,150,125,125};
        DrawRectangleRounded(currentPlayerTurnBackground, 0.15, 0 ,RAYWHITE);
        DrawRectangleRoundedLinesEx(currentPlayerTurnBackground,0.15,0,2,(Color){0xE2,0xDF,0xD2,255});
        DrawRectangleRoundedLinesEx((Rectangle){currentPlayerTurnBackground.x + 5,currentPlayerTurnBackground.y + 5, currentPlayerTurnBackground.width - 10, currentPlayerTurnBackground.height - 10},0.15,0,2,BLACK);
        
        int xOffsetPlayerTurnIcon =  (currPlayer == 0) ?5:0;
        Rectangle currentPlayerTurnIcon = (Rectangle){currentPlayerTurnBackground.x + xOffsetPlayerTurnIcon,currentPlayerTurnBackground.y + 5, 125,125};
        DrawTexturePro(playerIcons.at(currPlayer),playerIconSize,currentPlayerTurnIcon,(Vector2){0,0},0.0f,RAYWHITE);
    }

    void drawGameBoard(std::vector<Texture2D> playerIcons,int &currPlayer, bool &gameEnded) {
        bool isHoveringNow[MAX_SQUARE_COUNT] = {false};
        Rectangle fieldSquarePos[MAX_SQUARE_COUNT] = {0};

        const float squareSize = 100;
        const float padding = 20;
        const float startXpos = 20.0f;
        const float startYpos = 30;

        
        for(int i = 0; i < MAX_SQUARE_COUNT; ++i)  {
            fieldSquarePos[i].x = startXpos + (squareSize + padding) * (i%3);
            fieldSquarePos[i].y = startYpos + (squareSize + padding) * (i/3);
            fieldSquarePos[i].width = squareSize;
            fieldSquarePos[i].height = squareSize;
        }

        if(!gameEnded)  {
            Vector2 mousePoint = GetMousePosition();
    
            for(int i = 0; i < MAX_SQUARE_COUNT; ++i)  {
                bool hoveringNow = CheckCollisionPointRec(mousePoint, fieldSquarePos[i]);
                if(hoveringNow && !isHoveringNow[i]) {
                    isHoveringNow[i] = true;
                    fieldSquarePos[i].y -= 10;
                }
                else  {
                    isHoveringNow[i] = false;
                }
                
                if(hoveringNow && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(currGameState[i] == 0)  {
                        currGameState[i] = (currPlayer == 0) ? 1:2;
                        currPlayer = 1 - currPlayer;
                    }
                }
            }
        }

        for(int i = 0; i < MAX_SQUARE_COUNT; ++i)  {
            DrawRectangleRounded(fieldSquarePos[i],0.15,0,RAYWHITE);
            DrawRectangleRoundedLinesEx(fieldSquarePos[i],0.15,0,3,(Color){0xE2,0xDF,0xD2,255});

            if(currGameState[i] == 1)  {
                DrawTexturePro(playerIcons.at(currGameState[i] - 1),playerIconSize,(Rectangle){fieldSquarePos[i].x + 5,fieldSquarePos[i].y + 5, 100, 100},(Vector2){0,0},0,RAYWHITE);
            }
            else if(currGameState[i] == 2) {
                DrawTexturePro(playerIcons.at(currGameState[i] - 1),playerIconSize,(Rectangle){fieldSquarePos[i].x,fieldSquarePos[i].y + 5, 100, 100},(Vector2){0,0},0,RAYWHITE);
            }
        }
    }

    int checkGameStatus(bool gameEnded)  {
        const int WIN_COMBINATION[8][3] = {
            {0,1,2},
            {3,4,5},
            {6,7,8},
            {0,3,6},
            {1,4,7},
            {2,5,8},
            {0,4,8},
            {2,4,6}
        };

        int result = 0;

        for(int i = 0; i < 8; ++i)  {
            int a = WIN_COMBINATION[i][0];
            int b = WIN_COMBINATION[i][1];
            int c = WIN_COMBINATION[i][2];

            if(currGameState[a] != 0 && currGameState[a] == currGameState[b] && currGameState[b] == currGameState[c])  {
                result = currGameState[a];
            }
        }
        
        if(result == 0)  {
            bool isDraw = true;
            for(int i = 0; i < 9; ++i)  {
                if(currGameState[i] == 0)  {
                    isDraw = false;
                    break;
                }
            }
            if(isDraw)  {
                result = 3;
            }
        }

        
        return result;
    }


    int main() {
        srand(time(NULL));

        Player player1;
        Player player2;

        std::vector<Player> players = {player1,player2};
        
        InitWindow(600, 400, "Tic Tac Toe game");
        
        //Quick Tip: must load the texture 
        Texture2D ClickToPlayText = LoadTexture("assets/ClickToPlay.png");
        
        Texture2D InitialBackground = LoadTexture("assets/InitialBackground.png");
        Texture2D Stage1_bckg = LoadTexture("assets/background/Stage-1_Background.jpg");
        Texture2D Stage2_bckg = LoadTexture("assets/background/Stage-2_Background.jpg");
        Texture2D Stage3_bckg = LoadTexture("assets/background/Stage-3_Background.jpg");

        Texture2D playerIcon1 = LoadTexture("assets/spritesheet/DinoSprites - doux.png");
        Texture2D playerIcon2 = LoadTexture("assets/spritesheet/DinoSprites - mort.png");
        
        double x = 0.0;

        int currPlayer = 0;
        
        bool hasStarted = false;
        bool loadNewBackground = false;
        bool gameEnded = false;
                
        std::vector<Texture2D> backgrounds = {Stage1_bckg, Stage2_bckg, Stage3_bckg};
        std::vector<Texture2D> playerIcons = {playerIcon1, playerIcon2};
        int vecIndex = rand() % backgrounds.size();
        while (!WindowShouldClose()) {
            
            //Initialize
            x += 0.1;
            
            //Drawing -----------
            BeginDrawing();
            ClearBackground(SKYBLUE);

            //Setting the background to SKY_BLUE
            if(GetKeyPressed() > 0 && !hasStarted) {
                hasStarted = true;
                loadNewBackground = true;   
            }

            if(!hasStarted) {
                DrawTexture(InitialBackground, 0,0,RAYWHITE);
                DrawTexture(ClickToPlayText, 97, 265 + static_cast<int>(getSineValue(x)),RAYWHITE);
            }
            else if(hasStarted && loadNewBackground)  {
                ClearBackground(RAYWHITE);
                DrawTexture(backgrounds.at(vecIndex),-100,0,RAYWHITE);
                drawScoreBoard(players,playerIcons,currPlayer);
                drawGameBoard(playerIcons,currPlayer,gameEnded);

                int result;
                result = checkGameStatus(gameEnded);
                std::string endGameAnnoucement;
                if(result == 1 || result == 2)  {
                    float xOffset = (result == 2)? 5:0;

                    Color backgroundResultColor = (result == 2)? (Color){0xFF,0x00,0x00,255}:(Color){0x00,0x96,0xFF,255};
                    Color backgroundResultLineColor = (result == 2)?(Color){0xE9,0x00,0x00,255}:(Color){0x64,0x95,0xED,255};

                    DrawRectangle(0,0,600,400,(Color){0,0,0,50});
                    DrawRectangleRounded((Rectangle){55,175,265 + xOffset,40},0.3,0,backgroundResultColor);
                    DrawRectangleRoundedLinesEx((Rectangle){55,175,265 + xOffset,40},0.3,0,2,backgroundResultLineColor);
                    endGameAnnoucement = "Player " + std::to_string(result) + " has won";
                    DrawText(endGameAnnoucement.c_str(),60,180,30, RAYWHITE);

                    gameEnded = true;
                }
                else if(result == 3)  {
                    DrawRectangle(0,0,600,400,(Color){0,0,0,50});
                    endGameAnnoucement = "This game is a draw";
                    DrawRectangleRounded((Rectangle){55,175,310,40},0.15,0,BLACK);
                    DrawText(endGameAnnoucement.c_str(), 60,180,30,RAYWHITE);
                    gameEnded = true;
                }

                if(result != 0 && gameEnded)  {
                    Vector2 mousePoint = GetMousePosition();
                    Rectangle restartButtonSize = (Rectangle){85,300,200,50};
                    DrawRectangleRounded(restartButtonSize,0.3,0,(Color){0x4C, 0xBB, 0x17, 255});
                    DrawText("Play again", 110,310,30,RAYWHITE);
                    if(CheckCollisionPointRec(mousePoint, restartButtonSize) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  {
                        if(result == 1)  {
                            players.at(0).updatePlayerPoint();
                        }
                        else if(result == 2){
                            players.at(1).updatePlayerPoint();
                        }
                        gameEnded = false;
                        result = 0;
                        std::fill(currGameState, currGameState + MAX_SQUARE_COUNT, 0);
                    }
                }
            }
            
            EndDrawing();
        }

        UnloadTexture(InitialBackground);
        UnloadTexture(ClickToPlayText);

        UnloadTexture(Stage1_bckg);
        UnloadTexture(Stage2_bckg);
        UnloadTexture(Stage3_bckg);

        UnloadTexture(playerIcon1);
        UnloadTexture(playerIcon2);

        CloseWindow();

        return 0;
    }