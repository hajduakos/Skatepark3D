#ifndef CONFIGURE_H
#define CONFIGURE_H

/*****************************************************************************/
/*                           Konfigurációs fejléc                            */
/*****************************************************************************/

// Debug mód és debug fájlnév
//#define DEBUG_MODE
#define DEBUG_FILENAME "park2.xml"
#define GRID_W 30
#define GRID_H 30
#define GRID_SIZE 0.5f

// Pálya beállítások
#define USE_SHADE_NORMALS // Árnyaló normálok használata
//#define SUNSET // Naplemente
#define START_FULL_SCREEN // Teljes képernyõn induljon-e
#define OFFSET 0.002 // Talaj és homok offszetje
#define WORLD_SIZE 1000 // Pálya mérete

// Tesszellációs beállítások
#define TESS_DEFAULT_QP 8       // Negyedív alapértelmezett tesszellációja
#define TESS_DEFAULT_QPC 8      // Negyedív sarok alapértelmezett tesszellációja
#define TESS_DEFAULT_QPOC 8     // Negyedív külsõ sarok alapértelmezett tesszellációja
#define TESS_DEFAULT_ROLLIN 11  // Rollin alapértelmezett tesszellációja
#define TESS_DEFAULT_PALMBODY 6 // Pálmafa törzs alapértelmezett tesszellációja
#define TESS_LAND 1             // Alaplap tesszellációja
#define TESS_SKY_U 15           // Égbolt vízszintes tesszellációja
#define TESS_SKY_T 6            // Égbolt függõleges tesszellációja

// Sebesség beállítások
#define SPEED_CAM_PHASE 30   // Kamera fázis
#define SPEED_CAM_Z_ANLGE 25 // Kamera Z szög
#define SPEED_CAM_ZOOM 1     // Kamera zoom
#define SPEED_CAM_MOVE_Z 5   // Kamera mozgás fel/le
#define SPEED_CAM_MOVE_FW 5  // Kamera mozgás elõre
#define SPEED_CLOUDPHASE 1   // Felhõ fázis
#define SPEED_MULT 1         // Szorzó az összes sebességre

// Elem beállítások
#define DEFAULT_STAIR_STEPS 4 // Lépcsõfokok alapértelmezett száma
#define ROLLIN_TENSION 0.75f  // Rollin ívessége

#define PALM_LEVELS 3                // Pálmafa szintek száma
#define PALM_LEAVES_ON_FIRST_LEVEL 6 // Pálmafa elsõ szinten lévõ levelei
#define PALM_FIRST_LEVEL_Z_ANGLE 20  // Elsõ szint Z szöge
#define PALM_Z_ANGLE_VAR 10          // Z szög ingadozása
#define PALM_LEVEL_Z_ANGLE_OFF 30    // Szintek közötti Z szög különbség
#define PALM_LEVEL_PHASE_OFF 30      // Fázistolás a szintek között
#define PALM_WIND_AMPL 3             // Szél amplitúdó
#define PALM_WIND_SPEED 0.667f       // Szél sebesség
#define PALM_COLOR_VAR 0.2f          // Levelek színének ingadozása
#define PALM_LEAF_SCALE_VAR 0.05f    // Levelek méretének ingadozása

#define YUCCA_RND_PARAMETER_NO 10    // Yukka random paraméterszáma
#define YUCCA_COLOR_VAR 0.1f         // Yukka leveleinek színingadozása
#define YUCCA_Z_ANGLE_VAR 20         // Yukka Z szög ingadozás
#define YUCCA_LEAF_SCALE_VAR 0.1f    // Yukka levélméret ingadozás
#define YUCCA_LEAF_ROLL_VAR 20       // Yukka levél csavarodás ingadozása

#endif
