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
#include "Edge.h"
#include "HandleHelper.h"

/**
 * HandleHelper class to handle the center handle.
 */
class CenterHandleHelper : public HandleHelper {

    // Constructor /////////////////////////////////////////////////////////////////////////////////
public:
    CenterHandleHelper()
	:HandleHelper(NULL, NULL, HandleType::HANDLE_CENTER){
    }

    // HandleHelper Methods ////////////////////////////////////////////////////////////////////////

    
    virtual void updateCropWindow(float x,
                          float y,
                          Rect imageRect,
                          float snapRadius) override {

        float left = Edge::LEFT_INSTANCE->getCoordinate();
        float top = Edge::TOP_INSTANCE->getCoordinate();
        float right = Edge::RIGHT_INSTANCE->getCoordinate();
        float bottom = Edge::BOTTOM_INSTANCE->getCoordinate();

         float currentCenterX = (left + right) / 2;
         float currentCenterY = (top + bottom) / 2;

         float offsetX = x - currentCenterX;
         float offsetY = y - currentCenterY;

        // Adjust the crop window.
        Edge::LEFT_INSTANCE->offset(offsetX);
        Edge::TOP_INSTANCE->offset(offsetY);
        Edge::RIGHT_INSTANCE->offset(offsetX);
        Edge::BOTTOM_INSTANCE->offset(offsetY);

        // Check if we have gone out of bounds on the sides, and fix.
        if (Edge::LEFT_INSTANCE->isOutsideMargin(imageRect, snapRadius)) {
            float offset = Edge::LEFT_INSTANCE->snapToRect(imageRect);
            Edge::RIGHT_INSTANCE->offset(offset);
        } else if (Edge::RIGHT_INSTANCE->isOutsideMargin(imageRect, snapRadius)) {
            float offset = Edge::RIGHT_INSTANCE->snapToRect(imageRect);
            Edge::LEFT_INSTANCE->offset(offset);
        }

        // Check if we have gone out of bounds on the top or bottom, and fix.
        if (Edge::TOP_INSTANCE->isOutsideMargin(imageRect, snapRadius)) {
            float offset = Edge::TOP_INSTANCE->snapToRect(imageRect);
            Edge::BOTTOM_INSTANCE->offset(offset);
        } else if (Edge::BOTTOM_INSTANCE->isOutsideMargin(imageRect, snapRadius)) {
            float offset = Edge::BOTTOM_INSTANCE->snapToRect(imageRect);
            Edge::TOP_INSTANCE->offset(offset);
        }
    }

    virtual void updateCropWindow(float x,
                          float y,
                          float targetAspectRatio,
                          Rect imageRect,
                          float snapRadius) override {

        updateCropWindow(x, y, imageRect, snapRadius);
    }
};
