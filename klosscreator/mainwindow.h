#pragma once

#include <klosscreator/constrain.h>

struct buffer;
struct camera;
struct cornerref;
struct document;
struct mat4;
struct ray;
struct ui_glwidget;

typedef struct mainwindow mainwindow;

mainwindow *create_mainwindow(void);
void destroy_mainwindow(mainwindow *win);
void show_mainwindow(mainwindow *win);
struct document *get_document(mainwindow *win);
struct ui_glwidget *get_glwidget(mainwindow *win);
struct camera *get_camera(mainwindow *win);
struct ray make_mouseray(mainwindow const *win, float mousex, float mousey);
struct buffer *get_cursorvertices(mainwindow const *win);
constraint get_constrainalgorithm(mainwindow const *win);
bool pick_vertex(mainwindow const *win, float mousex, float mousey, struct cornerref *cornerref);
struct mat4 projectionmatrix(mainwindow const *win);
struct mat4 modelviewmatrix(mainwindow const *win);
