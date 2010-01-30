/*
 * Copyright (c) 2010 University of Michigan, Ann Arbor.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of Michigan, Ann Arbor. The name of the University 
 * may not be used to endorse or promote products derived from this 
 * software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Authors:
 *        Ari Grant, grantaa@umich.edu
 *        Sugih Jamin, jamin@eecs.umich.edu
 */

#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

#include "math.h"

#ifdef _WIN32
#include "windows.h"
#endif

extern "C" {
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
}

#include "rasterizer.h"

#define SAMPLES 4

#define epsilon 1e-25

#define SAMPLE_OFFSET 0.25

bool pointIsInRect( XVec2f &point, XVec4f &rect ) {
	return point.x() >= rect(0) &&
		point.x() <= rect(0) + rect(2) &&
		point.y() >= rect(1) && 
		point.y() <= rect(1) + rect(3);
}

//void drawPoint( XVec2f &point, XVec4f &pointColor, XVec4f &clipView ) {
void drawPoint( XVec2f &point, XVec4f &pointColor) 
{
	
	/* This sets the pixel point to have the color pointColor. clipView is
	 four integers (viewLowerLeftX, viewLowerLeftY, width, height). */
	
	glColor4fv( pointColor );
	glVertex2f( point.x(), point.y() );
}

Line::Line() {
	vertex0 = XVec2f(-1,-1);
	vertex1 = XVec2f(-1,-1);
	
	color0 = XVec4f(1,0,0,1);
	color1 = XVec4f(0,1,0,1);
	
	isAntialiased = false;
}

template<int dim, class T>
void swap(XVec<dim, T> & p0, XVec<dim, T> & p1) 
{
    XVec2f tmp = p0;
    p0 = p1;
    p1 = tmp;
}

void swap(int & p0, int & p1)
{
    if (p0 != p1) {
	p0 ^= p1, p1 ^= p0, p0 ^= p1;
    }
}

void swap(float & p0, float & p1)
{
    float tmp = p0;
    p0 = p1;
    p1 = tmp;
}

// Implement the line drawing algorithm
void drawline(XVec2f p0, XVec2f p1, XVec4f p0color, XVec4f p1color, bool ispos, bool isswap, bool isAA)
{
    //printf("%d %d %d %d\n", x0, y0, x1, y1);
    int x0 = (int)round(p0.x());
    int y0 = (int)round(p0.y());
    int x1 = (int)round(p1.x());
    int y1 = (int)round(p1.y());
    
    int x = x0; 
    int y = y0;
    int dx = x1 - x0;
    int dy = y1 - y0;
    float fmid = dx * (y0 + 0.5) - dy * (x0 + 1) + x0 * y1 - x1 * y0;

    XVec4f delta_color = (p1color - p0color) / dx;
    XVec4f curcolor0 = p0color;
    XVec4f curcolor1 = XVec4f(0, 0, 0, 0);
    
    XVec2f real_p0, real_p1;
    for (;  x <= p1.x(); x++) {
	real_p0 = XVec2f(x, y);
	if (isAA) {
	    if (fmid < -epsilon) {
		real_p1 = XVec2f(x, y+1);
	    }
	    else {
		real_p1 = XVec2f(x, y-1);
	    }
	    curcolor1.alpha() = fabs(fmid) / dx * curcolor0.alpha();
	    curcolor0.alpha() = (1 - fabs(fmid)/dx) * curcolor0.alpha();
	}

	if (!ispos) {
	    real_p0.y() = y0 * 2 - real_p0.y();
	    real_p1.y() = y0 * 2 - real_p1.y();
	}
	if (isswap) {
	    swap(real_p0.x(), real_p0.y());
	    swap(real_p1.x(), real_p1.y());
	}
	    
	drawPoint(real_p0, curcolor0);
	if (isAA) {
	    drawPoint(real_p1, curcolor1);
	}

	curcolor0.alpha() = curcolor1.alpha() + curcolor0.alpha();
	curcolor0 += delta_color;
	curcolor1 = curcolor0;
	curcolor1.alpha() = 0;
	if (fmid < 0) {
	    y++;
	    fmid += (dx - dy);
	}
	else
	    fmid -= dy;


	// Update the color for the next point
    }
} 

XVec2f intersection(XVec2f & p0, XVec2f & p1, XVec2f & p2, XVec2f & p3)
{
    float c1 = p1.x() * p0.y() - p0.x() * p1.y();
    float c2 = p3.x() * p2.y() - p2.x() * p3.y();
    float a = -(p1.y() - p0.y());
    float b = p1.x() - p0.x();
    float c = -(p3.y() - p2.y());
    float d = p3.x() - p2.x();
    
    XVec2f p = XVec2f(d * c1 - b * c2, -c * c1 + a * c2);
    p = p / (a * d - b * c);
    
    return p;
}


