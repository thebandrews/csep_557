// 
// impressionistDoc.cpp
//
// It basically maintain the Image for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "triangleBrush.h"
#include "rotatingTriangleBrush.h"
#include "lineBrush.h"
#include "circleBrush.h"
#include "scatteredPointsBrush.h"
#include "scatteredLinesBrush.h"
#include "scatteredCirclesBrush.h"


#define DESTROY(p)  {  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
    // Set NULL image name as init. 
    m_imageName[0]  ='\0';

    m_nWidth            = -1;
    m_ucImage           = NULL;
    m_ucPainting        = NULL;
    m_ucPreviewBackup   = NULL;


    // create one instance of each brush
    ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
    ImpBrush::c_pBrushes    = new ImpBrush* [ImpBrush::c_nBrushCount];

    ImpBrush::c_pBrushes[BRUSH_POINTS]  = new PointBrush( this, "Points" );

    // Note: You should implement these 5 brushes.
    ImpBrush::c_pBrushes[BRUSH_LINES]
    = new LineBrush( this, "Lines" );
    ImpBrush::c_pBrushes[BRUSH_CIRCLES]
    = new CircleBrush( this, "Circles" );
    ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
    = new ScatteredPointsBrush( this, "Scattered Points" );
    ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
    = new ScatteredLinesBrush( this, "Scattered Lines" );
    ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
    = new ScatteredCirclesBrush( this, "Scattered Circles" );
    ImpBrush::c_pBrushes[BRUSH_TRIANGLES]
    = new TriangleBrush( this, "Triangles" );
    ImpBrush::c_pBrushes[BRUSH_ROTATING_TRIANGLE]
    = new RotatingTriangleBrush( this, "Rotating Triangle" );

    // make one of the brushes current
    m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
    m_pUI = ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
    return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
    m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Called by the UI when the user changes the 
// stroke direction type.
// type: one of the defined stroke direction types.
//---------------------------------------------------------
void ImpressionistDoc::setStrokeDirection(int type)
{
    m_pUI->setStrokeDirection(type);
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
    return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
GLfloat ImpressionistDoc::getAlpha()
{
    return m_pUI->getAlpha();
}

//---------------------------------------------------------
// Returns the size of the line.
//---------------------------------------------------------
int ImpressionistDoc::getLineSize()
{
    return m_pUI->getLineSize();
}

//---------------------------------------------------------
// Returns the angle of the line.
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
    return m_pUI->getLineAngle();
}

