#include "window.hpp"

Window::Window( uint8_t w, uint8_t h, TwoWire *twi,
                int8_t rst_pin, u_int32_t preclk,
                uint32_t postclk, u_int8_t p ) :
                Adafruit_SSD1327(w,h, twi, rst_pin, preclk, postclk)
                , currentPos{0,0,static_cast<u_int8_t>(this->width()),static_cast<u_int8_t>(this->height())}
                , padding(p)
                {
}

size_t Window::write( uint8_t c ) {
    if (!gfxFont) { // 'Classic' built-in font
        if (c == '\n') {              // Newline?
            cursor_x = currentPos[0] + padding;               // Reset x to zero,
            cursor_y += (textsize_y * 8) + currentPos[1] + padding; // advance y one line
        } else if (c != '\r') {       // Ignore carriage returns
            if (wrap && ((cursor_x + textsize_x * 6) > (currentPos[2] - padding))) { // Off right?
                cursor_x = currentPos[0] + padding;                                       // Reset x to zero,
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

Window::Positions Window::splitHorizontal( const u_int8_t &padding,
                                           const float &split_ratio,
                                           const u_int8_t &round,
                                           const u_int8_t &deepness,
                                           u_int8_t clr_left, u_int8_t clr_right) {
    u_int8_t split;
    auto p = padding;
    Positions pos;
    pos.first = std::vector<u_int8_t>();
    pos.second = std::vector<u_int8_t>();

        split = static_cast<u_int8_t>(split_ratio * static_cast<float>(this->width()));
        u_int8_t win1_w, win_h, win2_w;
        for( int i = 0; i < deepness; i++ ) {
            win1_w = split - 2*p;
            win_h = this->height() - 2*p;
            win2_w = this->width() - (split + 2*p);
            this->drawRoundRect(p, p, win1_w, win_h, round,clr_left);
            this->drawRoundRect( split + p, p,  win2_w, win_h, round, clr_right);
            p+=padding;
        }
        pos.first.push_back( p );
        pos.first.push_back( p );
        pos.first.push_back( win1_w );
        pos.first.push_back( win_h );

        pos.second.push_back( win2_w );
        pos.second.push_back( p );
        pos.second.push_back( this->width() - p );
        pos.second.push_back(  win_h );
    return pos;
}

size_t Window::printToWindow( const String &s, const std::vector<u_int8_t> &p ) {
    currentPos = p;
    cursor_x = currentPos[0] + padding;
    cursor_y = currentPos[1] + padding;
    printCurrentPos();

    this->print(s);

    return 1;
}
