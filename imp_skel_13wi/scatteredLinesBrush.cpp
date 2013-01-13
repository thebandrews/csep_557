//
// ScatteredLinesBrush.cpp
//
// The implementation of ScatteredLines Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredLinesBrush.h"

#include <math.h>
#include <time.h>


#define PI 3.14159265       // Used for circle calculation


extern float frand();

ScatteredLinesBrush::ScatteredLinesBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
    m_clickCount = 0;
}

void ScatteredLinesBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
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

void ScatteredLinesBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "ScatteredLinesBrush::BrushMove document is NULL\n" );
        return;
    }

    GLint size = pDoc->getSize();
    GLint lineCount = 4; // Draw up to 4 randomly placed lines.

    GLint A1x,A2x,AxMid,Ay,Ox,Oy;

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
    // Generate random number of lines
    //
    GLint randomLineCount = rand() % lineCount + 1;

    glBegin( GL_LINES );

    //
    // Generate pointCount number of points in the size region
    //
    for(GLint i = 0; i <= randomLineCount; i++)
    {
        //
        // Compute Y Coordinates
        //
        Ay = rand() % size + Oy;

        //
        // For each line generate start/end of lines
        //
        A1x = rand() % size + Ox;
        A2x = A1x + size;
        AxMid = .5*(A1x + A2x);


        //
        // Color sampling is different for each location
        //
        temp_point.x = source.x + (AxMid - target.x);
        temp_point.y = source.y + (Ay - target.y);
        SetColor( temp_point ); 

        glVertex2d( A1x, Ay );
        glVertex2d( A2x, Ay );
    }

    glEnd();
}

void ScatteredLinesBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

