#include "StartScene.h"
#include "IntroScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* StartScene::scene() {
    CCScene *scene = CCScene::create();
    StartScene *layer = StartScene::create();
    scene->addChild(layer);
    return scene;
}

bool StartScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
	logoTimeCounter = 3;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/StartSceneUI/StartSceneUI.json"));  

	rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->getChildByName("logoPanel")->setVisible(true);
	rootPanel->getChildByName("titlePanel")->setVisible(false);

    this->addChild(uiLayer);

	this->schedule(schedule_selector(StartScene::logoUpdateTimer), 1.0f);
    
    return true;
}

void StartScene::logoUpdateTimer(float dt) {
	logoTimeCounter--;
	if(logoTimeCounter <= 0) {
		this->unschedule(schedule_selector(StartScene::logoUpdateTimer));
		rootPanel->getChildByName("logoPanel")->setVisible(false);
		rootPanel->getChildByName("titlePanel")->setVisible(true);
		UIImageView* ordinaryBtn = (UIImageView*) rootPanel->getChildByName("titlePanel")->getChildByName("userPanel")->getChildByName("ordinaryBtn");
		ordinaryBtn->addTouchEventListener(this, toucheventselector(StartScene::onClickOrdinaryBtn));
	}
}

void StartScene::onClickOrdinaryBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, IntroScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}