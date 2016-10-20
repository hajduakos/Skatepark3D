#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <GL/gl.h>
#include <iostream>
#include "configure.h"

struct Vector {
    float x, y, z;
    
    Vector( ) { 
        x = y = z = 0;
    }
    Vector(float x0, float y0, float z0 = 0) { 
        x = x0; y = y0; z = z0;
    }
    Vector operator*(const float a) const { 
        return Vector(x * a, y * a, z * a); 
    }
    Vector operator/(const float a) const { 
        return Vector(x / a, y / a, z / a); 
    }
    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y, z + v.z); 
    }
    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z); 
    }
    float operator*(const Vector& v) const { 	// dot product
        return (x * v.x + y * v.y + z * v.z); 
    }
    Vector operator%(const Vector& v) const { 	// cross product
        return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    float Length() const { return sqrt(x * x + y * y + z * z); }
    
    void Normalize(){
        float l = 1.0f/this->Length();
        x*=l;
        y*=l;
        z*=l;
    }
};

struct Color {
    float r, g, b;
    
    Color( ) { 
        r = g = b = 0;
    }
    Color(float r0, float g0, float b0) { 
        r = r0; g = g0; b = b0;
    }
    Color operator*(float a) const { 
        return Color(r * a, g * a, b * a); 
    }
    Color operator*(const Color& c) const { 
        return Color(r * c.r, g * c.g, b * c.b); 
    }
    Color operator+(const Color& c) const {
        return Color(r + c.r, g + c.g, b + c.b); 
    }
    static Color Color255(int r, int g, int b){
        return Color(r/255.0f,g/255.0f,b/255.0f);   
    }
};

struct Triangle{
    static void Draw(const Vector& a, const Vector& b, const Vector& c){
        Vector n = (b-a)%(c-a);
        n.Normalize();
        glNormal3f(n.x,n.y,n.z);
        glBegin(GL_TRIANGLES);
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
        glEnd();
    }
    static void Draw(const Vector& a, const Vector& b, const Vector& c, Vector na, Vector nb, Vector nc){
        #ifdef USE_SHADE_NORMALS
        glBegin(GL_TRIANGLES);
            glNormal3f(na.x,na.y,na.z);
            glVertex3f(a.x,a.y,a.z);
            glNormal3f(nb.x,nb.y,nb.z);
            glVertex3f(b.x,b.y,b.z);
            glNormal3f(nc.x,nc.y,nc.z);
            glVertex3f(c.x,c.y,c.z);
        glEnd();
        #else
        Draw(a,b,c);
        #endif
    }
    
    static void DrawQuad(const Vector& a, const Vector& b, const Vector& c, const Vector& d){
        Vector n = (b-a)%(c-a);
        glNormal3f(n.x,n.y,n.z);
        glBegin(GL_QUADS);
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
            glVertex3f(d.x,d.y,d.z);
        glEnd();
    }
    
    static void DrawQuad(const Vector& a, const Vector& b, const Vector& c, const Vector& d, Vector na, Vector nb, Vector nc, Vector nd){
        #ifdef USE_SHADE_NORMALS
        glBegin(GL_QUADS);
            glNormal3f(na.x,na.y,na.z);
            glVertex3f(a.x,a.y,a.z);
            glNormal3f(nb.x,nb.y,nb.z);
            glVertex3f(b.x,b.y,b.z);
            glNormal3f(nc.x,nc.y,nc.z);
            glVertex3f(c.x,c.y,c.z);
            glNormal3f(nd.x,nd.y,nd.z);
            glVertex3f(d.x,d.y,d.z);
        glEnd();
        #else
        DrawQuad(a,b,c,d);
        #endif
        
    }
};

struct Material{
    GLfloat kd[4];
    GLfloat ks[4];
    GLfloat ka[4];
    GLfloat shine;

    void SetKd(float r, float g, float b, float a=1.0f){
        kd[0]=r; kd[1]=g; kd[2]=b; kd[3]=a;
    }
    
    void SetKd(Color c,float a=1.0f){
        SetKd(c.r,c.g,c.b,a);
    }
    
    void SetKs(float r, float g, float b, float a=1.0f){
        ks[0]=r; ks[1]=g; ks[2]=b; ks[3]=a;
    }
    
    void SetKs(Color c,float a=1.0f){
        SetKs(c.r,c.g,c.b,a);
    }
    
    void SetKa(float r, float g, float b, float a=1.0f){
        ka[0]=r; ka[1]=g; ka[2]=b; ka[3]=a;
    }
    
    void SetKa(Color c,float a=1.0f){
        SetKa(c.r,c.g,c.b,a);
    }
    
    void Apply(){
        glMaterialfv(GL_FRONT,GL_DIFFUSE,kd);
        glMaterialfv(GL_FRONT,GL_SPECULAR,ks);
        glMaterialfv(GL_FRONT,GL_AMBIENT,ka);
        glMaterialf(GL_FRONT,GL_SHININESS,shine);
    }
};

#endif
