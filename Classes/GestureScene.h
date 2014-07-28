#ifndef __Gesture_SCENE_H__
#define __Gesture_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <cstdlib>
#include <cmath>

#define SCALE_SIZE 250
#define PARAM_PHI 0.618034
#define HALF_DIAGONAL 176.776695
#define THETA_A -0.78539815
#define THETA_B 0.78539815
#define THETA_C 0.0349065844444444

using namespace cocos2d;
using namespace cocos2d::gui;
using namespace std;

class MatchingResult {
public:
	MatchingResult(string, float);

public: 
	string templateName;
	float templateScore;
};

class MatchingTemplate {
public:
	MatchingTemplate(string, vector<CCPoint>);

public:
	string templateName;
	vector<CCPoint> samples;
};

class GestureScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();

    CREATE_FUNC(GestureScene);
	void BGTouchEvent(CCObject* obj, TouchEventType type);

	virtual void registerWithTouchDispatcher(void);

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	virtual void draw();  

private:
	void updateXYLabel(CCPoint);
	vector<CCPoint> resamplePoints(vector<CCPoint>, int);
	float getPathLength(vector<CCPoint>);
	float getDistance(CCPoint, CCPoint);
	float getInadicativeAngle(vector<CCPoint>);
	CCPoint getCentroid(vector<CCPoint>);
	vector<CCPoint> rotatePointsBy(vector<CCPoint>, float);
	vector<CCPoint> scalePointsTo(vector<CCPoint>, float);
	vector<CCPoint> translatePointsTo(vector<CCPoint>, CCPoint);
	MatchingResult doRecognize(vector<CCPoint>, vector<MatchingTemplate>);
	float getDistAtBestAngle(vector<CCPoint>, MatchingTemplate, float, float, float);
	float getDistAtAngle(vector<CCPoint>, MatchingTemplate, float);
	float getPathDist(vector<CCPoint>, vector<CCPoint>);
	void initTemplates();
	void checkMatchingGesture();
	void generateIntoTemplate(string, vector<CCPoint>);

private:
	vector<CCPoint> mPoints;
	UILabel* xLabel;
	UILabel* yLabel;
	UILabel* resultLabel;
	vector<MatchingTemplate> mTemplates;
	bool isOnTouch;
	
};

#endif // __Gesture_SCENE_H__
