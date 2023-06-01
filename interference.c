#include<stdio.h>
#include<string.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

struct Data {
        float l,D,d,B,x,y,p1,p2;
        int n, s, a1, b1, a2, b2, v;
    } data;

void DrawWaves(int, int, int, int);

void DrawScreen(int, int, int, int);

int main(){
    int screenWidth = 800;
    int screenHeight = 450;

    data.s = 0;

    const char *GroupBox000Text = "Enter data for interference";
    const char *Label004Text = "Wavelength (m):";
    const char *Label005Text = "Slit Separation (m):";
    const char *Label006Text = "Screen Dsitance (m):";
    const char *Button008Text = "SIMULATE";
    char Label010Text[50] = "Width(m): ";
    char Label012Text[50] = "Bright Fringe Position(m): ";
    char Label013Text[50] = "Dark Fringe Position(m): ";
    const char *Spinner011Text = "Order:                 ";
    const char *SpeedText = "wavelength/sec :   ";
    
    Vector2 anchor01 = { 408, 4 };
    
    bool TextBox001EditMode = false;
    char TextBox001Text[10] = "20";
    bool TextBox002EditMode = false;
    char TextBox002Text[10] = "100";
    bool TextBox003EditMode = false;
    char TextBox003Text[10] = "200";
    bool Spinner011EditMode = false;
    int Spinner011Value = 2;

    bool SpeedBoxEditMode = false;
    int SpeedValue = 3;

    Rectangle layoutRecs[16] = {
        (Rectangle){ anchor01.x + -400, anchor01.y + 47, 241, 332 },
        (Rectangle){ anchor01.x + -288, anchor01.y + 61, 121, 23 },
        (Rectangle){ anchor01.x + -288, anchor01.y + 93, 121, 24 },
        (Rectangle){ 120, 128, 121, 24 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 61, 100, 23 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 92, 103, 24 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 125, 103, 24 },
        (Rectangle){ anchor01.x + -342, anchor01.y + 218, 120, 24 },
        (Rectangle){ anchor01.x + -400, anchor01.y + 245, 241, 10 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 258, 120, 24 },
        (Rectangle){ anchor01.x + -287, anchor01.y + 155, 119, 24 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 280, 120, 24 },
        (Rectangle){ anchor01.x + -392, anchor01.y + 303, 120, 24 },
        (Rectangle){ 254, 51, 491, 394 },
        (Rectangle){750,51,45,394},
        (Rectangle){anchor01.x + -288, anchor01.y + 185, 121, 23},
    };

    void Button008(){
        sscanf(TextBox001Text, "%f", &data.l);
        sscanf(TextBox002Text, "%f", &data.d);
        sscanf(TextBox003Text, "%f", &data.D);
        
        data.n = Spinner011Value;
        data.v = SpeedValue;

        data.x = data.n*data.l*sqrt(pow(data.D,2)/(pow(data.d,2)-pow(data.n*data.l,2)) + (float)1/4);
        data.y = ((float)2*data.n+1)/2*data.l*sqrt(pow(data.D,2)/(pow(data.d,2)-pow(((float)2*data.n+1)/2*data.l,2)) + (float)1/4);
        data.B = data.x/data.n;

        if(data.l/data.d >= (float)5/layoutRecs[13].height && data.l/data.d <= (float)150/3){
            data.s = 1;

            for(int i = 125; i<=layoutRecs[13].height; i++){
                if(data.l*i/data.d>=3){
                    data.a1 = (layoutRecs[13].height - i)/2;
                    data.b1 = data.a1+i;
                    data.p1 = data.d/i;
                    break;
                }
            }
        } else {
            data.s = 0;
        }

        data.p2 = 2.0*data.y/layoutRecs[13].height;
        data.a2 = (data.y - data.d/2)/data.p2;
        data.b2 = (data.y + data.d/2)/data.p2;

        sprintf(Label010Text, "Width(m): %f", data.x/data.n);
        sprintf(Label012Text, "Bright Fringe Position(m): %f", data.x);
        sprintf(Label013Text, "Dark Fringe Position(m): %f", data.y);

        FILE *fp = fopen("PREVDATA.txt","w");
        fprintf(fp, "%s\n%s\n%s\n%d\n%d\n", TextBox001Text, TextBox002Text, TextBox003Text, Spinner011Value,SpeedValue);
        fclose(fp);
    }

    FILE *fp = fopen("PREVDATA.txt","r");

    if(fp != NULL){
        fscanf(fp, "%s%s%s%d%d", TextBox001Text, TextBox002Text, TextBox003Text, &Spinner011Value, &SpeedValue);
    }

    fclose(fp);

    Button008();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(screenWidth, screenHeight, "Interference Simulation");
    SetWindowMinSize(screenWidth, screenHeight);
    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        if(IsWindowResized()){
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            layoutRecs[13].width = screenWidth - layoutRecs[13].x - layoutRecs[14].width - 10;
            layoutRecs[13].height = screenHeight - layoutRecs[13].y - 5;

            layoutRecs[14].x = screenWidth - 50;
            layoutRecs[14].height = screenHeight - layoutRecs[14].y - 5;

            Button008();
        }
 
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            DrawText("Interference Simulation", screenWidth/2 - 175 , 6, 30, BLUE);
            
            GuiGroupBox(layoutRecs[0], GroupBox000Text);
            if (GuiTextBox(layoutRecs[1], TextBox001Text, 10, TextBox001EditMode)) TextBox001EditMode = !TextBox001EditMode;
            if (GuiTextBox(layoutRecs[2], TextBox002Text, 10, TextBox002EditMode)) TextBox002EditMode = !TextBox002EditMode;
            if (GuiTextBox(layoutRecs[3], TextBox003Text, 10, TextBox003EditMode)) TextBox003EditMode = !TextBox003EditMode;
            GuiLabel(layoutRecs[4], Label004Text);
            GuiLabel(layoutRecs[5], Label005Text);
            GuiLabel(layoutRecs[6], Label006Text);
            if (GuiButton(layoutRecs[7], Button008Text)) Button008();
            GuiLine(layoutRecs[8], NULL);
            GuiLabel(layoutRecs[9], Label010Text);
            if(GuiSpinner(layoutRecs[15], SpeedText, &SpeedValue, 0, 10, SpeedBoxEditMode)) SpeedBoxEditMode = !SpeedBoxEditMode;
            if (GuiSpinner(layoutRecs[10], Spinner011Text, &Spinner011Value, 0, 100, Spinner011EditMode)) Spinner011EditMode = !Spinner011EditMode;
            GuiLabel(layoutRecs[11], Label012Text);
            GuiLabel(layoutRecs[12], Label013Text);
            GuiPanel(layoutRecs[13],"test");
            GuiPanel(layoutRecs[14],"test");

            DrawWaves(layoutRecs[13].x, layoutRecs[13].y, layoutRecs[13].width, layoutRecs[13].height);
            DrawScreen(layoutRecs[14].x, layoutRecs[14].y, layoutRecs[14].width, layoutRecs[14].height);            

        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}

