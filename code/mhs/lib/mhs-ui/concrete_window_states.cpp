#include "concrete_window_states.hpp"
/* Fullscreen Mode for the Window*/
void DrawFullScreen::enter( Window *window ) {

}

/*Draw fullscreen window*/
void DrawFullScreen::drawWindow( Window *window ) {

}

void DrawFullScreen::exit( Window *window ) {

}

WindowState &DrawFullScreen::getInstance() {
    static DrawFullScreen singleton;
    return singleton;
}

/* Draw Vertical split window*/
void DrawVertSplit::enter( Window *window ) {

}

void DrawVertSplit::drawWindow( Window *window ) {

}

void DrawVertSplit::exit( Window *window ) {

}

WindowState &DrawVertSplit::getInstance() {
    static DrawVertSplit singleton;
    return singleton;
}

/* Draw horizontal split */
void DrawHorizSplit::enter( Window *window ) {

}

void DrawHorizSplit::drawWindow( Window *window ) {

}

void DrawHorizSplit::exit( Window *window ) {

}

WindowState &DrawHorizSplit::getInstance() {
    static DrawHorizSplit singleton;
    return singleton;
}