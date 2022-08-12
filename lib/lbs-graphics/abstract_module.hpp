#pragma once

class Event; //forward declaration

class AbstractModule
{
public:
	/* enable*/
	virtual void enter() = 0;
	virtual void update( Event e ) = 0;
	virtual void exit() = 0;
	virtual ~AbstractModule() = 0;
};