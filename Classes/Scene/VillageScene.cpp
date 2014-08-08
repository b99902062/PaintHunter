#include "VillageScene.h"
#include "MainlineScene.h"
#include "FreezoneScene.h"
#include "StoreScene.h"
#include "HomeScene.h"
#include "SummonScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* VillageScene::scene() {
    CCScene *scene = CCScene::create();
    VillageScene *layer = VillageScene::create();
    scene->addChild(layer);
    return scene;
}

bool VillageScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/VillageSceneUI/VillageSceneUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	rootPanel->getChildByName("backgroundScrollView")->getChildByName("mainlineBtn")->addTouchEventListener(this, toucheventselector(VillageScene::onClickMainlineBtn));
	rootPanel->getChildByName("backgroundScrollView")->getChildByName("freezoneBtn")->addTouchEventListener(this, toucheventselector(VillageScene::onClickFreezoneBtn));
	rootPanel->getChildByName("backgroundScrollView")->getChildByName("storeBtn")->addTouchEventListener(this, toucheventselector(VillageScene::onClickStoreBtn));
	rootPanel->getChildByName("backgroundScrollView")->getChildByName("homeBtn")->addTouchEventListener(this, toucheventselector(VillageScene::onClickHomeBtn));
	rootPanel->getChildByName("backgroundScrollView")->getChildByName("summonBtn")->addTouchEventListener(this, toucheventselector(VillageScene::onClickSummonBtn));

    this->addChild(uiLayer);

    return true;
}

void VillageScene::onClickMainlineBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, MainlineScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void VillageScene::onClickFreezoneBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, FreezoneScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void VillageScene::onClickStoreBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, StoreScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void VillageScene::onClickHomeBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, HomeScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void VillageScene::onClickSummonBtn(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, SummonScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void VillageScene::backToVillage(CCObject *pSender, TouchEventType type) {
	CCTransitionScene* transition = CCTransitionPageTurn::create(0.5f, VillageScene::scene(), false);
	CCDirector::sharedDirector()->replaceScene(transition);
}