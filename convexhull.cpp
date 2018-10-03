//
// Created by XYX on 2018/2/23.
//

#include <cmath>
#include <vector>
#include "point.h"
#include "stack.h"
#include "convexhull.h"
#include <iostream>
#include <algorithm>

using namespace std;

// uses a sorting algorithm of your choice (must be fully implemented by you)
// to sort the points in v to satisfy the following criteria:
// 1. v[0] must contain the point with the smallest y-coordinate.
//      If multiple points have the same smallest y-coordinate, v[0]
//      must contain the one among those with the smallest x-coordinate.
// 2. The remaining indices i contain the points, sorted in increasing order
//      by the angle that the point forms with v[0] and the x-axis. THat is,
// 	one of the legs of the angle is represened by the line through v[0] and
//	v[i], and the other is the x-axis.
void sortByAngle(vector<Point>& v)  {
    if (v.size()==0){
        return;
    }

    //find v[0]
    double smallestY = v[0].y;
    int smallestIndex = 0;
    for (int i=1;i<v.size();i++){
        if (v[i].y < smallestY || (v[i].y == smallestY && v[i].x < v[smallestIndex].x))  {
            smallestY = v[i].y;
            smallestIndex = i;
        }
    }

    //swap
    swap(v[0],v[smallestIndex]);

    //step 2: sort the rest of the vector
    //by using selection sort

    for (int i=1;i<v.size()-1;i++)  {
        int smallest = i;
        // find the index of the smallest element
        for (int j = i+1;j<v.size();j++)  {
            if (findAngle(v[0],v[j]) < findAngle(v[0],v[smallest]) )  {
                smallest = j;
            }
            else if (findAngle(v[0],v[j]) == findAngle(v[0],v[smallest]))  {
                if (sqDistance(v[0],v[j]) < sqDistance(v[0],v[smallest]))  {
                    smallest =j;
                }
            }
        }
        //swap the smallest with the current item
        Point temp = v[i];
        v[i] = v[smallest];
        v[smallest] = temp;
    }

    return;
}

// determines whether a path from p1 to p2 to p3 describes a counterclockwise turn
bool ccw(Point p1, Point p2, Point p3){
    //todo
    double a = (p2.x-p1.x) * (p3.y-p1.y) - (p2.y-p1.y) * (p3.x - p1.x);
    if (a>0){
        return true;
    }
    else return false;
}

int ccwHelper(Point p1, Point p2, Point p3){
    double a = (p2.x-p1.x) * (p3.y-p1.y) - (p2.y-p1.y) * (p3.x - p1.x);
    if (ccw(p1,p2,p3))
        return 1;
    else if (a==0)
        return 0;
    return -1;
}

// returns a vector of points representing the convex hull of v
// if c is the vector representing the convex hull, then c[k], c[k+1]
// is an edge in the convex hull, for all k in 0 to n-1, where n is the
// number of points in the hull. c[n-1],c[0] is also an edge in the hull.
// The returned vector should be a subset of v
// Input: v - a vector of points in arbitrary order
vector<Point> getConvexHull(vector<Point>& v){
    //todo
    if (v.size()==0){
        return v;
    }

    //cout << "checkpoint 1" << endl;
    sortByAngle(v);

    if (v.size() < 3) {
        return v;
    }

    Stack st;
    st.push(v[0]);
    st.push(v[1]);
    st.push(v[2]);

        for (int i = 3; i < v.size(); i++) {
            Point peek = st.pop();

            while (ccwHelper(st.peek(),peek, v[i])!= 1) {
                peek = st.pop();
            }
            st.push(peek);
            st.push(v[i]);
        }

    vector<Point> res;
    while (!st.isEmpty()){
        res.push_back(st.pop());
    }
        return res;
}

//swap two points
void swap(Point & p1, Point & p2){
    Point temp;
    temp = p1;
    p1 = p2;
    p2 = temp;
}

//find the angle between 2 points
double findAngle (Point p1, Point p2) {
    return atan2(p2.y - p1.y, p2.x - p1.x);
}

double sqDistance(Point p1, Point p2)  {
    return ((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y));
}