#include <iostream>   //debug

#include "sl.h"       //grafic

#include <windows.h>    //Sleep
#include <mmsystem.h>    //audio

#include <cstdlib>
#include <time.h>
#include <ctime>                //ceas
#include <sstream>
#include <iomanip>

#include <fstream>      //fisiere

#include <algorithm>  //sortare struct scoruri pentru clasament

#include <conio.h>  // pentru tastare grafica

using namespace std;


struct Piece
{
    int Island[4][4];
    bool Compass;
    int Orientation;
    int Poz;
    bool Placed=false;
};

struct Chal
{
    int CompassO;
    int FV[9];
    int WinFV[9];
    int WinCompassO;
};

struct BoardType
{
    int dim;
    Piece P[17];
    int Q[5];
    Chal Challenge;
};

struct Texture
{
    int compass[5];
    int piece[9];
    int island[17];
    bool TextureNotLoaded=true;
    int wintex;
    int save;
    int leaderboard;
};

struct PlayersInTheLeaderBoard
{
    char Name[50];
    int FinalTime;
};

struct LeaderBoard
{
    PlayersInTheLeaderBoard Players[6];
    int counter=0;
    int char_counter;
    char ActualPlayer[50];
};

struct Game
{
    int Board[7][7];
    int iBoard[7][7];
    Texture tex;
    BoardType B;
    LeaderBoard L;
    int start_time=clock();
    int end_time;
    int current_time;
    int MouseX;
    int MouseY;
    int fullscreen=1;
    bool win=false;
    bool game_song=false;
    bool win_song=false;
    bool menu_song=false;
    bool AudioOn=true;
    bool AutoSolved=false;
    bool RandomNotLoaded=true;
    bool TimeStarted=false;
    bool LoadOption=false;
    bool Saving=false;
    bool ScoresSorted=false;
    bool NameEntered=false;
    int Solution[5];
    int ScreenState=1;
};

void RotateMatrix(Piece &X)
{
    for (int i=1; i<=3; i++)
    {
        for (int j=i+1; j<=3; j++)
        {
            swap(X.Island[i][j],X.Island[j][i]);
        }
    }

    for (int i=1;i<=3;i++)
    {
        int start=1;
        int end=3;
        while(start<end)
        {
            swap(X.Island[i][start],X.Island[i][end]);
            start++;
            end--;
        }
    }
}

void InitialRotate(Game &game)
{
    for(int i=1;i<5;i++) // pentru piesa 1,2,3,4
        for(int j=1;j<5;j++) // rotim si salvam fiecare stadiu rotit
        {
            RotateMatrix(game.B.P[i]);
            if(j<4)
                for(int x=1;x<4;x++)
                    for(int y=1;y<4;y++)
                        game.B.P[i+j*4].Island[x][y]=game.B.P[i].Island[x][y];
        }
}

void BoardPreset(Game &game)
{
    game.Board[1][1]=0;
    game.Board[1][2]=6;
    game.Board[1][3]=1;
    game.Board[1][4]=6;
    game.Board[1][5]=4;
    game.Board[1][6]=0;
    game.Board[2][1]=2;
    game.Board[2][2]=0;
    game.Board[2][3]=7;
    game.Board[2][4]=0;
    game.Board[2][5]=2;
    game.Board[2][6]=7;
    game.Board[3][1]=5;
    game.Board[3][2]=0;
    game.Board[3][3]=0;
    game.Board[3][4]=5;
    game.Board[3][5]=3;
    game.Board[3][6]=0;
    game.Board[4][1]=6;
    game.Board[4][2]=1;
    game.Board[4][3]=3;
    game.Board[4][4]=0;
    game.Board[4][5]=0;
    game.Board[4][6]=0;
    game.Board[5][1]=2;
    game.Board[5][2]=7;
    game.Board[5][3]=0;
    game.Board[5][4]=4;
    game.Board[5][5]=6;
    game.Board[5][6]=8;
    game.Board[6][1]=3;
    game.Board[6][2]=4;
    game.Board[6][3]=8;
    game.Board[6][4]=2;
    game.Board[6][5]=3;
    game.Board[6][6]=7;
    for(int i=1;i<=6;i++)
        for(int j=1;j<=6;j++)
            game.iBoard[i][j]=game.Board[i][j];
    game.B.P[1].Island[1][1]=0;
    game.B.P[1].Island[1][2]=1;
    game.B.P[1].Island[1][3]=0;
    game.B.P[1].Island[2][1]=0;
    game.B.P[1].Island[2][2]=0;
    game.B.P[1].Island[2][3]=0;
    game.B.P[1].Island[3][1]=0;
    game.B.P[1].Island[3][2]=1;
    game.B.P[1].Island[3][3]=0;
    game.B.P[2].Island[1][1]=0;
    game.B.P[2].Island[1][2]=0;
    game.B.P[2].Island[1][3]=0;
    game.B.P[2].Island[2][1]=0;
    game.B.P[2].Island[2][2]=1;
    game.B.P[2].Island[2][3]=0;
    game.B.P[2].Island[3][1]=0;
    game.B.P[2].Island[3][2]=1;
    game.B.P[2].Island[3][3]=0;
    game.B.P[3].Island[1][1]=0;
    game.B.P[3].Island[1][2]=0;
    game.B.P[3].Island[1][3]=1;
    game.B.P[3].Island[2][1]=0;
    game.B.P[3].Island[2][2]=0;
    game.B.P[3].Island[2][3]=0;
    game.B.P[3].Island[3][1]=1;
    game.B.P[3].Island[3][2]=0;
    game.B.P[3].Island[3][3]=0;
    game.B.P[4].Island[1][1]=0;
    game.B.P[4].Island[1][2]=0;
    game.B.P[4].Island[1][3]=1;
    game.B.P[4].Island[2][1]=0;
    game.B.P[4].Island[2][2]=0;
    game.B.P[4].Island[2][3]=0;
    game.B.P[4].Island[3][1]=0;
    game.B.P[4].Island[3][2]=1;
    game.B.P[4].Island[3][3]=0;

    for(int i=1;i<9;i++)
        game.B.Challenge.FV[i]=0;
    game.B.Challenge.FV[2]=2;
    game.B.Challenge.FV[3]=3;
    game.B.Challenge.CompassO=2;
    InitialRotate(game);
}

