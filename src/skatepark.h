#ifndef SKATEPARK_H
#define SKATEPARK_H

#include <list>
#include <map>
#include "camera.h"
#include "util.h"
#include "objects.h"
#include "tinyxml/tinyxml.h"

using namespace std;

// Skatepark oszt�ly
class Skatepark{
    list<IDrawable*> objects; // P�lyaelemek
    map<string,Material> materials; // Anyagok
    float x0,y0; // Alaplap koordin�t�i
    const float w; // Alaplap m�rete
    Camera cam; // Kamera
    float cloudPhase; // Felh�k f�zisa
    int animClouds; // Mozogjanak-e a felh�k
    unsigned int texId; // Felh� text�ra ID
    bool grid; // Tengelyek �s r�csvonal mutat�sa
    
public:
    GLfloat kd[4]; // Diff�z sz�n
    GLfloat ka[4]; // Ambiens sz�n
   
    // Destruktor, felszabad�t�s
    ~Skatepark(){
        for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) delete (*i);
    }
    
    // Konstruktor f�jlb�l
    Skatepark(const char* filename);
    
    // T�rol�s
    void AddObject(IDrawable* obj){
        objects.push_back(obj);
    }
    
    // Kirajzol�s
    void Draw();
    
    // �gbolt kirajzol�sa
    void DrawSky();
    
    // Alap kirajzol�sa
    void DrawLand();
    
    // Tengelyek kirajzol�sa
    void DrawAxes();
    
    // R�csvonal rajzol�sa
    void DrawGrid();
    
    // Anim�ci�
    void Animate(float dt){
        // P�lyaelemek
        for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) (*i)->Animate(dt);
        // Felh�k
        cloudPhase+=dt*SPEED_CLOUDPHASE*animClouds;
        if(cloudPhase>360) cloudPhase-=360;
        // Kamera
        cam.Animate(dt);
    }
     
    // Felh�k anim�l�s�nak ki/be kapcsol�sa   
    void ToggleAnimClouds(){
        animClouds=1-animClouds;   
    }
    
    // Tengelyek �s r�csvonal mutat�s�nak ki/be kapcsol�sa
    void ToggleGrid(){
        grid=!grid;
    }
    
    // Kamera lek�rdez�se
    Camera* GetCamera(){
        return &cam;   
    }
    
    // XML feldolgoz� seg�df�ggv�ny, egy float attrib�tumot olvas be
    bool ReadAttrf(TiXmlElement* element,const char* attribute,float* target, float defValue=0.0f){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%f",target); return true; }
        *target = defValue; return false;
    }
    
    // XML feldolgoz� seg�df�ggv�ny, egy int attrib�tumot olvas be
    bool ReadAttri(TiXmlElement* element,const char* attribute,int* target, int defValue=0){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%d",target); return true; }
        *target = defValue; return false;
    }
    
    // XML feldolgoz� seg�df�ggv�ny, 3 float attrib�tumot olvas be
    bool ReadAttrf3(TiXmlElement* element,const char* attribute, float *t1, float *t2, float *t3, float def1=0.0f, float def2=0.0f, float def3=0.0f){
        if(element->Attribute(attribute)){ sscanf(element->Attribute(attribute),"%f %f %f",t1,t2,t3); return true; }
        *t1=def1; *t2=def2; *t3=def3; return false;
    }
};

#endif
