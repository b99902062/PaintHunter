#include "GestureScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* GestureScene::scene() {
    CCScene *scene = CCScene::create();
    GestureScene *layer = GestureScene::create();
    scene->addChild(layer);
    return scene;
}

bool GestureScene::init() {
    if ( !CCLayer::init() ) 
        return false;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	this->setTouchEnabled(true);

	UILayer* uiLayer = UILayer::create();  
    uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("UI/GestureUI/GestureUI.json"));  

	UIPanel* rootPanel = (UIPanel*)uiLayer->getWidgetByName("rootPanel");
	xLabel = (UILabel*) rootPanel->getChildByName("touchXLabel");
	yLabel = (UILabel*) rootPanel->getChildByName("touchYLabel");
	resultLabel = (UILabel*) rootPanel->getChildByName("gestureNameLabel");

    this->addChild(uiLayer, 2);

	initTemplates();

    return true;
}

void GestureScene::draw() {
	if(isOnTouch) {
		ccPointSize(15);  
		ccDrawColor4B(255, 255, 0, 64);  

		if(mPoints.size() > 0) {
			glLineWidth( 50.0f );
			ccDrawPoint(mPoints[0]);  
			for(int i=1; i<mPoints.size(); i++)
				ccDrawLine(mPoints[i-1], mPoints[i]);
		}
		
	}
}

void GestureScene::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -2);
}

void GestureScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
	mPoints.clear();
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);

	isOnTouch = true;
}

void GestureScene::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);
}

void GestureScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint pnt = pTouch->getLocation();
	mPoints.push_back(pnt);
	updateXYLabel(pnt);
	checkMatchingGesture();

	isOnTouch = false;
}

void GestureScene::updateXYLabel(CCPoint pnt) {

	std::ostringstream stringStreamX;
	std::ostringstream stringStreamY;

	stringStreamX << "Touch X: " <<  pnt.x;
	stringStreamY << "Touch Y: " <<  pnt.y;

	xLabel->setText(stringStreamX.str());
	yLabel->setText(stringStreamY.str());
}

vector<CCPoint> GestureScene::resamplePoints(vector<CCPoint> points, int spacedPointsNum) {
	/* n = spacedPointsCount
	   I = spacedParamI
	   D = totalDistance
	*/

	float spacedParamI = getPathLength(points) / (spacedPointsNum-1);
	float totalDistance = 0;

	vector<CCPoint> newPoints;
	newPoints.push_back(points[0]);

	for(int i=1; i<points.size(); i++) {
		float dist = getDistance(points[i-1], points[i]);
		if(totalDistance + dist >= spacedParamI) {
			float scale = (spacedParamI - totalDistance) / dist;
			float qx = points[i-1].x + scale * (points[i].x - points[i-1].x);
			float qy = points[i-1].y + scale * (points[i].y - points[i-1].y);
			CCPoint qPnt = CCPoint(qx, qy);
			newPoints.push_back(qPnt);
			points.insert(points.begin() + i, qPnt);
			totalDistance = 0;
		}
		else {
			totalDistance += dist;
		}
	}
	return newPoints;
}

float GestureScene::getPathLength(vector<CCPoint> points) {
	float dist = 0;
	for(int i=1; i < points.size(); i++) 
		dist += getDistance(points[i], points[i-1]);
	return dist;
}

float GestureScene::getDistance(CCPoint pnt1, CCPoint pnt2) {
	float dx = pnt1.x - pnt2.x;
	float dy = pnt1.y - pnt2.y;
	return sqrt(dx * dx + dy * dy);
}

float GestureScene::getInadicativeAngle(vector<CCPoint> points) {
	CCPoint centroid = getCentroid(points);
	return atan2(centroid.y - points[0].y, centroid.x - points[0].x);
}

CCPoint GestureScene::getCentroid(vector<CCPoint> points) {
	float cx = 0;
	float cy = 0;
	for(vector<CCPoint>::iterator it = points.begin(); it != points.end(); it++) {
		cx += it->x / points.size(); 
		cx += it->y / points.size(); 
	}
	return CCPoint(cx, cy);
}

