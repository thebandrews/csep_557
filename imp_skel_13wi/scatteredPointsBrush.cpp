//
// ScatteredPointsBrush.cpp
//
// The implementation of ScatteredPoints Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredPointsBrush.h"

#include <math.h>

#define PI 3.14159265       // Used for circle calculation


extern float frand();

ScatteredPointsBrush::ScatteredPointsBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
}

void ScatteredPointsBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize( (float)size );

    BrushMove( source, target );
}

void ScatteredPointsBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "ScatteredPointsBrush::BrushMove document is NULL\n" );
        return;
    }

    GLint size = pDoc->getSize();
    GLint segments = 40; // Increase segment size to increase accuracy
    GLint Ax,Ay;

    glBegin( GL_POLYGON );
    SetColor( source );

    //
    // Compute ScatteredPoints points (from 0-360 degrees)
    //
    for(int i = 0; i < 360; i += (360 / segments))
    {
        //
        // Convert Degrees to radians and calculate x,y
        //
        GLfloat radian = i * (PI / 180);
        Ax = target.x + cos(radian) * (.5*size);
        Ay = target.y + sin(radian) * (.5*size);

        glVertex2d(Ax, Ay);
    }

    glEnd();
}

void ScatteredPointsBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

