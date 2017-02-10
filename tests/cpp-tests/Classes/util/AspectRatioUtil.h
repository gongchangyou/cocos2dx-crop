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
using namespace cocos2d;
/**
 * Utility class for handling calculations involving a fixed aspect ratio.
 */
namespace AspectRatioUtil {
    /**
     * Calculates the aspect ratio given a rectangle.
     */
    float calculateAspectRatio(float left, float top, float right, float bottom) ;

    /**
     * Calculates the aspect ratio given a rectangle.
     */
    float calculateAspectRatio(Rect rect) ;

    /**
     * Calculates the x-coordinate of the left edge given the other sides of the rectangle and an
     * aspect ratio.
     */
    float calculateLeft(float top, float right, float bottom, float targetAspectRatio) ;

    /**
     * Calculates the y-coordinate of the top edge given the other sides of the rectangle and an
     * aspect ratio.
     */
    float calculateTop(float left, float right, float bottom, float targetAspectRatio) ;

    /**
     * Calculates the x-coordinate of the right edge given the other sides of the rectangle and an
     * aspect ratio.
     */
    float calculateRight(float left, float top, float bottom, float targetAspectRatio) ;

    /**
     * Calculates the y-coordinate of the bottom edge given the other sides of the rectangle and an
     * aspect ratio.
     */
    float calculateBottom(float left, float top, float right, float targetAspectRatio) ;

    /**
     * Calculates the width of a rectangle given the top and bottom edges and an aspect ratio.
     */
    float calculateWidth(float height, float targetAspectRatio) ;

    /**
     * Calculates the height of a rectangle given the left and right edges and an aspect ratio.
     */
    float calculateHeight(float width, float targetAspectRatio) ;
};
