#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include <GL/GLU.h>
#include <GL/GLUT.h>

// Kamera osztály
class Camera{
    float phase;   // Fázis
    float zangle;  // Dõlésszög a Z tengelytõl
    float eyedist; // Szempozíció távolsága a középponttól
    Vector center; // Középpont
    Vector light;  // Fényforrás helye
    
public:
    // Animációk
    int animPhase;  // Fázis
    int animZangle; // Z szög
    int animZoom;   // Zoom
    int animMoveZ;  // Fel-le mozgás
    int animMoveFw; // Elõre mozgás
    
    // Konstruktor, inicializálás
    Camera(){
        phase=0;
        zangle=20;
        eyedist=10;
        animPhase=animZangle=animZoom=animMoveZ=animMoveFw=0;
        center = Vector(0,0,1);
        light = Vector(5,1,5);
    }
    
    // Animáció
    void Animate(float dt){
        // Animálás
        if(animPhase!=0){
            float dph = dt*SPEED_CAM_PHASE;
            SetPhase(phase+dph*animPhase);
        }
        if(animZangle!=0){
            float dph = dt*SPEED_CAM_Z_ANLGE;
            SetZangle(zangle+dph*animZangle);
        }
        if(animZoom!=0){
            float dph = dt*SPEED_CAM_ZOOM;
            if(animZoom<0) SetEyedist(eyedist*pow(0.5f,dph));
            else SetEyedist(eyedist/pow(0.5f,dph));
        }
        if(animMoveZ!=0){
            float dph = dt*SPEED_CAM_MOVE_Z;
            MoveZ(dph*animMoveZ);
        }
        if(animMoveFw!=0){
            float dph = dt*SPEED_CAM_MOVE_FW;
            MoveForward(dph);
        }
        Vector eye = Vector(cos(zangle*M_PI/180)*cos(phase*M_PI/180),cos(zangle*M_PI/180)*sin(phase*M_PI/180),sin(zangle*M_PI/180))*eyedist + center;
        
        // Kamera transzformáció
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye.x,eye.y,eye.z,center.x,center.y,center.z,0,0,1);
        
        // Fényforrás megadása
        GLfloat I[]= {1,1,1,1};
        GLfloat pos[] = {light.x,light.y,light.z,0};
        glLightfv(GL_LIGHT0,GL_DIFFUSE,I);
        glLightfv(GL_LIGHT0,GL_AMBIENT,I);
        glLightfv(GL_LIGHT0,GL_SPECULAR,I);
        glLightfv(GL_LIGHT0,GL_POSITION,pos);
        glEnable(GL_LIGHT0);
    }
    
    // Lekérdezõ függvények
    float GetPhase(){ return phase; }
    float GetZangle(){ return zangle; }
    float GetEyedist(){ return eyedist; }
    
    // Beállító függvények
    void SetPhase(float value){
        phase=value;
        while(phase<0) phase+=360;
        while(phase>360) phase-=360;
    }
    void SetZangle(float value){
        zangle=value;
        if(zangle<1) zangle=1;
        if(zangle>89) zangle=89;
    }
    void SetEyedist(float value){
        eyedist = value;
        if(eyedist<0.1f) eyedist=0.1f;
    }
    void MoveZ(float z){
        center.z += z;
        if(center.z<0) center.z=0;
    }
    void MoveForward(float step){
        center.x-=step*cos(phase*M_PI/180);   
        center.y-=step*sin(phase*M_PI/180);
    }
    void SetLight(Vector light){
        this->light=light;
    }
};
#endif
