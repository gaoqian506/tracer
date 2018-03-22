/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   objviewer.cpp
 * Author: gq
 *
 * Created on March 22, 2018, 8:46 AM
 */

#include <cstdlib>
#include "tracer/tracer.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    Tracer tracer;
    tracer.load("data/obj/cow.obj");
    tracer.show();
    
    return 0;
}

