#pragma once
#include "cocos2d.h"
#include "handle/HandleHelper.h"
using namespace cocos2d;

class CropImage : public Layer{
public:
	static CropImage* create(const std::string& filename);
	virtual bool initWithFile(const std::string& fileName);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void cropImage(const std::function<void(const std::string&)>& callback);
	CropImage();
	virtual ~CropImage();
private:
	float m_scale;
	std::string m_fileName;
	CCSprite * m_sprite;
	DrawNode * m_drawNode;
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;

	int mHandleRadius = 24;
	int mSnapRadius = 3;
	int mBorderThickness = 3;
	int mCornerThickness = 5;
	int mGuideThickness = 1;
	int mCornerLength = 20;

	void initCropWindow(Rect imageRect);
	HandleHelper * mPressedHandleHelper;
	Vec2 mTouchOffset;
	Rect mImageRect;

	void drawBorder();
	void drawCorner();
	void drawGuidelines();
	void drawDarkenedSurroundingArea();

	float m_posY;
};
