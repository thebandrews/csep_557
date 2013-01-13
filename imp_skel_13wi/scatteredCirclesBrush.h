//
// ScatteredCirclesBrush.h
//
// The header file for Triangle Brush. 
//

#ifndef SCATTEREDCIRCLESBRUSH_H
#define SCATTEREDCIRCLESBRUSH_H

#include "impBrush.h"

class ScatteredCirclesBrush : public ImpBrush
{
private:
    int m_clickCount;

public:
    ScatteredCirclesBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

    void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
    char* BrushName( void );
};

#endif
