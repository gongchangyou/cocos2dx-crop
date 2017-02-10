

#include "CropImage.h"
#include "edge/Edge.h"
#include "util/HandleUtil.h"
using namespace cocos2d;

const Color4F border_color = Color4F(1, 1, 1, 0.5);
const Color4F corner_color = Color4F(1, 1, 1, 1);
const Color4F surrounding_color = Color4F(0, 0, 0, 0.5);
const Color4F fill_color = Color4F(0, 1, 0, 0);
CropImage::CropImage()
:m_scale(0)
,m_fileName("")
,m_drawNode(nullptr)
,m_sprite(nullptr)
,m_posY(0){}

CropImage::~CropImage() {
}

CropImage* CropImage::create(const std::string& filename)
{
    CropImage *sprite = new (std::nothrow) CropImage();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool CropImage::initWithFile(const std::string & filename) {
	m_fileName = filename;
	m_drawNode = DrawNode::create();
	this->addChild(m_drawNode);

	m_drawNode->setPosition(Vec2(0,0));
	
	//touch
	this->setTouchEnabled(true);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CropImage::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CropImage::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CropImage::onTouchEnded, this);
	listener->onTouchCancelled= CC_CALLBACK_2(CropImage::onTouchCancelled, this);
	listener->setSwallowTouches(false);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto s = Director::getInstance()->getWinSize();
	m_sprite = Sprite::create(filename);
	m_scale = s.width / m_sprite->getContentSize().width;
    
	m_sprite->setScale(m_scale);

    m_sprite->setAnchorPoint(Point::ZERO);
    m_sprite->setPosition(Point::ZERO);

	this->addChild(m_sprite, -1);
	mImageRect = m_sprite->getTextureRect();
	initCropWindow(Rect(m_sprite->getPositionX(), m_sprite->getPositionY(), mImageRect.size.width * m_scale, mImageRect.size.height * m_scale));
    
    
	return true;
}

bool CropImage::onTouchBegan(Touch *pTouch, Event *pEvent) {
	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
	float right = Edge::RIGHT_INSTANCE->getCoordinate();
	float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();
	
	auto pos = pTouch->getLocation();
	float x = pos.x;
	float y = pos.y;
	mPressedHandleHelper = HandleUtil::getPressedHandle(x, y, left, top, right, bottom, mHandleRadius);

	// Calculate the offset of the touch point from the precise location of the handle.
	// Save these values in member variable 'mTouchOffset' so that we can maintain this offset as we drag the handle
	if (mPressedHandleHelper != nullptr) {
		HandleUtil::getOffset(mPressedHandleHelper, x, y, left, top, right, bottom, mTouchOffset);
	}
	return true;
}

void CropImage::onTouchMoved(Touch *pTouch, Event *pEvent) {
	if (mPressedHandleHelper == nullptr) {
		return;
	}
	auto pos = pTouch->getLocation();

	float x = pos.x;
	float y = pos.y;
	// Adjust the coordinates for the finger position's offset (i.e. the distance from the initial touch to the precise handle location).
	// We want to maintain the initial touch's distance to the pressed handle so that the crop window size does not "jump".
	x += mTouchOffset.x;
	y += mTouchOffset.y;

	// Calculate the new crop window size/position.
	mPressedHandleHelper->updateCropWindow(x, y, mImageRect, mSnapRadius);
}
void CropImage::onTouchEnded(Touch *pTouch, Event *pEvent) {
	if (mPressedHandleHelper != nullptr) {
		mPressedHandleHelper = nullptr;
	}
}
void CropImage::onTouchCancelled(Touch *pTouch, Event *pEvent) {
	this->onTouchEnded(pTouch, pEvent);
} 

