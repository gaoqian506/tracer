/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tracer.h
 * Author: gq
 *
 * Created on March 21, 2018, 11:10 AM
 */

#ifndef TRACER_H
#define TRACER_H

#include <string>
//#include "sceneloader.h"
#include <optixu/optixpp_namespace.h>
#include <OptiXMesh.h>



class Tracer {
    
public:
    Tracer();
    virtual ~Tracer();
    
    void trace(std::string objfile);
private:
    
    void bake(const OptiXMesh& mesh);
    
    //SceneLoader loader;
    optix::Context context_;
    int width_;
    int height_;

};

#endif /* TRACER_H */

