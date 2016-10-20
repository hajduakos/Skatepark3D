/*****************************************************************************/
/*                                   Elemek                                  */
/*****************************************************************************/

#ifndef OBJECTS_H
#define OBJECTS_H

#include "util.h"
#include <cstdlib>
#include "configure.h"

/*****************************************************************************/
/*                        Kirajzolható elem interfésze                       */
/*****************************************************************************/
class IDrawable{
public:
    virtual void Draw()=0;
    virtual int CountTriangles(){return 0;}
    virtual void Animate(float dt){}
};

/*****************************************************************************/
/*                            Egyszerû pályaelem                             */
/*****************************************************************************/
class Object : public IDrawable{
public:
    Vector translate; // Eltolás
    Vector scale;     // Méretezés
    float rotation;   // Forgatás (Z körül)
    Material * mat;   // Anyag
    
    // Konstruktor
    Object(){
        // Inicializálás
        translate=Vector(0,0,0);
        scale=Vector(1,1,1);
        rotation=0;
        mat=NULL;
    }
    
    // Absztrakt függvény, amit a leszármazottaknak felül kell definiálni
    // Ebben már csak a csúcspontok felsorolását kell megvalósítani
    virtual void DrawObject()=0;
    
    // Kirajzolás
    void Draw(){
        // Transzformáció alkalmazása
        // (méretezés -> forgatás -> eltolás sorrendben)
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(translate.x,translate.y,translate.z);
        glRotatef(rotation,0,0,1);
        glScalef(scale.x,scale.y,scale.z);
        // Anyagjellemzõ beállítása, ha van
        if(mat) mat->Apply();
        // Rajzolás
        DrawObject();
        glPopMatrix();
    }
};

/*****************************************************************************/
/*                     Egyszerû pályaelem leszármazottak                     */
/*****************************************************************************/

// Blokk
class Block : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 10;}
};

// Lépcsõ
class Stair : public Object{
    int step; // Lépcsõfokok száma
public:
    Stair(int step=DEFAULT_STAIR_STEPS):step(step){}
    void DrawObject();   
    virtual int CountTriangles(){return step*10;}
};

// Lejtõ
class Slope : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 6;}
};

// Lejtõ sarok
class SlopeCorner : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 3;}
};

// Lejtõ piramis sarok
class SlopeCornerPyramid : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 4;}
};

// Lejtõ belsõ sarok
class SlopeInnerCorner : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 8;}
};

// Negyedív
class QuarterPipe : public Object{
    int np; // Tesszelláció
public:
    QuarterPipe(int np=TESS_DEFAULT_QP):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 2+np*4;}
};

// Negyedív sarok
class QuarterPipeCorner : public Object{
    int np; // Tesszelláció
public:
    QuarterPipeCorner(int np=TESS_DEFAULT_QPC):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 4+np*np*2+np*3;}
};

// Negyedív külsó sarok
class QuarterPipeOuterCorner : public Object{
    int np; // Tesszelláció
public:
    QuarterPipeOuterCorner(int np=TESS_DEFAULT_QPOC):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return np*np*2+np*2;}
};

// Rollin (indító)
class Rollin : public Object{
    int np; // Tesszelláció
public:
    Rollin(int np=TESS_DEFAULT_ROLLIN):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 2+np*4;}
};

/*****************************************************************************/
/*                          Speciális pályaelemek                            */
/*****************************************************************************/

// Korlát
class Rail : public IDrawable{
    float ln,thi,h,off; // Hossz, vastagság, magasság, egyik végpont függõleges eltolása
    void CreateBlock(Vector ptop,Vector pbot, float thi2); // Segédfüggvény
    void CreateRail(Vector p1, Vector p2, float thi2); // Segédfüggvény
public:
    Material * mat; // Anyag
    Vector translate; // Eltolás
    float rotation; // Forgatás
    // Konstruktor
    Rail(float length,float height,float thickness,float offset){
        ln=length; thi=thickness; h=height; off=offset; mat=NULL;
    }
    void Draw();
    virtual int CountTriangles(){return 28;}
};

// Padka
class Ledge : public IDrawable{
    // Hossz, szélesség, magasság, egyik végpont függõleges eltolása, zártszelvény mérete
    float length, width, height, offset, railsize;
public:
    Material * mat; // Anyag
    Material * railMat; // Zártszelvény anyaga
    Vector translate; // Eltolás
    float rotation; // Forgatás
    // Konstruktor
    Ledge(float length, float width, float height, float offset, float railsize){
        this->length=length; this->width=width; this->height=height; this->offset=offset; this->railsize=railsize;
        mat = railMat = NULL;   
    }
    void Draw();
    virtual int CountTriangles(){return 18;}
};

// Pálmafa törzs egy blokkja
class PalmBody : public IDrawable{
    int np; // Tesszelláció
public:
    PalmBody(int np=6):np(np){}
    void Draw();
    virtual int CountTriangles(){return np*3;}
};

// Pálmalevél
class PalmLeaf : public IDrawable{
    int n;
public:
    PalmLeaf(int n=4):n(n){}
    void Draw();
    virtual int CountTriangles(){return 2+(n-1)*8;}
};

// Véletlenszámgeneráló min és max között (segédfüggvény)
float RandNum(float min, float max);

// Pálmafa
class PalmTree : public IDrawable{
    PalmBody body; // Test
	PalmLeaf leaf; // Levél
	
