#pragma once
#include <cstdarg>
#include "abstract_windowState.hpp"
#include "window.hpp"

class WindowFullscreen : public AbstractWindowState
{

public:
    using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
    /* create some true Singletons */
    WindowFullscreen(WindowFullscreen const&) = delete;
    WindowFullscreen(WindowFullscreen &&) = delete;
    WindowFullscreen& operator=(WindowFullscreen const&) = delete;
    WindowFullscreen& operator=(WindowFullscreen &&) = delete;
    static AbstractWindowState& getInstance();

    bool equals( const AbstractWindowState &b ) override;
    void enter( Window *win... ) override;
    void draw( Window *win... ) override;
    void exit( Window *win... ) override;

    const winArea &getWindowArea() const;
    void setWindowArea(const winArea &windowArea );

private:
    WindowFullscreen() = default;
    winArea windowArea;
};

class WindowVertSplit : public AbstractWindowState
{
public:
    std::pair<u_int8_t, u_int8_t> winUpSize, winDownSize;

    WindowVertSplit(WindowFullscreen const&) = delete;
    WindowVertSplit(WindowVertSplit &&) = delete;
    WindowVertSplit& operator=(WindowVertSplit const&) = delete;
    WindowVertSplit& operator=(WindowVertSplit &&) = delete;
    static AbstractWindowState& getInstance();

    bool equals( const AbstractWindowState &b ) override;
    void enter( Window *win... ) override;
    void draw( Window *win... ) override;
    void exit( Window *win... ) override;

private:
    WindowVertSplit() = default;
};

class WindowHorizSplit : public AbstractWindowState
{
public:
    using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness

    WindowHorizSplit(WindowHorizSplit const&) = delete;
    WindowHorizSplit(WindowHorizSplit &&) = delete;
    WindowHorizSplit& operator=(WindowHorizSplit const&) = delete;
    WindowHorizSplit& operator=(WindowHorizSplit &&) = delete;
    static AbstractWindowState& getInstance();


    bool equals( const AbstractWindowState &b ) override;
    void enter( Window *win... ) override;
    void draw( Window *win... ) override;
    void exit( Window *win... ) override;

    const winArea &getWindowAreaLeft() const;
    void setWindowAreaLeft( const winArea &windowAreaLeft );
    const winArea &getWindowAreaRight() const;
    void setWindowAreaRight( const winArea &windowAreaRight );

private:
    WindowHorizSplit() = default;
    winArea windowAreaLeft, windowAreaRight;
};
