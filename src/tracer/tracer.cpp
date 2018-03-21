/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tracer.cpp
 * Author: gq
 * 
 * Created on March 21, 2018, 11:10 AM
 */

#include "tracer/tracer.h"


Tracer::Tracer() {
}


Tracer::~Tracer() {
}


void Tracer::trace(std::string objfile) {
    loader.load(objfile);
//    objfile = "aaa";
    
}

