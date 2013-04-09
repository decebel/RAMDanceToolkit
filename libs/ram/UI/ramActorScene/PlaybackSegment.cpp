#include "PlaybackSegment.h"
#include "ramActorManager.h"

#pragma mark -
#pragma mark constructor, destructor

PlaybackSegment::PlaybackSegment(string segmentName)
{
    name = segmentName;
    
	init();
	
	btnHideActor = new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show");
	btnPlayActor = new ofxUIImageToggle(32, 32, &bPaused, ramToResourcePath("Images/play.png"),"pause");
	btnCueActor = new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"resetPos");
	btnDeleteActor = new ofxUIImageButton(32, 32, false, ramToResourcePath("Images/delete.png"),"delete");
}

PlaybackSegment::~PlaybackSegment()
{
	delete btnHideActor;
	delete btnPlayActor;
	delete btnCueActor;
	delete btnDeleteActor;
}




#pragma mark -
#pragma mark public methods
ramActorUISegmentType PlaybackSegment::getType()
{
    return RAM_UI_SEGMENT_TYPE_PLAYBACK;
}

void PlaybackSegment::update()
{
	session.updatePlayhead();
}

ofxUICanvasPlus* PlaybackSegment::createPanel(const string targetName)
{
    name = targetName;
    
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
    const float padding = ramGetGUI().kXInit*2;
	
	
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	child->addSpacer(width-padding, 2);
	
	
	/// Icons   
	child->addWidgetDown(btnHideActor);
	child->addWidgetRight(btnPlayActor);
	child->addWidgetRight(btnCueActor);
	child->addWidgetRight(btnDeleteActor);
	
	
	/// actor color
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	child->addSlider("R", 0, 1, &jointColor.r, 44, height);
	child->addSlider("G", 0, 1, &jointColor.g, 44, height);
	child->addSlider("B", 0, 1, &jointColor.b, 45, height);
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
    
	/// actor position
	child->add2DPad("Position",
					ofPoint(-500, 500),
					ofPoint(-500, 500),
					&position,
					width-padding, 100);
	
	child->autoSizeToFitWidgets();
	
	ofAddListener(child->newGUIEvent, this, &PlaybackSegment::onValueChanged);
	
	return child;
}


void PlaybackSegment::pause(bool bPause)
{
    bPaused = bPause;
    
	if (!bPaused)
		session.play();
	else
		session.stop();
    
    btnPlayActor->setValue(bPaused);
}

void PlaybackSegment::deleteSelf()
{
    ramActorManager::instance().removeNodeArray(name);
    parent->removeControlSegment(name);
}





#pragma mark -
#pragma mark Events

void PlaybackSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string widgetName = e.widget->getName();
	
	if (widgetName == "pause")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		const bool value = toggle->getValue();
		pause(value);
	}
    
	if (widgetName == "delete")
	{
        deleteSelf();
	}
	
	saveCache();
}




#pragma mark -
#pragma mark private methods

void PlaybackSegment::init()
{
    BaseSegment::init();
}


