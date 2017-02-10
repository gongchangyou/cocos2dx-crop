#pragma once
#include "AspectRatioUtil.h"

enum EdgeType {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

class Edge {

public:
    Edge(EdgeType type)
	:m_type(type)
	{}

	virtual ~Edge() {}

	static const float POSITIVE_INFINITY;
	static const float NEGATIVE_INFINITY;
	static const int MIN_CROP_LENGTH_PX = 100;

	void setCoordinate(float coordinate) {
		m_coordinate = coordinate;
	}
	float getCoordinate() { return m_coordinate; }
	void offset(float distance) { m_coordinate += distance; }
	bool operator==(const Edge& x) const {
		return m_type == x.getType();
	}

	EdgeType getType() const  {
		return m_type;
	}

	/**
	* Sets the Edge to the given x-y coordinate but also adjusting for snapping to the image bounds
	* and parent view border constraints.
	*
	* @param x               the x-coordinate
	* @param y               the y-coordinate
	* @param imageRect       the bounding rectangle of the image
	* @param imageSnapRadius the radius (in pixels) at which the edge should snap to the image
	*/
	void adjustCoordinate(float x, float y, Rect imageRect, float imageSnapRadius, float aspectRatio) {
		switch (m_type) {
		case EdgeType::LEFT:
			m_coordinate = adjustLeft(x, imageRect, imageSnapRadius, aspectRatio);
			break;
		case EdgeType::TOP:
			m_coordinate = adjustTop(y, imageRect, imageSnapRadius, aspectRatio);
			break;
		case EdgeType::RIGHT:
			m_coordinate = adjustRight(x, imageRect, imageSnapRadius, aspectRatio);
			break;
		case EdgeType::BOTTOM:
			m_coordinate = adjustBottom(y, imageRect, imageSnapRadius, aspectRatio);
			break;
		}
	}

	/**
	* Adjusts this Edge position such that the resulting window will have the given aspect ratio.
	*
	* @param aspectRatio the aspect ratio to achieve
	*/
	void adjustCoordinate(float aspectRatio);
	/**
	* Returns whether or not you can re-scale the image based on whether any edge would be out of
	* bounds. Checks all the edges for a possibility of jumping out of bounds.
	*
	* @param edge        the Edge that is about to be expanded
	* @param imageRect   the rectangle of the picture
	* @param aspectRatio the desired aspectRatio of the picture
	*
	* @return whether or not the new image would be out of bounds.
	*/
	bool isNewRectangleOutOfBounds(Edge edge, Rect imageRect, float aspectRatio) {

		float offset = edge.snapOffset(imageRect);

		switch (m_type) {

		case EdgeType::LEFT:

			if (edge == EdgeType::TOP) {

				float top = imageRect.origin.y + imageRect.size.height;
				float bottom = Edge::BOTTOM_INSTANCE->getCoordinate() - offset;
				float right = Edge::RIGHT_INSTANCE->getCoordinate();
				float left = AspectRatioUtil::calculateLeft(top, right, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);

			}
			else if (edge == EdgeType::BOTTOM) {

				float bottom = imageRect.origin.y;
				float top = Edge::TOP_INSTANCE->getCoordinate() - offset;
				float right = Edge::RIGHT_INSTANCE->getCoordinate();
				float left = AspectRatioUtil::calculateLeft(top, right, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);
			}
			break;

		case EdgeType::TOP:

			if (edge == EdgeType::LEFT) {

				float left = imageRect.origin.x;
				float right = Edge::RIGHT_INSTANCE->getCoordinate() - offset;
				float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();
				float top = AspectRatioUtil::calculateTop(left, right, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);

			}
			else if (edge == EdgeType::RIGHT) {

				float right = imageRect.origin.x + imageRect.size.width;
				float left = Edge::LEFT_INSTANCE->getCoordinate() - offset;
				float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();
				float top = AspectRatioUtil::calculateTop(left, right, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);
			}
			break;

		case EdgeType::RIGHT:

			if (edge == EdgeType::TOP) {

				float top = imageRect.origin.y + imageRect.size.height;
				float bottom = Edge::BOTTOM_INSTANCE->getCoordinate() - offset;
				float left = Edge::LEFT_INSTANCE->getCoordinate();
				float right = AspectRatioUtil::calculateRight(left, top, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);

			}
			else if (edge == EdgeType::BOTTOM) {

				float bottom = imageRect.origin.y;
				float top = Edge::TOP_INSTANCE->getCoordinate() - offset;
				float left = Edge::LEFT_INSTANCE->getCoordinate();
				float right = AspectRatioUtil::calculateRight(left, top, bottom, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);
			}
			break;

		case EdgeType::BOTTOM:

			if (edge == EdgeType::LEFT) {

				float left = imageRect.origin.x;
				float right = Edge::RIGHT_INSTANCE->getCoordinate() - offset;
				float top = Edge::TOP_INSTANCE->getCoordinate();
				float bottom = AspectRatioUtil::calculateBottom(left, top, right, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);

			}
			else if (edge == EdgeType::RIGHT) {

				float right = imageRect.origin.x + imageRect.size.width;
				float left = Edge::LEFT_INSTANCE->getCoordinate() - offset;
				float top = Edge::TOP_INSTANCE->getCoordinate();
				float bottom = AspectRatioUtil::calculateBottom(left, top, right, aspectRatio);

				return isOutOfBounds(top, left, bottom, right, imageRect);

			}
			break;
		}
		return true;
	}

