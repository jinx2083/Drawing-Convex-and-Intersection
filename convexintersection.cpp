//
// Created by XYX on 2018/2/23.
#include <cmath>
#include <vector>
#include "point.h"
#include "convexintersection.h"
#include "convexhull.h"
#include "iostream"

using namespace std;



// determines whether a point p1 is inside or outside
// of a line segment defined by sp1 and sp2
// sp1 and sp2 should be consistently provided
// from the convex hull algorithm such that the edges
// are listed in a CCW direction
// p1 belongs to the subject polygon
// sp1 and sp2 belong to the clipping polygon
bool inside(Point p1, Point sp1, Point sp2)  {
    return ccw(sp1,sp2,p1);
}

// computes the point of intersection between the line segment
// defined by s1 and s2, and the infinite line defined by
// i1 and i2.
// Assume that this will only be called when the intersection exists.
Point computeIntersection(Point s1, Point s2, Point i1, Point i2)  {

    Point p(
            ((  (s1.x * s2.y - s1.y * s2.x)*(i1.x - i2.x) - (s1.x - s2.x)*(i1.x * i2.y - i1.y * i2.x)  )
                         /
                         (  (s1.x - s2.x)*(i1.y - i2.y) - (s1.y - s2.y)*(i1.x - i2.x)  ) )
            , ((  (s1.x * s2.y - s1.y * s2.x)*(i1.y - i2.y) - (s1.y - s2.y)*(i1.x * i2.y - i1.y * i2.x)  )
               /
    (  (s1.x - s2.x)*(i1.y - i2.y) - (s1.y - s2.y)*(i1.x - i2.x)  )) );
    ;
    return p;
}

// returns a vector containing a sequence of points defining
// the intersection of two convex polygons poly1 and poly2
// Inputs: poly1 and poly2 - sequences of points defining the
//                           borders of convex polygons
vector<Point> getConvexIntersection(vector<Point>& poly1, vector<Point>& poly2)  {
    cout << "start getting intersection ..." << endl;
    vector<Point> nt;
    if (poly1.size()==0 || poly2.size()==0){
        return nt;
    }
    vector<Point> v1 = clip(poly1,poly2);
    vector<Point> v2 = clip(poly2,poly1);
    vector<Point> v3 = findIntersction(poly1,poly2);

    for (int i=0;i<v2.size();i++){
        v1.push_back(v2[i]);
    }

    for (int i=0;i<v3.size();i++){
        v1.push_back(v3[i]);
    }
    sortByAngle(v1);
    return v1;
}

//poly1:use this shape to clip the other shape（clip polygon）
//poly2: this shape is to be clipped(subject polygon)
//produces the convex points in poly2 that is inside poly1
//every point in poly2 is clipped by every edge of poly1, if the point is in poly1, add the point to the result vector
vector<Point> clip(vector<Point> & poly1, vector<Point> & poly2)  {
    vector<Point> outputList;
    if (poly1.size()==0 || poly2.size()==0){
        return outputList;
    }

    vector<Point> clipP = poly1;
    vector<Point> subjectP = poly2;

    clipP.push_back(poly1[0]);

    //loop：切每个点
    //outer loop: test every point in poly2
    for (int i=0;i<subjectP.size();i++){
        bool youCanAddIt = true;
        //每条边都切一下
        int k = 0;
        while (k<clipP.size()-1) {
            //inner loop: if the point is outside the edge, set the boolean value to false
            if (inside(subjectP[i],clipP[k],clipP[k+1])){
                youCanAddIt=false;
            }
            k++;
        }
        if (youCanAddIt){
            //if the point is inside poly1 add it to the result vector
            outputList.push_back(subjectP[i]);
            cout <<"added a point" <<endl;
        }
    }

    return outputList;
}

vector<Point> findIntersction(vector<Point> & poly1,vector<Point> & poly2){
    vector<Point> outputList;
    vector<Point> v1 = poly1;
    vector<Point> v2 = poly2;
    v1.push_back(poly1[0]);
    v2.push_back(poly2[0]);

    cout << "shape1: ";
    for (int i=0;i<v1.size();i++){
        cout << "(" << v1[i].x << "," << v1[i].y << ") ";
    }
    cout << "" << endl;

    cout << "shape2: ";
    for (int i=0;i<v2.size();i++){
        cout << "(" << v2[i].x << "," << v2[i].y << ") ";
    }
    cout << "" << endl;

    for (int i=0;i<v1.size()-1;i++){
        for (int k=0;k<v2.size()-1;k++){
            if (intersect(v1[i],v1[i+1],v2[k],v2[k+1])){
                cout << "these two edges intersects:" ;
                cout <<"(" << v1[i].x << "," << v1[i].y << ") "
                <<"(" << v1[i+1].x << "," << v1[i+1].y << ") ";
                cout <<"(" << v2[k].x << "," << v2[k].y << ") "
                <<"(" << v2[k+1].x << "," << v2[k+1].y << ") " << endl;
                outputList.push_back(computeIntersection(v1[i],v1[i+1],v2[k],v2[k+1]));
                cout << "added point: (" << computeIntersection(v1[i],v1[i+1],v2[k],v2[k+1]).x <<","<< computeIntersection(v1[i],v1[i+1],v2[k],v2[k+1]).y <<")"<<endl;

            }
        }
    }
    return outputList;
}

//helper function: if two line segments intersect, return true;
//otherwise return false
bool intersect(Point p1, Point p2, Point sp1, Point sp2)
{
    if ( max(p1.x, p2.x)<min(sp1.x, sp2.x) )
    {
        return false;
    }
    if ( max(p1.y, p2.y)<min(sp1.y, sp2.y) )
    {
        return false;
    }
    if ( max(sp1.x, sp2.x)<min(p1.x, p2.x) )
    {
        return false;
    }
    if ( max(sp1.y, sp2.y)<min(p1.y, p2.y) )
    {
        return false;
    }
    if ( mult(sp1, p2, p1)*mult(p2, sp2, p1)<0 )
    {
        return false;
    }
    if ( mult(p1, sp2, sp1)*mult(sp2, p2, sp1)<0 )
    {
        return false;
    }
    return true;
}
//helper function: calculate cross product
double mult(Point a, Point b, Point c)
{
    return (a.x-c.x)*(b.y-c.y)-(b.x-c.x)*(a.y-c.y);
}