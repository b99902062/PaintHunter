#include "MainlineScene.h"
#include "VillageScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* MainlineScene::scene() {
    CCScene *scene = CCScene::create();
    MainlineScene *layer = MainlineScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainlineScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/MainlineSceneUI/MainlineSceneUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->getChildByName("backBtn")->addTouchEventListener(this, toucheventselector(VillageScene::backToVillage));
	
    this->addChild(uiLayer);

    return true;
}
