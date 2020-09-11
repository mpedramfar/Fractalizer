#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H


#include <gtkmm.h>
#include <cairomm/context.h>
#include <cmath>
#include <complex>


#define TOP_ITERATION 200


using namespace std;

class DrawingCanvas : public Gtk::DrawingArea
{
public:
	DrawingCanvas(int);
	virtual ~DrawingCanvas();

	static bool			is_zooming_to_mandel;
	static complex<double>		julia_c;
	static int 			palette[TOP_ITERATION][3];

	complex<double>			int_to_complex(int x, int y);
	complex<double>			int_to_complex(complex<int>);
	complex<int>			complex_to_int(complex<double>);

	void				zoom(bool zoom_in, int x, int y);
	void				save_fractal(std::string filepath);
	void				getColor(int iter, long double *R, long double *G, long double *B);

protected:
	int				id;
	int				MAX_ITERATION;
	Glib::RefPtr<Gdk::Pixbuf>	image;

	complex<double>			real_center;
	double				real_height;
	double				real_width;	
	int				height;
	int				width;


	virtual bool	on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	guint8 		*drawMandel();
	guint8		*drawJulia();

	void		update_size();
};

#endif