/*
 * clipLine
 * 
 * Implement the Cyrus-Beck algorithm to clip line into the clipping window
 * @input 
 * rect define the clipping window
 * v0, v1 are the original end points of the line
 * p0, p1 are the end points of the clipped line
 * 
 * @return
 * If the line outside the window, return false
 * Otherwize return true
 */
bool clipLine(XVec4f & rect, XVec2f & v0, XVec2f & v1, XVec4f & c0, XVec4f & c1, 
	      XVec2f & p0, XVec2f & p1, XVec4f & pc0, XVec4f & pc1)
{
    p0 = v0;
    p1 = v1;
    // Corners starting from lower left and in ccw order
    XVec2f corners [4] = {XVec2f(rect.x(), rect.y()),
			  XVec2f(rect.x() + rect.z() - 1, rect.y()),
			  XVec2f(rect.x() + rect.z() - 1, rect.y() + rect.w() - 1),
			  XVec2f(rect.x(), rect.y() + rect.w() - 1)};
    // Outside normal vector for each side, from lower side in ccw order
    XVec2f normal [4] = {XVec2f(0.0, -1.0), 
			 XVec2f(1.0, 0.0),
			 XVec2f(0.0, 1.0),
			 XVec2f(-1.0, 0.0)};
    // The line vector
    XVec2f u = v1 - v0;
    float t[4] = {0};
    
    int paral = 0; /* If the line is not parallel to any edge, it is 0
		      If the line is parallel to right or left edge, it is 1
		      If the line is parallel to top or bottom edge, it is 2
		   */

// Calculate the value of parameter of the intersection
    for (int i = 0; i < 4; i++) {
	// Projection length on the normal vector
	float proLen = - normal[i].dot(u);
	if (fabs(proLen) < epsilon)
	    paral = 2 - (i & 1);
	else {
	    XVec2f tmpPoint = corners[i];
	    t[i] = normal[i].dot(v0 - tmpPoint) / proLen;
	}
    }

    float tl = numeric_limits<float>::infinity();
    float te = - numeric_limits<float>::infinity();
    
    for (int i = 0; i < 4; i++) {
	if (normal[i].dot(u) < -epsilon)
	    te = (t[i] > te) ? t[i] : te;
	else if (normal[i].dot(u) > epsilon)
	    tl = (t[i] < tl) ? t[i] : tl;
    }

    bool isInside = true; // Check whether the line segment is inside the clipping window
    if (tl < te || tl < 0 || te > 1)
	isInside = false;
    else {
	if (paral) {
	    if (paral == 1) {
		if (v0.x() < corners[0].x() || v0.x() > corners[1].x())
		    isInside = false;
	    }
	    else {
		if (v0.y() < corners[0].y() || v0.y() > corners[3].y())
		    isInside = false;
	    }
	}
	if (isInside) {
	    p0 = v0 + u * max(0.0f, te);
	    pc0 = c0 + (c1 - c0) * max(0.0f, te);
	    p1 = v0 + u * min(1.0f, tl);
	    pc1 = c0 + (c1 - c0) * min(1.0f, tl);
	}
    }
    
    return isInside;
}


void Line::drawInRect( XVec4f &rect ) {
	/* This function draws the line from point0 to point1 with endpoints
	 colored color1 and color2 respectively. It should call
	 drawPoint(...) to set each pixel. For the third argument
	 pass on the variable clipView. */
	
	/* The color should be interpolated across the line, it should be
	 one pixel thick, and clipped against the given rect. */
	
	/* YOUR CODE HERE */

    /* Draw line */
    XVec2f p0, p1;
    XVec4f p0color, p1color;
    if (clipLine(rect, vertex0, vertex1, color0, color1, p0, p1, p0color, p1color)) {
	
	// The line is in the clipping window
	//draw the clipped line
	int ispos, isswap; /* To record whether the line is positive scope and 
			      whether the coordinates are swapped.*/
	p0.x() = round(p0.x());
	p0.y() = round(p0.y());
	p1.x() = round(p1.x());
	p1.y() = round(p1.y());
	/*p0.x() = floor(p0.x());
	p0.y() = floor(p0.y());
	p1.x() = floor(p1.x());
	p1.y() = floor(p1.y());*/
	float dx = p1.x() - p0.x();
	float dy = p1.y() - p0.y();

	if (dy * dx >= 0) {
	    ispos = 1;
	    //Check whether swap around x = y is needed.
	    if (dx * dx > dy * dx)
		isswap = 0;
	    else {
		isswap = 1;
		swap(p0.x(), p0.y());
		swap(p1.x(), p1.y());
	    }
	    // Make the two points in correct order
	    if (p1.y() - p0.y() < 0 || p1.x() - p0.x() < 0) {
		swap(p0, p1);
		XVec4f tmp = p0color;
		p0color = p1color;
		p1color = tmp;
	    }
	} else {
	    ispos = 0;
	    //Check whether swap around y = x is needed.
	    if (dy * dx > - dx * dx) 
		isswap = 0;
	    else {
		isswap = 1;
		swap(p0.x(), p0.y());
		swap(p1.x(), p1.y());
	    }
	    // Make the two points in correct order.
	    if (p1.x() - p0.x() < 0) {
		// Swap the two points
		swap(p0, p1);
		XVec4f tmp = p0color;
		p0color = p1color;
		p1color = tmp;
	    }
	    //If the line slope is negative, flip the line relative to the y axis
	    p1.y() = 2 * p0.y() - p1.y();
	}

	drawline(p0, p1, p0color, p1color, ispos, isswap, isAntialiased);
    }
    
}

