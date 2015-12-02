/*=======================================================================
Copyright 2007 Marcelo Kallmann. All Rights Reserved.
This software is distributed for noncommercial use only, without
any warranties, and provided that all copies contain the full copyright
notice licence.txt located at the base folder of the distribution.
=======================================================================*/

# include <string.h>
# include <stdio.h>
# include <iostream>
# include "gs_image.h"

# include "ogl_tools.h"

using namespace std;

# define TRACE  // comment this to hide messages

# ifdef TRACE
# define GS_TRACE2(msg) cout<<msg<<endl
# else
# define GS_TRACE2(msg)
# endif

//=========================== GsImage ============================

GsImage::GsImage()
{
	_w = _h = 0;
	_img = 0;
}

GsImage::~GsImage()
{
	init(0,0);
}

void GsImage::init(int w, int h)
{	
	delete[] _img;
	
	if (w <= 0 || h <= 0) w = h = 0;
	int s = w*h;
	
	if (s){
		_img = new GsColor[s];
	}else{
		_img = NULL;
	}
	
	_w = w;
	_h = h;
}

# define PutInt(i)   fwrite(&i,4/*bytes*/,1/*num items*/,f)
# define PutShort(s) fwrite(&s,2/*bytes*/,1/*num items*/,f)

bool GsImage::save(const char* filename)
{
	FILE* f = fopen(filename, "wb");
	if (!f) return false;

	int i = 0;
	int offset = 14 + 40;
	//int dw = 4-(_w%4); if ( dw==4 ) dw=0;
	int dw = (_w % 4);

	int filesize = 14 /*header*/ + 40 /*info*/ + (_w*_h * 3) + (_h*dw);

	// 14 bytes of header:
	fprintf(f, "BM"); // 2 bytes : signature
	PutInt(filesize); // file size
	PutInt(i);        // reserved (zeros)
	PutInt(offset);   // offset to the data

	// 40 bytes of info header:
	int infosize = 40;
	short planes = 1;
	short bits = 24;
	int compression = 0; // no compression
	int compsize = 0;    // no compression
	int hres = 600;
	int vres = 600;
	int colors = 0;
	int impcolors = 0;   // important colors: all
	PutInt(infosize); // size of info header
	PutInt(_w);       // width
	PutInt(_h);       // height
	PutShort(planes);
	PutShort(bits);
	PutInt(compression);
	PutInt(compsize);
	PutInt(hres);
	PutInt(vres);
	PutInt(colors);
	PutInt(impcolors);

	int w, h;
	GsColor* pixel;
	for (h = _h - 1; h >= 0; h--)
	{
		pixel = line(h);
		for (w = 0; w < _w; w++)
		{
			fputc(pixel->b, f); // B
			fputc(pixel->g, f); // G
			fputc(pixel->r, f); // R
			pixel++;
		}
		for (w = 0; w < dw; w++) fputc(0, f);
	}

	fclose(f);
	return true;
}

# define GETC  fgetc(f)
# define FREAD (buf,size) fread(buf,1,size,f)
# define FSEEK (offset) fseek(f,offset,SEEK_SET)

static unsigned short read_word(FILE* f) // read 16-bit unsigned integer
{
	gsbyte b0 = GETC;
	gsbyte b1 = GETC;
	return ((b1 << 8) | b0);
}

