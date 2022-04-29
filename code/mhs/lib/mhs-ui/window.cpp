#include "window.hpp"
#include "concrete_windowStates.hpp"

Window::Window(const int16_t& w,const int16_t& h) : Adafruit_GFX(w, h) {
    currentState = &WindowFullscreen::getInstance();
    configs = &WindowConfigs::getInstance();
}

void Window::setState( AbstractWindowState &newState ) {
    currentState->exit(this);
    currentState = &newState;
    currentState->enter(this);
}

void Window::draw() {
    currentState->draw(this);
}

/*
 * if the current window doesn't equal Fullscreen we have 2 Windows available
 * accessed through the variadic arg
 */
void Window::print( const String &s... ) {
    if( !getCurrentState()->equals(WindowFullscreen::getInstance())) {
        va_list args;
        va_start(args, s);

        bool firstWin = va_arg(args, bool);

        va_end(args);

    }
}

/* print / println -> write(u_int8, u_int8) -> write(u_int8) */
size_t Window::write( uint8_t c ) {
    if (!gfxFont) { // 'Classic' built-in font

        if (c == '\n') {              // Newline?
            cursor_x = configs->getWinPadding().first;               // Reset x to zero,
            cursor_y += (textsize_y * 8) + configs->getWinPadding().second; // advance y one line
        } else if (c != '\r') {       // Ignore carriage returns
            if (wrap && ((cursor_x + textsize_x * 6) > ( configs.)) { // Off right?
                cursor_x = 0;                                       // Reset x to zero,
                cursor_y += textsize_y * 8; // advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                     textsize_y);
            cursor_x += textsize_x * 6; // Advance x one char
        }

    } else { // Custom font
        if (c == '\n') {
            cursor_x = 0;
            cursor_y +=
                    (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if (c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
                uint8_t w = pgm_read_byte(&glyph->width),
                        h = pgm_read_byte(&glyph->height);
                if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if (wrap && ((cursor_x + textsize_x * (xo + w)) > _width)) {
                        cursor_x = 0;
                        cursor_y += (int16_t)textsize_y *
                                    (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                    }
                    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                             textsize_y);
                }
                cursor_x +=
                        (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
            }
        }
    }
    return 1;
}
