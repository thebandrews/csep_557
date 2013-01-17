//
// transformations.h
//
// The header file for basic transformations. 
//

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "impressionist.h"

class Transformations
{
public:
    Transformations(void);

    static void Rotate(GLint &x, GLint &y, GLint degrees);
    static void Translate(GLint &x, GLint &y, const GLint Tx, const GLint Ty);
};

#endif
