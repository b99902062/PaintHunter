#include "GestureScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* GestureScene::scene() {
    CCScene *scene = CCScene::create();
    GestureScene *layer = GestureScene::create();
    scene->addChild(layer);
    return scene;
}

bool GestureScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	this->setTouchEnabled(true);

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/GestureUI/GestureUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	xLabel = (UILabel*) rootPanel->getChildByName("touchXLabel");
	yLabel = (UILabel*) rootPanel->getChildByName("touchYLabel");
	resultLabel = (UILabel*) rootPanel->getChildByName("gestureNameLabel");

    this->addChild(uiLayer, 2);

    return true;
}

void GestureScene::draw() {
	if(isOnTouch) {
		ccPointSize(15);  
		ccDrawColor4B(255, 255, 0, 64);  

		if(mPoints.size() > 0) {
			glLineWidth( 50.0f );
			ccDrawPoint(mPoints[0]);  
			for(int i=1; i<mPoints.size(); i++)
				ccDrawLine(mPoints[i-1], mPoints[i]);
		}
		
	}
}

void GestureScene::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -2);
}

void GestureScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
	mPoints.clear();
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);

	isOnTouch = true;
}

void GestureScene::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);
}

void GestureScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);
	//checkMatchingGesture();

	isOnTouch = false;
}

void GestureScene::updateXYLabel(CCPoint pnt) {

	std::ostringstream stringStreamX;
	std::ostringstream stringStreamY;

	stringStreamX << "Touch X: " <<  pnt.x;
	stringStreamY << "Touch Y: " <<  pnt.y;

	xLabel->setText(stringStreamX.str());
	yLabel->setText(stringStreamY.str());
}


