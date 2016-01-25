// 
// ramSceneManager.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ofMain.h"

#include "ramBaseScene.h"
#include "ramGlobal.h"
#include "ramActorsScene.h"


class ramSceneManager : public ramGlobalShortcut
{
public:
	
	static ramSceneManager& instance();

	void setup();
	void addScene(ramBaseScene* scene);
    
    void allocateFbos(int w, int h);
    void deallocateFbos();
    
    void setScreen(int sceneId, int screenId, bool enabled);
    void setAllScreens(int sceneId, bool enabled);
    
	size_t getNumScenes() const;
	size_t findtSceneIndex(string name) const;
    ramBaseScene* getScene(size_t index);
	inline const ramBaseScene* getScene(size_t index) const
    {
        return const_cast<ramBaseScene*>(getScene(index));
    }
	
	ramActorsScene* getActorsScene();
    inline const ramActorsScene* getActorsScene() const
    {
        return const_cast<ramActorsScene*>(getActorsScene());
    }
	void setShowAllActors(bool showAllActors);
	bool getShowAllActors() const;
    
protected:
	void enableAllEvents();
	void disableAllEvents();

	void actorSetup(ramActor &actor);
	void actorExit(ramActor &actor);

	void rigidSetup(ramRigidBody &rigid);
	void rigidExit(ramRigidBody &rigid);

	vector<ramBaseScene*> scenes;
    vector<vector<bool> > screens;
	
	void update(ofEventArgs& args);
	void draw(ofEventArgs& args);
	void exit(ofEventArgs& args);
	
private:
    void updateOsc();
    
	static ramSceneManager *_instance;
	ramSceneManager();
	ramSceneManager(const ramSceneManager&);
	ramSceneManager& operator=(const ramSceneManager&);
	
	ramActorsScene* actorsScene;
    bool isAllocatedFbos;
    
    ramOscReceiveTag oscReceiverTag;
    
};
