#include "p2Defs.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "j1GUIElement.h"
#include "j1GUIButton.h"
#include "j1GUIImage.h"
#include "j1GUIScrollbar.h"
#include "j1GUIText.h"
#include "j1App.h"


j1GUI::j1GUI() : j1Module()
{
	name.create("GUI");
}

// Destructor
j1GUI::~j1GUI()
{}

// Called before render is available
bool j1GUI::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1GUI::Start()
{
	return true;
}

// Update all guis
bool j1GUI::PreUpdate()
{
	bool ret = true;

	return ret;
}

// Called after all Updates
bool j1GUI::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1GUI::CleanUp()
{
	bool ret = true;

	return ret;
}