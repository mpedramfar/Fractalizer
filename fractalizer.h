#ifndef FRACTALIZER_H
#define FRACTALIZER_H

#include "drawingcanvas.h"
#include <gtkmm.h>
//#include <gtkmm/window.h>
//#include <gtkmm/frame.h>
//#include <gtkmm/checkbutton.h>

class Fractalizer : public Gtk::Window
{

public:
	Fractalizer();
	virtual ~Fractalizer();

protected:  
	void on_button_clicked();
	void on_save_button_clicked();
	bool on_mandel_box_button_press(GdkEventButton* ev);
	bool on_julia_box_button_press(GdkEventButton* ev);

	Gtk::Box		drawing_box;
	Gtk::Box		control_box;
	Gtk::Box		main_box;

	Gtk::Frame		mandel_frame;
	Gtk::EventBox		mandel_box;
	DrawingCanvas		mandel_canvas;
  
	Gtk::Frame		julia_frame;
	Gtk::EventBox		julia_box;
	DrawingCanvas		julia_canvas;

	Gtk::CheckButton	m_button;
	Gtk::Button		save_button;
};


#endif // FRACTALIZER_H

