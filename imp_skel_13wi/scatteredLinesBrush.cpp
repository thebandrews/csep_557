//
// ScatteredLinesBrush.cpp
//
// The implementation of ScatteredLines Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredLinesBrush.h"
#include "transformations.h"

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

    int width = pDoc->getLineSize();
    
    glLineWidth( (float)width );

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
    int width = pDoc->getLineSize();
    int angle = pDoc->getLineAngle();
    int direction = pDoc->getStrokeDirection();

    GLint lineCount = 4; // Draw up to 4 randomly placed lines.
    GLint A1x,A2x,AxMid,A1y,A2y,Ox,Oy,Qx,Qy;
    ImpBrush::Point temp_point;


    if(direction == STROKE_DIRECTION_GRADIENT)
    {
        //
        // Use sobel gradient
        //
        angle = SobelGradient(source);
    }

    

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


    //
    // Generate pointCount number of points in the size region
    //
    for(GLint i = 0; i <= randomLineCount; i++)
    {
        //
        // Compute Y Coordinates
        //
        A1y = rand() % size + Oy;
        A2y = A1y;

        //
        // For each line generate start/end of lines
        //
        A1x = rand() % size + Ox;
        A2x = A1x + size;
        AxMid = .5*(A1x + A2x);

        Qx = AxMid;
        Qy = A1y;

        //
        // Color sampling is different for each location
        //
        temp_point.x = source.x + (AxMid - target.x);
        temp_point.y = source.y + (A1y - target.y);
        SetColor( temp_point ); 

        glBegin( GL_LINES );

        //
        // Translate to the origin
        //
        Transformations::Translate(A1x, A1y, -Qx, -Qy);
        Transformations::Translate(A2x, A2y, -Qx, -Qy);


        //
        // Rotate
        //
        Transformations::Rotate(A1x, A1y, angle);
        Transformations::Rotate(A2x, A2y, angle);


        //
        // Translate back
        //
        Transformations::Translate(A1x, A1y, Qx, Qy);
        Transformations::Translate(A2x, A2y, Qx, Qy);

        glVertex2d( A1x, A1y );
        glVertex2d( A2x, A2y );

        glEnd();
    }
}

void ScatteredLinesBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
    // do nothing so far
}

GLint ScatteredLinesBrush::SobelGradient( const ImpBrush::Point source)
{
    // do nothing so far
    ImpressionistDoc* pDoc = GetDocument();
    GLubyte color[3];
    ImpBrush::Point tempPoint = source;
    GLdouble atan = 0.0;
    GLint angle = 0.0;

    // a0
    tempPoint.x = source.x - 1;
    tempPoint.y = source.y + 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a0 = Transformations::YPrime(color);

    // a1
    tempPoint.x = source.x;
    tempPoint.y = source.y + 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a1 = Transformations::YPrime(color);

    // a2
    tempPoint.x = source.x + 1;
    tempPoint.y = source.y + 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a2 = Transformations::YPrime(color);

    // a7
    tempPoint.x = source.x - 1;
    tempPoint.y = source.y;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a7 = Transformations::YPrime(color);

    // a3
    tempPoint.x = source.x + 1;
    tempPoint.y = source.y;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a3 = Transformations::YPrime(color);

    // a6
    tempPoint.x = source.x - 1;
    tempPoint.y = source.y - 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a6 = Transformations::YPrime(color);

    // a5
    tempPoint.x = source.x;
    tempPoint.y = source.y - 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a5 = Transformations::YPrime(color);

    // a4
    tempPoint.x = source.x + 1;
    tempPoint.y = source.y - 1;
    memcpy ( color, pDoc->GetOriginalPixel( tempPoint ), 3 );
    GLdouble a4 = Transformations::YPrime(color);

    GLdouble s_x = (a2 + 2*a3 + a4) - (a0 + 2*a7 + a6);
    GLdouble s_y = (a0 + 2*a1 + a2) - (a6 + 2*a5 + a4);

    atan = atan2(s_y,s_x);
    angle = atan * (180 / PI);

    //
    // Adjust to 360 degree scale
    //
    if(angle < 0)
    {
        angle = 360 + angle;
    }

    // add 90 deg to make perpendicular 
    angle = (GLint)(angle + 90); // % 360;
    angle %= 360;

    return angle;
}