void Remove(int quadrant,Game &game)
{
        int i,j;
        if(quadrant==1)
        {
            for(i=1;i<=3;i++)
                for(j=1;j<=3;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==2)
        {
            for(i=1;i<=3;i++)
                for(j=4;j<=6;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==3)
        {
            for(i=4;i<=6;i++)
                for(j=1;j<=3;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==4)
        {
            for(i=4;i<=6;i++)
                for(j=4;j<=6;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else
        {
            cout<<"eroare cadran la remove"<<endl;
        }
}

int Placement(Piece X, int quadrant, Game &game)
{
        Remove(quadrant,game);
        if(quadrant==1)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i][j]=game.Board[i][j]*X.Island[i][j];
        }
        else if(quadrant==2)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i][j+3]=game.Board[i][j+3]*X.Island[i][j];
        }
        else if(quadrant==3)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i+3][j]=game.Board[i+3][j]*X.Island[i][j];
        }
        else if(quadrant==4)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i+3][j+3]=game.Board[i+3][j+3]*X.Island[i][j];
        }
        else
        {
            cout<<"eroare cadran la placement"<<endl;
        }
        return 0;
}

void RandomizedPreset(Game &game)
{
    int randP1,randP2,randP3,randP4;
    int randQ1,randQ2,randQ3,randQ4;
    srand(time(0));
    int contor=0,a,b;
    for(int i=1;i<=6;i++)
        for(int j=1;j<=6;j++)
            {
                game.iBoard[i][j]=rand()%8+1;

            }     //Random Board

    while(contor<5 or contor>15)
    {
        contor=rand()%36;
    }
    for(int i=1;i<=contor;i++)
    {
        a=rand()%6+1;
        b=rand()%6+1;
        while(game.iBoard[a][b]==0)
        {
            a=rand()%6+1;
            b=rand()%6+1;
        }
        game.iBoard[a][b]=0;
    }

    do
    {
        randP1=rand()%16+1;
        randP2=rand()%16+1;
        randP3=rand()%16+1;
        randP4=rand()%16+1;
    }while(randP1%4==randP2%4 || randP1%4==randP3%4 || randP1%4==randP4%4 || randP2%4==randP3%4 || randP2%4==randP4%4 || randP3%4==randP4%4);
    if(randP1%4==0)
        game.B.Challenge.CompassO=randP1/4;
    else if(randP2%4==0)
        game.B.Challenge.CompassO=randP2/4;
    else if(randP3%4==0)
        game.B.Challenge.CompassO=randP3/4;
    else if(randP4%4==0)
        game.B.Challenge.CompassO=randP4/4;
    for(int i=1;i<rand()%100;i++)
    {
        do
        {
            randQ1=rand()%4+1;
            randQ2=rand()%4+1;
            randQ3=rand()%4+1;
            randQ4=rand()%4+1;
        }while(randQ1==randQ2 || randQ1==randQ3 || randQ1==randQ4 || randQ2==randQ3 || randQ2==randQ4 || randQ3==randQ4);
    }
    Placement(game.B.P[randP1],randQ1,game);
    Placement(game.B.P[randP2],randQ2,game);
    Placement(game.B.P[randP3],randQ3,game);
    Placement(game.B.P[randP4],randQ4,game);
    for(int i=0;i<9;i++)
        game.B.Challenge.FV[i]=0;

    for(int i=1;i<=6;i++)
        for(int j=1;j<=6;j++)
            game.B.Challenge.FV[game.Board[i][j]]++;

    game.Solution[randQ1]=randP1;
    game.Solution[randQ2]=randP2;
    game.Solution[randQ3]=randP3;          //salvam solutia
    game.Solution[randQ4]=randP4;

    Remove(1,game);
    Remove(2,game);
    Remove(3,game);                        //resetam tabla
    Remove(4,game);


}

