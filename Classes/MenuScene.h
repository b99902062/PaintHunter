#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class MenuScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();

    CREATE_FUNC(MenuScene);
	void gestureBtnTouchEvent(CCObject* obj, TouchEventType type);
};

#endif // __MENU_SCENE_H__
