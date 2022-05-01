#include "concrete_windowStates.hpp"


std::pair<u_int8_t, u_int8_t> operator+( const std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs) {
    return {lhs.first+rhs.first, lhs.second+rhs.second};
}

std::pair<u_int8_t, u_int8_t>& operator+=( std::pair<u_int8_t,u_int8_t>& lhs, const std::pair<uint8_t, uint8_t>& rhs) {
    lhs = lhs + rhs;
    return lhs;
}

/* ######################## Fullscreen ########################### */

void WindowFullscreen::enter( Window *win... ) {
    win->clearDisplay();
    win->configs->setActiveWindow({{0,0},{win->width(), win->height()}});
}

void WindowFullscreen::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowFullscreen::getInstance());

    /* get optional arguments for window roundness and the iterations */
    va_list args;
    va_start(args, win);
    u_int8_t roundness = static_cast<u_int8_t>(va_arg(args, int));
    u_int8_t iterations = static_cast<u_int8_t>(va_arg(args, int));
    va_end(args);
    if (iterations <= 0) iterations = 1;

    /* get global window settings */
    std::pair<u_int8_t, u_int8_t> globPad = win->configs->getWinPadding();
    std::pair<std::pair<u_int8_t,u_int8_t>, std::pair<u_int8_t,u_int8_t>> activeWindow = win->configs->getActiveWindow();

    std::pair<u_int8_t, u_int8_t> _p = globPad;
    u_int8_t x0,y0,x1,y1;
    for(int i = 0; i < iterations; i++) {
        x0 = activeWindow.first.first + _p.first;
        y0 = activeWindow.first.second + _p.second;
        x1 = activeWindow.second.first - 2*_p.first;
        y1 = activeWindow.second.second - 2*_p.second;

        win->drawRoundRect( x0, y0, x1, y1,
                            roundness,
                            static_cast<uint16_t>(WindowConfigs::CLRS::CLR_ACTIVE)); // only one window to draw it active
                            _p += globPad;
    }
    this->setWindowArea( {{x0, y0},{x1, y1}});
    win->display(); // render to screen
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

const WindowFullscreen::winArea &WindowFullscreen::getWindowArea() const {
    return windowArea;
}

void WindowFullscreen::setWindowArea(const WindowFullscreen::winArea &windowArea ) {
    WindowFullscreen::windowArea = windowArea;
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
    win->clearDisplay();
    win->configs->setActiveWindow({{0,0},{win->width(), win->height()}});
}

void WindowHorizSplit::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowHorizSplit::getInstance());
    /* get optional arguments for window roundness and the iterations */
    va_list args;
    va_start(args, win);
    int roundness = va_arg(args, int);
    int iterations = va_arg(args, int);
    double splitValue = va_arg(args, double); //expected value from 0-1
    va_end(args);
    if (iterations <= 0) iterations = 1;
    if(roundness < 0) roundness = 0;
    if (splitValue <= 0) splitValue = 0.5;

    /* get global window settings */
    std::pair<u_int8_t, u_int8_t> globPad = win->configs->getWinPadding();
    std::pair<std::pair<u_int8_t,u_int8_t>, std::pair<u_int8_t,u_int8_t>> activeWindow = win->configs->getActiveWindow();

    u_int8_t split = static_cast<u_int8_t>(splitValue * win->width()); // the split value in px
    std::pair<u_int8_t, u_int8_t> _p = globPad;
    u_int8_t win1x0,win1y0, win1Width, winHeight,win2x0, win2y0, win2Width;
    for(int i = 0; i < iterations; i++) {
        win1x0 = activeWindow.first.first + _p.first;
        win1y0 = activeWindow.first.second + _p.second;
        win1Width = split - static_cast<u_int8_t>(1.5f * static_cast<float>(_p.first));

        win2x0 = split + static_cast<u_int8_t>(0.5f*static_cast<float>(_p.first));
        win2y0 = win1y0;
        win2Width = win->width() - split - static_cast<u_int8_t>(1.5f * static_cast<float>(_p.first));

        winHeight = activeWindow.second.second - 2 * _p.second;

        win->drawRoundRect(win1x0, win1y0, win1Width, winHeight,
                           roundness,
                           static_cast<uint16_t>(WindowConfigs::CLRS::CLR_ACTIVE)); // winLeft

        win->drawRoundRect(win2x0, win2y0, win2Width, winHeight,
                           roundness,
                           static_cast<uint16_t>(WindowConfigs::CLRS::CLR_PASSIVE)); // winLeft

        _p += globPad;
    }
    //this->setWindowArea( {{x0, y0},{x1, y1}});
    this->setWindowAreaLeft({{win1x0, win1y0},{ win1Width, winHeight}});
    this->setWindowAreaRight({{win2x0, win2y0},{ win->width() - _p.first, winHeight}});
    win->display(); // render to screen
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

const WindowHorizSplit::winArea &WindowHorizSplit::getWindowAreaLeft() const {
    return windowAreaLeft;
}

void WindowHorizSplit::setWindowAreaLeft(const WindowHorizSplit::winArea &windowAreaLeft ) {
    WindowHorizSplit::windowAreaLeft = windowAreaLeft;
}

const WindowHorizSplit::winArea &WindowHorizSplit::getWindowAreaRight() const {
    return windowAreaRight;
}

void WindowHorizSplit::setWindowAreaRight(const WindowHorizSplit::winArea &windowAreaRight ) {
    WindowHorizSplit::windowAreaRight = windowAreaRight;
}
