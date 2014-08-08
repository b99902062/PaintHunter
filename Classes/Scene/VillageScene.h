#ifndef __Village_SCENE_H__
#define __Village_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class VillageScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(VillageScene);

	void backToVillage(CCObject*, TouchEventType);

private:
	void onClickMainlineBtn(CCObject*, TouchEventType);
	void onClickFreezoneBtn(CCObject*, TouchEventType);
	void onClickStoreBtn(CCObject*, TouchEventType);
	void onClickHomeBtn(CCObject*, TouchEventType);
	void onClickSummonBtn(CCObject*, TouchEventType);

	
};

#endif // __Village_SCENE_H__
