//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>        // FLTK file chooser
#include <FL/Fl_Native_File_Chooser.H>    // FLTK add-on: native file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>

#include "impressionist.h"
#include "originalView.h"
#include "paintView.h"
#include "FltDesignUI.h"

#include "impBrush.h"

const int FLT_WIDTH = 5;
const int FLT_HEIGHT = 5;

class ImpressionistUI {
public:
    ImpressionistUI();
    ~ImpressionistUI();

    // The FLTK widgets
    Fl_Window*                  m_mainWindow;
    Fl_Menu_Bar*                m_menubar;

    PaintView*                  m_paintView;
    OriginalView*               m_origView;
    //Fl_File_Chooser*          m_chooser;
    Fl_Native_File_Chooser*     m_nativeChooser;

    // filter kernel dialog
    FltDesignUI*                fltDesignUI;

    // for brush dialog
    Fl_Window*                  m_brushDialog;
    Fl_Choice*                  m_BrushTypeChoice;
    Fl_Choice*                  m_StrokeDirectionChoice;

    Fl_Slider*                  m_BrushSizeSlider;
    Fl_Slider*                  m_LineSizeSlider;
    Fl_Slider*                  m_LineAngleSlider;

    Fl_Button*                  m_ClearCanvasButton;

    // Member functions
    void                        setDocument(ImpressionistDoc* doc);
    ImpressionistDoc*           getDocument();

    void                        show();
    void                        resize_windows(int w, int h);

    //
    // Brush size interface
    //
    int                         getSize();
    void                        setSize(int size);

    //
    // Line brush size (width) interface.
    //
    int                         getLineSize();

    //
    // Line brush angle interface.
    //
    int                         getLineAngle();

    //
    // Line brush stroke direction interface.
    //
    int                         getStrokeDirection();
    void                        setStrokeDirection(int type);


    // Callbacks for the image filter dialogue (different from
    // the other callbacks because they aren't static)
    void                        initFltDesignUI(void);
    void                        updateFilter(void);

    void                        cancelFilter(void);
    void                        applyFilter(void);
    void                        previewFilter(void);


private:
    ImpressionistDoc*    m_pDoc;        // pointer to document to communicate with the document

    //
    // Brush Size attribute
    //
    int        m_nSize;

    //
    // Line Size attribute
    //
    int         m_lSize;

    //
    // Line Size attribute
    //
    int         m_lAngle;

    //
    // Line Stroke Direction attribute
    //
    int         m_lStrokeDirection;

    // These attributes are set by the filter kernel UI
    double fltKernel[FLT_WIDTH*FLT_HEIGHT];        //the kernel of the image filter
    double scale,offset;                        //the divisor and offset parameter, 
    //applied after convolving to transform 
    //the filtered image into the range [0,255]


    // Static class members
    static Fl_Menu_Item        menuitems[];
    static Fl_Menu_Item        brushTypeMenu[NUM_BRUSH_TYPE+1];
    static Fl_Menu_Item        strokeDirectionMenu[NUM_STROKE_DIRECTIONS+1];

    static ImpressionistUI*    whoami(Fl_Menu_* o);

    // Helper function
    const char* fileDialog(Fl_Native_File_Chooser::Type dialogType, const char* filter, const char* title=NULL);

    // All callbacks here.  Callbacks are declared 
    // static
    static void    cb_load_image(Fl_Menu_* o, void* v);
    static void    cb_save_image(Fl_Menu_* o, void* v);
    static void    cb_brushes(Fl_Menu_* o, void* v);
    static void    cb_filter_kernel(Fl_Menu_* o, void* v);
    static void    cb_clear_canvas(Fl_Menu_* o, void* v);
    static void    cb_exit(Fl_Menu_* o, void* v);
    static void    cb_about(Fl_Menu_* o, void* v);
    static void    cb_brushChoice(Fl_Widget* o, void* v);
    static void    cb_strokeDirection(Fl_Widget* o, void* v);
    static void    cb_clear_canvas_button(Fl_Widget* o, void* v);
    static void    cb_sizeSlides(Fl_Widget* o, void* v);
    static void    cb_lineSizeSlides(Fl_Widget* o, void* v);
    static void    cb_lineAngleSlides(Fl_Widget* o, void* v);

};

#endif