Triangle::Triangle() {
	Line::Line();
	
	vertex2 = XVec2f(-1,-1);
	color2 = XVec4f(0,0,1,1);
}

float distRatio(XVec2f & p, XVec2f & q, XVec2f & r, XVec2f & m)
/* Return the ratio f_pq(m) / f_pq(r). If the area of the triangle is 0 and f_pq(m) is not 0,
   return -1
*/
{
    float dy = p.y() - q.y();
    float dx = p.x() - q.x();
    float c = p.x() * q.y() - q.x() * p.y();
    float fm = dy * m.x() - dx * m.y() + c;
    float fr = dy * r.x() - dx * r.y() + c;
    if (fr == 0 && fm) {
	return -1;
    }
    else {
	return fm / fr;
    }
}

bool Triangle::containsPoint( XVec2f &point, XVec4f &pointColor ) {
	/* This function returns whether the given point is inside the given
	 triangle. If it is, then pointColor is set to  the color of the point
	 inside the triangle by interpolating the colors of the vertices. */
	
	/* YOUR CODE HERE */
    float gamma = distRatio(vertex0, vertex1, vertex2, point);
    float beta = distRatio(vertex0, vertex2, vertex1, point);
    float alpha = distRatio(vertex1, vertex2, vertex0, point);
    /* set the point's color to blue for now */
    if (gamma >= 0 && alpha >= 0 && beta >= 0) {
	pointColor = alpha * color0 + beta * color1 + gamma * color2;
	return true;
    }
    else {
	pointColor = XVec4f(0.0,0.0,1.0,1.0);
	return false;
    }
}

// The anti-aliased version of contains point
bool containsAntialiasedPoint(Triangle * tri, XVec2f &point, XVec4f & pointColor) 
{
    XVec2f subSamples [] = {XVec2f(point.x() - SAMPLE_OFFSET, point.y() - SAMPLE_OFFSET), 
			    XVec2f(point.x() + SAMPLE_OFFSET, point.y() - SAMPLE_OFFSET),
			    XVec2f(point.x() + SAMPLE_OFFSET, point.y() + SAMPLE_OFFSET),
			    XVec2f(point.x() - SAMPLE_OFFSET, point.y() + SAMPLE_OFFSET)};
    
    int numTurnOn = 0;
    XVec4f tmpColor = XVec4f(0, 0, 0, 0);
    pointColor = tmpColor;
    for (int i = 0; i < 4; i++) {
	if (tri->containsPoint(subSamples[i], tmpColor)) {
	    ++numTurnOn;
	    pointColor += tmpColor;
	}
    }
    if (numTurnOn) {
	pointColor /= numTurnOn;
	pointColor.alpha() = pointColor.alpha() * numTurnOn / 4;
	return true;
    }
    else
	return false;
}

void Triangle::drawInRect( XVec4f &rect ) 
{
    /* This function draws the triangle t. It should call drawPoint(...)
       to set each pixel. */
    
	/* YOUR CODE HERE */
    // Calcolate the bounding box, denoted by xmin, xmax, ymin and ymax
    int xmin = static_cast<int>(floor(min(min(vertex0.x(), vertex1.x()), vertex2.x())));
    int xmax = static_cast<int>(ceil(max(max(vertex0.x(), vertex1.x()), vertex2.x())));
    int ymin = static_cast<int>(floor(min(min(vertex0.y(), vertex1.y()), vertex2.y())));
    int ymax = static_cast<int>(ceil(max(max(vertex0.y(), vertex1.y()), vertex2.y())));
    
    // Check each point in the bounding box
    for (int y = ymin; y <= ymax; y++) {
	for (int x = xmin; x <= xmax; x++) {
	    // Do I need to check the center of the pixel?
	    XVec2f point = XVec2f(x+0.5, y+0.5);
	    XVec4f pointColor;
	    if (isAntialiased) {
		if (containsAntialiasedPoint(this, point, pointColor))
		    drawPoint(point, pointColor);
	    }
	    else {
		if (containsPoint(point, pointColor))
		    drawPoint(point, pointColor);
	    }
	}
    }
}

