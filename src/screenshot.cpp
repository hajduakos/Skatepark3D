#include "screenshot.h"
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <cstdio>

bool SaveBMP (char *filename, int w, int h, unsigned char *BMP_Data) { 
    FILE *file_ptr; 
    BITMAPINFOHEADER bmp_infoh; 
    BITMAPFILEHEADER bmp_fileh; 
    unsigned int counter; 
    unsigned char tempRGB; 
    
    file_ptr = fopen (filename, "wb"); 
    if (!file_ptr) return false; 
    
    bmp_fileh.bfOffBits = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER); 
    bmp_fileh.bfReserved1 = 0; 
    bmp_fileh.bfReserved2 = 0; 
    bmp_fileh.bfSize = sizeof (BITMAPFILEHEADER); 
    bmp_fileh.bfType = 0x4D42; 
    
    bmp_infoh.biBitCount = 24; 
    bmp_infoh.biClrImportant = 0; 
    bmp_infoh.biClrUsed = 0; 
    bmp_infoh.biCompression = BI_RGB; 
    bmp_infoh.biHeight = h; 
    bmp_infoh.biPlanes = 1; 
    bmp_infoh.biSize = sizeof (BITMAPINFOHEADER); 
    bmp_infoh.biSizeImage = w * abs (h) * 3; 
    bmp_infoh.biWidth = w; 
    bmp_infoh.biXPelsPerMeter = 0; 
    bmp_infoh.biYPelsPerMeter = 0;
    for (counter = 0; counter < bmp_infoh.biSizeImage; counter += 3){ 
        tempRGB = BMP_Data [counter]; 
        BMP_Data [counter] = BMP_Data [counter + 2]; 
        BMP_Data [counter + 2] = tempRGB; 
    } 
    
    fwrite (&bmp_fileh, 1, sizeof (BITMAPFILEHEADER), file_ptr); 
    fwrite (&bmp_infoh, 1, sizeof (BITMAPINFOHEADER), file_ptr); 
    fwrite (BMP_Data, 1, bmp_infoh.biSizeImage, file_ptr); 
    
    fclose (file_ptr); 
    return true; 
} 

unsigned char* BMPw_Data; // BMP Data
int sno=0; // Screenshot-ok száma

void Screenshot() 
{ 
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    int w = width-(width%4);
    int h = height;
    
    BMPw_Data = new unsigned char[w*h*3]; 
    glReadPixels (0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, BMPw_Data);
    
    char filename[50];
    time_t rawtime;
    struct tm * ti;
    time(&rawtime);
    ti = localtime(&rawtime);
    sprintf(filename,"%02d%02d%02d_%02d%02d%02d_%03d.bmp",(1900+ti->tm_year)%100,ti->tm_mon+1,ti->tm_mday,ti->tm_hour,ti->tm_min,ti->tm_sec,sno%1000);
    sno++;
    
    if(SaveBMP(filename, w, h, BMPw_Data)) std::cout<<"Screenshot taken: "<<filename<<" size: "<<w<<"x"<<h<<std::endl;
    else std::cout<<"Error taking screenshot"<<std::endl;
    
    delete[] BMPw_Data; 
} 
