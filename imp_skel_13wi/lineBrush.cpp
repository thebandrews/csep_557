//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize( (float)size );

    BrushMove( source, target );
}

void LineBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "LineBrush::BrushMove  document is NULL\n" );
        return;
    }

    int size = pDoc->getSize();
    int width = pDoc->getLineSize();
    GLint Ax,Ay,Bx,By;

    //
    // Compute Line points
    //
    Ax = target.x - (.5*size);
    Bx = target.x + (.5*size);
    Ay = target.y;
    By = target.y;


    glBegin( GL_POLYGON );
    SetColor( source );

    glVertex2i( Ax, Ay );
    glVertex2i( Bx, By );
    glVertex2i( Bx, By + width );
    glVertex2i( Ax, Ay + width );

    glEnd();
}

void LineBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

