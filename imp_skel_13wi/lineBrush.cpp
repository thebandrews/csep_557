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

#include <math.h>

#define PI 3.14159265

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
    int direction = pDoc->getStrokeDirection();

    if(direction == STROKE_DIRECTION_GRADIENT)
    {
        //
        // Use sobel gradient
        //
        angle = SobelGradient(source);
    }

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


GLint LineBrush::SobelGradient( const ImpBrush::Point source)
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

