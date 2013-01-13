//
// ScatteredCirclesBrush.cpp
//
// The implementation of ScatteredCircles Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredCirclesBrush.h"

#include <math.h>
#include <time.h>


#define PI 3.14159265       // Used for circle calculation

extern float frand();

ScatteredCirclesBrush::ScatteredCirclesBrush( ImpressionistDoc* pDoc, char* name ) :
    ImpBrush(pDoc,name)
{
    m_clickCount = 0;
}

void ScatteredCirclesBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
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

void ScatteredCirclesBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg=pDoc->m_pUI;

    if ( pDoc == NULL ) {
        printf( "ScatteredCirclesBrush::BrushMove document is NULL\n" );
        return;
    }

    GLint size = pDoc->getSize();
    GLint circleCount = 4; // Draw up to 4 randomly placed circles.
    GLint segments = 40; // Increase segment size to increase accuracy
    GLint Ax,Ay,Bx,By,Ox,Oy;

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
    // Generate random number of circles
    //
    GLint randomCircleCount = rand() % circleCount + 1;


    //
    // Generate pointCount number of points in the size region
    //
    for(GLint i = 0; i <= randomCircleCount; i++)
    {
        //
        // Compute new circle origin
        //
        Ay = rand() % size + Oy;
        Ax = rand() % size + Ox;

        glBegin( GL_POLYGON );

        //
        // Color sampling is different for each location
        //
        temp_point.x = source.x + (Ax - target.x);
        temp_point.y = source.y + (Ay - target.y);
        SetColor( temp_point ); 


        //
        // Draw the circle
        //
        //
        for(int j = 0; j < 360; j += (360 / segments))
        {
            

            //
            // Convert Degrees to radians and calculate x,y
            //
            GLfloat radian = j * (PI / 180);
            Bx = Ax + cos(radian) * (.5*size);
            By = Ay + sin(radian) * (.5*size);

            glVertex2d(Bx, By);
        }

        glEnd();
    }

    
}

void ScatteredCirclesBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

