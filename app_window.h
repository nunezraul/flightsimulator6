
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H
#define _USE_MATH_DEFINES

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "so_texture.h";
# include "so_textured_tube.h"
# include "so_capsule.h"
# include "curve_eval.h"
# include <cmath>

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
//    GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;

    // My scene objects:
    SoAxis _axis;
    SoModel _model, _model2, _model3, _model4, _model5, _model6, _city;
	SoTriangles _floor, _side1, _side2, _side3, _side4;
	SoTexturedTube _side;
	SoCapsule _sun;

    // Scene data:
    bool  _viewaxis, animate, resetanim, camera, sunanim;
    GsModel _gsm, _gsm2, _gsm3, _gsm4, _gsm5, _gsm6, _building;
    GsLight _light, _shadow;
	GLuint *textures = new GLuint[2];
    
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy, rotate, speed, xview, yview;
	float _turnlr, _turnud = 0, _wingsflyR = 0, _wingsflyL = 0, _animinc = 1, _backL = 0, _backR = 0;
	int _w, _h;
	GsVec R, SR;
	double lasttime, lasttime2;
	float sunx, suny, sunz, sunxc = 0, sunxz = 0;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void glutIdle();
 };

#endif // APP_WINDOW_H
