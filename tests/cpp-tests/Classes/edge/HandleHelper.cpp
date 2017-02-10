
#include "HandleHelper.h"
#include "CornerHandleHelper.h"
#include "CenterHandleHelper.h"
#include "VerticalHandleHelper.h"
#include "HorizontalHandleHelper.h"

Edge * Edge::LEFT_INSTANCE = new Edge(EdgeType::LEFT);
Edge * Edge::RIGHT_INSTANCE = new Edge(EdgeType::RIGHT);
Edge * Edge::TOP_INSTANCE = new Edge(EdgeType::TOP);
Edge * Edge::BOTTOM_INSTANCE = new Edge(EdgeType::BOTTOM);

HandleHelper * HandleHelper::TOP_LEFT = new CornerHandleHelper(Edge::TOP_INSTANCE, Edge::LEFT_INSTANCE, HandleType::HANDLE_TOP_LEFT);

HandleHelper * HandleHelper::TOP_RIGHT = new CornerHandleHelper(Edge::TOP_INSTANCE, Edge::RIGHT_INSTANCE, HandleType::HANDLE_TOP_RIGHT);
HandleHelper * HandleHelper::BOTTOM_LEFT = new CornerHandleHelper(Edge::BOTTOM_INSTANCE, Edge::LEFT_INSTANCE, HandleType::HANDLE_BOTTOM_LEFT);
HandleHelper * HandleHelper::BOTTOM_RIGHT = new CornerHandleHelper(Edge::BOTTOM_INSTANCE, Edge::RIGHT_INSTANCE, HandleType::HANDLE_BOTTOM_RIGHT);
HandleHelper * HandleHelper::LEFT = new VerticalHandleHelper(Edge::LEFT_INSTANCE, HandleType::HANDLE_LEFT);
HandleHelper * HandleHelper::TOP = new HorizontalHandleHelper(Edge::TOP_INSTANCE, HandleType::HANDLE_TOP);
HandleHelper * HandleHelper::RIGHT = new VerticalHandleHelper(Edge::RIGHT_INSTANCE, HandleType::HANDLE_RIGHT);
HandleHelper * HandleHelper::BOTTOM = new HorizontalHandleHelper(Edge::BOTTOM_INSTANCE, HandleType::HANDLE_BOTTOM);
HandleHelper * HandleHelper::CENTER = new CenterHandleHelper();

HandleHelper::HandleHelper(Edge * horizontalEdge, Edge * verticalEdge , const HandleType handleType) {
	mHorizontalEdge = horizontalEdge;
	mVerticalEdge = verticalEdge;
	mActiveEdges = EdgePair(mHorizontalEdge, mVerticalEdge);
	m_handleType = handleType;
}
void HandleHelper::updateCropWindow(float x,
                          float y,
                          Rect imageRect,
                          float snapRadius) {

	 EdgePair activeEdges = getActiveEdges();
	 Edge * primaryEdge = activeEdges.primary;
	 Edge * secondaryEdge = activeEdges.secondary;

	if (primaryEdge != nullptr)
		primaryEdge->adjustCoordinate(x, y, imageRect, snapRadius, UNFIXED_ASPECT_RATIO_CONSTANT);

	if (secondaryEdge != nullptr)
		secondaryEdge->adjustCoordinate(x, y, imageRect, snapRadius, UNFIXED_ASPECT_RATIO_CONSTANT);
}
void HandleHelper::updateCropWindow(float x,
	float y,
	float targetAspectRatio,
	Rect imageRect,
	float snapRadius) {}

EdgePair HandleHelper::getActiveEdges() {
	return mActiveEdges;
}
EdgePair HandleHelper::getActiveEdges(float x, float y, float targetAspectRatio) {

	// Calculate the aspect ratio if this handle were dragged to the given x-y coordinate.
	 float potentialAspectRatio = getAspectRatio(x, y);

	// If the touched point is wider than the aspect ratio, then x is the determining side. Else, y is the determining side.
	if (potentialAspectRatio > targetAspectRatio) {
		mActiveEdges.primary = mVerticalEdge;
		mActiveEdges.secondary = mHorizontalEdge;
	} else {
		mActiveEdges.primary = mHorizontalEdge;
		mActiveEdges.secondary = mVerticalEdge;
	}
	return mActiveEdges;
}

float HandleHelper::getAspectRatio(float x, float y) {

	// Replace the active edge coordinate with the given touch coordinate.
	 float left = (mVerticalEdge == Edge::LEFT_INSTANCE) ? x : Edge::LEFT_INSTANCE->getCoordinate();
	 float top = (mHorizontalEdge == Edge::TOP_INSTANCE) ? y : Edge::TOP_INSTANCE->getCoordinate();
	 float right = (mVerticalEdge == Edge::RIGHT_INSTANCE) ? x : Edge::RIGHT_INSTANCE->getCoordinate();
	 float bottom = (mHorizontalEdge == Edge::BOTTOM_INSTANCE) ? y : Edge::BOTTOM_INSTANCE->getCoordinate();

	return AspectRatioUtil::calculateAspectRatio(left, top, right, bottom);
}
