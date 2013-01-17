//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include "transformations.h"


extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    int width = pDoc->getLineSize();
    
    glLineWidth( (float)width );

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
    int angle = pDoc->getLineAngle();


    GLint Ax,Ay,Bx,By,Qx,Qy;

    //
    // Compute Line points
    //
    Ax = target.x - (.5*size);
    Bx = target.x + (.5*size);
    Qx = target.x;
    Ay = target.y;
    By = target.y;
    Qy = target.y;


    //
    // Translate to the origin
    //
    Transformations::Translate(Ax, Ay, -Qx, -Qy);
    Transformations::Translate(Bx, By, -Qx, -Qy);


    //
    // Rotate
    //
    Transformations::Rotate(Ax, Ay, angle);
    Transformations::Rotate(Bx, By, angle);


    //
    // Translate back
    //
    Transformations::Translate(Ax, Ay, Qx, Qy);
    Transformations::Translate(Bx, By, Qx, Qy);

    SetColor( source );
    glBegin( GL_LINES );

    glVertex2i( Ax, Ay );
    glVertex2i( Bx, By );

    glEnd();
}


void LineBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

