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

