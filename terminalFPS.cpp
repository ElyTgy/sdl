/*
g++ terminalFPS.cpp -o raycaster -I/opt/homebrew/opt/ncurses/include/ -L/opt/homebrew/opt/ncurses/lib -lncursesw
*/

#include <string>
#include <vector>
#include <iostream>
#include <locale.h>
#include <wchar.h>
#include <ncursesw/ncurses.h>

int main(){
    //varaibles
    int nScreenWidth;
    int nScreenHeight;
    
    setlocale(LC_ALL, "");

    //initialize
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    getmaxyx(stdscr, nScreenHeight, nScreenWidth);
    
    float fPlayerX = 7;
    float fPlayerY = 5;
    float fPlayerA = 3.14/2; //angle. Starting angle looking straight ahead

    float fFOV = 3.14/4; //arbitrary 45 degrees
    float fDepth = 16.0f;

    bool run = true;

    std::vector<float> distances;

    int mapWidth = 16;
    std::string map;
    map+="################";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="#..............#";
    map+="################";


    while(run){
        //handle raycasting
        for(int x=0; x<nScreenWidth; ++x){
            //get distance from the players current ray to the wall
            float fRayAngle = (fPlayerA - fFOV/2) + ((float)x/(float)nScreenWidth)*fFOV;

            float fDistaneToWall = 0;
            bool bHitWall = false;

            //goes counterclockwise
            float fcoordX = cosf(fRayAngle);
            float fcoordY = sinf(fRayAngle);

            while(!bHitWall && fDistaneToWall < fDepth){
                fDistaneToWall += 0.1;
                int TestX = (int)(fPlayerX + fcoordX*fDistaneToWall);
                int TestY = (int)(fPlayerY + fcoordY*fDistaneToWall);
                if(map[TestY*mapWidth+TestX] == '#'){bHitWall = true;}
                if(TestX < 0 || TestX >= mapWidth || TestY < 0 || TestY > mapWidth){
                    bHitWall = true;
                    fDistaneToWall = fDepth;
                }
            }

            float wallHeight = (int)((float)nScreenHeight/fDistaneToWall);
            float ceilingY = nScreenHeight / 2 - wallHeight / 2;
            float floorY = nScreenHeight / 2 + wallHeight / 2;

            for(int y = 0; y < nScreenHeight; ++y){
                if(y < ceilingY){
                    mvaddch(y, x, ' ');  
                }
                else if( y < floorY){
                    char wcWallSymbol;
                    if (fDistaneToWall <= 0.0f) {
                        wcWallSymbol = ' ';
                    }
                    else if (fDistaneToWall < 2.0f) {
                        wcWallSymbol = '#'; // █
                    }
                    else if (fDistaneToWall < 4.0f) {
                        wcWallSymbol = '='; // ▓
                    }
                    else if (fDistaneToWall < 6.0f) {
                        wcWallSymbol = '-'; // ▒
                    }
                    else if (fDistaneToWall < 8.0f) {
                        wcWallSymbol = '.'; // ░
                    }
                    else {
                        wcWallSymbol = ' ';
                    }

                    mvaddch(y, x, wcWallSymbol);
                }
                else{
                    mvaddch(y, x, ' ');
                }
            }
        }

        //handle map
        for(int i = 0; i < mapWidth; ++i){
            for(int j = 0; j < mapWidth; ++j){
                mvaddch(j, i, map[j * mapWidth + i]);
            }
        }
        mvaddch((int)fPlayerY, (int)fPlayerX, '@');


        //handle character input
        refresh();
        char ch = getch();

        float fDelta = 0.5;
        float newX = fPlayerX;
        float newY = fPlayerY;

        if(ch == 'w' || ch == 'W'){
            newX += cos(fPlayerA) * fDelta; 
            newY += sin(fPlayerA) + fDelta;
        }
        else if(ch == 's' || ch == 'S'){
            newX -= cos(fPlayerA) * fDelta; 
            newY -= sin(fPlayerA) + fDelta;
        }
        else if(ch == 'a' || ch == 'A'){
            fPlayerA -= 0.05;
        }
        else if(ch == 'd' || ch == 'D'){
            fPlayerA += 0.05;
        }
        else if(ch == 'q' || ch == 'Q'){
            run = false;
        }

        if(map[(int)newY*mapWidth+(int)newX] != '#'){
            fPlayerX = newX;
            fPlayerY = newY;
        }

        clear();
        napms(30);
    }

    endwin();
    return 0;
}