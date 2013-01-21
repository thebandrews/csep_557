//
// RotatingTriangleBrush.h
//
// The header file for RotatingTriangle Brush. 
//

#ifndef ROTATINGTRIANGLEBRUSH_H
#define ROTATINGTRIANGleBRUSH_H

#include "impBrush.h"

class RotatingTriangleBrush : public ImpBrush
{
private:
    int m_rotation;
public:
    RotatingTriangleBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

    void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
    char* BrushName( void );
};

#endif
