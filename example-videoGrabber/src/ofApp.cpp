
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//--------------------------------------------------------------
void mouseScroll(GLFWwindow* window, double x, double y)
{
    
    ((ofApp *)ofGetAppPtr())->cefgui->mouseWheel(x*10, y*10);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setup()
{
    int argc = 0;
    char** argv;
    
    // add scrolling callback
    glfwSetScrollCallback( ((ofAppGLFWWindow *) ofGetWindowPtr())->getGLFWWindow(), mouseScroll);
    
    cefgui = initofxCEF(argc, argv);

    ofSetVerticalSync(false);
    ofDrawBitmapMode mode;
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL );
    
    
    camWidth 		= 320;	// try to grab at this size.
    camHeight 		= 240;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    videoInverted 	= new unsigned char[camWidth*camHeight*3];
    videoTexture.allocate(camWidth,camHeight, GL_RGB);
    
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete cefgui;
}

//--------------------------------------------------------------
void ofApp::update()
{
    
    cefgui->update();

    if (ofGetFrameNum() == 1){
        string path = "file://" + ofToDataPath("html/index.html", true);
//        path = "http://haikusinteractifs.com/";
//        path = "http://mrdoob.com/#/137/voxels_liquid";
//        path = "http://threejs.org/examples/";
        //path = "http://www.thefamilyfarmer.com";

        cefgui->load(path.c_str());
    }
    else if (ofGetFrameNum() == 300) {
        string command = "sendMessageToJS('This is OF talking to you! (sent on frame number 100)')";
        cefgui->executeJS((const char*) command.c_str());
    }
        
    vidGrabber.update();
        
    if (vidGrabber.isFrameNew()){
        int totalPixels = camWidth*camHeight*3;
        unsigned char * pixels = vidGrabber.getPixels();
        for (int i = 0; i < totalPixels; i++){
            videoInverted[i] = 255 - pixels[i];
        }
        videoTexture.loadData(videoInverted, camWidth, camHeight, GL_RGB);
    }

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255,0,255);
    
    ofSetHexColor(0xffffff);
    vidGrabber.draw(300,300);
    videoTexture.draw(300+camWidth,300,camWidth*2,camHeight*2);
    
    cefgui->draw();
    
//    ofPushMatrix();
//    ofTranslate(ofPoint(ofGetMouseX(), ofGetMouseY()));
//    ofScale(2,2);
//    ofDrawBitmapStringHighlight("openframeworks !! up arrow to load new url", 0,0);
//    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::gotMessageFromJS(string strMessage)
{
    cout << "Got a message from JS: " << strMessage << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == OF_KEY_UP){
        string message = ofSystemTextBoxDialog("url to load");
        if (message.length() > 0){
            cefgui->load(message.c_str());
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    if (cefgui!= NULL){
        cefgui->mouseMove(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mouseMove(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mousePressed(x, y);
        //--------------------------------------------------------------
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    if (cefgui!= NULL){
        cefgui->reshape(w,h);
        cefgui->renderHandler->init();
        cefgui->browser->Reload();
    }
    // this is failing
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
