#include "objects.h"
#include "util.h"
#include <algorithm>

/*****************************************************************************/
/*                     Egyszerû pályaelem leszármazottak                     */
/*****************************************************************************/

// Blokk
void Block::DrawObject(){
    Triangle::DrawQuad(Vector(0.5f,-0.5f,1),Vector(0.5f,-0.5f,0),Vector(0.5f,0.5f,0),Vector(0.5f,0.5f,1));
    Triangle::DrawQuad(Vector(0.5f,0.5f,1),Vector(0.5f,0.5f,0),Vector(-0.5f,0.5f,0),Vector(-0.5f,0.5f,1));
    Triangle::DrawQuad(Vector(-0.5f,0.5f,1),Vector(-0.5f,0.5f,0),Vector(-0.5f,-0.5f,0),Vector(-0.5f,-0.5f,1));
    Triangle::DrawQuad(Vector(-0.5f,-0.5f,1),Vector(-0.5f,-0.5f,0),Vector(0.5f,-0.5f,0),Vector(0.5f,-0.5f,1));
    Triangle::DrawQuad(Vector(-0.5f,-0.5f,1),Vector(0.5f,-0.5f,1),Vector(0.5f,0.5f,1),Vector(-0.5f,0.5f,1));    
}

// Lépcsõ
void Stair::DrawObject(){
    float scale = 1.0f/step;
    Vector a(0.5f,-0.5f,1),b(0.5f,-0.5f+scale,1),d(-0.5f,-0.5f,1),c(-0.5f,-0.5f+scale,1);
    Vector e(0.5f,-0.5f,0),f(0.5f,-0.5f+scale,0),h(-0.5f,-0.5f,0),g(-0.5f,-0.5f+scale,0);
    for(int i=0;i<step;i++){
        Triangle::DrawQuad(a,b,c,d);
        Triangle::DrawQuad(a,e,f,b);
        Triangle::DrawQuad(b,f,g,c);
        Triangle::DrawQuad(c,g,h,d);
        Triangle::DrawQuad(d,h,e,a);
        
        a.y+=scale; b.y+=scale; c.y+=scale; d.y+=scale;
        e.y+=scale; f.y+=scale; g.y+=scale; h.y+=scale;
        a.z-=scale; b.z-=scale; c.z-=scale; d.z-=scale;
    }  
}

