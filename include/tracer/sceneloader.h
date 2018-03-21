/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sceneloader.h
 * Author: gq
 *
 * Created on March 21, 2018, 11:11 AM
 */

#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>

class SceneLoader {
public:
    SceneLoader();
    virtual ~SceneLoader();
    
    void load(std::string objfile);
private:

};

#endif /* SCENELOADER_H */