static unsigned int read_dword(FILE* f) // read 32-bit unsigned integer
{
	gsbyte b0 = GETC;
	gsbyte b1 = GETC;
	gsbyte b2 = GETC;
	gsbyte b3 = GETC;
	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

typedef unsigned int U32;
bool GsImage::load(const char* filename)
{
	GS_TRACE2("Loading " << filename << ":");
	union PalColor { U32 u; gsbyte c[3]; };

	FILE* f = fopen(filename, "rb");
	if (!f) { return false; }

	unsigned  bitsperpixel,
		depth = 3,	// bytes per output pixel
		compression,	// Type of compression
		colorsused,	// Number of colors used
		repcount,     // Number of times to repeat
		temp, 		// Temporary color
		align,		// Alignment bytes
		datasize;	    // number of bytes in image data set

	bool		flip = false;	// if true, image is top-to-bottom
	PalColor	*colormap;      // Colormap
	bool		mask = false;	// single bit mask follows image data
	bool		use565 = false; // Use 565 instead of 555 for 16-bit data

	// Get the header
	gsbyte byte = GETC; // Check "BM" sync chars
	gsbyte bit = GETC;
	if (byte != 'B' || bit != 'M') { fclose(f); return false; }

	read_dword(f);	// Skip size
	read_word(f);	// Skip reserved stuff
	read_word(f);
	unsigned offbits = read_dword(f);   // Read offset to image data
	unsigned info_size = read_dword(f); // The bitmap information

	GS_TRACE2("offbits:" << offbits << " info_size:" << info_size);

	if (info_size < 40)
	{ // Old Windows/OS2 BMP header, unclear if negative numbers or numbers > 32767 are correct:
		int w = read_word(f); //if (w&0x8000) w = (w^0xffff)+1;
		int h = read_word(f); //if (h&0x8000) {h = (h^0xffff)+1; flip=true;}
		init(w, h);
		read_word(f);
		bitsperpixel = read_word(f);
		compression = 0;
		colorsused = 0;
		repcount = info_size - 12;
	}
	else
	{ // New BMP header, must take absolute value:
		unsigned w = read_dword(f); if (w & 0x80000000) w = (w ^ 0xffffffff) + 1;
		unsigned h = read_dword(f); if (h & 0x80000000) { h = (h ^ 0xffffffff) + 1; flip = true; }
		read_word(f);
		bitsperpixel = read_word(f);
		compression = read_dword(f);
		datasize = read_dword(f);
		read_dword(f);
		read_dword(f);
		colorsused = read_dword(f);
		read_dword(f);
		repcount = info_size - 40;
		if (!compression && bitsperpixel >= 8 && w > 32 / bitsperpixel)
		{
			unsigned maskSize = (((w*(bitsperpixel / 8) + 3)&~3)*h) + (((((w + 7) / 8) + 3)&~3)*h);
			if (maskSize == 2 * datasize)
			{
				mask = true; h = (h / 2); depth = 4;
			}
		}
		init(w, h);
	}

	GS_TRACE2("depth:" << depth);
	GS_TRACE2("size w:" << w() << " h:" << h());
	GS_TRACE2("bitsperpixel:" << bitsperpixel << ' ' << "compression:" << compression << ' ' << "colorsused:" << colorsused << ' ' << "repcount:" << repcount);

	// Skip remaining header bytes
	while (repcount > 0) { GETC; repcount--; }

	// Check header data
	if (!w() || !h() || !bitsperpixel) { fclose(f); return false; }

	// Get colormap
	if (colorsused == 0 && bitsperpixel <= 8) colorsused = 1 << bitsperpixel;
	colormap = 0;
	if (bitsperpixel != 24) colormap = (PalColor*) new U32[256];

	// Read BGR color
	for (repcount = 0; repcount<colorsused; repcount++)
	{
		if (fread(&colormap[repcount], 1, 3, f) != 3)
			return false;
		if (info_size>12) GETC; // Skip pad byte for new BMP files...
	}

	// Read first dword of colormap. It tells us if 5:5:5 or 5:6:5 for 16 bit
	if (bitsperpixel == 16) use565 = (read_dword(f) == 0xf800);

	// Setup image and buffers
	if (offbits) fseek(f, offbits, SEEK_SET);

	gsbyte *array = &_img[0].r;

	// Read the image data

	//int color = 0;
	repcount = 0;
	align = 0;
	temp = 0;

	for (int y = 0; y<h(); y++)
	{ //gsbyte* ptr = array + (flip ? y : h()-y-1) * w() * depth;
		gsbyte* ptr = array + (flip ? y : h() - y - 1) * w() * 4; // GsImage has fixed depth of 4
		int x;
		switch (bitsperpixel)
		{
		case 1: // Bitmap
			for (x = w(), bit = 128; x>0; x--)
			{
				if (bit == 128) byte = (gsbyte)GETC;
				if (byte & bit)
				{
					*ptr++ = colormap[1].c[2];
					*ptr++ = colormap[1].c[1];
					*ptr++ = colormap[1].c[0];
				}
				else
				{
					*ptr++ = colormap[0].c[2];
					*ptr++ = colormap[0].c[1];
					*ptr++ = colormap[0].c[0];
				}
				*ptr++ = 1;
				if (bit > 1) bit >>= 1; else bit = 128;
			}
			for (temp = w() * 3; temp & 3; temp++) { GETC; }
			break;

		case 24: // 24-bit RGB
			for (x = w(); x > 0; x--, ptr += 4)
			{
				ptr[2] = GETC;
				ptr[1] = GETC;
				ptr[0] = GETC;
				ptr[3] = 255;
			}
			// Read remaining bytes to align to 32 bits
			for (temp = w() * 3; temp & 3; temp++) { GETC; }
			break;

		default:
			{ cout << "Not tested yet bmps==%d bits per pixel. Not loaded.\n";
			delete[] colormap;
			fclose(f);
			return false;
			   }

		}
	}

	if (mask)
	{
		for (int y = 0; y<h(); y++)
		{
			gsbyte* ptr = array + (flip ? y : h() - y - 1) * w() * depth + 3;
			for (int x = w(), bit = 128; x > 0; x--, ptr += 4)
			{
				if (bit == 128) byte = (gsbyte)GETC;
				if (byte & bit) *ptr = 0; else *ptr = 255;
				if (bit > 1) bit >>= 1; else bit = 128;
			}
			// Read remaining bytes to align to 32 bits
			for (temp = (w() + 7) / 8; temp & 3; temp++) GETC;
		}
	}

	// Close the file and return
	fclose(f);
	delete[] colormap;
	GS_TRACE2("Loaded.");

	return true;
}

