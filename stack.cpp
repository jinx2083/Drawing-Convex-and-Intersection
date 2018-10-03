//
// Created by XYX on 2018/2/23.
//

#include "stack.h"


using namespace std;

// returns the top element of the stack
// returns a default (0,0) point if stack is empty
//todo
Point Stack::peek() {
    if (isEmpty()){
        return Point(0,0);
    }
    else return st[size()-1];
}

// returns number of elements in the stack
//todo
int Stack::size() {
    return st.size();
}

// returns true if the stack is empty, false otherwise
//todo
bool Stack::isEmpty() {
    return st.empty();
}

// mutators
// adds an item to the top of the stack
//todo
void Stack::push(Point p) {
    st.push_back(p);
}



// removes and returns the top element of the stack
// returns a default (0,0) point if stack is empty
//todo
Point Stack::pop() {
    if (isEmpty()){
        return Point(0,0);
    }
    else  {
        Point p = st[size()-1];
        st.pop_back();
        return p;
    }
}



