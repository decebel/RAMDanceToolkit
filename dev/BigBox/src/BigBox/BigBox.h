#pragma once
#include <numeric>
#include "ramSceneBase.h"


namespace gl
{
	//ax + by + cz + d = 0;
	static const float kGroundPlaneXUp[] = { 1.0, 0.0, 0.0, 1.0 };
	static const float kGroundPlaneYUp[] = { 0.0, 1.0, 0.0, 1.0 };
	static const float kGroundPlaneZUp[] = { 0.0, 0.0, 1.0, 1.0 };
	
	inline void calcShadowMatrix(const float groundplane[4],
								 const float lightpos[3],
								 float shadowMatrix[16]);
	
	
	void calcShadowMatrix(const float groundplane[],
						  const float lightpos[],
						  float shadowMatrix[])
	{
		float dot = inner_product(groundplane, groundplane+4, lightpos, 0.f);
		
		for(int y = 0; y < 4;++y) {
			for(int x = 0; x < 4; ++x) {
				
				shadowMatrix[y*4+x] = - groundplane[y]*lightpos[x];
				
				if (x == y) shadowMatrix[y*4+x] += dot;
			}
		}
	}
}



class BigBox : public ramSceneBase
{

	string keyMasterSize, keyLineWidth;
	
public:
	
	BigBox()
	{
		sceneName = "Big Box";
		
		// ---
		
		keyMasterSize = "Master size";
		keyLineWidth = "line Width";
	}
	
	void refreshControlPanel(ofxAutoControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addSlider(keyMasterSize, 10, 10, 1000);
		guiPtr->addSlider(keyLineWidth, 10, 1, 100);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			guiPtr->addSlider(ramActor::getJointName(i), 0, 0, 1000);
	}
	
	void setup()
	{
		const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
		gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	}
	
	void update()
	{
		if(guiPtr->hasValueChanged(keyMasterSize))
		{
			for (int i=0; i<23; i++)
				guiPtr->setValueF(ramActor::getJointName(i), guiPtr->getValueF(keyMasterSize));
			guiPtr->clearAllChanged();
		}
	}
	
	void draw()
	{
		
	}
	
	void drawActor(ramActor& actor)
	{
		ofColor currSklColor(110, 20, 20);
		ofColor recSklColor(20, 20, 110);
		ofColor shadowColor(0, 30);
		float lineWidth = gui.getValueF(keyLineWidth);
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			const ramNode &node = actor.getNode(i);
			float boxSize = (i==ramActor::JOINT_HEAD) ? 6 : 3;
			float bigBoxSize = gui.getValueF(getJointName(i));
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushMatrix();
			ofPushStyle();
			ofNoFill();
			
			glEnable(GL_DEPTH_TEST);
			
			// big box
			ofSetColor(recSklColor);
			
			node.transformBegin();
			ofSetLineWidth(lineWidth);
			ofBox(bigBoxSize);
			node.transformEnd();
			
			// actor
			ofSetColor(currSklColor);
			ofSetLineWidth(1);
			ofBox(node, boxSize);
			if (node.hasParent())
				ofLine(node, *node.getParent());
			
			
			// shadows
			glDisable(GL_DEPTH_TEST);
			glMultMatrixf(shadowMat.getPtr());
			ofEnableAlphaBlending();
			ofSetColor(shadowColor);
			
			node.transformBegin();
			ofSetLineWidth(lineWidth);
			ofBox(bigBoxSize);
			node.transformEnd();
			
			ofSetLineWidth(1);
			ofBox(node, boxSize);
			if (node.hasParent())
				ofLine(node, *node.getParent());
			
			ofPopStyle();
			glPopMatrix();
			glPopAttrib();
		}
		
	}
	
	void drawRigidBody(ramRigidBody& rigid)
	{
		
	}
	
	void drawFloor()
	{
		
	}
	
	
private:
	
	ofMatrix4x4 shadowMat;
};

