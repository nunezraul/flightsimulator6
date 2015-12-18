# include "so_textured_tube.h"

SoTexturedTube::SoTexturedTube()
{
	_numpoints = 0;
}

void SoTexturedTube::init(const char *file, GLuint *textures)
{

	// Build program:
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../shaders/vsh_smtl_tex_gouraud.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/fsh_tex_gouraud.glsl");
	_prog.init_and_link(_vsh, _fsh);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use at least 1 vertex array
	gen_buffers(2);       // will use at least 1 buffer
	_prog.uniform_locations(11); // declare here uniforms
								 //_prog.uniform_location ( 0, "vTransf" ); // each name must appear in the shader
								 //_prog.uniform_location ( 1, "vProj" );
								 //...
	_prog.uniform_location(0, "vTransf");
	_prog.uniform_location(1, "vProj");
	_prog.uniform_location(2, "lPos");
	_prog.uniform_location(3, "la");
	_prog.uniform_location(4, "ld");
	_prog.uniform_location(5, "ls");
	_prog.uniform_location(6, "ka");
	_prog.uniform_location(7, "kd");
	_prog.uniform_location(8, "ks");
	_prog.uniform_location(9, "sh");
	_prog.uniform_location(10, "Tex1");



	GsImage I;
	if (!I.load("../texture/backtoback.bmp"))
		std::cout << "COULD NOT LOAD IMAGE!\n";
	else
		std::cout << "loaded\n";
	//glGenTextures(2, texture); // ids start at 1
	glBindTexture(GL_TEXTURE_2D, *textures);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	I.init(0, 0); // free image from CPU 


}

void SoTexturedTube::build(float len, float rt, float rb, float nfaces, GLuint * texture)
{
	float pi = 3.14159265358979323846;
	//int i;

	P.clear(); N.clear(); T.clear();// set size to zero, just in case
									//P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

									//Drawing side cylinder from triangles with smooth shading

	for (float i = 0; i < nfaces; i++)
	{
		//Side triangles
		P.push_back(GsVec(cos(i * 2 * pi / nfaces) * rt, len, sin(i * 2 * pi / nfaces) *rt));
		N.push_back(GsVec(cos(i * 2 * pi / nfaces), 0, sin(i * 2 * pi / nfaces)));
		T.push_back(GsVec2((i / nfaces) / 2.f, 0.0f));

		P.push_back(GsVec(cos(i * 2 * pi / nfaces) * rb, -len, sin(i * 2 * pi / nfaces) *rb));
		N.push_back(GsVec(cos(i * 2 * pi / nfaces), 0, sin(i * 2 * pi / nfaces)));
		T.push_back(GsVec2((i / nfaces) / 2.f, 0.5f));

		P.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces) * rb, -len, sin((i + 1) * 2 * pi / nfaces) *rb));
		N.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces), 0, sin((i + 1) * 2 * pi / nfaces)));
		T.push_back(GsVec2(((i + 1) / nfaces) / 2.f, 0.5f));

		P.push_back(GsVec(cos(i * 2 * pi / nfaces) * rt, len, sin(i * 2 * pi / nfaces) *rt));
		N.push_back(GsVec(cos(i * 2 * pi / nfaces), 0, sin(i * 2 * pi / nfaces)));
		T.push_back(GsVec2((i / nfaces) / 2.f, 0.0f));

		P.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces) * rt, len, sin((i + 1) * 2 * pi / nfaces) *rt));
		N.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces), 0, sin((i + 1) * 2 * pi / nfaces)));
		T.push_back(GsVec2(((i + 1) / nfaces) / 2.f, 0.0f));

		P.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces) * rb, -len, sin((i + 1) * 2 * pi / nfaces) *rb));
		N.push_back(GsVec(cos((i + 1) * 2 * pi / nfaces), 0, sin((i + 1) * 2 * pi / nfaces)));
		T.push_back(GsVec2(((i + 1) / nfaces) / 2.f, 0.5f));
	}


	P.push_back(GsVec(-25.f, -5.0f, -25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0.5f, 0.0f));

	P.push_back(GsVec(-25.f, -5.0f, 25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0.5f, 1.0f));

	P.push_back(GsVec(25.f, -5.0f, 25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1.0f, 1.0f));

	P.push_back(GsVec(25.f, -5.0f, -25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1.0f, 0.0f));

	P.push_back(GsVec(-25.f, -5.0f, -25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0.5f, 0.0f));

	P.push_back(GsVec(25.f, -5.0f, 25.f));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1.0f, 1.0f));

	glBindVertexArray(va[1]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnableVertexAttribArray(0); // for each buffer there will be one vertex atribute
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), &P[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // tell what 

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // texture
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization

	glBindVertexArray(0);
	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); N.resize(0); T.resize(0);
}

void SoTexturedTube::draw(GsMat& tr, GsMat& pr, const GsLight &l, float &col, GLuint *texture)
{
	Material m(GsColor::white, GsColor::white, GsColor::red, 1.0);

	glUseProgram(_prog.id);
	glBindVertexArray(va[1]);

	glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);
	float f[4]; // we convert below our color values to be in [0,1]  
	glUniform3fv(_prog.uniloc[2], 1, l.pos.e);
	glUniform4fv(_prog.uniloc[3], 1, l.amb.get(f));
	glUniform4fv(_prog.uniloc[4], 1, l.dif.get(f));
	glUniform4fv(_prog.uniloc[5], 1, l.spe.get(f));
	glUniform4fv(_prog.uniloc[6], 1, m.amb.get(f));
	glUniform4fv(_prog.uniloc[7], 1, m.dif.get(f));
	glUniform4fv(_prog.uniloc[8], 1, m.spe.get(f));
	glUniform1fv(_prog.uniloc[9], 1, &m.sh);

	glBindVertexArray(va[1]);
	// call a draw function for the arrays here
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.

}

