//
//  dpHakoniwaMagnetPendulum.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#include "dpHakoniwaMagnetPendulum.h"

void dpHakoniwaMagnetPendulum::setupControlPanel() {
    
    mMotionExtractor.setupControlPanel(this,ofPoint(340,30));
    ramGetGUI().addToggle("Test mode", &bTestMode);
    
    ramGetGUI().addSeparator();
    ramGetGUI().addButton("ALL MAGNETS ON");
    ramGetGUI().addButton("ALL MAGNETS OFF");
    
    for (int i = 0; i<NMAGNETS; i++ ) {
        ramGetGUI().addToggle("ON/OFF_MAGNET"+ofToString(i+1), &bOn[i]);
    }

    ramGetGUI().addSeparator();
    ramGetGUI().addButton("INVERSE ALL");
    ramGetGUI().addButton("RESTORE ALL");

    for (int i = 0; i<NMAGNETS; i++ ) {
        ramGetGUI().addToggle("INVERSE_MAGNET"+ofToString(i+1), &bInversed[i]);
    }
    
    ramGetGUI().addSlider("Distance Threshold", 2.0f, 200.0f, &distanceThreshold);

    vector<string> modename;
    modename.push_back("ALL 3DANCERS MODE");
    modename.push_back("2EACH 3DANCERS MODE");
    
    ramGetGUI().addRadioGroup("mode", modename, &mode);
    
    bTestMode = true;
    distanceThreshold = 20.0f;
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent,this,&dpHakoniwaMagnetPendulum::guiEvent);
    
}

void dpHakoniwaMagnetPendulum::setup() {
    
    mSenderOnOff.setup("192.168.20.52",8528);
    mSenderInverse.setup("192.168.20.72", 8528);

    bHideNodeView = true;
    
}

void dpHakoniwaMagnetPendulum::sendOsc() {
    
    {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/magpen");
        
        for (int i = 0; i < NMAGNETS; i++) {
            m.addIntArg(bOn[i]); // Magnets ON/OFF (1:ON, 0:OFF)
        }
        
        mSenderOnOff.sendMessage(m);
    }
    
    {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/magpen");
        
        for (int i = 0; i < NMAGNETS; i++) {
            m.addIntArg(bInversed[i]); // Magnets Inversed (1: Inversed, 0: Normal)
        }
        
        mSenderInverse.sendMessage(m);
    }
}

void dpHakoniwaMagnetPendulum::guiEvent(ofxUIEventArgs &e) {

    string name = e.widget->getName();
    
    if (name == "ALL MAGNETS ON") {
        for (int i = 0; i < NMAGNETS; i++) {
            bOn[i] = true;
        }
    } else if (name == "ALL MAGNETS OFF") {
        for (int i = 0; i < NMAGNETS; i++) {
            bOn[i] = false;
        }
    } else if (name == "INVERSE ALL") {
        for (int i = 0; i < NMAGNETS; i++) {
            bInversed[i] = true;
        }
    } else if (name == "RESTORE ALL") {
        for (int i = 0; i < NMAGNETS; i++) {
            bInversed[i] = false;
        }
    }
}

void dpHakoniwaMagnetPendulum::update() {
    
    mMotionExtractor.update();
    
    if (!bTestMode) {
        if (mode == 1) {
            
            float d1 = mMotionExtractor.getDistanceAt(0, 1);
            float d2 = mMotionExtractor.getDistanceAt(2, 3);
            float d3 = mMotionExtractor.getDistanceAt(4, 5);
            
            if (d1 < distanceThreshold && d1 != 0.0f) {
                bOn[0] = true;
                bOn[1] = true;
            } else {
                bOn[0] = false;
                bOn[1] = false;
            }
            if (d2 < distanceThreshold && d2 != 0.0f) {
                bOn[2] = true;
                bOn[3] = true;
            } else {
                bOn[2] = false;
                bOn[3] = false;
            }
            if (d3 < distanceThreshold && d3 != 0.0f) {
                bOn[4] = true;
                bOn[5] = true;
            } else {
                bOn[4] = false;
                bOn[5] = false;
            }
        } else if (mode ==0) {
            
            if (mMotionExtractor.getDistanceAt(0, 1) > distanceThreshold &&
                mMotionExtractor.getDistanceAt(2, 3) > distanceThreshold &&
                mMotionExtractor.getDistanceAt(4, 5) > distanceThreshold) {
                for (int i = 0; i < 6; i++) bOn[i] = false;
            } else {
                for (int i = 0; i < 6; i++) bOn[i] = true;
            }
            
        }
    }
    
    if (ofGetKeyPressed(49)){
        bHideNodeView = !bHideNodeView;
    }
    
    sendOsc();
    
}

