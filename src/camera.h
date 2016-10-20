#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include <GL/GLU.h>
#include <GL/GLUT.h>

// Kamera oszt�ly
class Camera{
    float phase;   // F�zis
    float zangle;  // D�l�ssz�g a Z tengelyt�l
    float eyedist; // Szempoz�ci� t�vols�ga a k�z�ppontt�l
    Vector center; // K�z�ppont
    Vector light;  // F�nyforr�s helye
    
public:
    // Anim�ci�k
    int animPhase;  // F�zis
    int animZangle; // Z sz�g
    int animZoom;   // Zoom
    int animMoveZ;  // Fel-le mozg�s
    int animMoveFw; // El�re mozg�s
    
    // Konstruktor, inicializ�l�s
    Camera(){
        phase=0;
        zangle=20;
        eyedist=10;
        animPhase=animZangle=animZoom=animMoveZ=animMoveFw=0;
        center = Vector(0,0,1);
        light = Vector(5,1,5);
    }
    
    // Anim�ci�
    void Animate(float dt){
        // Anim�l�s
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
        
        // Kamera transzform�ci�
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye.x,eye.y,eye.z,center.x,center.y,center.z,0,0,1);
        
        // F�nyforr�s megad�sa
        GLfloat I[]= {1,1,1,1};
        GLfloat pos[] = {light.x,light.y,light.z,0};
        glLightfv(GL_LIGHT0,GL_DIFFUSE,I);
        glLightfv(GL_LIGHT0,GL_AMBIENT,I);
        glLightfv(GL_LIGHT0,GL_SPECULAR,I);
        glLightfv(GL_LIGHT0,GL_POSITION,pos);
        glEnable(GL_LIGHT0);
    }
    
    // Lek�rdez� f�ggv�nyek
    float GetPhase(){ return phase; }
    float GetZangle(){ return zangle; }
    float GetEyedist(){ return eyedist; }
    
    // Be�ll�t� f�ggv�nyek
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
