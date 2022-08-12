#pragma once

//forward declaration
class Event;

class Graphics;

class AbstractModule
{
public:
	/* enable*/
	virtual void enter( Graphics* graphics ) = 0;
	virtual void update( Graphics* graphics, Event e ) = 0;
	virtual void exit() = 0;
	virtual ~AbstractModule() = 0;
};