#ifndef SENCE_H
#define SENCE_H

#include<graphics.h>
#include"camera.h"

class sence
{
public:
	sence() = default;
	~sence() = default;
	virtual void on_enter(){}
	virtual void on_update(int delta){}
	virtual void on_draw(const Camera& camera){}
	virtual void on_input(const ExMessage& msg){}
	virtual void on_exit(){}
private:

};

#endif 
