# include "so_capsule.h"

SoCapsule::SoCapsule()
{
	_numpoints = 0;
}

void SoCapsule::init()
{
	// Build program:
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../shaders/vsh_mcol_flat.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/fsh_flat.glsl");
	_prog.init_and_link(_vsh, _fsh);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	_prog.uniform_locations(2); // will send 2 variables: the 2 matrices below
	_prog.uniform_location(0, "vTransf");
	_prog.uniform_location(1, "vProj");
}

void SoCapsule::build(float r, float sunx, float suny, float sunz)
{

	P.clear(); C.clear(); // set size to zero, just in case
	//P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below
								  //sphere
	int nfaces = 20;
	double rt = 5;
	double rb = 5;
	double len = 0;
	double pi = 3.14159265358979323846;
	for (int i = 0; i < nfaces; i++) //for sphere
	{
		for (int j = 0; j < nfaces; j++)
		{
			P.push_back(GsVec(rt*cos(j * 2 * pi / nfaces)*cos(i*pi / (2 * nfaces)) + sunx, rt*sin(i*pi / (2 * nfaces)) + len + suny, rt*sin(j * 2 * pi / nfaces)*cos(i*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rt*cos((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunx, rt*sin((i)*pi / (2 * nfaces)) + len + suny, rt*sin((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rt*cos((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, rt*sin((i + 1)*pi / (2 * nfaces)) + len + suny, rt*sin((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			C.push_back(GsColor::orange); C.push_back(GsColor::orange); C.push_back(GsColor::orange);

			P.push_back(GsVec(rt*cos((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, rt*sin((i + 1)*pi / (2 * nfaces)) + len + suny, rt*sin((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rt*cos((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunx, rt*sin((i)*pi / (2 * nfaces)) + len + suny, rt*sin((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rt*cos((j+1)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, rt*sin((i + 1)*pi / (2 * nfaces)) + len + suny, rt*sin((j+1)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			C.push_back(GsColor::orange); C.push_back(GsColor::orange); C.push_back(GsColor::orange);

			P.push_back(GsVec(rb*cos(j * 2 * pi / nfaces)*cos(i*pi / (2 * nfaces)) + sunx, -1 * rb*sin(i*pi / (2 * nfaces)) - len + suny, rb*sin(j * 2 * pi / nfaces)*cos(i*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rb*cos((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunx, -1 * rb*sin((i)*pi / (2 * nfaces)) - len + suny, rb*sin((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rb*cos((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, -1 * rb*sin((i + 1)*pi / (2 * nfaces)) - len + suny, rb*sin((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			C.push_back(GsColor::orange); C.push_back(GsColor::orange); C.push_back(GsColor::orange);

			P.push_back(GsVec(rb*cos((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, -1 * rb*sin((i + 1)*pi / (2 * nfaces)) - len + suny, rb*sin((j)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rb*cos((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunx, -1 * rb*sin((i)*pi / (2 * nfaces)) - len + suny, rb*sin((j + 1) * 2 * pi / nfaces)*cos((i)*pi / (2 * nfaces)) + sunz));
			P.push_back(GsVec(rb*cos((j+1)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunx, -1 * rb*sin((i + 1)*pi / (2 * nfaces)) - len + suny, rb*sin((j+1)* 2 * pi / nfaces)*cos((i + 1)*pi / (2 * nfaces)) + sunz));
			C.push_back(GsColor::orange); C.push_back(GsColor::orange); C.push_back(GsColor::orange);
		}
	}
	
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);
	

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0);
}

void SoCapsule::draw(GsMat& tr, GsMat& pr)
{
	// Draw Lines:
	glUseProgram(_prog.id);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}

