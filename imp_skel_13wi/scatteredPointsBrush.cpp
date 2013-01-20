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
#include <time.h>


#define PI 3.14159265       // Used for circle calculation


extern float frand();

ScatteredPointsBrush::ScatteredPointsBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
    m_clickCount = 0;
}

void ScatteredPointsBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    //
    // Point size is always 1. Slider will indicate
    // Dimensions / number of scattered points
    //
    glPointSize( 1.0 );

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
    GLint pointCount = .1 * (size * size); // Fill up 10% of the available pixels 
    pointCount = (pointCount < 1 ? 1 : pointCount); // Insure pointCount >= 1

    GLint Ax,Ay,Ox,Oy;

    ImpBrush::Point temp_point;

    //
    // Compute origin x,y values
    //
    Ox = target.x - (.5*size);
    Oy = target.y - (.5*size);

    //
    // Randomization for each click (cap at INT_MAX should we ever hit it)
    //
    m_clickCount >= INT_MAX ? m_clickCount = 0 : m_clickCount++;
    

    //
    // Seed random number using the time plus click count
    //
    srand ( time(NULL) + m_clickCount );


    //
    // Generate pointCount number of points in the size region
    //
    for(GLint i = 0; i <= pointCount; i++)
    {
        //
        // Compute X,Y Coordinates
        //
        Ax = rand() % size + Ox;
        Ay = rand() % size + Oy;

        //
        // Color sampling is different for each location
        //
        temp_point.x = source.x + (Ax - target.x);
        temp_point.y = source.y + (Ay - target.y);

        SetColor( temp_point ); 

        glBegin( GL_POINTS );

        glVertex2d( Ax, Ay );

        glEnd();
    }
}

void ScatteredPointsBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