void DrawWaves(int x, int y, int width, int height){
    if(data.s){
        DrawRectangle(x,y,width,height,BLACK);

        float y1,y2,sum;
        Color color;
        for(int i = 0; i<width; i++){
            for(int j = 0; j<height/2 + 1; j++){
                y1 = sin(2*PI*(data.l*data.v*GetTime() - sqrt(i*i + (j-data.a1)*(j-data.a1))*data.p1)/data.l);
                y2 = sin(2*PI*(data.l*data.v*GetTime() - sqrt(i*i + (j-data.b1)*(j-data.b1))*data.p1)/data.l);
                sum = y1 + y2;

                if(sum < 0){
                    color = BLUE;
                    sum /= -2;
                } else{
                    color = RED;
                    sum /= 2;
                }
                DrawPixel(x+i, y+j, Fade(color, sum));
                DrawPixel(x+i, y+height - j - 1, Fade(color, sum));
            }
        }

        DrawLine(x+data.D/data.p1,y,x+data.D/data.p1,y+height,BLUE);
        DrawLine(x, y + height/2 , x + width, y + height/2, GRAY);

        int p1 =  height/2 - data.x/data.p1, p2 = data.D/data.p1;
        if(p1 < 0){
            p2 += (data.D)/(data.x)*p1;
            p1 = 0;
        }

        if(p2 > width){
            p1 = data.x/data.D*p2;
            p2 = width;
        }

        DrawLine(x,y + height/2,x+p2,y+p1, WHITE);
        DrawLine(x,y + height/2,x+p2,y + height - p1, WHITE);

        DrawCircleSector((Vector2){x,y+data.a1} , 5 , 0 , 180 ,0,GREEN);
        DrawCircleSector((Vector2){x,y+data.b1} , 5 , 0 , 180 ,0,GREEN);
    } else {
        DrawText("NO PREVIEW AVAILABLE", x + width/2 - 110, y + height/2 - 20, 20, RED);
    }

}

void DrawScreen(int x, int y, int width, int height){
    DrawRectangle(x,y,width,height,BLACK);

    float path1, path2, pathDiff, tmp;

    for(int i = 1; i<height/2 + 1; i++){
        path1 = sqrt(pow(data.D,2) + pow((data.y - data.d/2) - i*data.p2,2));
        path2 = sqrt(pow(data.D,2) + pow((data.y + data.d/2) - i*data.p2,2));

        pathDiff = path1 - path2;
        tmp = (pathDiff<0)? -pathDiff: pathDiff;
        tmp /= data.l;
        pathDiff = (int)tmp;
        tmp -= pathDiff;
        tmp = (tmp>.5)? 1-tmp: tmp;
        tmp /= .5;
        tmp = 1 - tmp;

        DrawLine(x , y + i, x + width , y + i, Fade(RED,tmp));
        DrawLine(x , y + height - i - 1, x + width , y + height - i - 1, Fade(RED,tmp));
    }

    if(data.a2 > 0)
        DrawLine(x, y + data.a2, x + width, y + data.a2, GREEN);

    if(data.b2 < height)
        DrawLine(x, y + data.b2, x + width, y + data.b2, GREEN);
}
