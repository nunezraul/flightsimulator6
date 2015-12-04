//Copyright 2015. Patent pending.
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _fovy = GS_TORAD(120.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
   rotate = 0;
   speed = 0; _turnlr = 0;
   xview = 0; yview = 0;
   animate = false;
   sunx = 0.5f; suny = 1.0f; sunz = 0.8f;
   camera = true;
   sunanim = true;
 }

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
   _axis.init ();
   _floor.init( "../texture/_image.bmp", textures); _floor.build( 20.0f, -5.0f, 20.0f, textures);
   _side.init(); _side.build(25.0f, 20.0f, 20.0f, 25, textures);
   _sun.init(); 
   _sun.build(1.0f, 1, 1, 0);
   _building.load("../models/The_City.obj"); //_building.scale(.7f);
   _city.init(); _city.build(_building);

   //initiate models
   _model.init(); _model2.init(); _model3.init(); _model4.init(); _model5.init(); _model6.init();

   // set light:
   _light.set ( GsVec(0,0,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );
   _shadow.set(GsVec(0, 0, 10), GsColor(0, 0, 0, 255), GsColor::black, GsColor::black);

   // Load demo model:
   loadModel ( 1 );
 }

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<" ";
   std::cout<<"F:  "<<m.F.size()<<" ";
   std::cout<<"N:  "<<m.N.size()<<" ";
   std::cout<<"M:  "<<m.M.size()<<" ";
   std::cout<<"Fn: "<<m.Fn.size()<<" ";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f;
   GsString file;
   switch ( model )
    { case 1:	f=0.1f; 
				_gsm.load("../models/757body.obj");
				_gsm2.load("../models/757rightwing.obj");
				_gsm3.load("../models/757leftwing.obj"); 
				_gsm4.load("../models/757toptail.obj"); 
				_gsm5.load("../models/757leftback.obj"); 
				_gsm6.load("../models/757rightback.obj");
				std::cout << "Loading 757...\n";
				/*if (!_gsm.load("../models/757body.obj") || !_gsm2.load("../models/757rightwing.obj") || !_gsm3.load("../models/757leftwing.obj") || !_gsm4.load("../models/757toptail.obj") || !_gsm5.load("../models/757leftback.obj") || !_gsm6.load("../models/757rightback.obj")) {
					std::cout << "Error!\n";
				}*/
				printInfo(_gsm); printInfo(_gsm2); printInfo(_gsm3); printInfo(_gsm4); printInfo(_gsm5); printInfo(_gsm6);
				_gsm.scale(f); _gsm2.scale(f); _gsm3.scale(f); _gsm4.scale(f); _gsm5.scale(f); _gsm6.scale(f);
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				break;
      case 2:	f=0.20f;
				file = "../models/al.obj";
				_gsm.load( file );
				_gsm2.load("../models/blank.obj"); _gsm3.load("../models/blank.obj"); _gsm4.load("../models/blank.obj"); _gsm5.load("../models/blank.obj"); _gsm6.load("../models/blank.obj");
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				std::cout << "Loading " << file << "...\n";
				printInfo(_gsm);
				_gsm.scale(f);
				_model.build(_gsm);
				break;
      case 3:	f=0.10f; 
				file = "../models/f-16.obj";
				_gsm.load( file );
				_gsm2.load("../models/blank.obj"); _gsm3.load("../models/blank.obj"); _gsm4.load("../models/blank.obj"); _gsm5.load("../models/blank.obj"); _gsm6.load("../models/blank.obj");
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				std::cout << "Loading " << file << "...\n";
				printInfo(_gsm);
				_gsm.scale(f);
				_model.build(_gsm);
				break;
	  case 4:	f = .01f; 
				file = "../models/porsche.obj"; 
				_gsm.load( file );
				_gsm2.load("../models/blank.obj"); _gsm3.load("../models/blank.obj"); _gsm4.load("../models/blank.obj"); _gsm5.load("../models/blank.obj"); _gsm6.load("../models/blank.obj");
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				std::cout<<"Loading "<<file<<"...\n";
				printInfo(_gsm);
				_gsm.scale(f);
				_model.build(_gsm);
				break;
      default: return;
    }
	redraw();
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
   switch ( key )
   {
   case ' ': camera = !camera; redraw(); break;
	  case 27 : exit(1); // Esc was pressed
      /*case 's' : std::cout<<"Smoothing normals...\n";
                _gsm.smooth ( GS_TORAD(35) ); 
                 printInfo(_gsm);
                 _model.build(_gsm); 
                 redraw(); 
                 break;*/
      case 'f' : std::cout<<"Flat normals...\n";
                _gsm.flat();
                 printInfo(_gsm);
                 _model.build(_gsm); 
                 redraw(); 
                 break;
      case 'p' : if ( !_model.phong() )
                  { std::cout<<"Switching to phong shader...\n";
                    _model.phong(true);
                  }
                 redraw(); 
                 break;
      case 'g' : if ( _model.phong() )
                  { std::cout<<"Switching to gouraud shader...\n";
                    _model.phong(false);
                  }
                 redraw(); 
                 break;
	  case 'e': rotate += 1; redraw(); break;
	  case 'q': rotate -= 1; redraw(); break;
	  case 'w': _turnud -= 1; redraw(); break;
	  case 's': _turnud += 1; redraw(); break;
	  case 'a': _turnlr += 1; redraw(); break;
	  case 'd': _turnlr -= 1; redraw(); break;
	  case '[': speed -= .001; redraw(); break;
	  case '\'': speed += .001; redraw(); break;
	  case 'n': _wingsflyR++; redraw(); break;
	  case 'm': _wingsflyR--;  redraw(); break;
	  case 'b': _wingsflyL++; redraw(); break;
	  case 'v': _wingsflyL--;  redraw(); break;
	  case 'l': _backR++; redraw(); break;
	  case 'k': _backR--; redraw(); break;
	  case 'j': _backL++; redraw(); break;
	  case 'h': _backL--; redraw(); break;
	  case 'y': sunanim = !sunanim; redraw(); break;
	  case 't': if (!animate) { animate = true; resetanim = false; }
				else resetanim = true; redraw(); break;
      default : loadModel ( int(key-'0') );
                break;
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:      _roty-=incr; break;
      case GLUT_KEY_RIGHT:     _roty+=incr; break;
      case GLUT_KEY_UP:        _rotx+=incr; break;
      case GLUT_KEY_DOWN:      _rotx-=incr; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }
   if (sunanim) {
	   sunx = 2 * (cos(2 * M_PI*sunxc / 360) + sin(2 * M_PI*sunxc / 360)); suny = 20.0f; sunz = 2 * (-sin(2 * M_PI*sunxz / 360) + cos(2 * M_PI*sunxz / 360));
   }
   else {
	   sunx = 0.0; suny = 1; sunz = -.5;
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf, barrelroll, leftright, transf, updown, rightwing, leftwing, offsety, centerrwing, centerlwing, rl, rr, backR, backL, centerbackl, centerbackr, br, bl;
   GsMat rfrot, lfrot, rbrot, lbrot, rollyawpitch, ShadowT, sunrot;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   offsety.translation(GsVec(0.0f, -5.7f, 0.0f));

   //Rotate many degrees
   barrelroll.rotz(2 * M_PI * rotate / 360);
   leftright.roty(2 * M_PI * _turnlr / 360);
   updown.rotx(2 * M_PI * _turnud / 360);
   //Roll, yaw and pitch for the airplane
   rollyawpitch = leftright*updown*barrelroll;

   //Translate front wings to center
   centerrwing.translation(GsVec(-0.1f,-0.15f,0.0f)); centerlwing.translation(GsVec(0.1f, -0.15f, 0.0f));
   //Translate front wings back to airplane
   rr.translation(GsVec(0.1f, 0.15f, 0.0f)); rl.translation(GsVec(-0.1f, 0.15f, 0.0f));
   //Translate back wings to center
   centerbackl.translation(GsVec(-0.05f, -0.2f, 0.0f)); centerbackr.translation(GsVec(0.05f, -0.2f, 0.0f));
   //Translate back wings to airplane
   bl.translation(GsVec(0.05f, 0.2f, 0.0f)); br.translation(GsVec(-0.05f, 0.2f, 0.0f));
   //Rotate front wings
   rightwing.rotz(2 * M_PI * _wingsflyR / 360); leftwing.rotz(2 * M_PI * -_wingsflyL / 360);
   //Rotate back wings
   backR.rotz(2 * M_PI * _backR / 360); backL.rotz(2 * M_PI * -_backL / 360);
   //Sun rotation
   sunrot.translation(5.f, 50 * cos(2.f * M_PI * sunxc / 360.f), 50 * sin(2.f * M_PI * sunxc / 360.f));
   //Clean up draw function
   rfrot = rr*rightwing*centerrwing;
   lfrot = rl*leftwing*centerlwing;
   rbrot = br*backR*centerbackr;
   lbrot = bl*backL*centerbackl;


	//speed is fast
	//Translation matrix for pivot point
	GsVec P = GsVec(0.0f, 0.0f, speed);
	GsVec bd = leftright*updown*barrelroll*P;
	R = R + bd;
	transf.setrans(R);
	stransf.setrans(bd);

	GsVec sbd = leftright*P;
	SR = SR + sbd;
	ShadowT.setrans(SR);

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, camview2, _birdseye, persp, sproj;

   GsVec eye(0,0,0), center(0,0,0), up(0,1,0);
   GsVec eye2(0, 10, 0), center2(0, 0, 0), up2(0, 0, 1);
   eye += R + leftright*updown*barrelroll*GsVec(0,0,2);
   center += R + GsVec(0, 0, 0);

   
   
   float ground[4] = { 0, 1, 0, 4.99 };
   float light[4] = { sunx, suny, sunz, 0 };
   float  dot;
   GsMat shadowMat;

   dot = ground[0] * light[0] +
	   ground[1] * light[1] +
	   ground[2] * light[2] +
	   ground[3] * light[3];
    
   shadowMat.setl1(dot - light[0] * ground[0], 0.0 - light[0] * ground[1], 0.0 - light[0] * ground[2], 0.0 - light[0] * ground[3]);
   shadowMat.setl2(0.0 - light[1] * ground[0], dot - light[1] * ground[1], 0.0 - light[1] * ground[2], 0.0 - light[1] * ground[3]);
   shadowMat.setl3(0.0 - light[2] * ground[0], 0.0 - light[2] * ground[1], dot - light[2] * ground[2], 0.0 - light[2] * ground[3]);
   shadowMat.setl4(0.0 - light[3] * ground[0], 0.0 - light[3] * ground[1], 0.0 - light[3] * ground[2], dot - light[3] * ground[3]);
   
   //shadowMat = shadowMat*ry*rx;
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix
   camview2.lookat(eye2, center2, up2);

   float aspect=1.0f, znear=0.1f, zfar=100.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   if (camera) {
	   sproj = persp * camview; // set final scene projection
   }
   else {
	   sproj = persp * camview2;
   }

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.
   float col = 1;
   // Draw:
   //if ( _viewaxis ) _axis.draw ( stransf, sproj );
	_model.draw(stransf*transf*rollyawpitch, sproj, _light, 0);
	_model2.draw(stransf*transf*rollyawpitch*rfrot, sproj, _light, 0);
	_model3.draw(stransf*transf*rollyawpitch*lfrot, sproj, _light, 0);
	_model4.draw(stransf*transf*rollyawpitch, sproj, _light, 0);
	_model5.draw(stransf*transf*rollyawpitch*rbrot, sproj, _light, 0);
	_model6.draw(stransf*transf*rollyawpitch*lbrot, sproj, _light, 0);
	_floor.draw(stransf, sproj, _light, textures);
	_city.draw(stransf*offsety, sproj, _light, 0);
	_city.draw(stransf*shadowMat*offsety, sproj, _shadow, 0);
	//Shadows
	_model.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model2.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model3.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model4.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model5.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model6.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_side.draw(stransf, sproj, _light, col, textures);
	_sun.draw(stransf * sunrot, sproj);


   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::glutIdle() 
{
	double curtime = gs_time();
	if (curtime - lasttime > .01f && animate) {
		if (_wingsflyR >= 45 || _wingsflyR <= -45) {
			_animinc *= -1;
		}
		_wingsflyR += _animinc;
		_wingsflyL += _animinc;
		_backR += _animinc;
		_backL += _animinc;
		lasttime = curtime;
		if (resetanim == true) {
			if (_wingsflyR == 0) {
				animate = false;
			}
		}
		
		/*sunxc++;
		sunxz++;
		if (sunxc == 360) {
			sunxc = 0;
			sunxz = 0;
		}*/
		//std::cout << "_wingsfly: " << _wingsflyR << std::endl;
	}
	if (curtime - lasttime2 > .01f && sunanim) {
		sunxc += .5;
		sunxz += .5;
		lasttime = curtime;
		if (sunxc == 360) {
			sunxc = 0;
			sunxz = 0;
		}
		//std::cout << "_wingsfly: " << _wingsflyR << std::endl;
	}

	redraw();
}


