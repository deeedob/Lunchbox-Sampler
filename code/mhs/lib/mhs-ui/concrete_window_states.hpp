#pragma once
#include "window_state.hpp"
#include "window.hpp"



class DrawFullScreen : public WindowState
{
public:
    void enter( Window *window ) override;

    void drawWindow( Window *window ) override;

    void exit( Window *window ) override;

    static WindowState& getInstance();

private:
    DrawFullScreen() {}
    DrawFullScreen( const DrawFullScreen& other);
    DrawFullScreen& operator=( const DrawFullScreen& other);
};

class DrawVertSplit : public WindowState
{
public:
    void enter( Window *window ) override;

    void drawWindow( Window *window ) override;

    void exit( Window *window ) override;

    static WindowState& getInstance();

private:
    DrawVertSplit() {}
    DrawVertSplit( const DrawFullScreen& other);
    DrawVertSplit& operator=( const DrawFullScreen& other);
};


class DrawHorizSplit : public WindowState
{
public:
    void enter( Window *window ) override;

    void drawWindow( Window *window ) override;

    void exit( Window *window ) override;

    static WindowState& getInstance();

private:
    DrawHorizSplit() {}
    DrawHorizSplit( const DrawFullScreen& other);
    DrawHorizSplit& operator=( const DrawFullScreen& other);
};