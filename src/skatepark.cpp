#include "tinyxml/tinyxml.h"

#include "skatepark.h"
#include <iostream>
#include <list>
#include <map>
#include <string>
#include "cloud.h"
#include "objects.h"
#include "util.h"
#include "camera.h"
#include "configure.h"

using namespace std;

// Konstruktor f�jlb�l
Skatepark::Skatepark(const char* filename):w(WORLD_SIZE),cloudPhase(0),animClouds(1),grid(false){
    cam = Camera(); // Kamera inicializ�l�sa
    x0=-w/2; // M�retek
    y0=-w/2;
    
    // Megnyit�s
    TiXmlDocument doc(filename);
    if(!doc.LoadFile()){
        cout<<"Error opening file: "<<filename<<endl;
        cin.get();cin.get();
        exit(0);
    }
    
    // Park adatainak beolvas�sa
    TiXmlElement* root = doc.FirstChildElement("skatepark");
    
    ReadAttrf3(root,"kd",kd,kd+1,kd+2,0,0,0); // Diff�z sz�n
    ReadAttrf3(root,"ka",ka,ka+1,ka+2,0,0,0); // Ambiens sz�n
    float cover; // Felh�k s�r�s�ge
    ReadAttrf(root,"clouds",&cover,10);
    Vector light; // F�ny
    ReadAttrf3(root,"light",&light.x,&light.y,&light.z,0,1,5);
    cam.SetLight(light);
    
    // Text�ra gener�l�s
    float *tex = new float[256*256*3];
    cout<<"Generating sky texture...";
    float *map256 = new float[256*256];
    // Felh�k gener�l�sa
    GenerateClouds(map256,cover);
    // Text�ra sz�nez�se
    for(int i=0;i<256;i++){
        for(int j=0;j<256;j++){
            
            #ifndef SUNSET
            Color c1(.8118f,1,1),c2(.4f,.8f,1),c3(.1f,.2f,1);
            Color sky;
            float trsh=10;
            if(i<=trsh) sky = c1 + (c2+c1*(-1))*(i/trsh);
            else sky = c2 + (c3+c2*(-1))*sqrt((i-trsh)/(255-trsh));
            #else
            Color c1(1,.5f,0), c2(1,.5f,.7f), c3(.35f,.74f,1);
            Color sky;
            float trsh=40;
            if(i<=trsh) sky = c1 + (c2+c1*(-1))*(i/trsh);
            else sky = c2 + (c3+c2*(-1))*sqrt((i-trsh)/(255-trsh));
            #endif
            
            float c = map256[i*256+j]/255.0f;
            tex[i*256*3+j*3+0]=sky.r+(1-sky.r)*c;
            tex[i*256*3+j*3+1]=sky.g+(1-sky.g)*c;
            tex[i*256*3+j*3+2]=sky.b+(1-sky.b)*c;
        }
    }
    
    // Text�r�z�s inicializ�l�sa
    glGenTextures(1,&texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_FLOAT, tex);
    delete[] tex;
    delete[] map256;
    
    cout<<"done"<<endl;
    
    // Xml elemek bej�r�sa
    TiXmlNode* child;
    TiXmlElement* objNode;
    
    // Anyagok beolvas�sa
    int i=0;   
    cout<<"Adding materials... ";
    for( child = root->FirstChild("material"); child; child = child->NextSibling("material") ){
        objNode=child->ToElement();
        Material m;
        const char* matid = objNode->Attribute("id"); // ID
        ReadAttrf3(objNode,"kd",&m.kd[0],&m.kd[1],&m.kd[2],0,0,0); // Diff�z
        ReadAttrf3(objNode,"ks",&m.ks[0],&m.ks[1],&m.ks[2],0,0,0); // Spekul�ris
        ReadAttrf3(objNode,"ka",&m.ka[0],&m.ka[1],&m.ka[2],0,0,0); // Ambiens
        ReadAttrf(objNode,"shine",&m.shine,0); // F�nyess�g
        materials[matid]=m; // T�rol�s
        i++;
    }
    cout<<i<<" materials added"<<endl;
    
    // P�lyaelemek
    i=0;
    cout<<"Adding objects... ";
    for(child = root->FirstChild("object"); child; child = child->NextSibling("object")){
        objNode=child->ToElement();
        Object* obj;
        // T�pus
        const char* objtype = objNode->Attribute("type");
        
        // L�trehoz�s t�pus alapj�n
        if(strcmp(objtype,"quarterpipe")==0) obj=new QuarterPipe();
        else if(strcmp(objtype,"quarterpipecorner")==0) obj=new QuarterPipeCorner();
        else if(strcmp(objtype,"quarterpipeoutercorner")==0) obj=new QuarterPipeOuterCorner();
        else if(strcmp(objtype,"rollin")==0) obj=new Rollin();
        else if(strcmp(objtype,"slope")==0) obj=new Slope();
        else if(strcmp(objtype,"slopecorner")==0) obj=new SlopeCorner();
        else if(strcmp(objtype,"slopeinnercorner")==0) obj=new SlopeInnerCorner();
        else if(strcmp(objtype,"slopecornerpyramid")==0) obj=new SlopeCornerPyramid();
        else if(strcmp(objtype,"block")==0) obj=new Block();
        else if(strcmp(objtype,"stair")==0){
            int steps; // L�pcs�fokok sz�ma
            ReadAttri(objNode,"steps",&steps,DEFAULT_STAIR_STEPS);
            obj=new Stair(steps);
        }
        // Nem ismert elem
        else{
            cout<<endl<<"Warning: "<<"unknown object type: "<<objtype<<endl;
            continue;
        }
        
        // Eltol�s
        ReadAttrf(objNode,"dx",&obj->translate.x,0);
        ReadAttrf(objNode,"dy",&obj->translate.y,0);
        ReadAttrf(objNode,"dz",&obj->translate.z,0);
        // M�retez�s
        ReadAttrf(objNode,"sx",&obj->scale.x,1);
        ReadAttrf(objNode,"sy",&obj->scale.y,1);
        ReadAttrf(objNode,"sz",&obj->scale.z,1);
        // Forgat�s
        ReadAttrf(objNode,"rotate",&obj->rotation,0);
        // Anyagjellemz�
        if(objNode->Attribute("material"))
            obj->mat = &materials[objNode->Attribute("material")];
        else{
            cout<<endl<<"Warning: no material for object: " << objtype <<endl;
            continue;
        }
        // T�rol�s
        objects.push_back(obj);
        i++;
    }
    cout<<i<<" objects added"<<endl;
    
    // Korl�tok beolvas�sa
    i=0;
    cout<<"Adding rails... ";
    for(child = root->FirstChild("rail"); child; child = child->NextSibling("rail")){
        objNode=child->ToElement();
        Rail* obj;
        // Param�terek
        float length,thickness,offset,height;
        ReadAttrf(objNode,"length",&length,1); // Hossz
        ReadAttrf(objNode,"thickness",&thickness,0.02f); // Vastags�g
        ReadAttrf(objNode,"offset",&offset,0); // Egyik v�gpont eltol�sa f�gg�legesen
        ReadAttrf(objNode,"height",&height,0.3f); // Magass�g
        
        obj = new Rail(length,height,thickness,offset);
        // Eltol�s
        ReadAttrf(objNode,"dx",&obj->translate.x,0);
        ReadAttrf(objNode,"dy",&obj->translate.y,0);
        ReadAttrf(objNode,"dz",&obj->translate.z,0);
        // Forgat�s
        ReadAttrf(objNode,"rotate",&obj->rotation,0);
        // Anyag
        if(objNode->Attribute("material"))
            obj->mat = &materials[objNode->Attribute("material")];
        else{
            cout<<endl<<"Warning: no material for rail" <<endl;
            continue;
        }
        // T�rol�s
        objects.push_back(obj);
        i++;
    }
    cout<<i<<" rails added"<<endl;
    
    // Szeg�lyek beolvas�sa
    i=0;
    cout<<"Adding ledges... ";
    for(child = root->FirstChild("ledge"); child; child = child->NextSibling("ledge")){
        objNode=child->ToElement();
        Ledge* obj;

        float length,width,offset,height,railsize;
        // Param�terek
        ReadAttrf(objNode,"length",&length,1); // Hossz      
        ReadAttrf(objNode,"width",&width,0.1f); // Sz�less�g       
        ReadAttrf(objNode,"offset",&offset,0); // Egyik v�gpont f�gg�leges eltol�sa        
        ReadAttrf(objNode,"height",&height,0.3f); // Magass�g        
        ReadAttrf(objNode,"railsize",&railsize,width/4); // Szeg�lyvastags�g
        
        obj = new Ledge(length,width,height,offset,railsize);
        // Eltol�s
        ReadAttrf(objNode,"dx",&obj->translate.x,0);
        ReadAttrf(objNode,"dy",&obj->translate.y,0);
        ReadAttrf(objNode,"dz",&obj->translate.z,0);
        // Forgat�s
        ReadAttrf(objNode,"rotate",&obj->rotation,0);
        // Anyag
        if(objNode->Attribute("material"))
            obj->mat = &materials[objNode->Attribute("material")];
        else{
            cout<<endl<<"Warning: no material for ledge" <<endl;
            continue;
        }
        // Szeg�ly anyag
        if(objNode->Attribute("railmaterial"))
            obj->railMat = &materials[objNode->Attribute("railmaterial")];
        
        // T�rol�s
        objects.push_back(obj);
        i++;
    }
    cout<<i<<" ledges added"<<endl;
    
    // Egy�b elemek
    i=0;
    cout<<"Adding misc. objects... ";
    for(child = root->FirstChild("misc"); child; child = child->NextSibling("misc")){
        objNode=child->ToElement();
        IDrawable* obj;
        // T�pus
        const char* objtype = objNode->Attribute("type");
        
        // P�lmafa
        if(strcmp(objtype,"palmtree")==0){
            float blockH, blockDiam, leafLen, leafW;
            int leafNt, blockNo;
            // Param�terek
            ReadAttrf(objNode,"blockH",&blockH,.2f);
            ReadAttri(objNode,"blockNo",&blockNo,5);
            ReadAttrf(objNode,"blockDiam",&blockDiam,.2f);
            ReadAttrf(objNode,"leafLen",&leafLen,.7f);
            ReadAttrf(objNode,"leafW",&leafW,.2f);
            ReadAttri(objNode,"leafNt",&leafNt,4);
                
            PalmTree* obj2=new PalmTree(blockH,blockNo,blockDiam,leafLen,leafW,leafNt);
            // Eltol�s
            ReadAttrf(objNode,"dx",&obj2->translate.x,0);
            ReadAttrf(objNode,"dy",&obj2->translate.y,0);
            ReadAttrf(objNode,"dz",&obj2->translate.z,0);
            // Forgat�s
            ReadAttrf(objNode,"rotate",&obj2->rotation,0);
            
            obj=obj2;
        }
        // Homok
        else if(strcmp(objtype,"sand")==0){
            Sand* obj2=new Sand();
            // Eltol�s
            ReadAttrf(objNode,"dx",&obj2->translate.x,0);
            ReadAttrf(objNode,"dy",&obj2->translate.y,0);
            ReadAttrf(objNode,"dz",&obj2->translate.z,0);
            // M�retez�s
            ReadAttrf(objNode,"sx",&obj2->scale.x,1);
            ReadAttrf(objNode,"sy",&obj2->scale.y,1);
            // Forgat�s
            ReadAttrf(objNode,"rotate",&obj2->rotation,0);
            obj=obj2;
        }
        // Yukka
        else if(strcmp(objtype,"yucca")==0){
            float leafLen,leafW;
            int levels;
            // Param�terek
            ReadAttrf(objNode,"leafLen",&leafLen,.4f);
            ReadAttrf(objNode,"leafW",&leafW,0.04f);
            ReadAttri(objNode,"levels",&levels,6);
            
            Yucca* obj2 = new Yucca(leafLen,leafW,levels);
            // Eltol�s
            ReadAttrf(objNode,"dx",&obj2->translate.x,0);
            ReadAttrf(objNode,"dy",&obj2->translate.y,0);
            ReadAttrf(objNode,"dz",&obj2->translate.z,0);
            // Forgat�s
            ReadAttrf(objNode,"rotate",&obj2->rotation,0);
            obj=obj2;
            
        }
        // Ismeretlen t�pus
        else{
            cout<<endl<<"Warning: "<<"unknown object type: "<<objtype<<endl;
            continue;
        }
        // T�rol�s
        objects.push_back(obj);
        i++;
    }
    cout<<i<<" misc. objects added"<<endl;
    
    // H�romsz�gek sz�mol�sa
    int triangles=0;
    for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) triangles+=(*i)->CountTriangles();
    cout<<triangles<<" + "<<(2*TESS_LAND*TESS_LAND+2*TESS_SKY_U*TESS_SKY_T)<<" triangles total"<<endl;
}