vector<CCPoint> GestureScene::rotatePointsBy(vector<CCPoint> points, float w) {
	vector<CCPoint> newPoints;
	CCPoint centroid = getCentroid(points);
	float cosw = cos(w);
	float sinw = sin(w);
	for(int i=0; i<points.size(); i++) {
		float qx = (points[i].x - centroid.x) * cosw - (points[i].y - centroid.y) * sinw + centroid.x;
		float qy = (points[i].x - centroid.x) * sinw + (points[i].y - centroid.y) * cosw + centroid.y;
		newPoints.push_back(CCPoint(qx, qy));
	}
	return newPoints;
}

vector<CCPoint> GestureScene::scalePointsTo(vector<CCPoint> points, float scaleSize) {
	/** Get bounding box */
	float bboxMinX = INFINITY;
	float bboxMinY = INFINITY; 
	float bboxMaxX = -INFINITY;
	float bboxMaxY = -INFINITY;

	for(int i=0; i<points.size(); i++) {
		if(points[i].x < bboxMinX)
			bboxMinX = points[i].x;
		if(points[i].y < bboxMinY)
			bboxMinY = points[i].y;
		if(points[i].x > bboxMaxX)
			bboxMaxX = points[i].x;
		if(points[i].y > bboxMaxY)
			bboxMaxY = points[i].y;
	}
	float bWidth = bboxMaxX - bboxMinX;
	float bHeight = bboxMaxY - bboxMinY;
	
	vector<CCPoint> newPoints;

	for(int i=0; i<points.size(); i++) {
		float qx = points[i].x * scaleSize / bWidth;
		float qy = points[i].y * scaleSize / bHeight;
		newPoints.push_back(CCPoint(qx, qy));
	}
	return newPoints;
}

vector<CCPoint> GestureScene::translatePointsTo(vector<CCPoint> points, CCPoint kPoint) {
	CCPoint centroid = getCentroid(points);
	
	vector<CCPoint> newPoints;

	for(int i=0; i<points.size(); i++) {
		float qx = points[i].x + kPoint.x - centroid.x;
		float qy = points[i].y + kPoint.y - centroid.y;
		newPoints.push_back(CCPoint(qx, qy));
	}
	return newPoints;
}

MatchingResult GestureScene::doRecognize(vector<CCPoint> points, vector<MatchingTemplate> templates) {
	float b = INFINITY;
	int bestTemplateIndex = -1;

	for(int i=0; i<templates.size(); i++) {
		float d = getDistAtBestAngle(points, templates[i], THETA_A, THETA_B, THETA_C);
		if(d < b) {
			b = d;
			bestTemplateIndex = i;
		}
	}
	float score = 1 - b / HALF_DIAGONAL;
	return MatchingResult(templates[bestTemplateIndex].templateName, score);
}

float GestureScene::getDistAtBestAngle(vector<CCPoint> points, MatchingTemplate t, float theta1, float theta2, float theta3) {
	float x1 = PARAM_PHI * theta1 + (1 - PARAM_PHI) * theta2;
	float f1 = getDistAtAngle(points, t, x1);
	float x2 = (1 - PARAM_PHI) * theta1 + PARAM_PHI * theta2;
	float f2 = getDistAtAngle(points, t, x2);
	while(fabs(theta2 - theta1) > theta3) {
		if(f1 < f2) {
			theta2 = x2;
			x2 = x1;
			f2 = f1;
			x1 = PARAM_PHI * theta1 + (1 - PARAM_PHI) * theta2;
			f1 = getDistAtAngle(points, t, x1);
		}
		else {
			theta1 = x1;
			x1 = x2;
			f1 = f2;
			x2 = (1 - PARAM_PHI) * theta1 + PARAM_PHI * theta2;
			f2 = getDistAtAngle(points, t, x2);
		}
	}
	return min(f1, f2);
}

float GestureScene::getDistAtAngle(vector<CCPoint> points, MatchingTemplate t, float theta) {
	vector<CCPoint> newPoints = rotatePointsBy(points, theta);
	float dist = getPathDist(newPoints, t.samples);
	return dist;
}

float GestureScene::getPathDist(vector<CCPoint> pointsA, vector<CCPoint> pointsB) {
	float dist = 0;
	for(int i=0; i<pointsA.size(); i++) 
		dist += getDistance(pointsA[i], pointsB[i]);
	return dist / pointsA.size();
}

