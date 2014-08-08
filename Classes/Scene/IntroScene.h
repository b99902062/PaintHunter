#ifndef __Intro_SCENE_H__
#define __Intro_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class IntroScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(IntroScene);

private:
	void updateTimer(float);
	void onClickSkipBtn(CCObject*, TouchEventType);

private:
	int timeCounter;
};

#endif // __Intro_SCENE_H__