	/**
	* Snap this Edge to the given image boundaries.
	*
	* @param imageRect the bounding rectangle of the image to snap to
	*
	* @return the amount (in pixels) that this coordinate was changed (i.e. the new coordinate
	* minus the old coordinate value)
	*/
	float snapToRect(Rect imageRect) {

		float oldCoordinate = m_coordinate;

		switch (m_type) {
		case EdgeType::LEFT:
			m_coordinate = imageRect.origin.x;
			break;
		case EdgeType::TOP:
			m_coordinate = imageRect.origin.y + imageRect.size.height;
			break;
		case EdgeType::RIGHT:
			m_coordinate = imageRect.origin.x + imageRect.size.width;
			break;
		case EdgeType::BOTTOM:
			m_coordinate = imageRect.origin.y;
			break;
		}

		return m_coordinate - oldCoordinate;
	}

	/**
	* Returns the potential snap offset of snapToRect, without changing the coordinate.
	*
	* @param imageRect the bounding rectangle of the image to snap to
	*
	* @return the amount (in pixels) that this coordinate was changed (i.e. the new coordinate
	* minus the old coordinate value)
	*/
	float snapOffset(Rect imageRect) {

		float oldCoordinate = m_coordinate;
		float newCoordinate;

		switch (m_type) {
		case EdgeType::LEFT:
			newCoordinate = imageRect.origin.x;
			break;
		case EdgeType::TOP:
			newCoordinate = imageRect.origin.y + imageRect.size.height;
			break;
		case EdgeType::RIGHT:
			newCoordinate = imageRect.origin.x + imageRect.size.width;
			break;
		default: // EdgeType::BOTTOM
			newCoordinate = imageRect.origin.y;
			break;
		}

		return newCoordinate - oldCoordinate;
	}

	/**
	* Gets the current width of the crop window.
	*/
	static float getWidth() {
		return Edge::RIGHT_INSTANCE->getCoordinate() - Edge::LEFT_INSTANCE->getCoordinate();
	}

	/**
	* Gets the current height of the crop window.
	*/
	static float getHeight() {
		return Edge::TOP_INSTANCE->getCoordinate() - Edge::BOTTOM_INSTANCE->getCoordinate();
	}