void dpHakoniwaMagnetPendulum::drawActor(const ramActor &actor){
    ramDrawBasicActor(actor);
}

void dpHakoniwaMagnetPendulum::draw(){
    
    ramSetViewPort(dpGetFirstScreenViewPort()); //１枚目のscreenを描画に指定。ここの仕様変わります。
    ramBeginCamera();
    mMotionExtractor.draw();
    
    ramNode rn[10];
    for (int i = 0; i < 10; i++){
        rn[i] = mMotionExtractor.getNodeAt(i);
    }

    ofPushMatrix();
    ofNoFill();
    ofTranslate(200,0);
    
    for (int i = 0; i < 10; i++) {
        if (rn[i].getPosition().length() !=0) rn[i].draw();
    }
    
//    ofTranslate(200,0);
    
//    ofSetColor(255,0,0);
//    ofLine (-20,0,0,20,0,0);
//    ofSetColor(0,255,0);
//    ofLine (0,-20,0,0,20,0);
//    ofSetColor(0,0,255);
//    ofLine (0,0,-20,0,0,20);
    
    ofSetColor(255);
    
    ofVec3f axisEdge[3][10];
    
    for (int i = 0; i < 10; i++) {
//        ofTranslate(rn[i].getPosition());
        if (rn[i].getPosition().length() != 0) {
        ofQuaternion q = rn[i].getGlobalOrientation();
        float tA, tX, tY, tZ;
            q.getRotate(tA, tX, tY, tZ);
            
            ofTranslate(0,-100,0);
            
            ofPushMatrix();
            ofRotate(tA, tX, tY, tZ);
//        ofLine(0, 0, 0, tX * 100.0f, tY * 100.0f, tZ * 100.0f);
//            ofLine(0,0,0,100,0,0);
            
            ofSetColor(255,0,0);
            ofLine (-20,0,0,20,0,0);
            ofSetColor(0,255,0);
            ofLine (0,-20,0,0,20,0);
            ofSetColor(0,0,255);
            ofLine (0,0,-20,0,0,20);   // 先端の座標、赤(20,0,0)青(0,20,0)緑(0,0,20)に対して、xyZでグラフ表示してみる <-解析のため。
            
            ofPopMatrix();
            
            ofVec3f redPos = ofVec3f(20,0,0);
            axisEdge[0][i] = redPos.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f greenPos = ofVec3f(0,20,0);
            axisEdge[1][i] = greenPos.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f bluePos = ofVec3f(0,0,20);
            axisEdge[2][i] = bluePos.getRotated(tA, ofVec3f(tX, tY, tZ));
            
            ofSetColor(255,0,0);
            ofDrawBox(axisEdge[0][i], 10, 10, 10);
            ofSetColor(0,255,0);
            ofDrawBox(axisEdge[1][i], 10, 10, 10);
            ofSetColor(0,0,255);
            ofDrawBox(axisEdge[2][i], 10, 10, 10);
            
            
        }
    }
    
    
    
    ofPopMatrix();
    ramEndCamera();
    
    ofPushMatrix();
    
    
    
    
    ofPopMatrix();
    
    if (bHideNodeView) example_drawDump();
    
}

void dpHakoniwaMagnetPendulum::example_drawDump(){
    
    ofPushMatrix();
    ofTranslate(800, 10);
    
    for (int i = 0;i < mMotionExtractor.getNumPort();i++){
        ofPushMatrix();
        ofTranslate(0, i*90);
        
        ofVec3f vec = mMotionExtractor.getVelocityAt(i);
        float speed = mMotionExtractor.getVelocitySpeedAt(i);
        
        ofNoFill();
        ofRect(0, 0, 200, 80);
        ofFill();
        
        string info = "";
        info += "Port  :" + ofToString(i) + "\n";
        info += "Actor :" + mMotionExtractor.getActorNameAt(i) + "\n";
        info += "Joint :" + mMotionExtractor.getJointNameAt(i) + "\n";
        info += "Speed :" + ofToString(mMotionExtractor.getVelocitySpeedAt(i)) + "\n";
        
        ofSetColor(100);
        ofRect(10, 45, mMotionExtractor.getVelocitySpeedAt(i)*10.0, 15);
        
        ofSetColor(255);
        ofDrawBitmapString(info, 10, 15);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}





