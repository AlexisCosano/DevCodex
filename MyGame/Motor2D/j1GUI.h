#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"


// ---------------------------------------------------
class j1GUI : public j1Module
{
public:

	j1GUI();

	// Destructor
	virtual ~j1GUI();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
};


#endif __j1GUI_H__
