#pragma once

#include <ui/ui.h>
#include <QAction>
#include <QMainWindow>

struct Child
{
    void *data;
    void (*release)(void *data);
};

struct ui_window
{
    QMainWindow qwindow;
    QByteArray string;
    std::vector<Child> children;
};

struct ui_action : private QObject
{
    Q_OBJECT

public:
    ui_action(ui_window *parent)
        : qaction(new QAction(&parent->qwindow))
        , proc(NULL)
        , data(NULL)
        , parent(parent)
    {
        this->connect(qaction, SIGNAL(triggered()), this, SLOT(on_triggered()));
    }

public slots:
    void on_triggered()
    {
        if (proc)
        {
            proc(data);
        }
    }

public:
    QAction *qaction;
    ui_actionproc proc;
    void *data;
    ui_window *parent;
};

struct ui_actiongroup
{
    ui_actiongroup(ui_window *parent)
        : qactiongroup(new QActionGroup(&parent->qwindow))
        , parent(parent)
    {}

    QActionGroup *qactiongroup;
    ui_window *parent;
};
