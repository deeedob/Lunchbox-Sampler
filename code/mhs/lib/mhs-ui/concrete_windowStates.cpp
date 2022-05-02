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

void WindowFullscreen::setWindowArea(const WindowFullscreen::winArea &windowArea ) {
    WindowFullscreen::windowArea = windowArea;
}

AbstractWindowState::winArea WindowFullscreen::getWindowTextArea( bool b ) const {
    return windowArea;
}

/* ######################## Vertical Split ########################### */

void WindowVertSplit::enter( Window *win... ) {

}

void WindowVertSplit::draw( Window *win... ) {
    /* if delegating */
    //win->setState(WindowVertSplit::getInstance());
    va_list args;
    va_start(args, win);
    int roundness = va_arg(args, int);
    int iterations = va_arg(args, int);
    double splitValue = va_arg(args, double); //expected value from 0-1
    va_end(args);

    if (iterations <= 0) iterations = 1;
    if(roundness < 0) roundness = 0;
    if (splitValue <= 0.0 || splitValue > 1.0) splitValue = 0.5;

    /* get global window settings */
    std::pair<u_int8_t, u_int8_t> globPad = win->configs->getWinPadding();
    std::pair<std::pair<u_int8_t,u_int8_t>, std::pair<u_int8_t,u_int8_t>> activeWindow = win->configs->getActiveWindow();

    u_int8_t split = static_cast<u_int8_t>(splitValue * win->height()); // the split value in px
    int win1x0, win1y0, win1Width, win1Height,win2x0, win2y0, win2Width, win2Height;

    auto checkRange = [&]() {
        if(win1Height < 0) {
            win1x0 = win1y0 = win1Width = win1Height = 0;
        }
        if(win2Height < 0) {
            win2x0 = win2y0 = win2Width = win2Height = 0;
        }
    };
    /* init base window*/
    win1x0 = activeWindow.first.first + globPad.first;
    win1y0 = activeWindow.first.second + globPad.second;
    win1Width = activeWindow.second.first - 2 * globPad.first;
    win1Height = split - static_cast<u_int8_t>(1.5f*static_cast<float>(globPad.second));

    win2x0 = win1x0;
    win2y0 = split + static_cast<u_int8_t>(0.5f * static_cast<float>(globPad.second));
    win2Width = activeWindow.second.first - 2 * globPad.first;
    win2Height = activeWindow.second.second - split - static_cast<u_int8_t>(1.5f * static_cast<float>(globPad.second));

    checkRange();
    for(int i = 0; i < iterations; i++) {

        win->drawRoundRect(win1x0, win1y0, win1Width, win1Height,
                           roundness,
                           static_cast<uint8_t>(WindowConfigs::getActiveWinClrs().first)); // winUp

        win->drawRoundRect(win2x0, win2y0, win2Width, win2Height,
                           roundness,
                           static_cast<uint8_t>(WindowConfigs::getActiveWinClrs().second)); // winDown

        win1x0 += globPad.first;
        win1y0 += globPad.second;
        win1Width -= 2*globPad.first;
        win1Height -= 2*globPad.second;

        win2x0 += globPad.first;
        win2y0 += globPad.second;
        win2Width -= 2*globPad.first;
        win2Height -= 2*globPad.second;

        checkRange();
    }
    this->setWindowAreaUp({{win1x0, win1y0},{ win1Width, win1Height}});
    this->setWindowAreaDown({{win2x0, win2y0},{ win2Width, win2Height}});
    win->display(); // render to screen
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

WindowVertSplit::winArea WindowVertSplit::getWindowTextArea( bool b ) const {
    return b ? windowAreaDown : windowAreaUp;
}

void WindowVertSplit::setWindowAreaUp(
        const WindowVertSplit::winArea &windowAreaUp ) {
    WindowVertSplit::windowAreaUp = windowAreaUp;
}

void WindowVertSplit::setWindowAreaDown(
        const WindowVertSplit::winArea &windowAreaDown ) {
    WindowVertSplit::windowAreaDown = windowAreaDown;
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
    if (splitValue <= 0.0 || splitValue > 1.0) splitValue = 0.5;

    /* get global window settings */
    std::pair<u_int8_t, u_int8_t> globPad = win->configs->getWinPadding();
    std::pair<std::pair<u_int8_t,u_int8_t>, std::pair<u_int8_t,u_int8_t>> activeWindow = win->configs->getActiveWindow();

    u_int8_t split = static_cast<u_int8_t>(splitValue * win->width()); // the split value in px
    int win1x0, win1y0, win1Width, win1Height,win2x0, win2y0, win2Width, win2Height;

    auto checkRange = [&]() {
        if(win1Width < 0) {
            win1x0 = win1y0 = win1Width = win1Height = 0;
        }
        if(win2Width < 0) {
            win2x0 = win2y0 = win2Width = win2Height = 0;
        }
    };
    /* init base window*/
    win1x0 = activeWindow.first.first + globPad.first;
    win1y0 = activeWindow.first.second + globPad.second;
    win1Width = split - static_cast<u_int8_t>(1.5f*static_cast<float>(globPad.first));
    win1Height = activeWindow.second.second - 2 * globPad.second;

    win2x0 = split + static_cast<u_int8_t>(0.5f * static_cast<float>(globPad.first));
    win2y0 = win1y0;
    win2Width = activeWindow.second.first - split - static_cast<u_int8_t>(1.5f * static_cast<float>(globPad.first));
    win2Height = activeWindow.second.second - 2 * globPad.second;

    checkRange();
    for(int i = 0; i < iterations; i++) {

        win->drawRoundRect(win1x0, win1y0, win1Width, win1Height,
                           roundness,
                           static_cast<uint8_t>(WindowConfigs::getActiveWinClrs().first)); // winLeft

        win->drawRoundRect(win2x0, win2y0, win2Width, win2Height,
                           roundness,
                           static_cast<uint8_t>(WindowConfigs::getActiveWinClrs().second)); // winRight

        win1x0 += globPad.first;
        win1y0 += globPad.second;
        win1Width -= 2*globPad.first;
        win1Height -= 2*globPad.second;

        win2x0 += globPad.first;
        win2y0 += globPad.second;
        win2Width -= 2*globPad.first;
        win2Height -= 2*globPad.second;

        checkRange();
    }
    this->setWindowAreaLeft({{win1x0, win1y0},{ win1Width + win1x0, win1Height }});
    this->setWindowAreaRight({{win2x0, win2y0},{ win->width() - globPad.first * iterations, win2Height}});
    win->display(); // render to screen
}

void WindowHorizSplit::exit( Window *win... ) {
}

AbstractWindowState& WindowHorizSplit::getInstance() {
    static WindowHorizSplit singleton;
    return singleton
}

bool WindowHorizSplit::equals( const AbstractWindowState &b ) {
    if(const WindowHorizSplit* a = dynamic_cast<const WindowHorizSplit*>(&b)) {
        return true;
    }
    return false;
}

/* if true return second window */
AbstractWindowState::winArea WindowHorizSplit::getWindowTextArea( bool b ) const {
    return b ? windowAreaRight : windowAreaLeft;
}

void WindowHorizSplit::setWindowAreaLeft(
        const WindowHorizSplit::winArea &windowAreaLeft ) {
    WindowHorizSplit::windowAreaLeft = windowAreaLeft;
}

void WindowHorizSplit::setWindowAreaRight(
        const WindowHorizSplit::winArea &windowAreaRight ) {
    WindowHorizSplit::windowAreaRight = windowAreaRight;
}
