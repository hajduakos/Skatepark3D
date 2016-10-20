#ifndef CONFIGURE_H
#define CONFIGURE_H

/*****************************************************************************/
/*                           Konfigur�ci�s fejl�c                            */
/*****************************************************************************/

// Debug m�d �s debug f�jln�v
//#define DEBUG_MODE
#define DEBUG_FILENAME "park2.xml"
#define GRID_W 30
#define GRID_H 30
#define GRID_SIZE 0.5f

// P�lya be�ll�t�sok
#define USE_SHADE_NORMALS // �rnyal� norm�lok haszn�lata
//#define SUNSET // Naplemente
#define START_FULL_SCREEN // Teljes k�perny�n induljon-e
#define OFFSET 0.002 // Talaj �s homok offszetje
#define WORLD_SIZE 1000 // P�lya m�rete

// Tesszell�ci�s be�ll�t�sok
#define TESS_DEFAULT_QP 8       // Negyed�v alap�rtelmezett tesszell�ci�ja
#define TESS_DEFAULT_QPC 8      // Negyed�v sarok alap�rtelmezett tesszell�ci�ja
#define TESS_DEFAULT_QPOC 8     // Negyed�v k�ls� sarok alap�rtelmezett tesszell�ci�ja
#define TESS_DEFAULT_ROLLIN 11  // Rollin alap�rtelmezett tesszell�ci�ja
#define TESS_DEFAULT_PALMBODY 6 // P�lmafa t�rzs alap�rtelmezett tesszell�ci�ja
#define TESS_LAND 1             // Alaplap tesszell�ci�ja
#define TESS_SKY_U 15           // �gbolt v�zszintes tesszell�ci�ja
#define TESS_SKY_T 6            // �gbolt f�gg�leges tesszell�ci�ja

// Sebess�g be�ll�t�sok
#define SPEED_CAM_PHASE 30   // Kamera f�zis
#define SPEED_CAM_Z_ANLGE 25 // Kamera Z sz�g
#define SPEED_CAM_ZOOM 1     // Kamera zoom
#define SPEED_CAM_MOVE_Z 5   // Kamera mozg�s fel/le
#define SPEED_CAM_MOVE_FW 5  // Kamera mozg�s el�re
#define SPEED_CLOUDPHASE 1   // Felh� f�zis
#define SPEED_MULT 1         // Szorz� az �sszes sebess�gre

// Elem be�ll�t�sok
#define DEFAULT_STAIR_STEPS 4 // L�pcs�fokok alap�rtelmezett sz�ma
#define ROLLIN_TENSION 0.75f  // Rollin �vess�ge

#define PALM_LEVELS 3                // P�lmafa szintek sz�ma
#define PALM_LEAVES_ON_FIRST_LEVEL 6 // P�lmafa els� szinten l�v� levelei
#define PALM_FIRST_LEVEL_Z_ANGLE 20  // Els� szint Z sz�ge
#define PALM_Z_ANGLE_VAR 10          // Z sz�g ingadoz�sa
#define PALM_LEVEL_Z_ANGLE_OFF 30    // Szintek k�z�tti Z sz�g k�l�nbs�g
#define PALM_LEVEL_PHASE_OFF 30      // F�zistol�s a szintek k�z�tt
#define PALM_WIND_AMPL 3             // Sz�l amplit�d�
#define PALM_WIND_SPEED 0.667f       // Sz�l sebess�g
#define PALM_COLOR_VAR 0.2f          // Levelek sz�n�nek ingadoz�sa
#define PALM_LEAF_SCALE_VAR 0.05f    // Levelek m�ret�nek ingadoz�sa

#define YUCCA_RND_PARAMETER_NO 10    // Yukka random param�tersz�ma
#define YUCCA_COLOR_VAR 0.1f         // Yukka leveleinek sz�ningadoz�sa
#define YUCCA_Z_ANGLE_VAR 20         // Yukka Z sz�g ingadoz�s
#define YUCCA_LEAF_SCALE_VAR 0.1f    // Yukka lev�lm�ret ingadoz�s
#define YUCCA_LEAF_ROLL_VAR 20       // Yukka lev�l csavarod�s ingadoz�sa

#endif