// Lejtõ
void Slope::DrawObject(){
    Triangle::DrawQuad(Vector(0.5f,-0.5f,1),Vector(0.5f,0.5f,0),Vector(-0.5f,0.5f,0),Vector(-0.5f,-0.5f,1));
    Triangle::DrawQuad(Vector(-0.5f,-0.5f,1),Vector(-0.5f,-0.5f,0),Vector(0.5f,-0.5f,0),Vector(0.5f,-0.5f,1));
    Triangle::Draw(Vector(0.5f,-0.5f,1),Vector(0.5f,-0.5f,0),Vector(0.5f,0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(-0.5f,0.5f,0),Vector(-0.5f,-0.5f,0));
}

// Lejtõ sarok
void SlopeCorner::DrawObject(){
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(0.5f,-0.5f,0),Vector(-0.5f,0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(-0.5f,-0.5f,0),Vector(0.5f,-0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(-0.5f,0.5f,0),Vector(-0.5f,-0.5f,0));
}

// Lejtõ piramis sarok
void SlopeCornerPyramid::DrawObject(){
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(-0.5f,-0.5f,0),Vector(0.5f,-0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(-0.5f,0.5f,0),Vector(-0.5f,-0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(0.5f,-0.5f,0),Vector(0.5f,0.5f,0));
    Triangle::Draw(Vector(-0.5f,-0.5f,1),Vector(0.5f,0.5f,0),Vector(-0.5f,0.5f,0));
}

// Lejtõ belsõ sarok
void SlopeInnerCorner::DrawObject(){
    Triangle::Draw(Vector(.5f,-.5f,1),Vector(.5f,-.5f,0),Vector(.5f,.5f,0));      
    Triangle::Draw(Vector(-.5f,.5f,1),Vector(.5f,.5f,0),Vector(-.5f,.5f,0));
    Triangle::Draw(Vector(-.5f,.5f,1),Vector(-.5f,-.5f,1),Vector(.5f,-.5f,1));
    Triangle::Draw(Vector(.5f,-.5f,1),Vector(.5f,.5f,0),Vector(-.5f,.5f,1));
    Triangle::DrawQuad(Vector(-.5f,-.5f,1),Vector(-.5f,-.5f,0),Vector(.5f,-.5f,0),Vector(.5f,-.5f,1));
    Triangle::DrawQuad(Vector(-.5f,.5f,1),Vector(-.5f,.5f,0),Vector(-.5f,-.5f,0),Vector(-.5f,-.5f,1));
}

// Negyedív
void QuarterPipe::DrawObject(){
    // Hátlap
    Triangle::DrawQuad(Vector(-0.5f,-0.5f,1),Vector(-0.5f,-0.5f,0),Vector(0.5f,-0.5f,0),Vector(0.5f,-0.5f,1));
    
    float t1,t2,z1,z2,y1,y2;
    for(int i=0;i<np;i++){
        t1 = i/(float)np*M_PI/2;
        t2 = (i+1)/(float)np*M_PI/2;
        z1 = 1-sin(t1);
        z2 = 1-sin(t2);
        y1 = 0.5f-cos(t1);
        y2 = 0.5f-cos(t2);
        Triangle::Draw(Vector(0.5f,-0.5f,0),Vector(0.5f,y2,z2),Vector(0.5f,y1,z1));    // Oldallap
        Triangle::Draw(Vector(-0.5f,-0.5f,0),Vector(-0.5f,y1,z1),Vector(-0.5f,y2,z2)); // Oldallap
        // Íves lap, árnyaló normálokkal
        Triangle::DrawQuad(Vector(0.5f,y1,z1),Vector(0.5f,y2,z2),Vector(-0.5f,y2,z2),Vector(-0.5f,y1,z1),
                Vector(0,cos(t1),sin(t1)),Vector(0,cos(t2),sin(t2)),Vector(0,cos(t2),sin(t2)),Vector(0,cos(t1),sin(t1)));
    }
}

// Negyedív sarok (belsõ)
void QuarterPipeCorner::DrawObject(){
    Triangle::DrawQuad(Vector(-.5f,-.5f,1),Vector(-.5f,-.5f,0),Vector(.5f,-.5f,0),Vector(.5f,-.5f,1)); // Hátlap
    Triangle::DrawQuad(Vector(-.5f,.5f,1),Vector(-.5f,.5f,0),Vector(-.5f,-.5f,0),Vector(-.5f,-.5f,1)); // Hátlap
    
    float t1,t2,u1,u2;
    for(int t=0;t<np;t++){
        t1 = t/(float)np*M_PI/2-M_PI/2;
        t2 = (t+1)/(float)np*M_PI/2-M_PI/2;
        for(int u=0;u<np;u++){
            u1=(float)u/np*M_PI/2+M_PI;
            u2=(float)(u+1)/np*M_PI/2+M_PI;
            // Ívdarab
            Triangle::DrawQuad(Vector(cos(t2)*cos(u2)+.5f,cos(t2)*sin(u2)+.5f,sin(t2)+1),
                               Vector(cos(t1)*cos(u2)+.5f,cos(t1)*sin(u2)+.5f,sin(t1)+1),
                               Vector(cos(t1)*cos(u1)+.5f,cos(t1)*sin(u1)+.5f,sin(t1)+1),
                               Vector(cos(t2)*cos(u1)+.5f,cos(t2)*sin(u1)+.5f,sin(t2)+1),
                               Vector(-cos(t2)*cos(u2),-cos(t2)*sin(u2),-sin(t2)),
                               Vector(-cos(t1)*cos(u2),-cos(t1)*sin(u2),-sin(t1)),
                               Vector(-cos(t1)*cos(u1),-cos(t1)*sin(u1),-sin(t1)),
                               Vector(-cos(t2)*cos(u1),-cos(t2)*sin(u1),-sin(t2))
                               );
            // Felsõ lap
            if(t==np-1) Triangle::Draw(Vector(cos(t2)*cos(u1)+.5f,cos(t2)*sin(u1)+.5f,sin(t2)+1),
                                       Vector(-.5f,-.5f,1),
                                       Vector(cos(t2)*cos(u2)+.5f,cos(t2)*sin(u2)+.5f,sin(t2)+1));
            // Oldallap
            if(u==0) Triangle::Draw(Vector(cos(t1)*cos(u1)+.5f,cos(t1)*sin(u1)+.5f,sin(t1)+1),
                                    Vector(-.5f,.5f,0),
                                    Vector(cos(t2)*cos(u1)+.5f,cos(t2)*sin(u1)+.5f,sin(t2)+1));
            // Oldallap
            if(u==np-1) Triangle::Draw(Vector(cos(t2)*cos(u2)+.5f,cos(t2)*sin(u2)+.5f,sin(t2)+1),
                                       Vector(.5f,-.5f,0),
                                       Vector(cos(t1)*cos(u2)+.5f,cos(t1)*sin(u2)+.5f,sin(t1)+1));
        }
    }
}

// Negyedív külsõ sarok
void QuarterPipeOuterCorner::DrawObject(){
    float t1,t2,u1,u2,r1,r2,z1,z2;
    for(int t=0;t<np;t++){
        t1 = (float)t/np*M_PI/2;
        t2 = (float)(t+1)/np*M_PI/2;
        r1 = 1-cos(t1); z1 = 1-sin(t1);
        r2 = 1-cos(t2); z2 = 1-sin(t2);
        for(int u=0;u<np;u++){
            u1 = (float)u/np*M_PI/2;
            u2 = (float)(u+1)/np*M_PI/2;
            // Ív darab
            Triangle::DrawQuad(Vector(r1*cos(u1)-.5f,r1*sin(u1)-.5f,z1),
                               Vector(r2*cos(u1)-.5f,r2*sin(u1)-.5f,z2),
                               Vector(r2*cos(u2)-.5f,r2*sin(u2)-.5f,z2),
                               Vector(r1*cos(u2)-.5f,r1*sin(u2)-.5f,z1),
                               Vector(cos(u1),sin(u1),1)-Vector(r1*cos(u1),r1*sin(u1),z1),
                               Vector(cos(u1),sin(u1),1)-Vector(r2*cos(u1),r2*sin(u1),z2),
                               Vector(cos(u2),sin(u2),1)-Vector(r2*cos(u2),r2*sin(u2),z2),
                               Vector(cos(u2),sin(u2),1)-Vector(r1*cos(u2),r1*sin(u2),z1)
                               );
            // Oldallap
            if(u==0) Triangle::Draw(Vector(r1*cos(u1)-.5f,r1*sin(u1)-.5f,z1),Vector(-.5f,-.5f,0),Vector(r2*cos(u1)-.5f,r2*sin(u1)-.5f,z2));
            // Oldallap
            if(u==np-1) Triangle::Draw(Vector(r2*cos(u2)-.5f,r2*sin(u2)-.5f,z2),Vector(-.5f,-.5f,0),Vector(r1*cos(u2)-.5f,r1*sin(u2)-.5f,z1));
        }
    }
}

// Rollin (indító)
void Rollin::DrawObject(){
    float t1,t2,z1,z2,dif1,dif2;
    float tension=ROLLIN_TENSION;
    
    for(int t=0;t<np;t++){
        t1 = t/(float)np;
        t2 = (t+1)/(float)np;
        z1 = cos(t1*M_PI);
        if(z1>0){
            z1=pow(z1,tension);
            dif1=tension*pow(cos(t1*M_PI),tension-1)*(-1)*sin(t1*M_PI)*M_PI;
        }
        else{
            z1=-pow(-z1,tension);
            dif1=tension*pow(-cos(t1*M_PI),tension-1)*(-1)*sin(t1*M_PI)*M_PI;
        }
        z1=(z1+1)/2;
        
        z2 = cos(t2*M_PI);
        if(z2>0){
            z2=pow(z2,tension);
            dif2=tension*pow(cos(t2*M_PI),tension-1)*(-1)*sin(t2*M_PI)*M_PI;
        }
        else{
            z2=-pow(-z2,tension);
            dif2=tension*pow(-cos(t2*M_PI),tension-1)*(-1)*sin(t2*M_PI)*M_PI;
        }
        z2=(z2+1)/2;
        // Ív
        Triangle::DrawQuad(Vector(.5f,t1-.5f,z1),Vector(.5f,t2-.5f,z2),Vector(-.5f,t2-.5f,z2),Vector(-.5f,t1-.5f,z1),
                Vector(0,-dif1,1),Vector(0,-dif2,1),Vector(0,-dif2,1),Vector(0,-dif1,1));
        // Oldallapok
        Triangle::Draw(Vector(.5f,-.5f,0),Vector(.5f,t2-.5f,z2),Vector(.5f,t1-.5f,z1));
        Triangle::Draw(Vector(-.5f,-.5f,0),Vector(-.5f,t1-.5f,z1),Vector(-.5f,t2-.5f,z2));
    }
    // Hátlap
    Triangle::DrawQuad(Vector(-.5f,-.5f,1),Vector(-.5f,-.5f,0),Vector(.5f,-.5f,0),Vector(.5f,-.5f,1));   
}

/*****************************************************************************/
/*                          Speciális pályaelemek                            */
/*****************************************************************************/

// Segédfüggvény a korláthoz
void Rail::CreateBlock(Vector ptop,Vector pbot, float thi2){
    Vector a,b,c,d,e,f,g,h;
    a=d=e=h=ptop;
    b=c=f=g=pbot;
    
    a.x+=thi2; a.y-=thi2;
    b.x+=thi2; b.y-=thi2;
    d.x+=thi2; d.y+=thi2;
    c.x+=thi2; c.y+=thi2;
    h.x-=thi2; h.y+=thi2;
    g.x-=thi2; g.y+=thi2;
    e.x-=thi2; e.y-=thi2;
    f.x-=thi2; f.y-=thi2;
    
    Triangle::DrawQuad(e,a,d,h);
    Triangle::DrawQuad(e,f,b,a);
    Triangle::DrawQuad(a,b,c,d);
    Triangle::DrawQuad(d,c,g,h);
    Triangle::DrawQuad(h,g,f,e);
}

// Segédfüggvény a korláthoz
void Rail::CreateRail(Vector p1, Vector p2, float thi2){
    Vector a,b,c,d,e,f,g,h;
    a=b=c=d=p1;
    e=f=g=h=p2;
    
    a.y-=thi2; a.z+=thi2;
    e.y-=thi2; e.z+=thi2;
    b.y-=thi2; b.z-=thi2;
    f.y-=thi2; f.z-=thi2;
    c.y+=thi2; c.z-=thi2;
    g.y+=thi2; g.z-=thi2;
    d.y+=thi2; d.z+=thi2;
    h.y+=thi2; h.z+=thi2;
    
    Triangle::DrawQuad(e,a,d,h);
    Triangle::DrawQuad(h,d,c,g);
    Triangle::DrawQuad(g,c,b,f);
    Triangle::DrawQuad(f,b,a,e);
}

// Korlát
void Rail::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(translate.x,translate.y,translate.z);
    glRotatef(rotation,0,0,1);
    
    if(mat) mat->Apply();
    
    Vector p1b = Vector(-ln/2+thi/2-0.0001f,0,off);
    Vector p1t = p1b + Vector(0,0,h);
    Vector p2b = Vector(ln/2-thi/2+0.0001f,0,0);
    Vector p2t = p2b + Vector(0,0,h);
    CreateBlock(p1t,p1b,thi/2);
    CreateBlock(p2t,p2b,thi/2);
    
    Vector p1r = p1t + Vector(thi/2,0,-thi/2);
    Vector p2r = p2t + Vector(-thi/2,0,-thi/2);
    CreateRail(p2r,p1r,thi/2);
    
    glPopMatrix();
}

// Padka
void Ledge::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(translate.x,translate.y,translate.z);
    glRotatef(rotation,0,0,1);
    
    Vector a,b,c,d,e,f,g,h;
    a=b=c=d=Vector(length/2,0,0);
    e=f=g=h=Vector(-length/2,0,0);
    a.y=b.y=e.y=f.y=-width/2;
    c.y=d.y=g.y=h.y=width/2;
    a.z=d.z=height;
    f.z=g.z=offset;
    e.z=h.z=offset+height;
    
    if(mat) mat->Apply();
    Triangle::DrawQuad(e,a,d,h);
    Triangle::DrawQuad(h,d,c,g);
    Triangle::DrawQuad(f,b,a,e);
    Triangle::DrawQuad(a,b,c,d);
    Triangle::DrawQuad(h,g,f,e);
    
    float eps=0.001f;
    a.z+=eps;
    a.y-=eps;
    e.z+=eps;
    e.y-=eps;
    d.z+=eps;
    d.y+=eps;
    h.z+=eps;
    h.y+=eps;
    
    Vector ra1=a,ra2=a,re1=e,re2=e,rd1=d,rd2=d,rh1=h,rh2=h;
    ra1.z-=railsize;
    ra2.y+=railsize;
    re1.z-=railsize;
    re2.y+=railsize;
    rd1.z-=railsize;
    rd2.y-=railsize;
    rh1.z-=railsize;
    rh2.y-=railsize;
    
    if(railMat) railMat->Apply();
    Triangle::Draw(e,re1,ra1);
    Triangle::Draw(e,ra1,a);
    Triangle::Draw(e,a,ra2);
    Triangle::Draw(e,ra2,re2);
    Triangle::Draw(rh2,rd2,d);
    Triangle::Draw(rh2,d,h);
    Triangle::Draw(h,d,rd1);
    Triangle::Draw(h,rd1,rh1);
    
    glPopMatrix();
}

// Pálmafa törzs
void PalmBody::Draw(){
    Vector a,b,c,d;
	float r=0.7f;
	float t1,t2;
	for(int i=0;i<np;i++){
        t1 = i/(float)np*M_PI*2;
        t2 = (i+1)/(float)np*M_PI*2;
		a = Vector(cos(t1),sin(t1),2)*.5f;
		b = Vector(cos(t1),sin(t1),0)*.5f*r;
		c = Vector(cos(t2),sin(t2),0)*.5f*r;
		d = Vector(cos(t2),sin(t2),2)*.5f;
		Triangle::DrawQuad(a,b,c,d,Vector(a.x,a.y,0),Vector(b.x,b.y,0),Vector(c.x,c.y,0),Vector(d.x,d.y,0));
		Triangle::Draw(Vector(0,0,1),a,d);
	}   
}

// Pálmafa levél
void PalmLeaf::Draw(){
    float ovry=1.2f;
	float ovrx=1.0f/n*0.2f;
	Vector a,b,c,d,e,f,shd1,shd2;
	float zOff = sqrt(3)/2;
	float t1,t2,z1,z2,x1,x2,dy1,dy2;
	float nFloat=n;
	for(int i=0;i<n;i++){
		t1=i/nFloat*M_PI/3+M_PI/3;
		t2=(i+1)/nFloat*M_PI/3+M_PI/3;
		z1 = sin(t1)-zOff;
		z2 = sin(t2)-zOff;
		x1 = cos(t1)+.5f;
		x2 = cos(t2)+.5f;
		dy1 = .4f-.4f*i/nFloat;
		dy2 = .4f-.4f*(i+1)/nFloat;
		
		a = Vector(x2,.5f-dy2,z2);
		b = Vector(a.x,-a.y,a.z);
		shd1 = Vector(cos(t1),0,sin(t1));
		shd2 = Vector(cos(t2),0,sin(t2));
		if(i>0){
			c = Vector(x1+ovrx,-.5f*ovry+dy1,z1);
			d = Vector(x1,-.5f+dy1,z1);
			e = Vector(d.x,-d.y,d.z);
			f = Vector(c.x,-c.y,c.z);
			
			Triangle::Draw(a,e,f,shd2,shd1,shd1);
			Triangle::Draw(b,c,d,shd2,shd1,shd1);
			Triangle::DrawQuad(a,b,d,e,shd2,shd2,shd1,shd1);
			
			Triangle::Draw(f,e,a,shd1*-1,shd1*-1,shd2*-1);
			Triangle::Draw(d,c,b,shd1*-1,shd1*-1,shd2*-1);
			Triangle::DrawQuad(e,d,b,a,shd1*-1,shd1*-1,shd2*-1,shd2*-1);
		}
		else{
			Triangle::Draw(a,b,Vector(x1,0,z1),shd2,shd2,shd1);
			Triangle::Draw(b,a,Vector(x1,0,z1),shd2*-1,shd2*-1,shd1*-1);
		}
	}
}

// Véletlenszám generáló segédfüggvény
float RandNum(float min, float max){
    return min+(max-min)*rand()/(float)RAND_MAX;   
}

// Pálmafa rajzolás
void PalmTree::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(translate.x,translate.y,translate.z);
    glRotatef(rotation,0,0,1);
	
	// Törzs
	bodyMat.Apply();
	for(int i=0;i<blockNo;i++){
        glPushMatrix();
            glTranslatef(0,0,i*blockHeight);
            glScalef(blockDiam,blockDiam,blockHeight);
            body.Draw();
        glPopMatrix();   
    }
	
	// Levelek
	float sinph;
	for(int level=0;level<PALM_LEVELS;level++){ // Szintek
        int lNo = PALM_LEAVES_ON_FIRST_LEVEL-level;
        for(int i=0;i<lNo;i++){ // Szinten belüli levelek
            leafRandMat[level][i].Apply();
            glPushMatrix();
                sinph = sin(windPhase + leafRandWindPhase[level][i]);
                glTranslatef(0,0,blockNo*blockHeight);
                glRotatef(level*PALM_LEVEL_PHASE_OFF + i/(float)lNo*360,0,0,1);
                glRotatef(randPhase[level][i]+PALM_FIRST_LEVEL_Z_ANGLE+level*-PALM_LEVEL_Z_ANGLE_OFF+sinph*PALM_WIND_AMPL*leafRandWindAmpl[level][i],0,1,0);
                glScalef(leafLength*leafRandScale[level][i],leafWidth,leafLength*leafRandScale[level][i]*(1+sinph/5));
                leaf.Draw();
            glPopMatrix();
        }   
    }
	glPopMatrix();
}

