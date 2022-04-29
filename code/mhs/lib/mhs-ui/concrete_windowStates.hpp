#pragma once
#include <cstdarg>
#include "abstract_windowState.hpp"
#include "window.hpp"

class WindowFullscreen : public AbstractWindowState
{
    friend class Window;
public:
    std::pair<u_int8_t, u_int8_t> winSize;
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

    const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &getWindowArea() const;
    void setWindowArea(const std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> &windowArea );

private:
    WindowFullscreen() = default;
    std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>> windowArea;
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
    std::pair<u_int8_t, u_int8_t> winLeftSize, winRightSize;

    WindowHorizSplit(WindowHorizSplit const&) = delete;
    WindowHorizSplit(WindowHorizSplit &&) = delete;
    WindowHorizSplit& operator=(WindowHorizSplit const&) = delete;
    WindowHorizSplit& operator=(WindowHorizSplit &&) = delete;
    static AbstractWindowState& getInstance();

    bool equals( const AbstractWindowState &b ) override;
    void enter( Window *win... ) override;
    void draw( Window *win... ) override;
    void exit( Window *win... ) override;

private:
    WindowHorizSplit() = default;
};