void ChooseTexture(int centerX,int centerY,int Length,int i,int j,Game &game)
{
    if(game.tex.TextureNotLoaded)
    {

        game.tex.TextureNotLoaded=false;
        game.tex.piece[1]=slLoadTexture("1.png");
        game.tex.piece[2]=slLoadTexture("2.png");
        game.tex.piece[3]=slLoadTexture("3.png");
        game.tex.piece[4]=slLoadTexture("4.png");
        game.tex.piece[5]=slLoadTexture("5.png");
        game.tex.piece[6]=slLoadTexture("6.png");
        game.tex.piece[7]=slLoadTexture("7.png");
        game.tex.piece[8]=slLoadTexture("8.png");
        game.tex.island[1]=slLoadTexture("IS1.png");
        game.tex.island[2]=slLoadTexture("IS2.png");
        game.tex.island[3]=slLoadTexture("IS3.png");
        game.tex.island[4]=slLoadTexture("IS4.png");
        game.tex.island[5]=slLoadTexture("IS5.png");
        game.tex.island[6]=slLoadTexture("IS6.png");
        game.tex.island[7]=slLoadTexture("IS7.png");
        game.tex.island[8]=slLoadTexture("IS8.png");
        game.tex.island[9]=slLoadTexture("IS9.png");
        game.tex.island[10]=slLoadTexture("IS10.png");
        game.tex.island[11]=slLoadTexture("IS11.png");
        game.tex.island[12]=slLoadTexture("IS12.png");
        game.tex.island[13]=slLoadTexture("IS13.png");
        game.tex.island[14]=slLoadTexture("IS14.png");
        game.tex.island[15]=slLoadTexture("IS15.png");
        game.tex.island[16]=slLoadTexture("IS16.png");
        game.tex.compass[1]=slLoadTexture("C1.png");
        game.tex.compass[2]=slLoadTexture("C2.png");
        game.tex.compass[3]=slLoadTexture("C3.png");
        game.tex.compass[4]=slLoadTexture("C4.png");
        game.tex.wintex=slLoadTexture("WinTex.png");
        game.tex.save=slLoadTexture("Save.png");

    }
    slSetForeColor(1,1,1,1);
    if(game.Board[i][j]==1)
    {
        slSprite(game.tex.piece[1],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==2)
    {
        slSprite(game.tex.piece[2],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==3)
    {
        slSprite(game.tex.piece[3],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==4)
    {
        slSprite(game.tex.piece[4],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==5)
    {
        slSprite(game.tex.piece[5],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==6)
    {
        slSprite(game.tex.piece[6],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==7)
    {
        slSprite(game.tex.piece[7],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==8)
    {
        slSprite(game.tex.piece[8],centerX,centerY,Length,Length);
    }
}



void PlaceGameTexture(int centerX, int centerY,int ratio,int quadrant,Game &game)
{
    int M=ratio/45,i,j;
    slSetForeColor(0,0,0,1);

    if(quadrant==1)
    {
        i=1; j=1;
    }
    else if(quadrant==2)
    {
        i=1; j=4;
    }
    else if(quadrant==3)
    {
        i=4;j=1;
    }
    else if(quadrant==4)
    {
        i=4;j=4;
    }
    else
    {
        cout<<"eroare in functia Place graphic la cadran"<<endl<<endl;
        exit(0);
    }

    ChooseTexture(centerX-M*2,centerY+M*2,M*2,i,j,game);
    ChooseTexture(centerX,centerY+M*2,M*2,i,j+1,game);
    ChooseTexture(centerX+M*2,centerY+M*2,M*2,i,j+2,game);
    ChooseTexture(centerX-M*2,centerY,M*2,i+1,j,game);
    ChooseTexture(centerX,centerY,M*2,i+1,j+1,game);
    ChooseTexture(centerX+M*2,centerY,M*2,i+1,j+2,game);
    ChooseTexture(centerX-M*2,centerY-M*2,M*2,i+2,j,game);
    ChooseTexture(centerX,centerY-M*2,M*2,i+2,j+1,game);
    ChooseTexture(centerX+M*2,centerY-M*2,M*2,i+2,j+2,game);
}

void PlaceIslandsOffBoard(int width,int length, int ratio, Game &game)
{
    slSetForeColor(1,1,1,1);
    if(!game.B.P[1].Placed)
        slSprite(game.tex.island[1],width/2+width/2.6,length/6*5.2,ratio/12,ratio/12);
    if(!game.B.P[2].Placed)
        slSprite(game.tex.island[2],width/2+width/2.6,length/6*3.7,ratio/12,ratio/12);
    if(!game.B.P[3].Placed)
        slSprite(game.tex.island[3],width/2+width/2.6,length/6*2.2,ratio/12,ratio/12);
    if(!game.B.P[4].Placed)
        slSprite(game.tex.island[4],width/2+width/2.6,length/6*0.7,ratio/12,ratio/12);
}

void IslandChecker(int width,int length, int ratio,Game &game)
{
    slSetForeColor(1,1,1,1);
    if(game.B.Q[1])
    {
        slSprite(game.tex.island[game.B.Q[1]],width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[2])
    {
        slSprite(game.tex.island[game.B.Q[2]],width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[3])
    {
        slSprite(game.tex.island[game.B.Q[3]],width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[4])
    {
        slSprite(game.tex.island[game.B.Q[4]],width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);
    }
}

void RemoveIsland(int width,int length,int ratio,Game &game)
{
    if(slGetKey(SL_KEY_BACKSPACE))
    {
        if(game.B.Q[1] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            Remove(1,game);
            if(game.B.Q[1]%4==0)
                game.B.Q[1]=4;
            else
                game.B.Q[1]=game.B.Q[1]%4;
            game.B.P[game.B.Q[1]].Placed=false;
            game.B.Q[1]=0;
        }
        else if(game.B.Q[2] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            Remove(2,game);
            if(game.B.Q[2]%4==0)
                game.B.Q[2]=4;
            else
                game.B.Q[2]=game.B.Q[2]%4;
            game.B.P[game.B.Q[2]].Placed=false;
            game.B.Q[2]=0;
        }
        else if(game.B.Q[3] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            Remove(3,game);
            if(game.B.Q[3]%4==0)
                game.B.Q[3]=4;
            else
                game.B.Q[3]=game.B.Q[3]%4;
            game.B.P[game.B.Q[3]].Placed=false;
            game.B.Q[3]=0;
        }
        else if(game.B.Q[4] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            Remove(4,game);
            if(game.B.Q[4]%4==0)
                game.B.Q[4]=4;
            else
                game.B.Q[4]=game.B.Q[4]%4;
            game.B.P[game.B.Q[4]].Placed=false;
            game.B.Q[4]=0;
        }
    }
}

void PlaceChallenges(int width,int length, int ratio,Game &game)
{
    slSetForeColor(1,1,1,1);
    int i,j,cnt=9;
    if(game.B.Challenge.CompassO)
    {
        slSprite(game.tex.compass[game.B.Challenge.CompassO],width/16,length/10*cnt,ratio/45*2,ratio/45*2);
        cnt--;
    }
    for(i=1;i<=8;i++)
    {
        j=0;
        while(game.B.Challenge.FV[i]!=j && game.B.Challenge.FV[i]<5)
        {
            slSprite(game.tex.piece[i],width/16,length/10*cnt,ratio/45*2,ratio/45*2);
            cnt--;
            j++;
        }
    }
}

void DrawBoard(int width,int length,int ratio,Game &game)
{

    slSetBackColor(0.949,0.761,0.537);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/16,length/2,ratio/10,length); //coloana challenge
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/16,length/2,ratio/10,length);

    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/2,length/2,ratio/3,ratio/3); // patratul mare
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/2,ratio/3,ratio/3);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 1
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 2
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 3
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 4
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/5.5,length/2,ratio/30,ratio/10);                          // buton "menu"
    slSetForeColor(0,0,0,1);
    slText(width/5.5,length/2,"Menu");
    slRectangleOutline(width/5.5,length/2,ratio/30,ratio/10);

    slSetForeColor(1,1,1,1);                                                            //buton save
    slSprite(game.tex.save,width/3,length/12*11,ratio/15,ratio/15);

    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/2,length/12*11,ratio/8,ratio/30);                         //caseta ceas
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/12*11,ratio/8,ratio/30);

    stringstream ss;
    ss << setfill('0') << setw(2) << static_cast<int> (game.current_time)/1000/60 << ":" // Minutes
       << setfill('0') << setw(2) << static_cast<int> (game.current_time)/1000%60;// << ":" // Seconds
      // << setfill('0') << setw(2) << static_cast<int> (game.current_time)%1000/10;       // Miliseconds     //ceas
    string formatted_time = ss.str();
    const char* time_cstr = formatted_time.c_str();
    slText(width/2,length/12*11,time_cstr);

    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        if(slGetMouseX()>= width/5.5-ratio/30 and slGetMouseX()<=width/5.5+ratio/30 and slGetMouseY()<=length/2+ratio/10 and slGetMouseY()>=length/2-ratio/10)
        {
            if(game.AudioOn==true)
            {
                game.game_song=false;
                game.menu_song=false;
                game.win_song=false;
            }
            game.ScreenState = 1;
        }
        if(slGetMouseX()>= width/3-ratio/30 and slGetMouseX()<=width/3+ratio/30 and slGetMouseY()<=length/12*11+ratio/30 and slGetMouseY()>=length/12*11-ratio/30)
        {
            game.Saving=true;
            game.ScreenState=5;
        }
    }

    PlaceGameTexture(width/2-ratio/13,length/2+ratio/13,ratio,1,game);
    PlaceGameTexture(width/2+ratio/13,length/2+ratio/13,ratio,2,game);
    PlaceGameTexture(width/2-ratio/13,length/2-ratio/13,ratio,3,game);
    PlaceGameTexture(width/2+ratio/13,length/2-ratio/13,ratio,4,game);

    PlaceIslandsOffBoard(width,length,ratio,game);
    PlaceChallenges(width,length,ratio,game);

    IslandChecker(width,length,ratio,game);

    RemoveIsland(width,length,ratio,game);
}

int StartDragging(int width,int length, int ratio, Game &game)
{
    slSetForeColor(1,1,1,1);
    if(game.MouseX>=width/2+width/2.6-ratio/24 && game.MouseX<=width/2+width/2.6+ratio/24)
    {
        if(!game.B.P[1].Placed && game.MouseY<=length/6*5.2+ratio/24 && game.MouseY>=length/6*5.2-ratio/24)
        {
            game.B.P[1].Placed=true;
            return 1;
        }
        else if(!game.B.P[2].Placed && game.MouseY<=length/6*3.7+ratio/24 && game.MouseY>=length/6*3.7-ratio/24)
        {
            game.B.P[2].Placed=true;
            return 2;
        }
        else if(!game.B.P[3].Placed && game.MouseY<=length/6*2.2+ratio/24 && game.MouseY>=length/6*2.2-ratio/24)
        {
            game.B.P[3].Placed=true;
            return 3;
        }
        else if(!game.B.P[4].Placed && game.MouseY<=length/6*0.7+ratio/24 && game.MouseY>=length/6*0.7-ratio/24)
        {
            game.B.P[4].Placed=true;
            return 4;
        }
    }
    return 0;
}

void Drop(int width,int length,int ratio, int &dragging, Game &game)
{
    if(!slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {

        if(game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[1]].Placed=false;
            game.B.Q[1]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[1]],1,game);
        }
        else if(game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[2]].Placed=false;
            game.B.Q[2]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[2]],2,game);
        }
        else if(game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[3]].Placed=false;
            game.B.Q[3]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[3]],3,game);
        }
        else if(game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[4]].Placed=false;
            game.B.Q[4]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[4]],4,game);
        }
    }
}

