#ifndef __Start_SCENE_H__
#define __Start_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class StartScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();

    CREATE_FUNC(StartScene);

private:
	UIPanel* rootPanel;
	void logoUpdateTimer(float);
	void onClickOrdinaryBtn(CCObject*, TouchEventType);

private:
	int logoTimeCounter;
};

#endif // __Start_SCENE_H__