void GestureScene::checkMatchingGesture() {
	mPoints = resamplePoints(mPoints, 64);
	mPoints = rotatePointsBy(mPoints, -getInadicativeAngle(mPoints));
	mPoints = scalePointsTo(mPoints, 250);
	mPoints = translatePointsTo(mPoints, CCPoint(0, 0));
	MatchingResult result = doRecognize(mPoints, mTemplates);
	
	int percentage = result.templateScore * 100;

	std::ostringstream stringStream;
	stringStream << "Gesture: " <<  result.templateName << "(" << percentage << " % )";
	resultLabel->setText(stringStream.str());
}

void GestureScene::initTemplates() {
	CCPoint rectPoints[] = {CCPoint(286.336243, 497.743988), CCPoint(286.336243, 489.771454), CCPoint(286.336243, 483.825806), CCPoint(286.336243, 467.813141), CCPoint(286.336243, 457.813660), CCPoint(286.336243, 445.854828), CCPoint(286.336243, 425.855896), CCPoint(286.336243, 413.897064), CCPoint(288.363159, 387.952484), CCPoint(292.416992, 359.980988), CCPoint(296.335724, 332.009491), CCPoint(296.335724, 320.050659), CCPoint(296.335724, 308.091858), CCPoint(296.335724, 302.078644), CCPoint(296.335724, 292.079163), CCPoint(296.335724, 286.133545), CCPoint(296.335724, 274.107147), CCPoint(296.335724, 268.161530), CCPoint(296.335724, 262.148315), CCPoint(296.335724, 260.121399), CCPoint(296.335724, 258.162048), CCPoint(296.335724, 256.135132), CCPoint(296.335724, 254.175766), CCPoint(296.335724, 252.148849), CCPoint(302.416473, 252.148849), CCPoint(306.335175, 252.148849), CCPoint(322.415405, 256.135132), CCPoint(364.305084, 256.135132), CCPoint(380.250183, 254.175766), CCPoint(420.248077, 246.135651), CCPoint(436.193176, 236.203751), CCPoint(470.110321, 228.163635), CCPoint(486.190552, 228.163635), CCPoint(492.136169, 228.163635), CCPoint(498.081818, 228.163635), CCPoint(504.162567, 228.163635), CCPoint(510.108215, 228.163635), CCPoint(516.053833, 228.163635), CCPoint(522.134583, 228.163635), CCPoint(528.080261, 228.163635), CCPoint(534.025879, 228.163635), CCPoint(544.025330, 228.163635), CCPoint(556.051758, 228.163635), CCPoint(561.997375, 228.163635), CCPoint(569.969910, 228.163635), CCPoint(571.996826, 228.163635), CCPoint(577.942444, 228.163635), CCPoint(584.023254, 228.163635), CCPoint(585.915039, 228.163635), CCPoint(587.941956, 228.163635), CCPoint(587.941956, 234.176834), CCPoint(587.941956, 240.190018), CCPoint(587.941956, 246.135651), CCPoint(587.941956, 256.135132), CCPoint(587.941956, 268.161530), CCPoint(587.941956, 274.107147), CCPoint(587.941956, 284.106628), CCPoint(587.941956, 294.106110), CCPoint(587.941956, 310.051208), CCPoint(587.941956, 322.077606), CCPoint(587.941956, 332.009491), CCPoint(587.941956, 342.008972), CCPoint(591.995789, 353.967804), CCPoint(593.887573, 371.939819), CCPoint(597.941406, 403.897614), CCPoint(597.941406, 419.910278), CCPoint(597.941406, 431.869080), CCPoint(597.941406, 437.882294), CCPoint(597.941406, 443.827911), CCPoint(597.941406, 449.841125), CCPoint(597.941406, 455.854309), CCPoint(597.941406, 461.799957), CCPoint(597.941406, 467.813141), CCPoint(597.941406, 473.826324), CCPoint(597.941406, 479.771973), CCPoint(597.941406, 485.785156), CCPoint(597.941406, 491.798370), CCPoint(597.941406, 493.757721), CCPoint(597.941406, 499.770905), CCPoint(597.941406, 501.797821), CCPoint(597.941406, 507.743469), CCPoint(597.941406, 509.770386), CCPoint(597.941406, 511.729736), CCPoint(597.941406, 513.756653), CCPoint(597.941406, 519.702271), CCPoint(597.941406, 525.715515), CCPoint(597.941406, 527.742432), CCPoint(595.914490, 527.742432), CCPoint(583.888123, 521.729187), CCPoint(577.942444, 517.742920), CCPoint(549.970947, 509.770386), CCPoint(486.055420, 501.797821), CCPoint(452.138306, 501.797821), CCPoint(442.138824, 501.797821), CCPoint(432.139343, 501.797821), CCPoint(426.193726, 501.797821), CCPoint(416.194244, 501.797821), CCPoint(410.113495, 501.797821), CCPoint(388.222748, 501.797821), CCPoint(372.277649, 501.797821), CCPoint(362.278168, 501.797821), CCPoint(350.251770, 501.797821), CCPoint(344.306152, 501.797821), CCPoint(338.225403, 501.797821), CCPoint(336.333588, 501.797821), CCPoint(330.252838, 501.797821), CCPoint(324.307190, 501.797821), CCPoint(318.361572, 499.770905), CCPoint(310.253906, 499.770905), CCPoint(308.362091, 499.770905), CCPoint(308.362091, 499.770905)};
	CCPoint circlePoints[] = {CCPoint(406.194763, 174.247559), CCPoint(404.302979, 174.247559), CCPoint(402.276062, 174.247559), CCPoint(400.249146, 174.247559), CCPoint(398.222229, 174.247559), CCPoint(396.330444, 174.247559), CCPoint(394.303497, 174.247559), CCPoint(392.276581, 174.247559), CCPoint(390.249664, 174.247559), CCPoint(388.222748, 174.247559), CCPoint(384.304047, 174.247559), CCPoint(382.277130, 174.247559), CCPoint(378.223267, 174.247559), CCPoint(372.277649, 174.247559), CCPoint(364.305084, 174.247559), CCPoint(358.359467, 174.247559), CCPoint(352.278687, 174.247559), CCPoint(352.278687, 176.274475), CCPoint(346.333069, 176.274475), CCPoint(340.387451, 176.274475), CCPoint(338.360504, 178.301392), CCPoint(332.414886, 180.260757), CCPoint(330.387970, 180.260757), CCPoint(326.334137, 182.287674), CCPoint(320.388489, 186.273956), CCPoint(318.361572, 188.233307), CCPoint(316.334656, 188.233307), CCPoint(310.389008, 192.219589), CCPoint(308.362091, 194.246506), CCPoint(302.416473, 200.259705), CCPoint(292.416992, 210.191605), CCPoint(290.390076, 212.218521), CCPoint(290.390076, 214.245453), CCPoint(286.471375, 220.191086), CCPoint(284.444458, 222.218002), CCPoint(280.390594, 228.163635), CCPoint(274.444977, 242.149384), CCPoint(272.418060, 250.189499), CCPoint(270.391144, 252.148849), CCPoint(268.364227, 252.148849), CCPoint(268.364227, 254.175766), CCPoint(266.472412, 256.135132), CCPoint(266.472412, 258.162048), CCPoint(264.445496, 262.148315), CCPoint(264.445496, 268.161530), CCPoint(264.445496, 272.147797), CCPoint(262.418579, 276.134064), CCPoint(262.418579, 284.106628), CCPoint(262.418579, 290.119812), CCPoint(262.418579, 296.065460), CCPoint(258.364746, 302.078644), CCPoint(258.364746, 308.091858), CCPoint(258.364746, 314.037476), CCPoint(258.364746, 320.050659), CCPoint(258.364746, 326.063873), CCPoint(258.364746, 332.009491), CCPoint(258.364746, 342.008972), CCPoint(258.364746, 348.022156), CCPoint(258.364746, 353.967804), CCPoint(258.364746, 359.980988), CCPoint(258.364746, 369.980469), CCPoint(258.364746, 379.979950), CCPoint(258.364746, 391.938782), CCPoint(258.364746, 397.951965), CCPoint(258.364746, 407.883881), CCPoint(258.364746, 417.883331), CCPoint(258.364746, 423.896545), CCPoint(258.364746, 433.896027), CCPoint(258.364746, 435.855377), CCPoint(258.364746, 439.841644), CCPoint(258.364746, 441.868561), CCPoint(258.364746, 447.881775), CCPoint(260.391663, 447.881775), CCPoint(260.391663, 453.827393), CCPoint(260.391663, 459.840576), CCPoint(262.418579, 465.853790), CCPoint(264.445496, 467.813141), CCPoint(268.364227, 473.826324), CCPoint(268.364227, 475.785706), CCPoint(270.391144, 479.771973), CCPoint(272.418060, 481.798889), CCPoint(282.282410, 489.771454), CCPoint(288.363159, 497.743988), CCPoint(292.281860, 503.757202), CCPoint(298.362640, 513.756653), CCPoint(304.308258, 515.716003), CCPoint(306.335175, 517.742920), CCPoint(308.362091, 517.742920), CCPoint(314.307739, 519.702271), CCPoint(322.280273, 523.756165), CCPoint(326.334137, 525.715515), CCPoint(340.252319, 531.728699), CCPoint(344.306152, 531.728699), CCPoint(346.197937, 533.688049), CCPoint(348.224854, 533.688049), CCPoint(350.251770, 533.688049), CCPoint(350.251770, 535.714966), CCPoint(356.197418, 535.714966), CCPoint(362.278168, 537.674316), CCPoint(374.169434, 537.674316), CCPoint(380.250183, 537.674316), CCPoint(386.195831, 537.674316), CCPoint(392.141449, 537.674316), CCPoint(398.222229, 537.674316), CCPoint(408.221680, 537.674316), CCPoint(418.086029, 537.674316), CCPoint(434.166260, 537.674316), CCPoint(450.111389, 537.674316), CCPoint(462.137756, 537.674316), CCPoint(478.082886, 541.728149), CCPoint(488.082336, 541.728149), CCPoint(499.973602, 539.701233), CCPoint(502.000519, 539.701233), CCPoint(504.027435, 539.701233), CCPoint(504.027435, 537.674316), CCPoint(509.973083, 535.714966), CCPoint(516.053833, 531.728699), CCPoint(534.025879, 523.756165), CCPoint(539.971497, 521.729187), CCPoint(557.943542, 509.770386), CCPoint(569.969910, 495.784637), CCPoint(575.915527, 489.771454), CCPoint(579.969360, 481.798889), CCPoint(587.941956, 469.840057), CCPoint(591.860657, 461.799957), CCPoint(597.941406, 455.854309), CCPoint(601.860107, 449.841125), CCPoint(601.860107, 447.881775), CCPoint(605.913940, 441.868561), CCPoint(605.913940, 439.841644), CCPoint(605.913940, 437.882294), CCPoint(605.913940, 433.896027), CCPoint(605.913940, 431.869080), CCPoint(605.913940, 429.909729), CCPoint(605.913940, 423.896545), CCPoint(605.913940, 421.869629), CCPoint(605.913940, 417.883331), CCPoint(605.913940, 411.937714), CCPoint(605.913940, 409.910797), CCPoint(605.913940, 405.924530), CCPoint(605.913940, 399.911316), CCPoint(605.913940, 391.938782), CCPoint(605.913940, 385.925568), CCPoint(603.887024, 385.925568), CCPoint(603.887024, 379.979950), CCPoint(599.968323, 373.966736), CCPoint(599.968323, 371.939819), CCPoint(599.968323, 367.953552), CCPoint(599.968323, 362.007904), CCPoint(593.887573, 353.967804), CCPoint(593.887573, 348.022156), CCPoint(589.968872, 342.008972), CCPoint(585.915039, 330.050140), CCPoint(585.915039, 324.036957), CCPoint(584.023254, 318.023743), CCPoint(579.969360, 306.064911), CCPoint(575.915527, 298.092377), CCPoint(574.023743, 296.065460), CCPoint(569.969910, 290.119812), CCPoint(564.024292, 288.092896), CCPoint(564.024292, 282.079712), CCPoint(557.943542, 278.093414), CCPoint(554.024780, 272.147797), CCPoint(539.971497, 264.107666), CCPoint(536.052795, 258.162048), CCPoint(534.025879, 258.162048), CCPoint(528.080261, 254.175766), CCPoint(526.053284, 254.175766), CCPoint(522.134583, 250.189499), CCPoint(514.026917, 248.162582), CCPoint(514.026917, 246.135651), CCPoint(512.135132, 246.135651), CCPoint(512.135132, 244.176300), CCPoint(506.054382, 242.149384), CCPoint(504.027435, 242.149384), CCPoint(500.108734, 242.149384), CCPoint(498.081818, 240.190018), CCPoint(496.054901, 238.163101), CCPoint(490.109253, 234.176834), CCPoint(488.082336, 234.176834), CCPoint(486.055420, 234.176834), CCPoint(480.109802, 234.176834), CCPoint(478.082886, 232.217468), CCPoint(476.191071, 232.217468), CCPoint(474.164154, 230.190552), CCPoint(468.083405, 230.190552), CCPoint(466.191620, 230.190552), CCPoint(460.110840, 230.190552), CCPoint(460.110840, 228.163635), CCPoint(458.219055, 228.163635), CCPoint(456.192139, 228.163635), CCPoint(456.192139, 226.204269), CCPoint(454.165222, 226.204269), CCPoint(452.138306, 226.204269), CCPoint(452.138306, 224.177353), CCPoint(450.111389, 224.177353), CCPoint(450.111389, 222.218002), CCPoint(448.219574, 222.218002), CCPoint(446.192657, 222.218002), CCPoint(444.165741, 220.191086), CCPoint(442.138824, 220.191086), CCPoint(442.138824, 218.231720), CCPoint(440.247040, 218.231720), CCPoint(438.220123, 218.231720), CCPoint(436.193176, 218.231720), CCPoint(436.193176, 218.231720)};
	CCPoint triPoints[] = {CCPoint(450.111389, 505.784119), CCPoint(448.084442, 503.757202), CCPoint(442.138824, 495.784637), CCPoint(428.085510, 485.785156), CCPoint(408.221680, 459.840576), CCPoint(392.141449, 445.854828), CCPoint(386.195831, 441.868561), CCPoint(376.196350, 427.882813), CCPoint(368.223816, 421.869629), CCPoint(362.278168, 413.897064), CCPoint(360.251251, 413.897064), CCPoint(350.251770, 407.883881), CCPoint(336.333588, 391.938782), CCPoint(324.307190, 373.966736), CCPoint(312.280823, 365.994202), CCPoint(292.416992, 345.995239), CCPoint(268.364227, 326.063873), CCPoint(256.472961, 314.037476), CCPoint(226.474533, 296.065460), CCPoint(212.556351, 282.079712), CCPoint(198.503036, 272.147797), CCPoint(190.530487, 264.107666), CCPoint(178.504089, 250.189499), CCPoint(162.558990, 232.217468), CCPoint(152.559509, 222.218002), CCPoint(144.586960, 214.245453), CCPoint(142.560043, 214.245453), CCPoint(142.560043, 212.218521), CCPoint(146.613876, 212.218521), CCPoint(156.613358, 212.218521), CCPoint(162.558990, 208.232254), CCPoint(184.584854, 208.232254), CCPoint(196.476120, 208.232254), CCPoint(206.475586, 204.245972), CCPoint(256.472961, 196.273422), CCPoint(278.363678, 192.219589), CCPoint(326.334137, 188.233307), CCPoint(374.304565, 184.247025), CCPoint(402.276062, 184.247025), CCPoint(412.275543, 184.247025), CCPoint(428.220642, 184.247025), CCPoint(434.166260, 184.247025), CCPoint(440.247040, 184.247025), CCPoint(446.192657, 184.247025), CCPoint(448.219574, 184.247025), CCPoint(450.246490, 184.247025), CCPoint(454.165222, 184.247025), CCPoint(462.137756, 184.247025), CCPoint(472.137238, 184.247025), CCPoint(488.082336, 188.233307), CCPoint(504.162567, 188.233307), CCPoint(530.107178, 188.233307), CCPoint(541.998413, 188.233307), CCPoint(551.997864, 188.233307), CCPoint(564.024292, 188.233307), CCPoint(579.969360, 188.233307), CCPoint(581.996277, 188.233307), CCPoint(584.023254, 188.233307), CCPoint(589.968872, 188.233307), CCPoint(595.914490, 188.233307), CCPoint(601.995239, 188.233307), CCPoint(615.913452, 188.233307), CCPoint(625.912903, 188.233307), CCPoint(637.939270, 188.233307), CCPoint(643.884949, 188.233307), CCPoint(649.965698, 188.233307), CCPoint(651.857483, 188.233307), CCPoint(655.911316, 188.233307), CCPoint(657.938232, 188.233307), CCPoint(659.830017, 188.233307), CCPoint(667.937744, 188.233307), CCPoint(673.883362, 188.233307), CCPoint(675.910278, 188.233307), CCPoint(681.855896, 188.233307), CCPoint(687.801514, 188.233307), CCPoint(689.828430, 188.233307), CCPoint(689.828430, 190.260223), CCPoint(689.828430, 192.219589), CCPoint(689.828430, 196.273422), CCPoint(687.801514, 196.273422), CCPoint(687.801514, 198.232773), CCPoint(685.909729, 204.245972), CCPoint(683.882813, 206.205338), CCPoint(681.855896, 206.205338), CCPoint(679.828979, 208.232254), CCPoint(679.828979, 210.191605), CCPoint(679.828979, 212.218521), CCPoint(677.802063, 212.218521), CCPoint(677.802063, 214.245453), CCPoint(677.802063, 218.231720), CCPoint(675.910278, 220.191086), CCPoint(669.829529, 226.204269), CCPoint(663.883850, 240.190018), CCPoint(643.884949, 254.175766), CCPoint(635.912354, 266.134613), CCPoint(609.967773, 302.078644), CCPoint(585.915039, 322.077606), CCPoint(571.996826, 334.036407), CCPoint(564.024292, 345.995239), CCPoint(554.024780, 365.994202), CCPoint(538.079712, 379.979950), CCPoint(529.972046, 391.938782), CCPoint(524.026367, 409.910797), CCPoint(518.080750, 421.869629), CCPoint(510.108215, 429.909729), CCPoint(506.054382, 441.868561), CCPoint(490.109253, 461.799957), CCPoint(486.055420, 467.813141), CCPoint(486.055420, 473.826324), CCPoint(486.055420, 475.785706), CCPoint(484.028503, 477.812622), CCPoint(484.028503, 479.771973), CCPoint(484.028503, 481.798889), CCPoint(482.001587, 483.825806), CCPoint(482.001587, 489.771454), CCPoint(482.001587, 491.798370), CCPoint(480.109802, 491.798370), CCPoint(480.109802, 493.757721), CCPoint(478.082886, 499.770905), CCPoint(476.055939, 501.797821), CCPoint(474.029022, 503.757202), CCPoint(474.029022, 505.784119), CCPoint(472.137238, 505.784119), CCPoint(470.110321, 505.784119), CCPoint(470.110321, 505.784119)};
	
	vector<CCPoint> rectSamples (rectPoints, rectPoints + sizeof(rectPoints) / sizeof(CCPoint));
	vector<CCPoint> circleSamples (circlePoints, circlePoints + sizeof(circlePoints) / sizeof(CCPoint));
	vector<CCPoint> triSamples (triPoints, triPoints + sizeof(triPoints) / sizeof(CCPoint));

	generateIntoTemplate("Rectangle", rectSamples);
	generateIntoTemplate("Circle", circleSamples);
	generateIntoTemplate("Triangle", triSamples);
}

void GestureScene::generateIntoTemplate(string name, vector<CCPoint> points) {
	points = resamplePoints(points, 64);
	points = rotatePointsBy(points, -getInadicativeAngle(points));
	points = scalePointsTo(points, 250);
	points = translatePointsTo(points, CCPoint(0, 0));

	mTemplates.push_back(MatchingTemplate(name, points));
}

MatchingResult::MatchingResult(string name, float score) {
	this->templateName = name;
	this->templateScore = score;
}

MatchingTemplate::MatchingTemplate(string name, vector<CCPoint> points) {
	this->templateName = name;
	this->samples = points;
}
