
#include "Edge.h"


const float Edge::POSITIVE_INFINITY = 100000.f;
const float Edge::NEGATIVE_INFINITY = -100000.f;

void Edge::adjustCoordinate(float aspectRatio) {

	float left = LEFT_INSTANCE->getCoordinate();
	float top = TOP_INSTANCE->getCoordinate();
	float right = RIGHT_INSTANCE->getCoordinate();
	float bottom = BOTTOM_INSTANCE->getCoordinate();

	switch (m_type) {
	case EdgeType::LEFT:
		m_coordinate = AspectRatioUtil::calculateLeft(top, right, bottom, aspectRatio);
		break;
	case EdgeType::TOP:
		m_coordinate = AspectRatioUtil::calculateTop(left, right, bottom, aspectRatio);
		break;
	case EdgeType::RIGHT:
		m_coordinate = AspectRatioUtil::calculateRight(left, top, bottom, aspectRatio);
		break;
	case EdgeType::BOTTOM:
		m_coordinate = AspectRatioUtil::calculateBottom(left, top, right, aspectRatio);
		break;
	}
}
