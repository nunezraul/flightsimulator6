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
   curvegen = false;
   curving = false;
   ccount = 0;
   oldtrans.setrans(GsVec(0, 0, 0));
 }

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
	 _axis.init(); _curve.init();
   _floor.init( "../texture/_image.bmp", textures); _floor.build( 20.0f, -5.0f, 20.0f, textures);
   _side.init(); _side.build(25.0f, 20.0f, 20.0f, 25, textures);
   _sun.init(); 
   _sun.build(1.0f, 1, 1, 0);
   _building.load("../models/The_City.obj"); //_building.scale(.7f);
   _city.init(); _city.build(_building);
   _normal.init(); _tangent.init(); _bitangent.init();

   //initiate models
   _model.init(); _model2.init(); _model3.init(); _model4.init(); _model5.init(); _model6.init();

   // set light:
   _light.set ( GsVec(0,0,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );
   _shadow.set(GsVec(0, 0, 10), GsColor(0, 0, 0, 255), GsColor::black, GsColor::black);

   // Load demo model:
   loadModel ( 6 );
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
    { case 6:	f=0.1f; 
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
      case 7:	f=0.20f;
				file = "../models/al.obj";
				_gsm.load( file );
				_gsm2.load("../models/blank.obj"); _gsm3.load("../models/blank.obj"); _gsm4.load("../models/blank.obj"); _gsm5.load("../models/blank.obj"); _gsm6.load("../models/blank.obj");
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				std::cout << "Loading " << file << "...\n";
				printInfo(_gsm);
				_gsm.scale(f);
				_model.build(_gsm);
				break;
      case 8:	f=0.10f; 
				file = "../models/f-16.obj";
				_gsm.load( file );
				_gsm2.load("../models/blank.obj"); _gsm3.load("../models/blank.obj"); _gsm4.load("../models/blank.obj"); _gsm5.load("../models/blank.obj"); _gsm6.load("../models/blank.obj");
				_model.build(_gsm); _model2.build(_gsm2); _model3.build(_gsm3); _model4.build(_gsm4); _model5.build(_gsm5); _model6.build(_gsm6);
				std::cout << "Loading " << file << "...\n";
				printInfo(_gsm);
				_gsm.scale(f);
				_model.build(_gsm);
				break;
	  case 9:	f = .01f; 
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
      /*case 'o' : std::cout<<"Smoothing normals...\n";
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
	  case '1': curvegen = !curvegen; break;
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
	   sunx = 2 * (cos(2 * M_PI*sunxc / 360) + sin(2 * M_PI*sunxc / 360)); suny = 50.0f; sunz = 2 * (-sin(2 * M_PI*sunxz / 360) + cos(2 * M_PI*sunxz / 360));
   }
   else {
	   sunx = 0.0; suny = 1; sunz = -.5;
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf, barrelroll, leftright, transf, updown, rightwing, leftwing, offsety, centerrwing, centerlwing, rl, rr, backR, backL, centerbackl, centerbackr, br, bl;
   GsMat rfrot, lfrot, rbrot, lbrot, rollyawpitch, ShadowT, sunrot, camerarot, ctrans, ctrans2, ctrans3, ctrans4, frenet;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   camerarot = rx*ry; // set the scene transformation matrix

   if (curving) {
	   //ptrns *= -1;
	   ctrans.setrans(ptrns);
	   /*ctrans = oldtrans - ctrans;
	   std::cout << "ctrans:\n" << ctrans << std::endl;*/
	   frenet.setl1(ctangent.x, cbitangent.x, cnormal.x, 0);
	   frenet.setl2(ctangent.y, cbitangent.y, cnormal.y, 0);
	   frenet.setl3(ctangent.z, cbitangent.z, cnormal.z, 0);
	   //frenet.setl4(0, 0, 0, 1);
	   //frenet.setl4(ptrns);
   }
   else ctrans.setrans(GsVec(0, 0, 0));
   //ctrans2.roty(acos(dot(GsVec(0,0,1), cdiff)));
   //ctrans3.rotx(acos(dot(GsVec(1,0,0), cnormal)));
   //ctrans4.rotz(acos(dot(GsVec(0, 1, 0), cbitangent)));
   //std::cout << "ctrans:\n" << ctrans2 << std::endl << ctrans3 << std::endl;
   
   //set city in floor
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

	/*GsVec P = GsVec(0.0f, 0.0f, speed);
	frennet.setc1(cdiff);
	frennet.setc2(cbitangent);
	frennet.setc3(P);
	frennet.setc4(ptrns);*/
	//std::cout << "frennet:\n" << frennet << std::endl;

	//speed is fast
	//Translation matrix for pivot point
	GsVec P = GsVec(0.0f, 0.0f, speed);
	GsVec bd = leftright*updown*barrelroll*P;
	R = R + bd;
	transf.setrans(R);
	//transf *= ctrans;

	//rollyawpitch = ctrans3*ctrans2*leftright*updown*barrelroll;
	//stransf.setrans(bd);

	//GLfloat M[16] = { cdiff.x, cdiff.y, cdiff.z, 0, cbitangent.x, cbitangent.y, cbitangent.z, 0, cnormal.x, cnormal.y, cnormal.z, 0, R.x, R.y, R.z, 1 };
	//glLoadMatrixf(M);
	
	//set translations for shadows of airplane
	GsVec sbd = leftright*P;
	SR = SR + sbd;
	ShadowT.setrans(SR);

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, camview2, _birdseye, persp, sproj;
   
   //camera matrix setup
   GsVec eye(0,0,0), center(0,0,0), up(0,1,0);
   GsVec eye2(0, 10, 0), center2(0, 0, 0), up2(0, 0, 1);
   //set translation for the camera based on airplane
   eye += R + ctrans*leftright*updown*barrelroll*camerarot*GsVec(0,0,2);
   //center += R + GsVec(0, 0, 0);
   center = ctrans*cnormal;
   
   //Shadow matrix calculation
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
   
   //Curve generation
   if (curvegen) {
	   std::cout << "R: " << R << std::endl;
	   curvepoints.capacity(0);
	   controlpoints.push() = GsVec(rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-3)); controlpoints.push() = R;
	   for (int i = 0; i < 10; i++) {
		   float x = rand() % 20 + (-10);
		   float y = rand() % 20 + (-10);
		   float z = rand() % 20 + (-3);
		   controlpoints.push() = GsVec(x, y, z);
	   }
	   std::cout << "controlpoints: " << controlpoints << std::endl;
	   for (int j = 0; j < controlpoints.size() - 3; j++) {
		   GsArray<GsVec> pnts; pnts.push() = controlpoints[j]; pnts.push() = controlpoints[j + 1]; pnts.push() = controlpoints[j + 2]; pnts.push() = controlpoints[j + 3];
		   for (float i = 0; i <= 300; i++) {
				curvepoints.push() = bospline(i / (float)300, pnts);
				//c++;
		   }
	   }
	   oldtrans.setrans(curvepoints[0]);
	   _curve.build(curvepoints, GsColor::red);
	   //std::cout << "curvepoints: " << curvepoints << std::endl;
	   controlpoints.capacity(0);
	   curvegen = false;
	   curving = true;
   }


   /*if (curving) {
	   ctrans.setrans(ptrns);
   }
   else ctrans.setrans(GsVec(0, 0, 0));*/

   //fixed camera point
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix
   //camera follows airplane
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
   _tangent.build(_tan, GsColor::green);
   _normal.build(_norm, GsColor::yellow);
   _bitangent.build(_bit, GsColor::magenta);

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.
   float col = 1;

   // Draw:
	_model.draw(stransf*ctrans*frenet*transf*rollyawpitch, sproj, _light, 0);
	_model2.draw(stransf*ctrans*frenet*transf*rollyawpitch*rfrot, sproj, _light, 0);
	_model3.draw(stransf*ctrans*frenet*transf*rollyawpitch*lfrot, sproj, _light, 0);
	_model4.draw(stransf*ctrans*frenet*transf*rollyawpitch, sproj, _light, 0);
	_model5.draw(stransf*ctrans*frenet*transf*rollyawpitch*rbrot, sproj, _light, 0);
	_model6.draw(stransf*ctrans*frenet*transf*rollyawpitch*lbrot, sproj, _light, 0);
	_floor.draw(stransf, sproj, _light, textures);
	_city.draw(stransf*offsety, sproj, _light, 0);
	_city.draw(stransf*shadowMat*offsety, sproj, _shadow, 0);
	_curve.draw(stransf, sproj);
	_tangent.draw(stransf, sproj);
	_normal.draw(stransf, sproj);
	_bitangent.draw(stransf, sproj);
	//Shadows
	_model.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model2.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model3.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model4.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model5.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_model6.draw(stransf*ShadowT*shadowMat*rollyawpitch, sproj, _shadow, 1);
	_side.draw(stransf, sproj, _light, col, textures);
	_sun.draw(stransf * sunrot, sproj);

	oldtrans.setrans(ptrns);

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::glutIdle() 
{
	double curtime = gs_time();
	//Wing animation
	if (curtime - lasttime > .01f && animate) {
		if (_wingsflyR >= 45 || _wingsflyR <= -45) {
			_animinc *= -1;
		}
		//front wings
		_wingsflyR += _animinc;
		_wingsflyL += _animinc;
		//back wings
		_backR += _animinc;
		_backL += _animinc;
		lasttime = curtime;
		//wait until wings are back to zero to end animation
		if (resetanim == true) {
			if (_wingsflyR == 0) {
				animate = false;
			}
		}
		//std::cout << "_wingsfly: " << _wingsflyR << std::endl;
	}
	//Sun animation
	if (curtime - lasttime2 > .1f && sunanim) {
		sunxc += .1;
		sunxz += .1;
		lasttime2 = curtime;
		if (sunxc == 360) {
			sunxc = 0;
			sunxz = 0;
		}
	}
	//curving
	//std::cout << curvepoints << std::endl;
	if (curtime - lasttime3 > .01f && curving) {
		//std::cout << curvepoints[ccount + 1] << " " << curvepoints[ccount] << std::endl;
		cdiff = curvepoints[ccount + 1] - curvepoints[ccount];
		ctangent = cdiff; ctangent.normalize();
		
		ptrns = curvepoints[ccount] + ((float)ccount / (float)curvepoints.size())*ctangent;

		cbitangent = cross(cdiff, curvepoints[ccount] + curvepoints[ccount + 1]); cbitangent.normalize();
		cnormal = cross(cbitangent, ctangent); cnormal.normalize();

		//Draw tangent, normal, and bitangent lines
		_tan.capacity(0); _tan.push() = curvepoints[ccount]; _tan.push() = ctangent + curvepoints[ccount];
		_norm.capacity(0); _norm.push() = curvepoints[ccount]; _norm.push() = curvepoints[ccount] + cnormal;
		_bit.capacity(0); _bit.push() = curvepoints[ccount]; _bit.push() = curvepoints[ccount] + cbitangent;

		ccount++;
		if (ccount == curvepoints.size()) {
			ccount = 0;
			curving = false;
			curvepoints.capacity(0);
		}
		lasttime3 = curtime;
	}

	redraw();
}


