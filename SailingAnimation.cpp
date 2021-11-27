
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define PI 3.1415
#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

// global variables

bool activeTimer = false; // for linear motion (F1)
bool activeTimer2 = false; // for wave motion (F2)
bool flag = false;
float angle1;
int xS = 0, yS = 0; // sailingboat's initial position
int mouseX, mouseY; // mouse position
int heightOfMast = 45,
widthOfMast = 4,
heightOfSail1 = 42,
widthOfSail1 = 38,
heightOfSail2 = 32,
widthOfSail2 = 18,
heightOfSail3 = 10,
widthOfSail3 = 18,
heightOfHull = 15,
widthOfHull1 = 80,
widthOfHull2 = 40,
rOfNumCircle = 9,
rOfNumCircle2 = 3;
struct shape {
    int count, coX, coY;
};

shape move[10]; // array for 10 shapes
int r = 0;
int count = 0;


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawSailingBoat()
{
    for (int r = 1; r <= 10; r++)
    {
        xS = (move + r)->coX;
        yS = (move + r)->coY;

        // draw hull
        glColor3f(0.7, 0.3, 0);
        glBegin(GL_QUADS);
        glVertex2f(xS - widthOfMast - widthOfSail1, yS - heightOfMast);
        glVertex2f(xS + widthOfSail1, yS - heightOfMast);
        glVertex2f(xS + widthOfSail2, yS - heightOfMast - heightOfHull);
        glVertex2f(xS - widthOfSail2 - widthOfMast, yS - heightOfMast - heightOfHull);
        glEnd();

        // draw mast
        glColor3f(0.7, 0.3, 0);
        glBegin(GL_QUADS);
        glVertex2f(xS, yS);
        glVertex2f(xS - widthOfMast, yS);
        glVertex2f(xS - widthOfMast, yS - heightOfMast);
        glVertex2f(xS, yS - heightOfMast);
        glEnd();

        // draw sail2
        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_TRIANGLES);
        glVertex2f(xS - widthOfMast, yS - heightOfSail3 - heightOfSail2);
        glVertex2f(xS - widthOfMast - widthOfSail2, yS - heightOfSail2 - heightOfSail3);
        glVertex2f(xS - widthOfMast, yS - heightOfSail3);
        glEnd();
        // draw sail1
        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_TRIANGLES);
        glVertex2f(xS, yS);
        glVertex2f(xS, yS - heightOfSail1);
        glVertex2f(xS + widthOfSail1, yS - heightOfSail1);
        glEnd();

        // draw sail3
        glColor3f(1, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(xS - widthOfMast, yS);
        glVertex2f(xS - widthOfMast, yS - heightOfSail3);
        glVertex2f(xS - widthOfMast - widthOfSail3, yS - heightOfSail3);
        glEnd();

        // draw spaces for oar
        glColor3f(0.5, 0.3, 0.3);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++)
        {
            angle1 = 2 * PI * i / 100;
            glVertex2f((xS + 10) + rOfNumCircle2 * cos(angle1), (yS - 52) + rOfNumCircle2 * sin(angle1));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++)
        {
            angle1 = 2 * PI * i / 100;
            glVertex2f((xS)+rOfNumCircle2 * cos(angle1), (yS - 52) + rOfNumCircle2 * sin(angle1));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++)
        {
            angle1 = 2 * PI * i / 100;
            glVertex2f((xS - 10) + rOfNumCircle2 * cos(angle1), (yS - 52) + rOfNumCircle2 * sin(angle1));
        }
        glEnd();

        // draw circle wire for number
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; i++)
        {
            angle1 = 2 * PI * i / 100;
            glVertex2f((xS + 12) + rOfNumCircle * cos(angle1), (yS - 30) + rOfNumCircle * sin(angle1));
        }
        glEnd();

        glColor3f(0, 0, 0.3);
        vprint(xS + 6, yS - 33, GLUT_BITMAP_8_BY_13, "%d", r);

        //draw oar
        glColor3f(0.6, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(xS + 10, yS - 52);
        glVertex2f(xS - 15, yS - 72);
        glEnd();
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex2f(xS, yS - 52);
        glVertex2f(xS - 25, yS - 72);
        glEnd();
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex2f(xS - 10, yS - 52);
        glVertex2f(xS - 35, yS - 72);
        glEnd();
        glLineWidth(2);
    }
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 5, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw land

    glColor3f(0, 0.5, 0.2);
    glBegin(GL_POLYGON);
    glVertex2f(400, -400);
    glVertex2f(-400, -400);
    glVertex2f(-400, -300);
    glVertex2f(-360, -290);
    glVertex2f(-340, -300);
    glVertex2f(-300, -290);
    glVertex2f(-280, -300);
    glVertex2f(-240, -290);
    glVertex2f(-220, -300);
    glVertex2f(-180, -290);
    glVertex2f(-160, -300);
    glVertex2f(-120, -290);
    glVertex2f(-100, -300);
    glVertex2f(-60, -290);
    glVertex2f(-40, -300);
    glVertex2f(0, -290);
    glVertex2f(20, -300);
    glVertex2f(60, -290);
    glVertex2f(80, -300);
    glVertex2f(120, -290);
    glVertex2f(140, -300);
    glVertex2f(160, -290);
    glVertex2f(200, -300);
    glVertex2f(220, -290);
    glVertex2f(260, -300);
    glVertex2f(280, -290);
    glVertex2f(320, -300);
    glVertex2f(340, -290);
    glVertex2f(360, -300);
    glVertex2f(400, -290);
    glVertex2f(400, -300);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(400, 400);
    glVertex2f(-400, 400);
    glVertex2f(-400, 300);
    glVertex2f(-360, 310);
    glVertex2f(-340, 300);
    glVertex2f(-300, 310);
    glVertex2f(-280, 300);
    glVertex2f(-240, 310);
    glVertex2f(-220, 300);
    glVertex2f(-180, 310);
    glVertex2f(-160, 300);
    glVertex2f(-120, 310);
    glVertex2f(-100, 300);
    glVertex2f(-60, 310);
    glVertex2f(-40, 300);
    glVertex2f(0, 310);
    glVertex2f(20, 300);
    glVertex2f(60, 310);
    glVertex2f(80, 300);
    glVertex2f(120, 310);
    glVertex2f(140, 300);
    glVertex2f(160, 310);
    glVertex2f(200, 300);
    glVertex2f(220, 310);
    glVertex2f(260, 300);
    glVertex2f(280, 310);
    glVertex2f(320, 300);
    glVertex2f(340, 310);
    glVertex2f(360, 300);
    glVertex2f(400, 310);
    glVertex2f(400, 300);
    glEnd();

    // draw sailing boat

    if (flag)
    {
        drawSailingBoat();
    }

    // label for name and ID number
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-65, 395);
    glVertex2f(50, 395);
    glVertex2f(50, 350);
    glVertex2f(-65, 350);
    glEnd();
    glLineWidth(2);

    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    glVertex2f(-65, 395);
    glVertex2f(50, 395);
    glVertex2f(50, 350);
    glVertex2f(-65, 350);
    glEnd();

    glColor3f(0, 0, 0);
    vprint(-60, 380, GLUT_BITMAP_8_BY_13, "Nihat Ozdemir");
    vprint(-40, 360, GLUT_BITMAP_8_BY_13, "21802080");

    // information for user to start
    vprint(-250, -330, GLUT_BITMAP_8_BY_13, "Please press the left click the sea to create a sailingboat!");
    vprint(-250, -345, GLUT_BITMAP_8_BY_13, "Please press F1 to move or stop sailingboats in linear motion!");
    vprint(-250, -360, GLUT_BITMAP_8_BY_13, "Please press F2 to move or stop sailingboats in wavy motion!");
    vprint(-250, -375, GLUT_BITMAP_8_BY_13, "Please press ESC to exit program!");


    // current coordinats
    vprint(310, -380, GLUT_BITMAP_8_BY_13, "[%d,%d]", mouseX, mouseY);


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


    // if (key == ' ')
      //   activeTimer =! activeTimer;

     // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.

    if (key == GLUT_KEY_F1) // linear movement
        activeTimer = !activeTimer;
    else if (key == GLUT_KEY_F2) // wavy movement
        activeTimer2 = !activeTimer2;


    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    mouseX = x - winWidth / 2;
    mouseY = winHeight / 2 - y;


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        if (mouseY<300 && mouseY >-220)
        {

            (move + r)->count = count;
            (move + r)->coX = mouseX;
            (move + r)->coY = mouseY;
            count++;
            r++;
            flag = true;

        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    mouseX = x - winWidth / 2;
    mouseY = winHeight / 2 - y;



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (activeTimer)
    {
        // calculations for linear movement
        for (int i = 0; i <= 10; i++)
        {
            if ((move + i)->coX + 40 < 400)
                (move + i)->coX++;
            else
                (move + i)->coX = -400;
        }
    }
    else if (activeTimer2)
    {
        // calculations for wavy movement ( it is additional )
        for (int i = 0; i <= 10; i++)
        {
            if ((move + i)->coX + 40 < 400)
            {

                (move + i)->coY = (80 * sin(((move + i)->coX) / 20)) + 45;
                (move + i)->coX++;
            }
            else
                (move + i)->coX = -400;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(0));

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Nihat's Homework");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
