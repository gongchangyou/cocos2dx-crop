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
#pragma once
#include "HandleHelper.h"
//import com.gplus.lfs.cropper.util.AspectRatioUtil;

/**
 * Handle helper class to handle horizontal handles (i.e. top and bottom handles).
 */
class HorizontalHandleHelper : public HandleHelper {

    // Member Variables ////////////////////////////////////////////////////////////////////////////

private:
	Edge * mEdge;
public:

    // Constructor /////////////////////////////////////////////////////////////////////////////////

    HorizontalHandleHelper(Edge * edge, HandleType handleType)
	:HandleHelper(edge, nullptr, handleType) {
        mEdge = edge;
    }

    // HandleHelper Methods ////////////////////////////////////////////////////////////////////////

    virtual void updateCropWindow(float x,
                          float y,
                          float targetAspectRatio,
                          Rect imageRect,
                          float snapRadius) override {

        // Adjust this Edge accordingly.
        mEdge->adjustCoordinate(x, y, imageRect, snapRadius, targetAspectRatio);

        float left = Edge::LEFT_INSTANCE->getCoordinate();
        float right = Edge::RIGHT_INSTANCE->getCoordinate();

        // After this Edge is moved, our crop window is now out of proportion.
         float targetWidth = AspectRatioUtil::calculateWidth(Edge::getHeight(), targetAspectRatio);

        // Adjust the crop window so that it maintains the given aspect ratio by
        // moving the adjacent edges symmetrically in or out.
         float difference = targetWidth - Edge::getWidth();
         float halfDifference = difference / 2;
        left -= halfDifference;
        right += halfDifference;

        Edge::LEFT_INSTANCE->setCoordinate(left);
        Edge::RIGHT_INSTANCE->setCoordinate(right);

        // Check if we have gone out of bounds on the sides, and fix.
        if (Edge::LEFT_INSTANCE->isOutsideMargin(imageRect, snapRadius)
                && !mEdge->isNewRectangleOutOfBounds(*Edge::LEFT_INSTANCE, imageRect, targetAspectRatio)) {

             float offset = Edge::LEFT_INSTANCE->snapToRect(imageRect);
            Edge::RIGHT_INSTANCE->offset(-offset);
            mEdge->adjustCoordinate(targetAspectRatio);
        }

        if (Edge::RIGHT_INSTANCE->isOutsideMargin(imageRect, snapRadius)
                && !mEdge->isNewRectangleOutOfBounds(*Edge::RIGHT_INSTANCE, imageRect, targetAspectRatio)) {

             float offset = Edge::RIGHT_INSTANCE->snapToRect(imageRect);
            Edge::LEFT_INSTANCE->offset(-offset);
            mEdge->adjustCoordinate(targetAspectRatio);
        }
    }
};
