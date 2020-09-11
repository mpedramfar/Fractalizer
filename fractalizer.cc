#include "fractalizer.h"
#include <iostream>
#include <cairomm/context.h>

int	WINDOW_WIDTH	= 1200,
	WINDOW_HEIGHT	= 700;


Fractalizer::Fractalizer()
:	drawing_box(Gtk::ORIENTATION_HORIZONTAL),
	control_box(Gtk::ORIENTATION_HORIZONTAL),
	main_box(Gtk::ORIENTATION_VERTICAL),
	mandel_frame(),
	julia_frame(),
	mandel_box(),
	julia_box(),
	mandel_canvas(0),
	julia_canvas(1),
	m_button("Zoom into the Mandelbrot set"),
	save_button("Save")
{

	set_title("Fractalizer");
	set_border_width(10);
	set_size_request(WINDOW_WIDTH, WINDOW_HEIGHT);


	mandel_box.add(mandel_canvas);
	julia_box.add(julia_canvas);

	mandel_box.set_events(Gdk::BUTTON_PRESS_MASK);
	mandel_box.signal_button_press_event().connect( sigc::mem_fun(*this, &Fractalizer::on_mandel_box_button_press) );

	julia_box.set_events(Gdk::BUTTON_PRESS_MASK);
	julia_box.signal_button_press_event().connect( sigc::mem_fun(*this, &Fractalizer::on_julia_box_button_press) );


	mandel_frame.set_label("Mandelbrot");
	julia_frame.set_label("Julia");
	mandel_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	julia_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	mandel_frame.add(mandel_box);
	julia_frame.add(julia_box);


	drawing_box.set_spacing(10);
	drawing_box.pack_start(mandel_frame);
	drawing_box.pack_start(julia_frame);

	control_box.set_spacing(20);
	control_box.pack_start(m_button);
	control_box.pack_start(save_button);

	m_button.signal_clicked().connect(sigc::mem_fun(*this, &Fractalizer::on_button_clicked));
	save_button.signal_clicked().connect(sigc::mem_fun(*this, &Fractalizer::on_save_button_clicked));


	main_box.set_spacing(20);
	main_box.pack_start(drawing_box);
	main_box.pack_end(control_box, false, 300);



	add(main_box);  

	show_all_children();
}

Fractalizer::~Fractalizer()
{
}

bool Fractalizer::on_mandel_box_button_press(GdkEventButton *ev)
{
	if(DrawingCanvas::is_zooming_to_mandel){
		mandel_canvas.zoom(ev->button == 1, ev->x, ev->y);
	}else{
		DrawingCanvas::julia_c = mandel_canvas.int_to_complex(ev->x, ev->y);
		julia_canvas.queue_draw();
	}
	return true;
}

bool Fractalizer::on_julia_box_button_press(GdkEventButton *ev)
{
	julia_canvas.zoom(ev->button == 1, ev->x, ev->y);
	return true;
}

void Fractalizer::on_save_button_clicked()
{
	Gtk::FileChooserDialog dialog("Save Mandelbrot :",
		Gtk::FILE_CHOOSER_ACTION_SAVE); 
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK); 

	Glib::RefPtr<Gtk::FileFilter> filter_png = Gtk::FileFilter::create();
	filter_png->set_name(".png files");
	filter_png->add_mime_type("image/png"); // PNG
	dialog.add_filter(filter_png);
	Glib::RefPtr<Gtk::FileFilter> filter_jpeg = Gtk::FileFilter::create();
	filter_jpeg->set_name(".jpeg files");
	filter_jpeg->add_mime_type("image/jpeg"); // JPEG
	dialog.add_filter(filter_jpeg);
	Glib::RefPtr<Gtk::FileFilter> filter_bmp = Gtk::FileFilter::create();
	filter_bmp->set_name(".bmp files");
	filter_bmp->add_mime_type("image/bmp"); // BMP
	dialog.add_filter(filter_bmp);
	Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);


	int result = dialog.run();
	if(result == Gtk::RESPONSE_OK){
		std::string filepath = dialog.get_filename();
		mandel_canvas.save_fractal(filepath);
	}
	
	dialog.set_title("Save Julia:");

	result = dialog.run();
	if(result == Gtk::RESPONSE_OK){
		std::string filepath = dialog.get_filename();
		julia_canvas.save_fractal(filepath);
	}

}

void Fractalizer::on_button_clicked()
{
	DrawingCanvas::is_zooming_to_mandel = m_button.get_active();
}

