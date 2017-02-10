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
                                          float targetRadius) ;

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
                                 Vec2 & touchOffsetOutput) ;

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
                                                    float targetRadius) ;


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
                                                  float targetRadius) ;

    bool isWithinBounds(float x, float y, float left, float top, float right, float bottom) ;
}

