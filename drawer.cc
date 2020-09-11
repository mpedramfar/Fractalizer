#include "drawingcanvas.h"
#include <cairomm/context.h>
#include <cmath>
#include <complex>

using namespace std;



guint8 *DrawingCanvas::drawMandel(){
	guint8 *data = new guint8[height*width*3]; // Array of pixels, *3 for the 3 colors RGB
	int iter, index = 0;
	
	complex<double> z, c;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			c = int_to_complex(x, y);
			z = 0;			
			
			for(iter = 0; iter < MAX_ITERATION && norm(z) < 4; iter++){
				z = z*z + c;
			}

			data[index]   = palette[iter][0];
			data[index+1] = palette[iter][1];
			data[index+2] = palette[iter][2];
			index += 3;
		}
	}
	
	return data;
}

guint8 *DrawingCanvas::drawJulia(){
	guint8 *data = new guint8[height*width*3];
	int iter, index = 0;
	
	complex<double> z;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			z = int_to_complex(x, y);
			for(iter = 0; iter < MAX_ITERATION && norm(z) < 4; iter++){
				z = z*z + julia_c;
			}

			data[index]   = palette[iter][0];
			data[index+1] = palette[iter][1];
			data[index+2] = palette[iter][2];
			index += 3;
		}
	}
	
	bool draw_pc = false;
	if(!draw_pc)
		return data;

	z = 0;
	complex<int> p;
	for(int t = 0; t < 1000; t++){
		p = complex_to_int(z);
		index = 3 * (p.imag() * width + p.real());
		
		data[index]   = 0;
		data[index+1] = 255;
		data[index+2] = 0;
		
		z = z*z + julia_c;
	}


	return data;
}

