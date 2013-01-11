#include "ofApp.h"

string getAddress(ofxOscMessage& msg, int position) {
	vector<string> address = ofSplitString(msg.getAddress(), "/", true);
	return address[position];
}

bool checkAddress(ofxOscMessage& msg, string name, int position) {
	vector<string> address = ofSplitString(msg.getAddress(), "/", true);
	return address.size() > position && address[position] == name;
}

string formatPosition(ofVec3f point) {
	return ofToString((int) point.x) + "/" + ofToString((int) point.y) + "/" + ofToString((int) point.z);
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("port", 10001));
	settings.popTag();
}

void ofApp::update() {
	while(osc.hasWaitingMessages()) {
		ofxOscMessage msg;
		osc.getNextMessage(&msg);
		if(checkAddress(msg, "openni", 0)) {
			int deviceID = ofToInt(getAddress(msg, 1));
			OscOpenNI& openni = opennis[deviceID];
			if(checkAddress(msg, "user", 2)) {
				int xnId = ofToInt(getAddress(msg, 3));
				OscUser& user = openni.users[xnId];
				if(checkAddress(msg, "joints", 4)) {
					const int elementsPerOscJoint = 4;
					int numOscJoints = msg.getNumArgs() / elementsPerOscJoint;
					int j = 0;
					for(int i = 0; i < numOscJoints; i++) {
						OscJoint& joint = user.joints[i];
						joint.position.x = msg.getArgAsFloat(j++);
						joint.position.y = msg.getArgAsFloat(j++);
						joint.position.z = msg.getArgAsFloat(j++);
						joint.confidence = msg.getArgAsFloat(j++);
					}
				} else {
					user.isFound = msg.getArgAsInt32(0);
					user.isTracking = msg.getArgAsInt32(1);
					user.isSkeleton = msg.getArgAsInt32(2);
					user.isCalibrating = msg.getArgAsInt32(3);
				}
			}
		}
	}
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	ofScale(.05, .05, .05);
	ofDrawGrid(2000, 10, false, true, true, true);
	for(map<int, OscOpenNI>::iterator openniItr = opennis.begin(); openniItr != opennis.end(); openniItr++) {
		OscOpenNI& openni = openniItr->second;
		for(map<int, OscUser>::iterator userItr = openni.users.begin(); userItr != openni.users.end(); userItr++) {
			OscUser& user = userItr->second;
			ofSetColor(255);
			user.drawSkeleton();
			for(map<int, OscJoint>::iterator jointItr = user.joints.begin(); jointItr != user.joints.end(); jointItr++) {
				OscJoint& joint = jointItr->second;
				ofSetColor(ofColor::fromHsb(255 * joint.confidence, 255, 255));
				ofBox(joint.position, 10);
				ofSetColor(255);
				//ofDrawBitmapString(formatPosition(joint.position), joint.position);
			}
		}
	}
	cam.end();
}