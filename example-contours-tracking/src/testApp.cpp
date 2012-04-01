#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(0);
	
	movie.loadMovie("fingers.mov");
	movie.play();
	
	contourFinder.setMinAreaRadius(1);
	contourFinder.setMaxAreaRadius(100);
	contourFinder.setThreshold(15);
	contourFinder.getTracker().setMaximumAge(4);
	contourFinder.getTracker().setMaximumAge(32);
	
	showLabels = true;
}

void testApp::update() {
	movie.update();
	if(movie.isFrameNew()) {
		blur(movie, 10);
		contourFinder.findContours(movie);
	}
}

void testApp::draw() {
	ofSetBackgroundAuto(showLabels);
	if(showLabels) {
		ofSetColor(255);
		movie.draw(0, 0);
		contourFinder.draw();
		for(int i = 0; i < contourFinder.size(); i++) {
			ofPoint center = toOf(contourFinder.getCenter(i));
			ofPushMatrix();
			ofTranslate(center.x, center.y);
			ofDrawBitmapString(ofToString(contourFinder.getLabel(i)), 0, 0);
			ofVec2f velocity = toOf(contourFinder.getVelocity(i));
			ofScale(5, 5);
			ofLine(0, 0, velocity.x, velocity.y);
			ofPopMatrix();
		}
	} else {
		RectTracker& tracker = contourFinder.getTracker();
		for(int i = 0; i < contourFinder.size(); i++) {
			unsigned int label = contourFinder.getLabel(i);
			// only draw a line if this is not a new label
			if(tracker.existsPrevious(label)) {
				// use the label to pick a random color
				ofSeedRandom(label << 24);
				ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
				// get the tracked object (cv::Rect) at current and previous position
				cv::Rect& previous = tracker.getPrevious(label);
				cv::Rect& current = tracker.getCurrent(label);
				// get the centers of the rectangles
				ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
				ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
				ofLine(previousPosition, currentPosition);
			}
		}
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		showLabels = !showLabels;
	}
}