void Drag(int width,int length,int ratio,int &dragging,Game &game)
{
    slSetForeColor(1,1,1,1);
    if(dragging==0 && slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        dragging=StartDragging(width,length,ratio,game);
    }
    else if(dragging==1)
    {
        slSprite(game.tex.island[1],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==2)
    {
        slSprite(game.tex.island[2],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==3)
    {
        slSprite(game.tex.island[3],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==4)
    {
        slSprite(game.tex.island[4],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    if(dragging)
    {
        Drop(width,length,ratio,dragging,game);
    }
}

void DrawMenu(int width,int length,int ratio,Game &game)
{
    slSetBackColor(0.949,0.761,0.537);

    ifstream Save1("save1.out");
    if(Save1.is_open())
        game.LoadOption=true;
    ifstream Save2("save2.out");
    if(Save2.is_open())
        game.LoadOption=true;
    ifstream Save3("save3.out");
    if(Save3.is_open())
        game.LoadOption=true;
    ifstream Save4("save4.out");
    if(Save4.is_open())
        game.LoadOption=true;

    if(game.LoadOption)
    {
        slSetForeColor(0.4,0.765,0.851,1);
        slRectangleFill(width/2,length/2*1.75,ratio/15*1.5,ratio/15);            //buton Load
        slSetForeColor(0,0,0,1);
        slText(width/2,length/2*1.75,"Load");
        slRectangleOutline(width/2,length/2*1.75,ratio/15*1.5,ratio/15);
    }

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2*1.25,ratio/15*1.5,ratio/15);            //buton Play
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*1.25,"Play");
    slRectangleOutline(width/2,length/2*1.25,ratio/15*1.5,ratio/15);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2*0.75,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*0.75,"Settings");                                    //buton Settings
    slRectangleOutline(width/2,length/2*0.75,ratio/15*1.5,ratio/15);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2*0.25,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*0.25,"Exit");                                    //buton Exit
    slRectangleOutline(width/2,length/2*0.25,ratio/15*1.5,ratio/15);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width-width/8,length/8,ratio/20,ratio/30);
    slSetForeColor(1,1,1,1);
    slSprite(game.tex.leaderboard,width-width/8,length/8,ratio/20,ratio/30);                    //buton LeaderBoard
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width-width/8,length/8,ratio/20,ratio/30);

    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        if(slGetMouseX()>=width/2-width/16 and slGetMouseX()<=width/2+width/16)
            {
                if(slGetMouseY()<=length/2*1.25+ratio/30*1.5 and slGetMouseY()>=length/2*1.25-ratio/30*1.5 )
                {
                    game.ScreenState = 0;
                }
                else if(slGetMouseY()<=length/2*0.75+ratio/30*1.5  and slGetMouseY()>=length/2*0.75-ratio/30*1.5 )
                {
                    game.ScreenState = 2;
                    Sleep(100);
                }
                else if(slGetMouseY()<=length/2*0.25+ratio/30*1.5  and slGetMouseY()>=length/2*0.25-ratio/30*1.5 and game.ScreenState == 1)
                {
                    game.ScreenState = 3;
                }
                else if(slGetMouseY()<=length/2*1.75+ratio/30*1.5  and slGetMouseY()>=length/2*1.75-ratio/30*1.5)
                {
                    game.Saving=false;
                    game.ScreenState=5;
                }
            }

        if(slGetMouseX()>=width-width/8-ratio/40 and slGetMouseX()<=width-width/8+ratio/40)
            if(slGetMouseY()<=length/8+ratio/60 and slGetMouseY()>=length/8-ratio/60)
                game.ScreenState=6;
    }
}
void DrawSettings(int width,int length,int ratio,Game &game)
{
    slSetBackColor(0.949,0.761,0.537);
    slSetForeColor(0.4,0.765,0.851,1);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/10*9,"Settings");

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length-length/3,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length-length/3,"Audio");
    slRectangleOutline(width/2,length-length/3,ratio/15*1.5,ratio/15);
    slSetForeColor(0.4,0.765,0.851,1);
    //
    slRectangleFill(width/2,length-2*length/3,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length-2*length/3,"Menu");
    slRectangleOutline(width/2,length-2*length/3,ratio/15*1.5,ratio/15);

    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        Sleep(300);
        if(slGetMouseX()>=width/2-width/16 and slGetMouseX()<=width/2+width/16)
        {
            if(slGetMouseY()<=length-2*length/3+ratio/30*1.5-40 and slGetMouseY()>=length-2*length/3-ratio/30*1.5)
            {
                game.ScreenState = 1;
                Sleep(100);                  //buton menu
            }
            else if(slGetMouseY()<=length-length/3+ratio/30*1.5 and slGetMouseY()>=length-length/3-ratio/30*1.5)
            {
                if(game.AudioOn)
                {
                    game.menu_song=true;
                    game.win_song=true;
                    game.game_song=true;                                //opreste sunete
                    PlaySound(NULL,NULL,SND_ASYNC);
                    game.AudioOn=false;
                    Sleep(100);
                }
                else
                {
                    game.win_song=false;
                    game.game_song=false;
                    PlaySound("State1.wav",NULL,SND_ASYNC);
                    game.AudioOn=true;
                }
            }
        }
    }
}

