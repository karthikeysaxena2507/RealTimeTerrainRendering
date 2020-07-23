
#ifndef LANDSCAPE_H
#define LANDSCAPE_H
#include "Patch.h"
#define MAP_1024

#ifdef MAP_2048

// CREATING A 2048X2048 MAP
#define MAP_SIZE (2048)
#define NUM_PATCHES_PER_SIDE (32)

#else
#ifdef MAP_1024

// CREATING A 1024X1024 MAP
#define MAP_SIZE (1024)
#define NUM_PATCHES_PER_SIDE (16)

#else

// CREATING A 512X512 MAP
#define MAP_SIZE (512)
#define NUM_PATCHES_PER_SIDE (8)

#endif
#endif

#define MULT_SCALE (0.5f)

// NO. OF TRI TREE NODES TO BE ALLOCATED
#define POOL_SIZE (25000)

// DECIDING NO. OF PATCHES WE WANT IN A LANDSCAPE
#define PATCH_SIZE (MAP_SIZE/NUM_PATCHES_PER_SIDE)
#define TEXTURE_SIZE (128)

// DRAWING MODES OF DISPLAY
#define DRAW_USE_TEXTURE   (0)
#define DRAW_USE_LIGHTING  (1)
#define DRAW_USE_FILL_ONLY (2)
#define DRAW_USE_WIREFRAME (3)

// HOW TO ROTATE
#define ROTATE_PITCH (0)
#define ROTATE_YAW   (1)
#define ROTATE_ROLL	 (2)

// SOME MATHAMATICAL TERMS
#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))
#define DEG2RAD(a) (((a) * M_PI) / 180.0f)
#define M_PI (3.14159265358979323846f)


extern GLuint gTextureID;
extern int gDrawMode;
extern GLfloat gViewPosition[];
extern GLfloat gCameraRotation[];
extern GLfloat gClipAngle;
extern float gFrameVariance;
extern int gDesiredTris;
extern int gNumTrisRendered;
extern float gFovX;

extern void calcNormal(float v[3][3], float out[3]);
extern void ReduceToUnit(float vector[3]);


// Landscape Class
// Holds all the information to render an entire landscape.

class Landscape
{
protected:
	unsigned char *m_HeightMap;										// HeightMap of the Landscape
	Patch m_Patches[NUM_PATCHES_PER_SIDE][NUM_PATCHES_PER_SIDE];	// Array of patches

	static int	m_NextTriNode;										// Index to next free TriTreeNode
	static TriTreeNode m_TriPool[POOL_SIZE];						// Pool of TriTree nodes for splitting

	static int GetNextTriNode() { return m_NextTriNode; }
	static void SetNextTriNode( int nNextNode ) { m_NextTriNode = nNextNode; }

public:
	static TriTreeNode *AllocateTri();

	virtual void Init(unsigned char *hMap);
	virtual void Reset();
	virtual void Tessellate();
	virtual void Render();

};


#endif