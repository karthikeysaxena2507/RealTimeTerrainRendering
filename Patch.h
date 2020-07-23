
#ifndef PATCH_H
#define PATCH_H

#define VARIANCE_DEPTH (9)

class Landscape;


// TriTreeNode Struct

struct TriTreeNode
{
	TriTreeNode *LeftChild;
	TriTreeNode *RightChild;
	TriTreeNode *BaseNeighbor;
	TriTreeNode *LeftNeighbor;
	TriTreeNode *RightNeighbor;
};


// Patch Class
// Store information needed at the Patch level

class Patch
{
protected:
	unsigned char *m_HeightMap;									
	int m_WorldX, m_WorldY;										

	unsigned char m_VarianceLeft[ 1<<(VARIANCE_DEPTH)];			// Left variance tree
	unsigned char m_VarianceRight[1<<(VARIANCE_DEPTH)];			// Right variance tree

	unsigned char *m_CurrentVariance;							
	unsigned char m_VarianceDirty;								
	unsigned char m_isVisible;									

	TriTreeNode m_BaseLeft;										// Left base triangle tree node
	TriTreeNode m_BaseRight;									// Right base triangle tree node

public:
	// Some encapsulation functions & extras
	TriTreeNode *GetBaseLeft()  { return &m_BaseLeft; }
	TriTreeNode *GetBaseRight() { return &m_BaseRight; }
	char isDirty()     { return m_VarianceDirty; }
	int  isVisibile( ) { return m_isVisible; }
	void SetVisibility( int eyeX, int eyeY, int leftX, int leftY, int rightX, int rightY );

	virtual void Init( int heightX, int heightY, int worldX, int worldY, unsigned char *hMap );
	virtual void Reset();
	virtual void Tessellate();
	virtual void Render();
	virtual void ComputeVariance();

	virtual void			Split( TriTreeNode *tri);
	virtual void			RecursTessellate( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
	virtual void			RecursRender( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
	virtual unsigned char	RecursComputeVariance(	int leftX,  int leftY,  unsigned char leftZ,
													int rightX, int rightY, unsigned char rightZ,
													int apexX,  int apexY,  unsigned char apexZ,
													int node);
};


#endif