// Pálmafa animálás
void PalmTree::Animate(float dt){
    windPhase+=dt*M_PI*PALM_WIND_SPEED;
    if(windPhase>2*M_PI) windPhase-=2*M_PI;   
}

// Homok
void Sand::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(translate.x,translate.y,translate.z+OFFSET);
    glRotatef(rotation,0,0,1);
    glScalef(scale.x,scale.y,1);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0,0);                 glVertex3f(-.5f,-.5f,0);
        glTexCoord2f(scale.x*2,0);         glVertex3f( .5f,-.5f,0);
        glTexCoord2f(scale.x*2,scale.y*2); glVertex3f( .5f, .5f,0);
        glTexCoord2f(0,scale.y*2);         glVertex3f(-.5f, .5f,0);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Homok textúra inicializálása
unsigned int Sand::texId=0;
bool Sand::textureInit=false;

void Sand::InitTexture(){
    float tex[256][256][3];
    float rnd;
    for(int i=0;i<256;i++){
        for(int j=0;j<256;j++){
            rnd = rand()/(float)RAND_MAX;
            tex[i][j][0]=.7f+.3f*rnd;
            tex[i][j][1]=.7f+.3f*rnd;
            tex[i][j][2]=0.4f+0.6f*rnd;
        }
    }
    
    float *texBlur = new float[256*256*3];
    for(int i=0;i<256;i++) for(int j=0;j<256;j++) for(int k=0;k<3;k++) texBlur[i*256*3+j*3+k] = tex[i][j][k];
    for(int i=1;i<255;i++){
        for(int j=1;j<255;j++){
            for(int k=0;k<3;k++){
                texBlur[i*256*3+j*3+k] = (tex[i-1][j-1][k]+tex[i-1][j][k]+tex[i-1][j+1][k]+
                                          tex[i  ][j-1][k]+tex[i  ][j][k]+tex[i  ][j+1][k]+
                                          tex[i+1][j-1][k]+tex[i+1][j][k]+tex[i+1][j+1][k])/9.0f;
            }
        }
    }
    
    // Textúrázás inicializálása
    glGenTextures(1,&texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_FLOAT, texBlur);
}