bool CompareScores(PlayersInTheLeaderBoard a, PlayersInTheLeaderBoard b)
{
    return a.FinalTime<b.FinalTime;
}

void DrawLeaderBoard(int width,int length,int ratio,Game &game)
{
    slSetBackColor(0.949,0.761,0.537);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2,ratio/2,ratio/3);
    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/2,length/2,ratio/2.2,ratio/3.2);                 //tabel
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/2,ratio/2,ratio/3);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/8,length/2,ratio/20,ratio/30);
    slSetForeColor(0,0,0,1);
    slText(width/8,length/2,"Back");
    slRectangleOutline(width/8,length/2,ratio/20,ratio/30);

    slText(width/2,length-length/4,"Leaderboard");

    for(int i=1;i<=game.L.counter;i++)
    {
        slText(width/2-width/20,length/6*(4.7-i/1.5),game.L.Players[i].Name);
        slText(width/2,length/6*(4.7-i/1.5),"-");

        stringstream ss;
        ss<<setfill('0')<<setw(2)<<static_cast<int>(game.L.Players[i].FinalTime)/1000/60<<":"<<setfill('0')<<setw(2)<<static_cast<int>(game.L.Players[i].FinalTime)/1000%60;
        string formatted_time = ss.str();
        const char* Score = formatted_time.c_str();

        slText(width/2+width/20,length/6*(4.7-i/1.5),Score);
    }

     if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        //Sleep(300);
        if(slGetMouseX()>=width/8-ratio/40 && slGetMouseX()<=width/8+ratio/40 && slGetMouseY()<=length/2+ratio/60 && slGetMouseY()>=length/2-ratio/60)
        {
            game.ScreenState=1;
            game.menu_song=true;
        }
    }
}

