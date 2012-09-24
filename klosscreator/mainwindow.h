#pragma once

#include <klosscreator/constrain.h>

struct Buffer;
struct Camera;
struct CornerRef;
struct Document;
struct Mat4;
struct Ray;
struct ui_glwidget;

typedef struct mainwindow mainwindow;

mainwindow *create_mainwindow(void);
void destroy_mainwindow(mainwindow *win);
void show_mainwindow(mainwindow *win);
struct Document *get_document(mainwindow *win);
struct ui_glwidget *get_glwidget(mainwindow *win);
struct Camera *get_camera(mainwindow *win);
struct Ray make_mouseray(mainwindow const *win, float mousex, float mousey);
struct Buffer *get_cursorvertices(mainwindow const *win);
ConstrainAlgorithm get_constrainalgorithm(mainwindow const *win);
bool pick_vertex(mainwindow const *win, float mousex, float mousey, struct CornerRef *cornerref);
struct Mat4 projectionmatrix(mainwindow const *win);
struct Mat4 modelviewmatrix(mainwindow const *win);
