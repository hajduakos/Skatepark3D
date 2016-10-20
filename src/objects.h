/*****************************************************************************/
/*                                   Elemek                                  */
/*****************************************************************************/

#ifndef OBJECTS_H
#define OBJECTS_H

#include "util.h"
#include <cstdlib>
#include "configure.h"

/*****************************************************************************/
/*                        Kirajzolhat� elem interf�sze                       */
/*****************************************************************************/
class IDrawable{
public:
    virtual void Draw()=0;
    virtual int CountTriangles(){return 0;}
    virtual void Animate(float dt){}
};

/*****************************************************************************/
/*                            Egyszer� p�lyaelem                             */
/*****************************************************************************/
class Object : public IDrawable{
public:
    Vector translate; // Eltol�s
    Vector scale;     // M�retez�s
    float rotation;   // Forgat�s (Z k�r�l)
    Material * mat;   // Anyag
    
    // Konstruktor
    Object(){
        // Inicializ�l�s
        translate=Vector(0,0,0);
        scale=Vector(1,1,1);
        rotation=0;
        mat=NULL;
    }
    
    // Absztrakt f�ggv�ny, amit a lesz�rmazottaknak fel�l kell defini�lni
    // Ebben m�r csak a cs�cspontok felsorol�s�t kell megval�s�tani
    virtual void DrawObject()=0;
    
    // Kirajzol�s
    void Draw(){
        // Transzform�ci� alkalmaz�sa
        // (m�retez�s -> forgat�s -> eltol�s sorrendben)
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(translate.x,translate.y,translate.z);
        glRotatef(rotation,0,0,1);
        glScalef(scale.x,scale.y,scale.z);
        // Anyagjellemz� be�ll�t�sa, ha van
        if(mat) mat->Apply();
        // Rajzol�s
        DrawObject();
        glPopMatrix();
    }
};

/*****************************************************************************/
/*                     Egyszer� p�lyaelem lesz�rmazottak                     */
/*****************************************************************************/

// Blokk
class Block : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 10;}
};

// L�pcs�
class Stair : public Object{
    int step; // L�pcs�fokok sz�ma
public:
    Stair(int step=DEFAULT_STAIR_STEPS):step(step){}
    void DrawObject();   
    virtual int CountTriangles(){return step*10;}
};

// Lejt�
class Slope : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 6;}
};

// Lejt� sarok
class SlopeCorner : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 3;}
};

// Lejt� piramis sarok
class SlopeCornerPyramid : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 4;}
};

// Lejt� bels� sarok
class SlopeInnerCorner : public Object{
public:
    void DrawObject();
    virtual int CountTriangles(){return 8;}
};

// Negyed�v
class QuarterPipe : public Object{
    int np; // Tesszell�ci�
public:
    QuarterPipe(int np=TESS_DEFAULT_QP):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 2+np*4;}
};

// Negyed�v sarok
class QuarterPipeCorner : public Object{
    int np; // Tesszell�ci�
public:
    QuarterPipeCorner(int np=TESS_DEFAULT_QPC):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 4+np*np*2+np*3;}
};

// Negyed�v k�ls� sarok
class QuarterPipeOuterCorner : public Object{
    int np; // Tesszell�ci�
public:
    QuarterPipeOuterCorner(int np=TESS_DEFAULT_QPOC):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return np*np*2+np*2;}
};

// Rollin (ind�t�)
class Rollin : public Object{
    int np; // Tesszell�ci�
public:
    Rollin(int np=TESS_DEFAULT_ROLLIN):np(np){}
    void DrawObject();
    virtual int CountTriangles(){return 2+np*4;}
};

/*****************************************************************************/
/*                          Speci�lis p�lyaelemek                            */
/*****************************************************************************/

// Korl�t
class Rail : public IDrawable{
    float ln,thi,h,off; // Hossz, vastags�g, magass�g, egyik v�gpont f�gg�leges eltol�sa
    void CreateBlock(Vector ptop,Vector pbot, float thi2); // Seg�df�ggv�ny
    void CreateRail(Vector p1, Vector p2, float thi2); // Seg�df�ggv�ny
public:
    Material * mat; // Anyag
    Vector translate; // Eltol�s
    float rotation; // Forgat�s
    // Konstruktor
    Rail(float length,float height,float thickness,float offset){
        ln=length; thi=thickness; h=height; off=offset; mat=NULL;
    }
    void Draw();
    virtual int CountTriangles(){return 28;}
};

// Padka
class Ledge : public IDrawable{
    // Hossz, sz�less�g, magass�g, egyik v�gpont f�gg�leges eltol�sa, z�rtszelv�ny m�rete
    float length, width, height, offset, railsize;
public:
    Material * mat; // Anyag
    Material * railMat; // Z�rtszelv�ny anyaga
    Vector translate; // Eltol�s
    float rotation; // Forgat�s
    // Konstruktor
    Ledge(float length, float width, float height, float offset, float railsize){
        this->length=length; this->width=width; this->height=height; this->offset=offset; this->railsize=railsize;
        mat = railMat = NULL;   
    }
    void Draw();
    virtual int CountTriangles(){return 18;}
};

