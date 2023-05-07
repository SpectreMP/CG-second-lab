#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "stb-master/stb_easy_font.h"

#include "menu.h"

#define nameLen 20

int btnCnt = 0;
float mouseX,mouseY;

typedef struct
{
    char name[nameLen];
    float vert[8];
    char isHower, isDown, isDone;
    float buffer[50 * nameLen];
    int num_quads;
    float textPosX,textPosY,textScale;
    void (*action)();
} TBtn;

TBtn *btn = 0;


int Menu_AddButton(char *name, float x, float y, float width, float height, float textScale, void (*buttonAction)())
{
    btnCnt++;
    btn = realloc(btn, sizeof(btn[0]) * btnCnt);

    snprintf(btn[btnCnt-1].name, nameLen, "%s", name);
    float *vert = btn[btnCnt-1].vert;
    vert[0] = vert[6] = x;
    vert[2] = vert[4] = x + width;
    vert[1] = vert[3] = y;
    vert[5] = vert[7] = y + height;
    btn[btnCnt-1].isHower = 0;
    btn[btnCnt-1].isDown = 0;
    btn[btnCnt-1].isDone = 0;

    TBtn *b = btn + btnCnt - 1;
    b->num_quads = stb_easy_font_print(0,0, name, NULL, b->buffer, sizeof(b->buffer));
    b->textPosX = x + (width - stb_easy_font_width(name) * textScale) / 2.0;
    b->textPosY = y + (height - stb_easy_font_height(name) * textScale) / 2.0;;
    b->textPosY += textScale * 2;
    b->textScale = textScale;

    b->action = buttonAction;

    return btnCnt - 1;
}

void ShowButton(int buttonId)
{
    TBtn btn1 = btn[buttonId];
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert);
    glEnableClientState(GL_VERTEX_ARRAY);
        if(btn1.isDown) glColor3f(0.2f, 1.0f, 0.2f);
        else if (btn1.isHower) glColor3f(0.8f, 0.8f, 1);
        else glColor3f(0.6f,0.6f,0.8f);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1);
        glDrawArrays(GL_LINE_LOOP, 0,4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(btn1.textPosX, btn1.textPosY, 0);
        glScalef(btn1.textScale, btn1.textScale, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 4*sizeof(float), btn1.buffer);
            glDrawArrays(GL_QUADS, 0, btn1.num_quads*4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    if (btn1.isDown & !btn1.isDone)
    {
        btn1.action();
        btn[buttonId].isDone = 1;
    };
}

void Menu_ShowMenu()
{
    glPushMatrix();
        for (int i = 0; i < btnCnt; i++)
            ShowButton(i);
    glPopMatrix();
}

char isCoordInButton(int buttonId, float x, float y)
{
    float *vert = btn[buttonId].vert;
    return (x > vert[0]) && (y > vert[1]) &&(x < vert[4]) && (y < vert[5]);
}

int Menu_MouseMove(float x, float y)
{
    mouseX = x;
    mouseY = y;
    int moveBtn = -1;
    for(int i = 0; i < btnCnt; i++)
    {
        if (isCoordInButton(i, mouseX, mouseY))
        {
            btn[i].isHower = 1;
            moveBtn = i;
        }
        else
        {
            btn[i].isHower = 0;
            btn[i].isDown = 0;
        }
    }
    return moveBtn;
}

int Menu_MouseDown()
{
    int downBtn = -1;
    for (int i = 0; i < btnCnt; i++)
        if (isCoordInButton(i, mouseX, mouseY))
        {
            btn[i].isDown = 1;
            downBtn = i;
        }
    return downBtn;
}

void Menu_MouseUp()
{
    for (int i = 0; i < btnCnt; i++)
    {
        btn[i].isDown = 0;
        btn[i].isDone = 0;
    }
}

