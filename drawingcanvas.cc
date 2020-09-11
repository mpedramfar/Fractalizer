#include "drawingcanvas.h"
#include <cairomm/context.h>
#include <cmath>
#include <complex>

using namespace std;

bool		DrawingCanvas::is_zooming_to_mandel;
complex<double>	DrawingCanvas::julia_c;
int		DrawingCanvas::palette[TOP_ITERATION][3];



void DrawingCanvas::getColor(int iter, long double *R, long double *G, long double *B)
{
	int colorScheme = 4;
	switch (colorScheme)
	{
		case 1: // default rainbow scheme, uses a sine function
			*R = 127.5*sin(((TOP_ITERATION/600)*M_PI/MAX_ITERATION)*iter)+127.5;
			*G = 127.5*sin(((TOP_ITERATION/600)*M_PI/MAX_ITERATION)*iter+1)+127.5;
			*B = 127.5*sin(((TOP_ITERATION/600)*M_PI/MAX_ITERATION)*iter+2)+127.5;
			break;
		case 2: // clear scheme, is a linear function
			*R = 255.0*iter/(1.0*(MAX_ITERATION));
			*G = 255.0*iter/(1.6*(MAX_ITERATION));
			*B = 255.0*iter/(6*(MAX_ITERATION));
			break;
		case 3: // valentime scheme, is a sinusoidal logarithmic function
			*R = 127.5*sin(0.15*(log(iter)+0.5)) + 127.5;
			*G = 127.5*sin(0.15*iter+0.8) + 127.5;
			*B = 127.5*sin(0.15*iter+1.0) + 127.5;
			break;
		case 4:
			//MAX_ITERATION == 50
			if(iter <= 1){
				*R = 255;
				*G = 255;
				*B = 255;
			}else if(iter <= 10){
				*R = ((10 - iter) / 8.0) * 255;
				*G = ((10 - iter) / 8.0) * 255;
				*B = 255;
			}else if(iter <= 30){
				*R = (iter - 10) / 20.0 * 255;
				*G = 0;
				*B = (30 - iter) / 20.0 * 255;
			}else{
				*R = (50 - iter) / 20.0 * 255;
				*G = (iter - 30) / 20.0 * 255;
				*B = 0;
			};
			//*R = 255 - *R;
			//*G = 255 - *G;
			//*B = 255 - *B;
	}
	if ((iter==MAX_ITERATION)&&(colorScheme==1)) {
		*R = 0; // sets the negative space to black
		*G = 0;
		*B = 0;
	}
	if ((iter==MAX_ITERATION)&&(colorScheme==3)) {
		*R = 255; // sets negative space to white
		*G = 255;
		*B = 255;
	}
}


complex<double> DrawingCanvas::int_to_complex(int x, int y){
	double x1 = (x - width/2) * real_width/width;
	double y1 = (height/2 - y) * real_width/width;
	return complex<double> (x1,y1) + real_center;
}


complex<double> DrawingCanvas::int_to_complex(complex<int> p){
	return int_to_complex(p.real(),p.imag());
}

complex<int> DrawingCanvas::complex_to_int(complex<double> p){
	p -= real_center;
	int x = p.real() * width/real_width + width/2;
	int y = height/2 - p.imag() * width/real_width;
	return complex<int> (x,y);
}

void DrawingCanvas::zoom(bool zoom_in, int x, int y){
	double f = 1.5; //zooming factor
	complex<double> p = int_to_complex(x, y);
	if(zoom_in){
		real_width /= f;
		real_height /= f;
		real_center = real_center / f + p * (1.0 - 1.0/f);
	}else{
		real_width *= f;
		real_height *= f;
		real_center = real_center * f - p * (f - 1);
	}
	queue_draw();
}

void DrawingCanvas::save_fractal(std::string filepath)
/* This function saves the current pixbuf stored in "image", triggered by Save button */
{
	std::string filetype = "png"; // default type is .png
	std::string tempType = filepath.substr(filepath.find_last_of(".")+1);
	if (tempType == "png" || tempType == "jpeg" || tempType == "bmp") {
		filetype = tempType;
	}
	image->save(filepath,filetype);
}


void DrawingCanvas::update_size(){
	Gtk::Allocation allocation = get_allocation();
	width = allocation.get_width();
	height = allocation.get_height();

	real_height = real_width/width * height;
}


DrawingCanvas::DrawingCanvas(int id)
{
	real_center = 0;
	real_width = 4;
	update_size();

	julia_c = -1;

	this->id = id;

	MAX_ITERATION = 50;

	long double r, g, b;
	for(int i = 0; i < MAX_ITERATION; i++){
		getColor(i, &r, &g, &b);
		palette[i][0] = r;
		palette[i][1] = g;
		palette[i][2] = b;
	}
}

DrawingCanvas::~DrawingCanvas(){
}

bool DrawingCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	update_size();

	// RGB buffer. Just an array that's [R,G,B,...] each 8-bit
	guint8 *data;
	data = (id == 0 ? drawMandel() : drawJulia());


	// Rendering the RGB buffer:
	Gdk::Colorspace colorspace = Gdk::COLORSPACE_RGB; // creates the RGB colorspace
	image = Gdk::Pixbuf::create_from_data(data, colorspace, false, 8, width, height, width*3);
	Gdk::Cairo::set_source_pixbuf(cr, image, 0, 0);
	cr->rectangle(0,0,width,height);
	cr->fill();

	return true;
}
