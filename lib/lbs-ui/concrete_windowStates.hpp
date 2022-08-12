#pragma once
#include <cstdarg>
#include "abstract_windowState.hpp"
#include "window.hpp"

class WindowFullscreen : public AbstractWindowState
{

public:
	using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
	/* create some true Singletons */
	WindowFullscreen( WindowFullscreen const& ) = delete;
	WindowFullscreen( WindowFullscreen&& ) = delete;
	WindowFullscreen& operator=( WindowFullscreen const& ) = delete;
	WindowFullscreen& operator=( WindowFullscreen&& ) = delete;
	static AbstractWindowState& getInstance();
	
	bool equals( const AbstractWindowState& b ) override;
	void enter( Window* win... ) override;
	void draw( Window* win... ) override;
	void exit( Window* win... ) override;
	
	void setWindowArea( const winArea& windowArea );
	
	winArea getWindowTextArea( bool b ) const override;

private:
	WindowFullscreen() = default;
	winArea windowArea;
};

class WindowVertSplit : public AbstractWindowState
{
public:
	using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
	
	WindowVertSplit( const WindowFullscreen& ) = delete;
	WindowVertSplit( WindowVertSplit&& ) = delete;
	WindowVertSplit& operator=( WindowVertSplit const& ) = delete;
	WindowVertSplit& operator=( WindowVertSplit&& ) = delete;
	static AbstractWindowState& getInstance();
	
	bool equals( const AbstractWindowState& b ) override;
	winArea getWindowTextArea( bool b ) const override;
	void enter( Window* win... ) override;
	void draw( Window* win... ) override;
	
	void setWindowAreaUp( const winArea& windowAreaUp );
	
	void setWindowAreaDown( const winArea& windowAreaDown );
	
	void exit( Window* win... ) override;

private:
	WindowVertSplit() = default;
	winArea windowAreaUp, windowAreaDown;
};

class WindowHorizSplit : public AbstractWindowState
{
public:
	using winArea = std::pair<std::pair<u_int8_t, u_int8_t>, std::pair<u_int8_t, u_int8_t>>; //custom type for this pairing madness
	
	WindowHorizSplit( WindowHorizSplit const& ) = delete;
	WindowHorizSplit( WindowHorizSplit&& ) = delete;
	WindowHorizSplit& operator=( WindowHorizSplit const& ) = delete;
	WindowHorizSplit& operator=( WindowHorizSplit&& ) = delete;
	static AbstractWindowState& getInstance();
	
	winArea getWindowTextArea( bool b ) const override;
	
	void setWindowAreaLeft( const winArea& windowAreaLeft );
	
	void setWindowAreaRight( const winArea& windowAreaRight );
	
	bool equals( const AbstractWindowState& b ) override;
	void enter( Window* win... ) override;
	void draw( Window* win... ) override;
	void exit( Window* win... ) override;

private:
	WindowHorizSplit() = default;
	winArea windowAreaLeft, windowAreaRight;
};