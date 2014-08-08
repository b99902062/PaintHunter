#ifndef __Home_SCENE_H__
#define __Home_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class HomeScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(HomeScene);

private:
};

#endif // __Home_SCENE_H__