void LoadTo(Game &game, int File)
{
    ifstream fin;
    if(File==1)
    {
        fin.open("save1.out");
    }
    else if(File==2)
    {
        fin.open("save2.out");
    }
    else if(File==3)
    {
        fin.open("save3.out");
    }
    else if(File==4)
    {
        fin.open("save4.out");
    }
    int i,j;
    for(i=1;i<=6;i++)
    {
        for(j=1;j<=6;j++)
        {
            fin>>game.iBoard[i][j];
        }
    }
    for(i=1;i<9;i++)
        fin>>game.B.Challenge.FV[i];
    fin>>game.B.Challenge.CompassO;
    for(i=1;i<=4;i++)
        fin>>game.B.P[i].Placed;
    for(i=1;i<=4;i++)
        fin>>game.B.Q[i];
    for(i=1;i<=6;i++)
        for(j=1;j<=6;j++)
            fin>>game.Board[i][j];
    fin>>game.current_time;
    fin>>game.AutoSolved;
    for(i=1;i<=4;i++)
        fin>>game.Solution[i];
    game.start_time=clock()-game.current_time;
    game.TimeStarted=true;
    fin.close();
    if(game.AudioOn==true)
    {
        game.win_song=false;
        game.game_song=false;
        game.menu_song=false;
    }
    game.ScreenState=0;
}

void SaveTo(Game &game, int File)
{
    ofstream fout;
    if(File==1)
    {
        fout.open("save1.out");
    }
    else if(File==2)
    {
        fout.open("save2.out");
    }
    else if(File==3)
    {
        fout.open("save3.out");
    }
    else if(File==4)
    {
        fout.open("save4.out");
    }
    int i,j;
    for(i=1;i<=6;i++)
    {
        for(j=1;j<=6;j++)
        {
            fout<<game.iBoard[i][j]<<" ";
        }
        fout<<endl;
    }
    for(i=1;i<9;i++)
        fout<<game.B.Challenge.FV[i]<<" ";
    fout<<game.B.Challenge.CompassO<<endl;
    for(i=1;i<=4;i++)
        fout<<game.B.P[i].Placed<<" ";
    fout<<endl;
    for(i=1;i<=4;i++)
        fout<<game.B.Q[i]<<" ";
    fout<<endl;
    for(i=1;i<=6;i++)
    {
        for(j=1;j<=6;j++)
            fout<<game.Board[i][j]<<" ";
        fout<<endl;
    }
    fout<<game.current_time<<endl;
    fout<<game.AutoSolved<<endl;
    fout<<game.Solution[1]<<" "<<game.Solution[2]<<" "<<game.Solution[3]<<" "<<game.Solution[4];
    fout.close();
    game.ScreenState=1;
}

