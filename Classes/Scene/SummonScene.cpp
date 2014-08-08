#include "SummonScene.h"
#include "VillageScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* SummonScene::scene() {
    CCScene *scene = CCScene::create();
    SummonScene *layer = SummonScene::create();
    scene->addChild(layer);
    return scene;
}

bool SummonScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/SummonSceneUI/SummonSceneUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->getChildByName("backBtn")->addTouchEventListener(this, toucheventselector(VillageScene::backToVillage));
	
    this->addChild(uiLayer);

    return true;
}