// Yukka
void Yucca::Draw(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(translate.x,translate.y,translate.z+OFFSET);
    glRotatef(rotation,0,0,1);
    
    float z_angle;
    float z_offset = 90/(float)(levels-1)/2.0f;
    float xy_angle;
    int rndIdx;
    for(int lv=1;lv<levels;lv++){
        z_angle=90-90*lv/(float)(levels-1);
        for(int i=0;i<lv*4;i++){
            rndIdx = (i*13+lv)%YUCCA_RND_PARAMETER_NO;
            randMat[rndIdx].Apply();
            xy_angle = 360*i/(float)(lv*4);
            glPushMatrix();
            glRotatef(leaf_roll_rand[rndIdx],1,0,0);
            glRotatef(xy_angle,0,0,1);
            glRotatef(std::min(-z_angle-z_offset-z_angle_rand[rndIdx],-10.0f),0,1,0);
            glBegin(GL_TRIANGLES);
                glVertex3f(0,-leafW,0);
                glVertex3f(leafLen*leaf_length_rand[rndIdx],0,0);
                glVertex3f(0,leafW,0);
                glVertex3f(leafLen*leaf_length_rand[rndIdx],0,0);
                glVertex3f(0,-leafW,0);
                glVertex3f(0,leafW,0);
            glEnd();
            glPopMatrix();
        }
    }
	glPopMatrix();
}
