#include "MenuScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* MenuScene::scene() {
    CCScene *scene = CCScene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/MenuUI/MenuUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->addTouchEventListener(this, toucheventselector(MenuScene::BGTouchEvent));

    this->addChild(uiLayer);
    
    return true;
}

void MenuScene::BGTouchEvent(CCObject* obj, TouchEventType type) {
	/*
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, StageSelectionScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
	*/
}