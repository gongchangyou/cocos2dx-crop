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

/**
 * HandleHelper class to handle vertical handles (i.e. left and right handles).
 */
class VerticalHandleHelper : public HandleHelper {

    // Member Variables ////////////////////////////////////////////////////////////////////////////

private:
	Edge * mEdge;

public:

    // Constructor /////////////////////////////////////////////////////////////////////////////////

    VerticalHandleHelper(Edge * edge, HandleType handleType)
	: HandleHelper(edge, nullptr, handleType){
        
        mEdge = edge;
    }

    // HandleHelper Methods ////////////////////////////////////////////////////////////////////////

	virtual    void updateCropWindow(float x,
                          float y,
                          float targetAspectRatio,
                          Rect imageRect,
                          float snapRadius) {

        // Adjust this Edge accordingly.
        mEdge->adjustCoordinate(x, y, imageRect, snapRadius, targetAspectRatio);

        float top = Edge::TOP_INSTANCE->getCoordinate();
        float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();

        // After this Edge is moved, our crop window is now out of proportion.
         float targetHeight = AspectRatioUtil::calculateHeight(Edge::getWidth(), targetAspectRatio);

        // Adjust the crop window so that it maintains the given aspect ratio by
        // moving the adjacent edges symmetrically in or out.
         float difference = targetHeight - Edge::getHeight();
         float halfDifference = difference / 2;
        top -= halfDifference;
        bottom += halfDifference;

        Edge::TOP_INSTANCE->setCoordinate(top);
        Edge::BOTTOM_INSTANCE->setCoordinate(bottom);

        // Check if we have gone out of bounds on the top or bottom, and fix.
        if (Edge::TOP_INSTANCE->isOutsideMargin(imageRect, snapRadius)
                && !mEdge->isNewRectangleOutOfBounds(*Edge::TOP_INSTANCE, imageRect, targetAspectRatio)) {

             float offset = Edge::TOP_INSTANCE->snapToRect(imageRect);
            Edge::BOTTOM_INSTANCE->offset(-offset);
            mEdge->adjustCoordinate(targetAspectRatio);
        }

        if (Edge::BOTTOM_INSTANCE->isOutsideMargin(imageRect, snapRadius)
                && !mEdge->isNewRectangleOutOfBounds(*Edge::BOTTOM_INSTANCE, imageRect, targetAspectRatio)) {

             float offset = Edge::BOTTOM_INSTANCE->snapToRect(imageRect);
            Edge::TOP_INSTANCE->offset(-offset);
            mEdge->adjustCoordinate(targetAspectRatio);
        }
    }
};
