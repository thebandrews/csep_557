//
// RotatingTriangleBrush.cpp
//
// The implementation of RotatingTriangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "rotatingTriangleBrush.h"
#include "transformations.h"

extern float frand();

RotatingTriangleBrush::RotatingTriangleBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
    m_rotation = 0;
}

void RotatingTriangleBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize( (float)size );

    BrushMove( source, target );
}

void RotatingTriangleBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "RotatingTriangleBrush::BrushMove  document is NULL\n" );
        return;
    }

    //
    // Rotation 10 degrees
    //
    m_rotation += 10;
    m_rotation %= 360;


    int size = pDoc->getSize();
    int Ax,Ay,Bx,By,Cx,Cy,Qx,Qy;

    //
    // Compute RotatingTriangle points
    //
    Ax = target.x - (.5*size);
    Bx = target.x + (.5*size);
    Cx = target.x;
    Qx = Cx;
    Ay = target.y - (.5*size);
    By = target.y - (.5*size);
    Cy = target.y + (.5*size);
    Qy = Cy;

    SetColor( source );

    //
    // Translate, Rotate, Translate
    //
    glPushMatrix();
    glTranslatef(target.x, target.y, 0);
    glRotatef (m_rotation, 0.0, 0.0, 1.0);
    glTranslatef(-target.x, -target.y, 0);

    glBegin( GL_POLYGON );

    glVertex2i( Ax, Ay );
    glVertex2i( Bx, By );
    glVertex2i( Cx, Cy );

    glEnd();
    glPopMatrix();
}

void RotatingTriangleBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

