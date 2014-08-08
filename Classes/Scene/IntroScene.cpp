#include "IntroScene.h"
#include "VillageScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* IntroScene::scene() {
    CCScene *scene = CCScene::create();
    IntroScene *layer = IntroScene::create();
    scene->addChild(layer);
    return scene;
}

bool IntroScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
	timeCounter = 3;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/IntroSceneUI/IntroSceneUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");

    this->addChild(uiLayer);

	this->schedule(schedule_selector(IntroScene::updateTimer), 1.0f);
	rootPanel->getChildByName("animationPanel")->getChildByName("skipBtn")->addTouchEventListener(this, toucheventselector(IntroScene::onClickSkipBtn));
    
    return true;
}

void IntroScene::updateTimer(float dt) {
	timeCounter--;
	if(timeCounter <= 0) {
		this->unschedule(schedule_selector(IntroScene::updateTimer));
	}
}

void IntroScene::onClickSkipBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, VillageScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}