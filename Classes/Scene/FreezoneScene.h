#ifndef __Freezone_SCENE_H__
#define __Freezone_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class FreezoneScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(FreezoneScene);

private:
};

#endif // __Freezone_SCENE_H__