//---------------------------------------------------------
// Returns the angle of the line.
//---------------------------------------------------------
int ImpressionistDoc::getStrokeDirection()
{
    return m_pUI->getStrokeDirection();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(const char *iname) 
{
    // try to open the image to read
    unsigned char*	data;
    int				width, 
        height;

    if ( (data=load(iname, width, height))==NULL ) 
    {
        fl_alert("Can't load image file");
        return 0;
    }

    // reflect the fact of loading the new image
    m_nWidth		= width;
    m_nPaintWidth	= width;
    m_nHeight		= height;
    m_nPaintHeight	= height;

    // release old storage
    delete [] m_ucImage;
    delete [] m_ucPainting;
    delete [] m_ucPreviewBackup;

    m_ucImage		= data;

    // allocate space for draw view
    m_ucPainting		= new unsigned char [width*height*3];
    m_ucPreviewBackup	= new unsigned char [width*height*3];
    memset(m_ucPainting, 0, width*height*3);

    m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
        m_pUI->m_mainWindow->y(), 
        width*2, 
        height+25);

    // display it on origView
    m_pUI->m_origView->resizeWindow(width, height);	
    m_pUI->m_origView->refresh();

    // refresh paint view as well
    m_pUI->m_paintView->resizeWindow(width, height);	
    m_pUI->m_paintView->refresh();


    return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(const char *iname, const char * type, int quality) 
{

    save(iname, m_ucPainting, m_nPaintWidth, m_nPaintHeight, type, quality);

    return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

    // Release old storage
    if ( m_ucPainting ) 
    {
        delete [] m_ucPainting;

        // allocate space for draw view
        m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
        memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

        // refresh paint view as well	
        m_pUI->m_paintView->refresh();
    }

    return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
*    INPUT: 
*        sourceBuffer:       the original image buffer, 
*        srcBufferWidth:     the width of the image buffer
*        srcBufferHeight:    the height of the image buffer
*                            the buffer is arranged such that 
*                            origImg[3*(row*srcBufferWidth+column)+0], 
*                            origImg[3*(row*srcBufferWidth+column)+1], 
*                            origImg[3*(row*srcBufferWidth+column)+2]
*                            are R, G, B values for pixel at (column, row).
*        destBuffer:         the image buffer to put the resulting
*                            image in.  It is always the same size
*                            as the source buffer.
*
*      filterKernel:         the 2D filter kernel,
*          knlWidth:         the width of the kernel
*         knlHeight:         the height of the kernel
*
*        divisor, offset:    each pixel after filtering should be
*                            divided by divisor and then added by offset
*/
void ImpressionistDoc::applyFilter(const unsigned char* sourceBuffer,
                                   int srcBufferWidth, int srcBufferHeight,
                                   unsigned char* destBuffer,
                                   const double *filterKernel, 
                                   int knlWidth, int knlHeight, 
                                   double divisor, double offset )
{
    // This needs to be implemented for image filtering to work.

    //printf("srcBufferWidth: %d, srcBufferHeight: %d, knlWidth: %d, knlHeight: %d, divisor: %f, offset: %f\n", 
            //srcBufferWidth, srcBufferHeight, knlWidth, knlHeight, divisor, offset);

    //
    // Loop through entire source image
    //
    for(int row = 0; row < srcBufferHeight; row++)
    {
        for(int column = 0; column < srcBufferWidth; column++)
        {
            //
            // Get 5x5 r,g,b pixel matrix that surrounds the current pixel
            //
            //
            // Calculations can be made directly on the source buffer values. Creating r,g,b
            // matrices is not necessary but is added for my own understanding and 
            // helped to break the problem into smaller pieces. 
            // However, this will slow down the computations significantly.
            //
            GLint** red_matrix = new GLint*[knlHeight];
            GLint** green_matrix = new GLint*[knlHeight];
            GLint** blue_matrix = new GLint*[knlHeight];

            for(int i = 0; i < knlHeight; ++i)
            {
                red_matrix[i] = new GLint[knlWidth];
                green_matrix[i] = new GLint[knlWidth];
                blue_matrix[i] = new GLint[knlWidth];
            }

            //
            // Because the image is stored upside down, start at the bottom left corner
            // and work up copying the color pixels to the rgb matrices.
            //
            for(GLint y = (row - 2), a = knlHeight - 1; y < (row - 2 + knlHeight); y++, a--)
            {
                for(GLint x = (column - 2), b = 0; x < (column - 2 + knlWidth); x++, b++)
                {
                    //
                    // Handle edge case by extending edge pixels
                    //
                    GLint x0 = x < 0 ? 0 : x;
                    x0 = x >= srcBufferWidth ? (srcBufferWidth-1) : x;
                    GLint y0 = y < 0 ? 0 : y;
                    y0 = y >= srcBufferHeight ? (srcBufferHeight-1) : y;

                    red_matrix[a][b] = sourceBuffer[3*(y0*srcBufferWidth+x0)+0];
                    green_matrix[a][b] = sourceBuffer[3*(y0*srcBufferWidth+x0)+1];
                    blue_matrix[a][b] = sourceBuffer[3*(y0*srcBufferWidth+x0)+2];

                    //printf("rgb_1[%d][%d] = (%d,%d,%d)\n", a, b, red_matrix[a][b], green_matrix[a][b], blue_matrix[a][b]);
                }
            }

            //
            // Apply the filter kernel to each channel
            //
            GLdouble r_sum = 0.0;
            GLdouble g_sum = 0.0;
            GLdouble b_sum = 0.0;

            for(int i = 0; i < knlHeight; i++)
            {
                for(int j =0; j < knlWidth; j++)
                {
                    //printf("kernel_matrix[%d][%d] = %f\n", i, j, filterKernel[i*knlHeight + j]);
                    r_sum += red_matrix[i][j]*filterKernel[i*knlHeight + j];
                    g_sum += green_matrix[i][j]*filterKernel[i*knlHeight + j];
                    b_sum += blue_matrix[i][j]*filterKernel[i*knlHeight + j];
                }
            }

            //
            // Apply divisor
            //
            if(divisor > 0)
            {
                r_sum = (r_sum / divisor);
                g_sum = (g_sum / divisor);
                b_sum = (b_sum / divisor);
            }

            //
            // Apply offset
            //
            r_sum += offset;
            g_sum += offset;
            b_sum += offset;

            //
            // Value can only be in 0-255 range
            //
            r_sum = r_sum > 255 ? 255 : r_sum;
            g_sum = g_sum > 255 ? 255 : g_sum;
            b_sum = b_sum > 255 ? 255 : b_sum;

            r_sum = r_sum < 0 ? 0 : r_sum;
            g_sum = g_sum < 0 ? 0 : g_sum;
            b_sum = b_sum < 0 ? 0 : b_sum;


            //printf("rgb_3 = (%d,%d,%d)\n", r_sum, g_sum, b_sum);


            //
            // Set the RGB in the dest buffer
            //
            destBuffer[3*(row*srcBufferWidth+column)+0] = (GLint)r_sum;
            destBuffer[3*(row*srcBufferWidth+column)+1] = (GLint)g_sum;
            destBuffer[3*(row*srcBufferWidth+column)+2] = (GLint)b_sum;

            //printf("dest_rgb[%d][%d] = (%d,%d,%d)\n", row, column, r_sum, g_sum, b_sum);

            //
            // Clean up memory
            //
            for(int i = 0; i < knlHeight; ++i)
            {
                delete []red_matrix[i];
                delete []green_matrix[i];
                delete []blue_matrix[i];
            }

            delete []red_matrix;
            delete []green_matrix;
            delete []blue_matrix;
        }
    }


}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
    if ( x < 0 ) 
        x = 0;
    else if ( x >= m_nWidth ) 
        x = m_nWidth-1;

    if ( y < 0 ) 
        y = 0;
    else if ( y >= m_nHeight ) 
        y = m_nHeight-1;

    return (GLubyte*)(m_ucImage + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const ImpBrush::Point p )
{
    return GetOriginalPixel( p.x, p.y );
}



