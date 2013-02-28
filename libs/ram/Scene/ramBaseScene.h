#pragma once
#include "ofMain.h"

#include "ramGlobal.h"

#include "ramUnit.h"
#include "ramControlPanel.h"

/** Empty scene sample code

#pragma once

class EmptyScene : public ramBaseScene
{

   public:

    const string getName() { return "EmptyScene"; }

    float box_size;

    void setupControlPanel()
    {
        ramControlPanel &gui = ramGetGUI();

        gui.addPanel(getName());

        box_size = 50;
        gui.addSlider("box_size", &box_size);
    }

    void setup()
    {
    }


    void update()
    {
    }

    void draw()
    {
    }
 
    void drawActor(const ramActor& actor)
    {
        ramDrawBasicActor(actor);
        ramBox(actor.getNode(0), box_size);
    }

    void drawRigid(const ramRigidBody &rigid)
    {
    }

};
 */

class ramBaseScene : public ramUnit
{
public:

	ramBaseScene() : bEnabled(false) {}
	virtual ~ramBaseScene() {}
	
	virtual void setupControlPanel() {}
	
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	
	virtual void drawActor(const ramActor &actor) {}
	virtual void drawRigid(const ramRigidBody &rigid) {}

	virtual void drawHUD() {}

	inline void enable() { bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { setEnabled(!isEnabled()); }

	inline bool isEnabled() { return bEnabled; }

	inline void setEnabled(bool b)
	{
		if (bEnabled == b) return;

		bEnabled = b;

		if (b)
			onEnabled();
		else
			onDisabled();
	}

	virtual void onEnabled() { cout << "[Scene enabled] " << getName() << endl; }
	virtual void onDisabled() { cout << "[Scene disabled] " << getName() << endl; }

	// nodeArray events
	virtual void onActorSetup(const ramActor &actor) {}
	virtual void onActorExit(const ramActor &actor) {}

	virtual void onRigidSetup(const ramRigidBody &rigid) {}
	virtual void onRigidExit(const ramRigidBody &rigid) {}

	ramBaseScene* getPtr() { return this; }

private:

	bool bEnabled;

};