void DrawLoadScreen(int width,int length,int ratio,Game &game)
{
    bool Save1Exist=false;
    bool Save2Exist=false;
    bool Save3Exist=false;
    bool Save4Exist=false;

    if(game.Saving==true)
    {
        Save1Exist=true;
        Save2Exist=true;
        Save3Exist=true;
        Save4Exist=true;
    }

    if(game.AudioOn==true)
    {
        game.game_song=false;
        game.menu_song=false;
        game.win_song=false;
    }

    slSetBackColor(0.949,0.761,0.537);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/8,length/2,ratio/20,ratio/30);
    slSetForeColor(0,0,0,1);
    slText(width/8,length/2,"Back");
    slRectangleOutline(width/8,length/2,ratio/20,ratio/30);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/7*5,ratio/3,ratio/20);
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/7*5,ratio/3,ratio/20);
    ifstream Save1("save1.out");
    if(Save1.is_open())
    {
        Save1Exist=true;
        slText(width/2,length/7*5,"Save 1");
    }
    else
        slText(width/2,length/7*5,"No Save");
    Save1.close();

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/7*4,ratio/3,ratio/20);
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/7*4,ratio/3,ratio/20);
    ifstream Save2("save2.out");
    if(Save2.is_open())
    {
        Save2Exist=true;
        slText(width/2,length/7*4,"Save 2");
    }
    else
        slText(width/2,length/7*4,"No Save");
    Save2.close();


    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/7*3,ratio/3,ratio/20);
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/7*3,ratio/3,ratio/20);
    ifstream Save3("save3.out");
    if(Save3.is_open())
    {
        Save3Exist=true;
        slText(width/2,length/7*3,"Save 3");
    }
    else
        slText(width/2,length/7*3,"No Save");
    Save3.close();

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/7*2,ratio/3,ratio/20);
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/7*2,ratio/3,ratio/20);
    ifstream Save4("save4.out");
    if(Save4.is_open())
    {
        Save4Exist=true;
        slText(width/2,length/7*2,"Save 4");
    }
    else
        slText(width/2,length/7*2,"No Save");
    Save4.close();

    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        Sleep(300);
        if(slGetMouseX()>=width/8-ratio/40 && slGetMouseX()<=width/8+ratio/40 && slGetMouseY()<=length/2+ratio/60 && slGetMouseY()>=length/2-ratio/60)
        {
            game.ScreenState=1;
            game.menu_song=true;
        }
        if(slGetMouseX()>=width/2-ratio/6 && slGetMouseX()<=width/2+ratio/6)
        {
            if(slGetMouseY()<=length/7*5+ratio/40 && slGetMouseY()>=length/7*5-ratio/40 && Save1Exist)
            {
                if(game.Saving)
                    SaveTo(game,1);
                else
                    LoadTo(game,1);
            }
            else if(slGetMouseY()<=length/7*4+ratio/40 && slGetMouseY()>=length/7*4-ratio/40 && Save2Exist)
            {
                if(game.Saving)
                    SaveTo(game,2);
                else
                    LoadTo(game,2);
            }
            else if(slGetMouseY()<=length/7*3+ratio/40 && slGetMouseY()>=length/7*3-ratio/40 && Save3Exist)
            {
                if(game.Saving)
                    SaveTo(game,3);
                else
                    LoadTo(game,3);
            }
            else if(slGetMouseY()<=length/7*2+ratio/40 && slGetMouseY()>=length/7*2-ratio/40 && Save4Exist)
            {
                if(game.Saving)
                    SaveTo(game,4);
                else
                    LoadTo(game,4);
            }
        }
    }
}

void RotateGraphics(int width,int length,int ratio,Game &game)
{
    if(slGetMouseButton(SL_MOUSE_BUTTON_RIGHT))
    {
        if(game.B.Q[1] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.Q[1]=game.B.Q[1]+4;
            if(game.B.Q[1]>16)
                game.B.Q[1]=game.B.Q[1]%16;
            Placement(game.B.P[game.B.Q[1]],1,game);
        }
        else if(game.B.Q[2] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.Q[2]=game.B.Q[2]+4;
            if(game.B.Q[2]>16)
                game.B.Q[2]=game.B.Q[2]%16;
            Placement(game.B.P[game.B.Q[2]],2,game);
        }
        else if(game.B.Q[3] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.Q[3]=game.B.Q[3]+4;
            if(game.B.Q[3]>16)
                game.B.Q[3]=game.B.Q[3]%16;
            Placement(game.B.P[game.B.Q[3]],3,game);
        }
        else if(game.B.Q[4] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.Q[4]=game.B.Q[4]+4;
            if(game.B.Q[4]>16)
                game.B.Q[4]=game.B.Q[4]%16;
            Placement(game.B.P[game.B.Q[4]],4,game);
        }
        Sleep(250);
    }
}

bool WinCondition(Game &game)
{
    int i,j;
    for(i=0;i<9;i++)
        game.B.Challenge.WinFV[i]=0;
    for(i=1;i<=6;i++)
        for(j=1;j<=6;j++)
            game.B.Challenge.WinFV[game.Board[i][j]]++;
    for(i=1;i<9;i++)
        if(game.B.Challenge.WinFV[i]!=game.B.Challenge.FV[i])
        {
            return false;
        }
    for(i=1;i<=4;i++)
        if(game.B.Q[i]%4==0 && game.B.Q[i])
        {
            game.B.Challenge.WinCompassO=game.B.Q[i]/4;
        }
    if(game.B.Challenge.WinCompassO!=game.B.Challenge.CompassO)
    {
        return false;
    }
    return true;
}

void DrawWinScreen(int width,int length, int ratio,Game &game)
{
    slSprite(game.tex.wintex,width/2,length/2,ratio/3,ratio/3);

    if(!game.ScoresSorted)
    {
        slSetForeColor(0.325,0.76,0.295,1);
        slText(width/2,length-length/4,"NEW HIGHSCORE REGISTERED IN THE LEADERBOARD");
    }
    slRender();
}

void AutoSolve(Game &game)
{
    Remove(1,game);
    Remove(2,game);
    Remove(3,game);
    Remove(4,game);
    Placement(game.B.P[game.Solution[1]],1,game);
    Placement(game.B.P[game.Solution[2]],2,game);
    Placement(game.B.P[game.Solution[3]],3,game);
    Placement(game.B.P[game.Solution[4]],4,game);
    game.B.Q[1]=game.Solution[1];
    game.B.Q[2]=game.Solution[2];
    game.B.Q[3]=game.Solution[3];
    game.B.Q[4]=game.Solution[4];
    game.B.P[1].Placed=true;
    game.B.P[2].Placed=true;
    game.B.P[3].Placed=true;
    game.B.P[4].Placed=true;
    game.AutoSolved=true;
}