// P�lya kirajzol�sa
void Skatepark::Draw(){
    glLineWidth(1);
    DrawLand(); // Alaplap
    DrawSky(); // �gbolt
    for(list<IDrawable*>::iterator i=objects.begin();i!=objects.end();++i) (*i)->Draw(); // Elemek
    if(grid){
        DrawGrid(); // R�csvonal
        DrawAxes(); // Tengelyek
    }
}

// �gbolt kirajzol�sa
void Skatepark::DrawSky(){
    // 60�-os g�mb
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(cloudPhase,0,0,1); // �gbolt forgat�sa
    glTranslatef(0,0,-sqrt(3)/2*w); // Eltol�s a s�kra
    
    float t1,t2,u1,u2;
    Vector a,b,c,d;
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    int u,t;
    float tess_u=TESS_SKY_U, tess_t=TESS_SKY_T;
    for(u=0;u<TESS_SKY_U;u++){ // U: [0�:360�]
        u1 = u/tess_u*M_PI*2;
        u2 = (u+1)/tess_u*M_PI*2;
        for(t=0;t<TESS_SKY_T;t++){ // T: [60�:90�]
            t1 = t/tess_t*M_PI/6+M_PI/3;
            t2 = (t+1)/tess_t*M_PI/6+M_PI/3;
            a = Vector(cos(t1)*cos(u1),cos(t1)*sin(u1),sin(t1))*w;
            b = Vector(cos(t2)*cos(u1),cos(t2)*sin(u1),sin(t2))*w;
            c = Vector(cos(t2)*cos(u2),cos(t2)*sin(u2),sin(t2))*w;
            d = Vector(cos(t1)*cos(u2),cos(t1)*sin(u2),sin(t1))*w;
            
            // Text�rakoordin�t�k + cs�cskoordin�t�k
            glTexCoord2f(u/tess_u*2,t/tess_t);         glVertex3f(a.x,a.y,a.z);
            glTexCoord2f(u/tess_u*2,(t+1)/tess_t);     glVertex3f(b.x,b.y,b.z);
            glTexCoord2f((u+1)/tess_u*2,(t+1)/tess_t); glVertex3f(c.x,c.y,c.z);
            glTexCoord2f((u+1)/tess_u*2,t/tess_t);     glVertex3f(d.x,d.y,d.z);
        }
    }
    glEnd();
    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Alaplap kirajzol�sa
void Skatepark::DrawLand(){
    GLfloat ks[] = {0,0,0,0};
    glMaterialfv(GL_FRONT,GL_DIFFUSE,kd);
    glMaterialfv(GL_FRONT,GL_SPECULAR,ks);
    glMaterialfv(GL_FRONT,GL_AMBIENT,ka);
    float dx=w/(float)TESS_LAND;
    float dy=w/(float)TESS_LAND;
    
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    int i,j;
    for(i=0;i<TESS_LAND;i++){
        for(j=0;j<TESS_LAND;j++){
            glVertex3f(x0+i*dx,    y0+j*dy,    -OFFSET);
            glVertex3f(x0+i*dx+dx, y0+j*dy,    -OFFSET);
            glVertex3f(x0+i*dx+dx, y0+j*dy+dy, -OFFSET);
            glVertex3f(x0+i*dx,    y0+j*dy+dy, -OFFSET);
        }    
    }
    glEnd();
}

// Tengelyek kirajzol�sa
void Skatepark::DrawAxes(){
    glDisable(GL_LIGHTING);   
    glBegin(GL_LINES);
        glLineWidth(2);
        glColor3f(1,0,0);
        glVertex3f(0,0,OFFSET*2);
        glVertex3f(1000,0,OFFSET*2);
        glColor3f(0,1,0);
        glVertex3f(0,0,OFFSET*2);
        glVertex3f(0,1000,OFFSET*2);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1000);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINES);
        glLineWidth(2);
        glColor4f(.5f,0,0,.2f);
        glVertex3f(0,0,OFFSET*2);
        glVertex3f(1000,0,OFFSET*2);
        glColor4f(0,.5f,0,.2f);
        glVertex3f(0,0,OFFSET*2);
        glVertex3f(0,1000,OFFSET*2);
        glColor4f(0,0,.5f,.2f);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1000);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);   
}

// R�csvonal rajzol�sa
void Skatepark::DrawGrid(){
    glLineWidth(2);
    glColor3f(kd[0]/2,kd[1]/2,kd[2]/2);
    glDisable(GL_LIGHTING);   
    glBegin(GL_LINES);
    for(int x=-GRID_W;x<=GRID_W;++x){
        glVertex3f(x*GRID_SIZE,-GRID_H*GRID_SIZE,OFFSET*1.5f);
        glVertex3f(x*GRID_SIZE, GRID_H*GRID_SIZE,OFFSET*1.5f);
    }
    for(int y=-GRID_H;y<=GRID_H;++y){
        glVertex3f(-GRID_W*GRID_SIZE,y*GRID_SIZE,OFFSET*1.5f);
        glVertex3f( GRID_W*GRID_SIZE,y*GRID_SIZE,OFFSET*1.5f);
    }
    glEnd();
    glEnable(GL_LIGHTING);   
}
