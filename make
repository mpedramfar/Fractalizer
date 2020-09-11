#!/bin/bash

g++ main.cc fractalizer.cc drawingcanvas.cc drawer.cc -o Fractalizer `pkg-config gtkmm-3.0 --cflags --libs`