void StartGraphics(Game &game)
{
    cout<<"Please enter your name:";
    cin>>game.L.ActualPlayer;
    int x=GetSystemMetrics(SM_CXSCREEN),y= GetSystemMetrics(SM_CYSCREEN), ratio=x/y*2200, dragging=0;

    slWindow(x,y,"Pirates Hide And Seek",1);
    slSetFont(slLoadFont("whitrabt.ttf"),24);
    slSetTextAlign(SL_ALIGN_CENTER);

    game.tex.leaderboard=slLoadTexture("Leaderboard.png");

    game.B.Q[1]=0;
    game.B.Q[2]=0;
    game.B.Q[3]=0;
    game.B.Q[4]=0;
    game.end_time=0;
    game.ScreenState=1;
    game.L.char_counter=0;
    game.L.Players[1].FinalTime=999999;
    game.L.Players[2].FinalTime=999999;
    game.L.Players[3].FinalTime=999999;
    game.L.Players[4].FinalTime=999999;
    game.L.Players[5].FinalTime=999999;

    /*
        ScreenState:
        0 -> Game
        1 -> Menu
        2 -> Settings
        3 -> Exit
        4 -> WinScreen
        5 -> Save/Load Screen
        6 -> Leaderboard
    */

    while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
    {
        if(slGetDeltaTime()>0.35)
            cout<<"sub 30 frame-uri"<<endl;
        if(game.ScreenState==1)
        {
            if(!game.menu_song)
            {
                game.win_song=false;
                game.menu_song=true;                                          //muzica
                PlaySound("State1.wav",NULL,SND_ASYNC);
            }

            if(!game.ScoresSorted)
            {
                game.L.counter=0;
                ifstream fin("LeaderBoard.txt");
                while(fin>>game.L.Players[++game.L.counter].Name)
                {
                    fin>>game.L.Players[game.L.counter].FinalTime;                                                             //clasament
                }
                game.L.counter--;

                sort(game.L.Players+1,game.L.Players+6,CompareScores);

                game.ScoresSorted=true;
            }

            if(game.RandomNotLoaded)
            {
                RandomizedPreset(game);                                         //creeaza mereu un alt set de provocari

                int i,j;
                for( i=1;i<=6;i++)
                    for( j=1;j<=6;j++)
                        game.Board[i][j]=game.iBoard[i][j];
                for(i=1;i<=4;i++)                                               //reset joc
                    game.B.Q[i]=0;
                for(i=1;i<=16;i++)
                    game.B.P[i].Placed=false;

                game.AutoSolved=false;
                game.RandomNotLoaded=false;
                game.TimeStarted=false;
            }
            DrawMenu(x,y,ratio,game);
            slRender();
            Sleep(100);
        }
        else if(game.ScreenState==0)
        {
                if(!game.TimeStarted)
                {
                    game.start_time=clock();
                    game.TimeStarted=true;
                }
                if(!game.RandomNotLoaded)
                    game.RandomNotLoaded=true;                      //pentru a crea un posibil ulterior nou joc

                if(!game.game_song)
                {
                    game.menu_song=false;
                    game.game_song=true;                                        // muzica
                    PlaySound("State0.wav",NULL,SND_ASYNC);
                }
                game.MouseX=slGetMouseX();
                game.MouseY=slGetMouseY();

                game.current_time=clock()-game.start_time;
                if(slGetKey(SL_KEY_ENTER))
                {
                    AutoSolve(game);
                }
                game.win=WinCondition(game);
                if(game.win && !game.AutoSolved)
                {
                    game.end_time=clock()-game.start_time;
                   game.ScreenState=4;
                }

                DrawBoard(x,y,ratio,game);
                Drag(x,y,ratio,dragging,game);
                RotateGraphics(x,y,ratio,game);

                slRender();
        }
        else if(game.ScreenState==2)
        {
                DrawSettings(x,y,ratio,game);
                slRender();
        }
        else if(game.ScreenState == 3)
        {
            break;
        }
        else if(game.ScreenState==4)
        {
            if(!game.win_song)
            {
                game.win_song=true;
                game.game_song=false;                                   //muzica 4
                PlaySound("State4.wav",NULL,SND_ASYNC);
            }
            Sleep(500);

            if(game.L.counter<5)
            {
                game.L.counter++;
                strcpy(game.L.Players[game.L.counter].Name,game.L.ActualPlayer);
                game.L.Players[game.L.counter].FinalTime=game.end_time;
                                                                                                                // mai putin de 5 scoruri in tabela -> record nou
                ofstream fout("LeaderBoard.txt");
                for(int i=1;i<=game.L.counter;i++)
                    fout<<game.L.Players[i].Name<<" "<<game.L.Players[i].FinalTime<<endl;
                game.ScoresSorted=false;
            }

            else if(game.end_time<game.L.Players[5].FinalTime)
            {
                strcpy(game.L.Players[5].Name,game.L.ActualPlayer);
                game.L.Players[5].FinalTime=game.end_time;
                                                                                                                // record nou
                ofstream fout("LeaderBoard.txt");
                for(int i=1;i<=game.L.counter;i++)
                    fout<<game.L.Players[i].Name<<" "<<game.L.Players[i].FinalTime<<endl;
                game.ScoresSorted=false;
            }

            DrawWinScreen(x,y,ratio,game);

            game.win=false;

            Sleep(3000);

            game.ScreenState=1;
        }
        else if(game.ScreenState==5)
        {
            DrawLoadScreen(x,y,ratio,game);
            slRender();
        }
        else if(game.ScreenState==6)
        {
            DrawLeaderBoard(x,y,ratio,game);
            slRender();
        }
    }
    slClose();
}



int main()
{
    Game game;

    BoardPreset(game);
    StartGraphics(game);

    return 0;
}
