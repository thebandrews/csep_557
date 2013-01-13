//
// ScatteredPointsBrush.h
//
// The header file for Triangle Brush. 
//

#ifndef SCATTEREDPOINTSBRUSH_H
#define SCATTEREDPOINTSBRUSH_H

#include "impBrush.h"

class ScatteredPointsBrush : public ImpBrush
{
private:
    int m_clickCount;

public:
    ScatteredPointsBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

    void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
    void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
    char* BrushName( void );
};

#endif
