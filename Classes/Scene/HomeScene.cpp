#include "HomeScene.h"
#include "VillageScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* HomeScene::scene() {
    CCScene *scene = CCScene::create();
    HomeScene *layer = HomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool HomeScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/HomeSceneUI/HomeSceneUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->getChildByName("backBtn")->addTouchEventListener(this, toucheventselector(VillageScene::backToVillage));
	
    this->addChild(uiLayer);

    return true;
}
