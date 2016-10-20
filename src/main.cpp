#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>     
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "util.h"
#include "camera.h"
#include <string>
#include "skatepark.h"
#include "screenshot.h"

using namespace std;

// F�ggv�nyek deklar�ci�ja
void ReDraw();
void onIdle();
void onInit();
void onReshape(int w, int h);
void onSpecial(int key, int x,int y);
void onSpecialUp(int key, int x, int y);
void onKey(unsigned char key, int x,int y);
void onKeyUp(unsigned char key, int x, int y);

Skatepark *park; // P�lya

int width=800;     // Ablak sz�less�ge
int height=600;    // Ablak magass�ga
bool isFull=false; // Teljes k�perny�s m�d
bool isWire=false; // Dr�tv�z m�d

// Foprogram
int main(int argc, char *argv[]){
    // Ablak meretenek es poziciojanak beallitasa
    glutInitWindowSize(width,height);
    glutInitWindowPosition(100,100);
    // Glut konyvtar inicializalasa
    glutInit(&argc, argv);
    // Display mod inicializalasa (RGB, duplabuffer, melyseg teszt)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    // Ablak letrehozasa adott nevvel
    glutCreateWindow("Skatepark");
    
    onInit();
    
    // Keyboard es display callback-ek beallitasa fuggvenyekre
    glutDisplayFunc(ReDraw);
    glutIdleFunc(onIdle);
    glutReshapeFunc(onReshape);
    glutSpecialFunc(onSpecial);
    glutSpecialUpFunc(onSpecialUp);
    glutKeyboardFunc(onKey);
    glutKeyboardUpFunc(onKeyUp);
    
    // Esemenykezelo ciklus inditasa
    glutMainLoop();
    
    return 0;   
}

// �tm�retez�s (ablak l�trehoz�sakor is megh�v�dik)
void onReshape(int w, int h){
    // Viewport �s vet�t�si transzform�ci� be�ll�t�sa
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(54,(float)w/h,0.1f,4000);
    width=w;
    height=h;
}

// Inicializ�l�s
void onInit(){
        
    #ifndef DEBUG_MODE
    // Ha nem debug m�dban van, akkor beolvassa a f�jlnevet
    string s;
    cout<<" OpenGL skatepark "<<endl<<"------------------"<<endl;
    cout<<"Enter filename: ";
    cin>>s;
    #else
    // Debug m�dban adott a f�jln�v
    cout<<"DEBUG MODE"<<endl<<endl;
    cout<<"OpenGL version: "<<(const char*)glGetString(GL_VERSION)<<endl; // OpenGL verzi� ki�r�sa
    string s(DEBUG_FILENAME);
    #endif
    
    // Park l�trehoz�sa
    park = new Skatepark(s.c_str());    
    park->Animate(0); // Kezdeti anim�ci� (kamera miatt)
    
    // OpenGL param�terek
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    
    // Ha teljes k�perny�s m�d van indul�skor
    #ifdef START_FULL_SCREEN
    glutFullScreen();
    isFull=true;
    #endif
}

long unsigned nDisp = 0; // Kirajzol�sok sz�ma
// Display callback
void ReDraw(){
    glClearColor(1,1,1,0); // Torlesi szin (R,G,B,A)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Torles
    park->Draw(); // Park kirajzol�sa
    nDisp++;
    glutSwapBuffers(); // Rajzol�s v�ge, buffercsere
}

long lastTime = 0; // Utols� szimul�ci� ideje

// Idle esem�ny, anim�l�s
void onIdle() {
    long time = glutGet(GLUT_ELAPSED_TIME);	// program inditasa ota eltelt ido
    
    float dt = time-lastTime;
    if(dt>10){
        dt=dt/1000.0f*SPEED_MULT;
        park->Animate(dt);
        lastTime=time;
        glutPostRedisplay();
    }
}

// Speci�lis billenty�k kezel�se
void onSpecial(int key, int x,int y){
    switch(key){
        case GLUT_KEY_LEFT:  park->GetCamera()->animPhase=-1;  break;
        case GLUT_KEY_RIGHT: park->GetCamera()->animPhase=1;   break;
        case GLUT_KEY_UP:    park->GetCamera()->animZangle=1;  break;
        case GLUT_KEY_DOWN:  park->GetCamera()->animZangle=-1; break;
    }
}

// Speci�lis billenty�k kezel�se
void onSpecialUp(int key, int x, int y){
    if(key==GLUT_KEY_LEFT || key==GLUT_KEY_RIGHT) park->GetCamera()->animPhase=0;
    if(key==GLUT_KEY_UP   || key==GLUT_KEY_DOWN)  park->GetCamera()->animZangle=0;
}

// Billenty�k kezel�se
void onKey(unsigned char key, int x,int y){
    switch(key){
    case 'x': park->GetCamera()->animZoom=-1; break; // Zoom be
    case 'y': park->GetCamera()->animZoom=1; break; // Zoom ki
    case 'a': park->GetCamera()->animPhase=1-abs(park->GetCamera()->animPhase); break; // Auto anim�ci�
    case 'q': park->GetCamera()->animMoveZ=-1; break; // Mozg�s le
    case 'e': park->GetCamera()->animMoveZ=1; break; // Mozg�s fel
    case 'w': park->GetCamera()->animMoveFw=1; break; // Mozg�s el�re
    case 'c': park->ToggleAnimClouds(); break; // Felh� anim�ci�
    case 'f': isFull=!isFull; if(isFull) glutFullScreen(); else{ glutReshapeWindow(800,600); glutPositionWindow(100,100);}break; // Teljes k�perny�s m�d
    case 27: delete park; exit(0); // Kil�p�s
    case 'g': if(isWire) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); else glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); isWire = !isWire; break; // Dr�tv�z m�d
    case ' ': cout<<"Average FPS: "<<(nDisp/(float)lastTime*1000.0f)<<endl; break; // FPS
    case 'h': park->ToggleGrid(); break; // Tengelyek �s r�cs
    case 's': Screenshot(); break; // Screenshot
    }
}

// Billenty�k kezel�se
void onKeyUp(unsigned char key, int x, int y){
    if(key=='x' || key=='y') park->GetCamera()->animZoom=0;
    if(key=='q' || key=='e') park->GetCamera()->animMoveZ=0;
    if(key=='w') park->GetCamera()->animMoveFw=0;
}
