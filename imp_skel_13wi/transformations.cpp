#include "transformations.h"

#include <math.h>

#define PI 3.14159265       // Used for rotation

Transformations::Transformations(void) {}

//
// Rotate arbitrary point about the origin
//
void Transformations::Rotate(GLint &x, GLint &y, GLint degrees)
{
    GLfloat radian = degrees * (PI / 180);
    GLint org_x = x;
    GLint org_y = y;


    x = (org_x*cos(radian) + (org_y*(-sin(radian))));
    y = (org_x*sin(radian) + (org_y*cos(radian)));

}


//
// Translate a point by Tx,Ty
//
void Transformations::Translate(GLint &x, GLint &y, const GLint Tx, const GLint Ty)
{
    x = x + Tx;
    y = y + Ty;
}


GLdouble Transformations::YPrime(GLubyte *color)
{
    GLdouble kRGBToYPrime [3]  = {0.299, 0.587, 0.114};
    GLdouble y_prime2 = (color[0]*kRGBToYPrime[0] + color[1]*kRGBToYPrime[1] + color[2]*kRGBToYPrime[2]);

    //y_prime2 = y_prime2/255;

    return y_prime2;
}