void CropImage::initCropWindow(Rect imageRect) {
	float horizontalPadding = 0.1f * imageRect.size.width;
	float verticalPadding = 0.1f * imageRect.size.height;
	Size size = imageRect.size;
	float left = horizontalPadding;
	float right= size.width - horizontalPadding;
	float bottom = imageRect.origin.y + verticalPadding;
	float top = bottom + size.height- verticalPadding * 2;

	Edge::LEFT_INSTANCE->setCoordinate(left);
	Edge::TOP_INSTANCE->setCoordinate(top);
	Edge::RIGHT_INSTANCE->setCoordinate(right);
	Edge::BOTTOM_INSTANCE->setCoordinate(bottom);
	
	mImageRect = Rect(left - horizontalPadding, bottom -verticalPadding, imageRect.size.width, imageRect.size.height);
}

void CropImage::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
	m_drawNode->clear();
	drawBorder();
	drawCorner();
	drawGuidelines();
	drawDarkenedSurroundingArea();
}

void CropImage::drawBorder() {
	Vec2 points[4];

	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float right = Edge::RIGHT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
	float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();
	points[0] = Vec2(left, bottom);
	points[1] = Vec2(right, bottom);
	points[2] = Vec2(right, top);
	points[3] = Vec2(left, top);
	m_drawNode->drawPolygon(points, 4, fill_color, mBorderThickness, border_color);
	//m_drawNode->drawSegment(Vec2(20, 100), Vec2(100, 100), 5, Color4F(0, 1, 0, 1));
	//m_drawNode->drawSegment(Vec2(20, 200), Vec2(100, 500), 5, Color4F(0, 1, 0, 1));
}

void CropImage::drawCorner() {
	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float right = Edge::RIGHT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
	float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();

	auto left_bottom = Vec2(left, bottom);
	auto right_bottom = Vec2(right, bottom);
	auto right_top = Vec2(right, top);
	auto left_top = Vec2(left, top);
	
	m_drawNode->drawSegment(left_top, left_top - Vec2(0, mCornerLength), mCornerThickness, corner_color);
	m_drawNode->drawSegment(left_top, left_top + Vec2(mCornerLength, 0), mCornerThickness, corner_color);

	m_drawNode->drawSegment(right_top, right_top - Vec2(0, mCornerLength), mCornerThickness, corner_color);
	m_drawNode->drawSegment(right_top, right_top - Vec2(mCornerLength, 0), mCornerThickness, corner_color);

	m_drawNode->drawSegment(right_bottom, right_bottom + Vec2(0, mCornerLength), mCornerThickness, corner_color);
	m_drawNode->drawSegment(right_bottom, right_bottom - Vec2(mCornerLength, 0), mCornerThickness, corner_color);

	m_drawNode->drawSegment(left_bottom, left_bottom + Vec2(0, mCornerLength), mCornerThickness, corner_color);
	m_drawNode->drawSegment(left_bottom, left_bottom + Vec2(mCornerLength, 0), mCornerThickness, corner_color);
}

