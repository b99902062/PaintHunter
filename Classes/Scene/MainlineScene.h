#ifndef __Mainline_SCENE_H__
#define __Mainline_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class MainlineScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MainlineScene);

private:
};

#endif // __Mainline_SCENE_H__