	float blockHeight; // Törzs egy blokkjának magassága
    int blockNo;       // Törzs blokkjainak száma
    float blockDiam;   // Törzs egy blokkjának átmérõje
    float leafLength;  // Levélhossz
    float leafWidth;   // Levélszélesség
    
    // Random értékek
    float randPhase[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random fázis
    Material leafRandMat[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random anyag
    float leafRandScale[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random levélméret
    float leafRandWindPhase[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random szél fázis
    float leafRandWindAmpl[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random szél amplitúdó
	
	Material bodyMat; // Test anyaga
	
	float windPhase; // Szél fázisa
	
public:
    Vector translate; // Eltolás
    float rotation; // Forgatás
    
    // Konstruktor
    PalmTree(float blockHeight,int blockNo, float blockDiam, float leafLength, float leafWidth, int leafN):body(TESS_DEFAULT_PALMBODY),leaf(leafN){
        // Inicializálás
        this->blockHeight = blockHeight;
        this->blockNo = blockNo;
        this->blockDiam = blockDiam;
        this->leafLength = leafLength;
        this->leafWidth = leafWidth;
        windPhase=0;
        rotation=0;
		
		Color brown(.541f,.423f,.102f);
		bodyMat.SetKa(brown*.2f);
		bodyMat.SetKd(brown*.8f);
		bodyMat.SetKs(0,0,0);
		bodyMat.shine=1;
		
		Color green(.573f,.890f,.188f);
		
		// Random paraméterek generálása
		for(int i=0;i<PALM_LEVELS;i++) for(int j=0;j<PALM_LEAVES_ON_FIRST_LEVEL;j++){
            randPhase[i][j] = RandNum(-PALM_Z_ANGLE_VAR,PALM_Z_ANGLE_VAR); // Fázis
            
            // Anyag
            float rnd = RandNum(1-PALM_COLOR_VAR,1+PALM_COLOR_VAR);
            leafRandMat[i][j].SetKa(green*.4f*rnd);
            leafRandMat[i][j].SetKd(green*.4f*rnd);
            leafRandMat[i][j].SetKs(0,0,0);
            leafRandMat[i][j].shine=1;
            
            leafRandScale[i][j] = RandNum(1-PALM_LEAF_SCALE_VAR,1+PALM_LEAF_SCALE_VAR); // Levélméret
            leafRandWindPhase[i][j] = RandNum(0,2*M_PI); // Szélfázis
            leafRandWindAmpl[i][j] = RandNum(0.5f,1.5f); // Szél amplitúdó
        }
	}
	void Draw();
    virtual int CountTriangles(){
        int leaves=0;
        for(int i=0;i<PALM_LEVELS;i++) leaves+=PALM_LEAVES_ON_FIRST_LEVEL-i;
        return blockNo*body.CountTriangles() + leaves;
    }
    void Animate(float dt);
};

// Homok
class Sand : public IDrawable{
    static unsigned int texId; // Textúra ID (statikus)
    static bool textureInit;   // Inicializálva lett-e már a textúra (statikus)
    void InitTexture(); // Textúra inicializálása
public:
    // Konstruktor
    Sand(){
        if(!textureInit){ // Ha még nem volt inicializálva a textúra
            InitTexture(); // Inicializálás
            textureInit=true;
        }
    }
    Vector translate; // Eltolás
    Vector scale;     // Méretezés
    float rotation;   // Forgatás
    void Draw();
    virtual int CountTriangles(){ return 2; }
};

// Yukka
class Yucca : public IDrawable{
    float leafLen,leafW; // Levélhossz, levélszélesség
    float levels; // Szintek száma
    // Random paraméterek
    Material randMat[YUCCA_RND_PARAMETER_NO]; // Anyag
    float z_angle_rand[YUCCA_RND_PARAMETER_NO]; // Szög
    float leaf_length_rand[YUCCA_RND_PARAMETER_NO]; // Levélhossz
    float leaf_roll_rand[YUCCA_RND_PARAMETER_NO]; // Levél csavarodás
public:
    Vector translate; // Eltolás
    float rotation;   // Forgatás
    
    // Konstruktor
    Yucca(float leafLen, float leafW, float levels):leafLen(leafLen),leafW(leafW),levels(levels){
        // Inicializálás
        Color green(.201f,.941f,.343f);
        rotation=0;
        for(int i=0;i<YUCCA_RND_PARAMETER_NO;i++){ // Random paraméterek beállítása
            float rnd = RandNum(1-YUCCA_COLOR_VAR,1+YUCCA_COLOR_VAR);
            randMat[i].SetKa(green*.4f*rnd);
            randMat[i].SetKd(green*.4f*rnd);
            randMat[i].SetKs(0,0,0);
            randMat[i].shine=1;
            
            z_angle_rand[i] = RandNum(-YUCCA_Z_ANGLE_VAR,YUCCA_Z_ANGLE_VAR);
            leaf_length_rand[i] = RandNum(1-YUCCA_LEAF_SCALE_VAR,1+YUCCA_LEAF_SCALE_VAR);
            leaf_roll_rand[i] = RandNum(-YUCCA_LEAF_ROLL_VAR,YUCCA_LEAF_ROLL_VAR);
        }
    }
    void Draw();
    int CountTriangles(){
        int count=0;
        for(int lv=1;lv<levels;lv++) for(int i=0;i<lv*4;i++) count+=2;
        return count;
    }
};

#endif
