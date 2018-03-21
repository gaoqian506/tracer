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
    context_ = optix::Context::create();
    context_->setRayTypeCount( 2 );
}


Tracer::~Tracer() {
}


void Tracer::trace(std::string objfile) {
    
    OptiXMesh mesh;
    mesh.context = context_;
    loadMesh(objfile, mesh);
    bake(mesh);
   
}

void Tracer::bake(const OptiXMesh& mesh) {
    
    
    
}



/*
 
     
    //   loader.load(objfile);
//    objfile = "aaa";
 
 */