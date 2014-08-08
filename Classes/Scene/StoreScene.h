#ifndef __Store_SCENE_H__
#define __Store_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::gui;

class StoreScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StoreScene);

private:
};

#endif // __Store_SCENE_H__
