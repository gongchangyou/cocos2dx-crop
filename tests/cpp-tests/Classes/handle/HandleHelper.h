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
#include "EdgePair.h"

/**
 * Abstract helper class to handle operations on a crop window Handle.
 */
static const float UNFIXED_ASPECT_RATIO_CONSTANT = 1.f;
enum HandleType {
	HANDLE_TOP_LEFT,
	HANDLE_TOP_RIGHT,
	HANDLE_BOTTOM_LEFT,
	HANDLE_BOTTOM_RIGHT,
	HANDLE_LEFT,
	HANDLE_TOP,
	HANDLE_RIGHT,
	HANDLE_BOTTOM,
	HANDLE_CENTER
};
class HandleHelper {
    // Member Variables ////////////////////////////////////////////////////////
private:
    Edge * mHorizontalEdge;
    Edge * mVerticalEdge;
    // Save the Pair object as a member variable to avoid having to instantiate
    // a new Object every time getActiveEdges() is called.
    EdgePair mActiveEdges;
public:
	static HandleHelper * TOP_LEFT;
	static HandleHelper * TOP_RIGHT;
	static HandleHelper * BOTTOM_LEFT;
	static HandleHelper * BOTTOM_RIGHT;
	static HandleHelper * LEFT;
	static HandleHelper * TOP;
	static HandleHelper * RIGHT;
	static HandleHelper * BOTTOM;
	static HandleHelper * CENTER;
	CC_SYNTHESIZE(HandleType, m_handleType, HandleType);
    // Constructor /////////////////////////////////////////////////////////////////////////////////

    /**
     * Constructor.
     *
     * @param horizontalEdge the horizontal edge associated with this handle; may be null
     * @param verticalEdge   the vertical edge associated with this handle; may be null
     */
	
	HandleHelper(Edge * horizontalEdge, Edge * verticalEdge , const HandleType handleType );

    // Package-Private Methods /////////////////////////////////////////////////////////////////////

    /**
     * Updates the crop window by directly setting the Edge coordinates.
     *
     * @param x          the new x-coordinate of this handle
     * @param y          the new y-coordinate of this handle
     * @param imageRect  the bounding rectangle of the image
     * @param snapRadius the maximum distance (in pixels) at which the crop window should snap to
     *                   the image
     */
	virtual void updateCropWindow(float x,
		float y,
		Rect imageRect,
		float snapRadius);

    /**
     * Updates the crop window by directly setting the Edge coordinates; this method maintains a
     * given aspect ratio.
     *
     * @param x                 the new x-coordinate of this handle
     * @param y                 the new y-coordinate of this handle
     * @param targetAspectRatio the aspect ratio to maintain
     * @param imageRect         the bounding rectangle of the image
     * @param snapRadius        the maximum distance (in pixels) at which the crop window should
     *                          snap to the image
     */
	virtual void updateCropWindow(float x,
		float y,
		float targetAspectRatio,
		Rect imageRect,
		float snapRadius);

    /**
     * Gets the Edges associated with this handle (i.e. the Edges that should be moved when this
     * handle is dragged). This is used when we are not maintaining the aspect ratio.
     *
     * @return the active edge as a pair (the pair may contain null values for the
     * <code>primary</code>, <code>secondary</code> or both fields)
     */
    EdgePair getActiveEdges() ;

    /**
     * Gets the Edges associated with this handle as an ordered Pair. The <code>primary</code> Edge
     * in the pair is the determining side. This method is used when we need to maintain the aspect
     * ratio.
     *
     * @param x                 the x-coordinate of the touch point
     * @param y                 the y-coordinate of the touch point
     * @param targetAspectRatio the aspect ratio that we are maintaining
     *
     * @return the active edges as an ordered pair
     */
    EdgePair getActiveEdges(float x, float y, float targetAspectRatio) ;

    // Private Methods /////////////////////////////////////////////////////////////////////////////

    /**
     * Gets the aspect ratio of the resulting crop window if this handle were dragged to the given
     * point.
     *
     * @param x the x-coordinate
     * @param y the y-coordinate
     *
     * @return the aspect ratio
     */
    float getAspectRatio(float x, float y) ;
};
