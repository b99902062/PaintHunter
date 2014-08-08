#ifndef __GESTURE_RECOGNIZER_H__
#define __GESTURE_RECOGNIZER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <cstdlib>
#include <cmath>

#define RESAMPLE_POINT_COUNT 64
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

class GestureRecognizer {
public:
	GestureRecognizer();

private:
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
	void generateIntoTemplate(string, vector<CCPoint>);
	MatchingResult GestureRecognizer::checkMatchingGesture(vector<CCPoint>);

	// For Test
	void initTemplates();

private:
	vector<MatchingTemplate> mTemplates;
};

#endif // __GESTURE_RECOGNIZER_H__