	/**
	* Determines if this Edge is outside the inner margins of the given bounding rectangle. The
	* margins come inside the actual frame by SNAPRADIUS amount; therefore, determines if the point
	* is outside the inner "margin" frame.
	*/
	bool isOutsideMargin(Rect rect, float margin) {

		bool result;

		switch (m_type) {
		case EdgeType::LEFT:
			result = m_coordinate - rect.origin.x < margin;
			break;
		case EdgeType::TOP:
			result = (rect.origin.y + rect.size.height) - m_coordinate < margin;
			break;
		case EdgeType::RIGHT:
			result = rect.origin.x + rect.size.width - m_coordinate < margin;
			break;
		default: // EdgeType::BOTTOM
			result = m_coordinate - rect.origin.y < margin;
			break;
		}
		return result;
	}


private:
	EdgeType m_type;
	float m_coordinate;
	/**
	* Returns whether the new rectangle would be out of bounds.
	*
	* @param imageRect the Image to be compared with
	*
	* @return whether it would be out of bounds
	*/
	bool isOutOfBounds(float top, float left, float bottom, float right, Rect imageRect) {
		return (top > imageRect.origin.y + imageRect.size.height || left < imageRect.origin.x || bottom < imageRect.origin.y || right > imageRect.origin.x + imageRect.size.width);
	}
	// Private Methods /////////////////////////////////////////////////////////////////////////////

	/**
	* Get the resulting x-position of the left edge of the crop window given the handle's position
	* and the image's bounding box and snap radius.
	*
	* @param x               the x-position that the left edge is dragged to
	* @param imageRect       the bounding box of the image that is being cropped
	* @param imageSnapRadius the snap distance to the image edge (in pixels)
	*
	* @return the actual x-position of the left edge
	*/
	static float adjustLeft(float x, Rect imageRect, float imageSnapRadius, float aspectRatio) {

		float resultX;

		if (x - imageRect.origin.x < imageSnapRadius) {

			resultX = imageRect.origin.x;

		}
		else {

			// Select the minimum of the three possible values to use
			float resultXHoriz = POSITIVE_INFINITY;
			float resultXVert = POSITIVE_INFINITY;

			// Checks if the window is too small horizontally
			if (x >= Edge::RIGHT_INSTANCE->getCoordinate() - MIN_CROP_LENGTH_PX) {
				resultXHoriz = Edge::RIGHT_INSTANCE->getCoordinate() - MIN_CROP_LENGTH_PX;
			}
			// Checks if the window is too small vertically
			if (((Edge::RIGHT_INSTANCE->getCoordinate() - x) / aspectRatio) <= MIN_CROP_LENGTH_PX) {
				resultXVert = Edge::RIGHT_INSTANCE->getCoordinate() - (MIN_CROP_LENGTH_PX * aspectRatio);
			}
			resultX = std::min(x, std::min(resultXHoriz, resultXVert));
		}
		return resultX;
	}

	/**
	* Get the resulting x-position of the right edge of the crop window given the handle's position
	* and the image's bounding box and snap radius.
	*
	* @param x               the x-position that the right edge is dragged to
	* @param imageRect       the bounding box of the image that is being cropped
	* @param imageSnapRadius the snap distance to the image edge (in pixels)
	*
	* @return the actual x-position of the right edge
	*/
	static float adjustRight(float x, Rect imageRect, float imageSnapRadius, float aspectRatio) {

		float resultX;

		// If close to the edge...
		if (imageRect.origin.x + imageRect.size.width - x < imageSnapRadius) {

			resultX = imageRect.origin.x + imageRect.size.width;

		}
		else {

			// Select the maximum of the three possible values to use
			float resultXHoriz = NEGATIVE_INFINITY;
			float resultXVert = NEGATIVE_INFINITY;

			// Checks if the window is too small horizontally
			if (x <= Edge::LEFT_INSTANCE->getCoordinate() + MIN_CROP_LENGTH_PX) {
				resultXHoriz = Edge::LEFT_INSTANCE->getCoordinate() + MIN_CROP_LENGTH_PX;
			}
			// Checks if the window is too small vertically
			if (((x - Edge::LEFT_INSTANCE->getCoordinate()) / aspectRatio) <= MIN_CROP_LENGTH_PX) {
				resultXVert = Edge::LEFT_INSTANCE->getCoordinate() + (MIN_CROP_LENGTH_PX * aspectRatio);
			}
			resultX = std::max(x, std::max(resultXHoriz, resultXVert));
		}
		return resultX;
	}

