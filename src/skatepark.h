#ifndef SKATEPARK_H
#define SKATEPARK_H

#include <list>
#include <map>
#include "camera.h"
#include "util.h"
#include "objects.h"
#include "tinyxml/tinyxml.h"

using namespace std;

// Skatepark osztály
class Skatepark{
    list<IDrawable*> objects; // Pályaelemek
    map<string,Material> materials; // Anyagok
    float x0,y0; // Alaplap koordinátái
    const float w; // Alaplap mérete
    Camera cam; // Kamera
    float cloudPhase; // Felhõk fázisa
    int animClouds; // Mozogjanak-e a felhõk
    unsigned int texId; // Felhõ textúra ID
    bool grid; // Tengelyek és rácsvonal mutatása
    
public:
    GLfloat kd[4]; // Diffúz szín
    GLfloat ka[4]; // Ambiens szín
   
    // Destruktor, felszabadítás
    ~Skatepark(){
        for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) delete (*i);
    }
    
    // Konstruktor fájlból
    Skatepark(const char* filename);
    
    // Tárolás
    void AddObject(IDrawable* obj){
        objects.push_back(obj);
    }
    
    // Kirajzolás
    void Draw();
    
    // Égbolt kirajzolása
    void DrawSky();
    
    // Alap kirajzolása
    void DrawLand();
    
    // Tengelyek kirajzolása
    void DrawAxes();
    
    // Rácsvonal rajzolása
    void DrawGrid();
    
    // Animáció
    void Animate(float dt){
        // Pályaelemek
        for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) (*i)->Animate(dt);
        // Felhõk
        cloudPhase+=dt*SPEED_CLOUDPHASE*animClouds;
        if(cloudPhase>360) cloudPhase-=360;
        // Kamera
        cam.Animate(dt);
    }
     
    // Felhõk animálásának ki/be kapcsolása   
    void ToggleAnimClouds(){
        animClouds=1-animClouds;   
    }
    
    // Tengelyek és rácsvonal mutatásának ki/be kapcsolása
    void ToggleGrid(){
        grid=!grid;
    }
    
    // Kamera lekérdezése
    Camera* GetCamera(){
        return &cam;   
    }
    
    // XML feldolgozó segédfüggvény, egy float attribútumot olvas be
    bool ReadAttrf(TiXmlElement* element,const char* attribute,float* target, float defValue=0.0f){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%f",target); return true; }
        *target = defValue; return false;
    }
    
    // XML feldolgozó segédfüggvény, egy int attribútumot olvas be
    bool ReadAttri(TiXmlElement* element,const char* attribute,int* target, int defValue=0){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%d",target); return true; }
        *target = defValue; return false;
    }
    
    // XML feldolgozó segédfüggvény, 3 float attribútumot olvas be
    bool ReadAttrf3(TiXmlElement* element,const char* attribute, float *t1, float *t2, float *t3, float def1=0.0f, float def2=0.0f, float def3=0.0f){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%f %f %f",t1,t2,t3); return true; }
        *t1=def1; *t2=def2; *t3=def3; return false;
    }
};

#endif
