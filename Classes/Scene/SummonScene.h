#ifndef __Summon_SCENE_H__
#define __Summon_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class SummonScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(SummonScene);

private:
};

#endif // __Summon_SCENE_H__
