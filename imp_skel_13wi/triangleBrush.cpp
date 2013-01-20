//
// TriangleBrush.cpp
//
// The implementation of Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "TriangleBrush.h"

extern float frand();

TriangleBrush::TriangleBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
}

void TriangleBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize( (float)size );

    BrushMove( source, target );
}

void TriangleBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "TriangleBrush::BrushMove  document is NULL\n" );
        return;
    }

    int size = pDoc->getSize();
    int Ax,Ay,Bx,By,Cx,Cy;

    //
    // Compute Triangle points
    //
    Ax = target.x - (.5*size);
    Bx = target.x + (.5*size);
    Cx = target.x;
    Ay = target.y - (.5*size);
    By = target.y - (.5*size);
    Cy = target.y + (.5*size);

    SetColor( source );

    glBegin( GL_POLYGON );

    glVertex2i( Ax, Ay );
    glVertex2i( Bx, By );
    glVertex2i( Cx, Cy );

    glEnd();
}

void TriangleBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