// P�lmafa t�rzs egy blokkja
class PalmBody : public IDrawable{
    int np; // Tesszell�ci�
public:
    PalmBody(int np=6):np(np){}
    void Draw();
    virtual int CountTriangles(){return np*3;}
};

// P�lmalev�l
class PalmLeaf : public IDrawable{
    int n;
public:
    PalmLeaf(int n=4):n(n){}
    void Draw();
    virtual int CountTriangles(){return 2+(n-1)*8;}
};

// V�letlensz�mgener�l� min �s max k�z�tt (seg�df�ggv�ny)
float RandNum(float min, float max);

// P�lmafa
class PalmTree : public IDrawable{
    PalmBody body; // Test
	PalmLeaf leaf; // Lev�l
	
	float blockHeight; // T�rzs egy blokkj�nak magass�ga
    int blockNo;       // T�rzs blokkjainak sz�ma
    float blockDiam;   // T�rzs egy blokkj�nak �tm�r�je
    float leafLength;  // Lev�lhossz
    float leafWidth;   // Lev�lsz�less�g
    
    // Random �rt�kek
    float randPhase[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random f�zis
    Material leafRandMat[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random anyag
    float leafRandScale[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random lev�lm�ret
    float leafRandWindPhase[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random sz�l f�zis
    float leafRandWindAmpl[PALM_LEVELS][PALM_LEAVES_ON_FIRST_LEVEL]; // Random sz�l amplit�d�
	
	Material bodyMat; // Test anyaga
	
	float windPhase; // Sz�l f�zisa
	
public:
    Vector translate; // Eltol�s
    float rotation; // Forgat�s
    
    // Konstruktor
    PalmTree(float blockHeight,int blockNo, float blockDiam, float leafLength, float leafWidth, int leafN):body(TESS_DEFAULT_PALMBODY),leaf(leafN){
        // Inicializ�l�s
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
		
		// Random param�terek gener�l�sa
		for(int i=0;i<PALM_LEVELS;i++) for(int j=0;j<PALM_LEAVES_ON_FIRST_LEVEL;j++){
            randPhase[i][j] = RandNum(-PALM_Z_ANGLE_VAR,PALM_Z_ANGLE_VAR); // F�zis
            
            // Anyag
            float rnd = RandNum(1-PALM_COLOR_VAR,1+PALM_COLOR_VAR);
            leafRandMat[i][j].SetKa(green*.4f*rnd);
            leafRandMat[i][j].SetKd(green*.4f*rnd);
            leafRandMat[i][j].SetKs(0,0,0);
            leafRandMat[i][j].shine=1;
            
            leafRandScale[i][j] = RandNum(1-PALM_LEAF_SCALE_VAR,1+PALM_LEAF_SCALE_VAR); // Lev�lm�ret
            leafRandWindPhase[i][j] = RandNum(0,2*M_PI); // Sz�lf�zis
            leafRandWindAmpl[i][j] = RandNum(0.5f,1.5f); // Sz�l amplit�d�
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
    static unsigned int texId; // Text�ra ID (statikus)
    static bool textureInit;   // Inicializ�lva lett-e m�r a text�ra (statikus)
    void InitTexture(); // Text�ra inicializ�l�sa
public:
    // Konstruktor
    Sand(){
        if(!textureInit){ // Ha m�g nem volt inicializ�lva a text�ra
            InitTexture(); // Inicializ�l�s
            textureInit=true;
        }
    }
    Vector translate; // Eltol�s
    Vector scale;     // M�retez�s
    float rotation;   // Forgat�s
    void Draw();
    virtual int CountTriangles(){ return 2; }
};

// Yukka
class Yucca : public IDrawable{
    float leafLen,leafW; // Lev�lhossz, lev�lsz�less�g
    float levels; // Szintek sz�ma
    // Random param�terek
    Material randMat[YUCCA_RND_PARAMETER_NO]; // Anyag
    float z_angle_rand[YUCCA_RND_PARAMETER_NO]; // Sz�g
    float leaf_length_rand[YUCCA_RND_PARAMETER_NO]; // Lev�lhossz
    float leaf_roll_rand[YUCCA_RND_PARAMETER_NO]; // Lev�l csavarod�s
public:
    Vector translate; // Eltol�s
    float rotation;   // Forgat�s
    
    // Konstruktor
    Yucca(float leafLen, float leafW, float levels):leafLen(leafLen),leafW(leafW),levels(levels){
        // Inicializ�l�s
        Color green(.201f,.941f,.343f);
        rotation=0;
        for(int i=0;i<YUCCA_RND_PARAMETER_NO;i++){ // Random param�terek be�ll�t�sa
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
