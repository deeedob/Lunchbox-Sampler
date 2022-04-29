#include "concrete_windowStates.hpp"

/* ######################## Fullscreen ########################### */

void WindowFullscreen::enter( Window *win... ) {

}

void WindowFullscreen::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowFullscreen::getInstance());
}

void WindowFullscreen::exit( Window *win... ) {
}

AbstractWindowState& WindowFullscreen::getInstance() {
    static WindowFullscreen singleton;
    return singleton;
}


bool WindowFullscreen::equals( const AbstractWindowState &b ) {
    if(const WindowFullscreen* a = dynamic_cast<const WindowFullscreen*>(&b)) {
        return true;
    }
    return false;
}

/* ######################## Vertical Split ########################### */

void WindowVertSplit::enter( Window *win... ) {

}

void WindowVertSplit::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowVertSplit::getInstance());
}

void WindowVertSplit::exit( Window *win... ) {
}

AbstractWindowState& WindowVertSplit::getInstance() {
    static WindowVertSplit singleton;
    return singleton;
}


bool WindowVertSplit::equals( const AbstractWindowState &b ) {
    if(const WindowVertSplit* a = dynamic_cast<const WindowVertSplit*>(&b)) {
        return true;
    }
    return false;
}

/* ######################## Horizontal Split ########################### */

void WindowHorizSplit::enter( Window *win... ) {
}

void WindowHorizSplit::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowHorizSplit::getInstance());
}

void WindowHorizSplit::exit( Window *win... ) {
}

AbstractWindowState& WindowHorizSplit::getInstance() {
    static WindowHorizSplit singleton;
    return singleton;
}

bool WindowHorizSplit::equals( const AbstractWindowState &b ) {
    if(const WindowHorizSplit* a = dynamic_cast<const WindowHorizSplit*>(&b)) {
        return true;
    }
    return false;
}