	/**
	* Get the resulting y-position of the top edge of the crop window given the handle's position
	* and the image's bounding box and snap radius.
	*
	* @param y               the x-position that the top edge is dragged to
	* @param imageRect       the bounding box of the image that is being cropped
	* @param imageSnapRadius the snap distance to the image edge (in pixels)
	*
	* @return the actual y-position of the top edge
	*/
	static float adjustTop(float y, Rect imageRect, float imageSnapRadius, float aspectRatio) {

		float resultY;

		if (y - (imageRect.origin.y + imageRect.size.height) > imageSnapRadius) {

			resultY = imageRect.origin.y + imageRect.size.height;

		}
		else {

			// Select the minimum of the three possible values to use
			float resultYVert = NEGATIVE_INFINITY;
			float resultYHoriz = NEGATIVE_INFINITY;

			// Checks if the window is too small vertically
			if (y <= Edge::BOTTOM_INSTANCE->getCoordinate() + MIN_CROP_LENGTH_PX)
				resultYHoriz = Edge::BOTTOM_INSTANCE->getCoordinate() + MIN_CROP_LENGTH_PX;

			// Checks if the window is too small horizontally
			if (((y - Edge::BOTTOM_INSTANCE->getCoordinate() ) * aspectRatio) <= MIN_CROP_LENGTH_PX)
				resultYVert = Edge::BOTTOM_INSTANCE->getCoordinate() + (MIN_CROP_LENGTH_PX / aspectRatio);

			resultY = std::max(y, std::max(resultYHoriz, resultYVert));
		}
		return resultY;
	}

	/**
	* Get the resulting y-position of the bottom edge of the crop window given the handle's
	* position and the image's bounding box and snap radius.
	*
	* @param y               the x-position that the bottom edge is dragged to
	* @param imageRect       the bounding box of the image that is being cropped
	* @param imageSnapRadius the snap distance to the image edge (in pixels)
	*
	* @return the actual y-position of the bottom edge
	*/
	static float adjustBottom(float y, Rect imageRect, float imageSnapRadius, float aspectRatio) {

		float resultY;

		if (imageRect.origin.y - y > imageSnapRadius) {

			resultY = imageRect.origin.y;

		}
		else {

			// Select the maximum of the three possible values to use
			float resultYVert = POSITIVE_INFINITY;
			float resultYHoriz = POSITIVE_INFINITY;

			// Checks if the window is too small vertically
			if (y >= Edge::TOP_INSTANCE->getCoordinate() - MIN_CROP_LENGTH_PX) {
				resultYVert = Edge::TOP_INSTANCE->getCoordinate() - MIN_CROP_LENGTH_PX;
			}
			// Checks if the window is too small horizontally
			if (((Edge::TOP_INSTANCE->getCoordinate() - y) * aspectRatio) <= MIN_CROP_LENGTH_PX) {
				resultYHoriz = Edge::TOP_INSTANCE->getCoordinate() - (MIN_CROP_LENGTH_PX / aspectRatio);
			}
			resultY = std::min(y, std::min(resultYHoriz, resultYVert));
		}
		return resultY;
	}
public:
	static Edge * LEFT_INSTANCE;
	static Edge * RIGHT_INSTANCE;
	static Edge * TOP_INSTANCE;
	static Edge * BOTTOM_INSTANCE; 
};
