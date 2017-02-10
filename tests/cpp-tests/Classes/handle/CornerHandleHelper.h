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
#include "Edge.h"
#include "EdgePair.h"

/**
 * HandleHelper class to handle corner Handles (i.e. top-left, top-right, bottom-left, and
 * bottom-right handles).
 */
class CornerHandleHelper : public HandleHelper {

    // Constructor /////////////////////////////////////////////////////////////////////////////////
public:
    CornerHandleHelper(Edge * horizontalEdge, Edge * verticalEdge, HandleType handleType)
	:HandleHelper(horizontalEdge, verticalEdge, handleType) {
		
    }

    // HandleHelper Methods ////////////////////////////////////////////////////////////////////////

    virtual void updateCropWindow(float x,
                          float y,
                          float targetAspectRatio,
                          Rect imageRect,
                          float snapRadius) override {

         EdgePair activeEdges = getActiveEdges(x, y, targetAspectRatio);
         Edge * primaryEdge = activeEdges.primary;
         Edge * secondaryEdge = activeEdges.secondary;

        primaryEdge->adjustCoordinate(x, y, imageRect, snapRadius, targetAspectRatio);
        secondaryEdge->adjustCoordinate(targetAspectRatio);

        if (secondaryEdge->isOutsideMargin(imageRect, snapRadius)) {
            secondaryEdge->snapToRect(imageRect);
            primaryEdge->adjustCoordinate(targetAspectRatio);
        }
    }
};
