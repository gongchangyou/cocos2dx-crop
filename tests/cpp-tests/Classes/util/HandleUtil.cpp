/*
 * Copyright 2013, Edmodo, Inc. 
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this work except in compliance with the License.
 * You may obtain a copy of the License in the LICENSE file, or at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" 
 * BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language 
 * governing permissions and limitations under the License. 
 */


#include "HandleHelper.h"
#include "MathUtil.h"
#include <math.h>
#include "HandleUtil.h"

/**
 * Utility class to perform basic operations with Handles.
 */
namespace HandleUtil {

    // Public Methods //////////////////////////////////////////////////////////////////////////////

    /**
     * Determines which, if any, of the handles are pressed given the touch coordinates, the
     * bounding box, and the touch radius.
     *
     * @param x            the x-coordinate of the touch point
     * @param y            the y-coordinate of the touch point
     * @param left         the x-coordinate of the left bound
     * @param top          the y-coordinate of the top bound
     * @param right        the x-coordinate of the right bound
     * @param bottom       the y-coordinate of the bottom bound
     * @param targetRadius the target radius in pixels
     *
     * @return the Handle that was pressed; null if no Handle was pressed
     */
    HandleHelper * getPressedHandle(float x,
                                          float y,
                                          float left,
                                          float top,
                                          float right,
                                          float bottom,
                                          float targetRadius) {

        // Find the closest corner handle to the touch point.
        // If the touch point is in the target zone of this closest handle, then this is the pressed handle.
        // Else, check if any of the edges are in the target zone of the touch point.
        // Else, check if the touch point is within the crop window bounds; if so, then choose the center handle.

        HandleHelper * closestHandle =nullptr;
        float closestDistance = Edge::POSITIVE_INFINITY;

        float distanceToTopLeft = crop::MathUtil::calculateDistance(x, y, left, top);
        if (distanceToTopLeft < closestDistance) {
            closestDistance = distanceToTopLeft;
            closestHandle = HandleHelper::TOP_LEFT;
        }

        float distanceToTopRight = crop::MathUtil::calculateDistance(x, y, right, top);
        if (distanceToTopRight < closestDistance) {
            closestDistance = distanceToTopRight;
            closestHandle = HandleHelper::TOP_RIGHT;
        }

        float distanceToBottomLeft = crop::MathUtil::calculateDistance(x, y, left, bottom);
        if (distanceToBottomLeft < closestDistance) {
            closestDistance = distanceToBottomLeft;
            closestHandle = HandleHelper::BOTTOM_LEFT;
        }

        float distanceToBottomRight = crop::MathUtil::calculateDistance(x, y, right, bottom);
        if (distanceToBottomRight < closestDistance) {
            closestDistance = distanceToBottomRight;
            closestHandle = HandleHelper::BOTTOM_RIGHT;
        }

        if (closestDistance <= targetRadius) {
            return closestHandle;
        }

        // If we get to this point, none of the corner handles were in the touch target zone, so then we check the edges.
        if (HandleUtil::isInHorizontalTargetZone(x, y, left, right, top, targetRadius)) {
            return HandleHelper::TOP;
        } else if (isInHorizontalTargetZone(x, y, left, right, bottom, targetRadius)) {
            return HandleHelper::BOTTOM;
        } else if (isInVerticalTargetZone(x, y, left, top, bottom, targetRadius)) {
            return HandleHelper::LEFT;
        } else if (isInVerticalTargetZone(x, y, right, top, bottom, targetRadius)) {
            return HandleHelper::RIGHT;
        }

        // If we get to this point, none of the corners or edges are in the touch target zone.
        // Check to see if the touch point is within the bounds of the crop window. If so, choose the center handle.
        if (isWithinBounds(x, y, left, top, right, bottom)) {
            return HandleHelper::CENTER;
        }

        return nullptr ;
    }

    /**
     * Calculates the offset of the touch point from the precise location of the specified handle.
     * <p/>
     * The offset will be returned in the 'touchOffsetOutput' parameter; the x-offset will be the
     * first value and the y-offset will be the second value.
     */
    void getOffset(HandleHelper * handle,
                                 float x,
                                 float y,
                                 float left,
                                 float top,
                                 float right,
                                 float bottom,
                                 Vec2 & touchOffsetOutput) {

        float touchOffsetX = 0;
        float touchOffsetY = 0;

        // Calculate the offset from the appropriate handle.
        switch (handle->getHandleType()) {

            case HANDLE_TOP_LEFT:
                touchOffsetX = left - x;
                touchOffsetY = top - y;
                break;
            case HANDLE_TOP_RIGHT:
                touchOffsetX = right - x;
                touchOffsetY = top - y;
                break;
            case HANDLE_BOTTOM_LEFT:
                touchOffsetX = left - x;
                touchOffsetY = bottom - y;
                break;
            case HANDLE_BOTTOM_RIGHT:
                touchOffsetX = right - x;
                touchOffsetY = bottom - y;
                break;
            case HANDLE_LEFT:
                touchOffsetX = left - x;
                touchOffsetY = 0;
                break;
            case HANDLE_TOP:
                touchOffsetX = 0;
                touchOffsetY = top - y;
                break;
            case HANDLE_RIGHT:
                touchOffsetX = right - x;
                touchOffsetY = 0;
                break;
            case HANDLE_BOTTOM:
                touchOffsetX = 0;
                touchOffsetY = bottom - y;
                break;
            case HANDLE_CENTER:
                float centerX = (right + left) / 2;
                float centerY = (top + bottom) / 2;
                touchOffsetX = centerX - x;
                touchOffsetY = centerY - y;
                break;
        }

        touchOffsetOutput.x = touchOffsetX;
        touchOffsetOutput.y = touchOffsetY;
    }

    // Private Methods /////////////////////////////////////////////////////////////////////////////

    /**
     * Determines if the specified coordinate is in the target touch zone for a horizontal bar
     * handle.
     *
     * @param x            the x-coordinate of the touch point
     * @param y            the y-coordinate of the touch point
     * @param handleXStart the left x-coordinate of the horizontal bar handle
     * @param handleXEnd   the right x-coordinate of the horizontal bar handle
     * @param handleY      the y-coordinate of the horizontal bar handle
     * @param targetRadius the target radius in pixels
     *
     * @return true if the touch point is in the target touch zone; false otherwise
     */
    bool isInHorizontalTargetZone(float x,
                                                    float y,
                                                    float handleXStart,
                                                    float handleXEnd,
                                                    float handleY,
                                                    float targetRadius) {

        return (x > handleXStart && x < handleXEnd && fabs(y - handleY) <= targetRadius);
    }

    /**
     * Determines if the specified coordinate is in the target touch zone for a vertical bar
     * handle.
     *
     * @param x            the x-coordinate of the touch point
     * @param y            the y-coordinate of the touch point
     * @param handleX      the x-coordinate of the vertical bar handle
     * @param handleYStart the top y-coordinate of the vertical bar handle
     * @param handleYEnd   the bottom y-coordinate of the vertical bar handle
     * @param targetRadius the target radius in pixels
     *
     * @return true if the touch point is in the target touch zone; false otherwise
     */
    bool isInVerticalTargetZone(float x,
                                                  float y,
                                                  float handleX,
                                                  float handleYStart,
                                                  float handleYEnd,
                                                  float targetRadius) {

        return (fabs(x - handleX) <= targetRadius && y < handleYStart && y > handleYEnd);
    }

    bool isWithinBounds(float x, float y, float left, float top, float right, float bottom) {
        return x >= left && x <= right && y >= bottom && y <= top;
    }
}