void CropImage::drawGuidelines() {
	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float right = Edge::RIGHT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
	float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();

	float oneThirdCropWidth = Edge::getWidth() / 3;
	
	float x1 = left + oneThirdCropWidth;
	m_drawNode->drawSegment(Vec2(x1, bottom), Vec2(x1,top), mGuideThickness, border_color);
	float x2 = right - oneThirdCropWidth;
	m_drawNode->drawSegment(Vec2(x2, bottom), Vec2(x2,top), mGuideThickness, border_color);

	// Draw horizontal guidelines.
	float oneThirdCropHeight = Edge::getHeight() / 3;

	float y1 = top - oneThirdCropHeight;
	m_drawNode->drawSegment(Vec2(left, y1), Vec2(right, y1), mGuideThickness, border_color);
	float y2 = bottom + oneThirdCropHeight;
	m_drawNode->drawSegment(Vec2(left, y2), Vec2(right, y2), mGuideThickness, border_color);
}
void CropImage::drawDarkenedSurroundingArea() {

	Rect bitmapRect = mImageRect;

	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float right = Edge::RIGHT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
	float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();

	/*-
	-------------------------------------
	|                top                |
	-------------------------------------
	|      |                    |       |
	|      |                    |       |
	| left |                    | right |
	|      |                    |       |
	|      |                    |       |
	-------------------------------------
	|              bottom               |
	-------------------------------------
	*/

	// Draw "top", "bottom", "left", then "right" quadrants according to diagram above.
	float originLeft = mImageRect.origin.x;
	float originRight = mImageRect.origin.x + mImageRect.size.width;
	float originTop = mImageRect.origin.y + mImageRect.size.height;
	float originBottom = mImageRect.origin.y;
	Vec2 points0[4];
	points0[0] = Vec2(originLeft, originTop);
	points0[1] = Vec2(originRight, originTop);
	points0[2] = Vec2(originRight, top);
	points0[3] = Vec2(originLeft, top);
	m_drawNode->drawPolygon(points0, 4, surrounding_color, mGuideThickness, fill_color);

	Vec2 points1[4];
	points1[0] = Vec2(right, top);
	points1[1] = Vec2(originRight, top);
	points1[2] = Vec2(originRight, bottom);
	points1[3] = Vec2(right, bottom);
	m_drawNode->drawPolygon(points1, 4, surrounding_color, mGuideThickness, fill_color);

	Vec2 points2[4];
	points2[0] = Vec2(originLeft, bottom);
	points2[1] = Vec2(originRight, bottom);
	points2[2] = Vec2(originRight, originBottom);
	points2[3] = Vec2(originLeft, originBottom);
	m_drawNode->drawPolygon(points2, 4, surrounding_color, mGuideThickness, fill_color);

	Vec2 points3[4];
	points3[0] = Vec2(originLeft, top );
	points3[1] = Vec2(left, top );
	points3[2] = Vec2(left, bottom );
	points3[3] = Vec2(originLeft, bottom );
	m_drawNode->drawPolygon(points3, 4, surrounding_color, mGuideThickness, fill_color);
}

void CropImage::cropImage(const std::function<void(const std::string&)>& callback){
	float left = Edge::LEFT_INSTANCE->getCoordinate();
	float top = Edge::TOP_INSTANCE->getCoordinate();
//    float bottom =Edge::BOTTOM_INSTANCE->getCoordinate();
	
	RenderTexture * renderTexture = RenderTexture::create(Edge::getWidth() / m_scale, Edge::getHeight() / m_scale);
    float originY = m_sprite->getContentSize().height;
    auto spriteTmp = Sprite::create(m_fileName, Rect(left/m_scale , originY - top/m_scale,  Edge::getWidth()/m_scale, Edge::getHeight()/m_scale));
	spriteTmp->setAnchorPoint(Point::ZERO);
	
	renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	spriteTmp->visit();
	renderTexture->end();
	std::string crop_image_name = "crop.png";
	if (renderTexture->saveToFile(crop_image_name, Image::Format::PNG)) {
		auto fullPath = FileUtils::getInstance()->getWritablePath() + crop_image_name;
        log("fulpat=%s",fullPath.c_str());
		//使用schedule在下一帧中调用callback函数  
		auto scheduleCallback = [&, fullPath, callback](float dt) {
			callback(fullPath);
            
            //refresh image
            Director::getInstance()->getTextureCache()->reloadTexture(fullPath);
            Texture2D * texture = TextureCache::sharedTextureCache()->addImage(fullPath);
            m_sprite->setTexture(texture);
            m_sprite->setTextureRect(Rect(0,0, texture->getContentSize().width, texture->getContentSize().height));
            auto s = Director::getInstance()->getWinSize();
            m_scale = s.width / m_sprite->getContentSize().width;
            m_sprite->setScale(m_scale);
        };
		auto _schedule = Director::getInstance()->getRunningScene()->getScheduler();
		_schedule->schedule(scheduleCallback, this, 0.0f, 0, 0.0f, false, "crop");
	}

	return;
}
