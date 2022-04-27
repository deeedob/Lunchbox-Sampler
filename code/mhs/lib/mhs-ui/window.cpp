#include "window.hpp"

WindowConfigs &WindowConfigs::get() {
    static WindowConfigs* configs = new WindowConfigs();
    return *configs;
}

WindowConfigs::WindowConfigs( u_int8_t winPadx, u_int8_t winPady,
                              u_int8_t txtPadx, u_int8_t txtPady ) {
    currentWindowSize = {};
}

WindowConfigs *WindowConfigs::getInstance() {

}


Window::Window( uint8_t w, uint8_t h, TwoWire *twi,
                int8_t rst_pin, u_int32_t preclk,
                uint32_t postclk) :
                Adafruit_SSD1327(w,h, twi, rst_pin, preclk, postclk)
                , currentPos{0,0,static_cast<u_int8_t>(this->width()),static_cast<u_int8_t>(this->height())}
{
    windowPadding_x = 2;
    textPadding_x = 0;
    textPadding_y = 0;
}

Window::Positions Window::splitHorizontal( const u_int8_t &pad,
                                           const float &split_ratio,
                                           const u_int8_t &round,
                                           const u_int8_t &deepness,
                                           u_int8_t clr_left, u_int8_t clr_right) {
    u_int8_t split;
    auto p = pad;

    split = static_cast<u_int8_t>(split_ratio * static_cast<float>(this->width()));
    u_int8_t win1_w, win_h, win2_w;
    for( int i = 0; i < deepness; i++ ) {
        win1_w = split - 2*p;
        win_h = this->height() - 2*p;
        win2_w = this->width() - (split + 2*p);
        this->drawRoundRect(p, p, win1_w, win_h, round,clr_left);
        this->drawRoundRect( split + p, p,  win2_w, win_h, round, clr_right);
        p+=pad;
    }

    Positions pos;
    pos.first = { p, p, win1_w, win_h };
    pos.second = { split + p, p, static_cast<unsigned char>(this->width()-p), win_h };

    return pos;
}


size_t Window::printToWindow( const String &s, const std::vector<u_int8_t> &p ) {
    currentPos = p;
    cursor_x = currentPos[0] + windowPadding_x;
    cursor_y = currentPos[1] + windowPadding_x;

    this->print(s);
    return 1;
}

size_t Window::write( uint8_t c ) {
    if (!gfxFont) { // 'Classic' built-in font
        if (c == '\n') {              // Newline?
            cursor_x = currentPos[0] + windowPadding_x;               // Reset x to zero,
            cursor_y += textsize_y * 8; // advance y one line
        } else if (c != '\r') {       // Ignore carriage returns
            if (wrap && ((cursor_x + textsize_x * 6) > (currentPos[2] - windowPadding_x))) { // Off right?
                cursor_x = currentPos[0] + windowPadding_x;                                       // Reset x to zero,
                cursor_y += textsize_y * 8; // advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                     textsize_y);
            cursor_x += textsize_x * 6; // Advance x one char
        }

    } else { // Custom font

    }
    return 1;
}

size_t Window::_print( const String &s ) {
    uint8_t buffer[33];
    size_t count = 0;
    unsigned int index = 0;
    unsigned int len = s.length();
    while (len > 0) {
        s.getBytes(buffer, sizeof(buffer), index);
        unsigned int nbytes = len;
        if (nbytes > sizeof(buffer)-1) nbytes = sizeof(buffer)-1;
        index += nbytes;
        len -= nbytes;
        count += _write(buffer, nbytes);
    }
    return count;
}

size_t Window::_write( const uint8_t *buff, size_t size ) {
    if (buffer == nullptr) return 0;
    size_t count = 0;
    while (size--) count += write(*buffer++);
    return count;
}
