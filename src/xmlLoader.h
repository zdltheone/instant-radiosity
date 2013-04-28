#ifndef __xmlloader_h__
#define __xmlloader_h__

#include "scene.h"
#include "tinyxml2.h"
#include "geometryObjects.h"
#include <string>
#include <iostream>

class XMLLoader {
public:
    static Scene* parseSceneXML(std::string fpath);
};

#endif