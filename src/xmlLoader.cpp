#include "xmlLoader.h"
using namespace tinyxml2;

Scene* XMLLoader::parseSceneXML(std::string fpath) {
    int ret = 0;
    tinyxml2::XMLDocument document;
    XMLError error = document.LoadFile(fpath.c_str());
  
    if (error != XML_SUCCESS) {
	    return NULL;
    }
    XMLElement* root = document.RootElement();
    XMLElement* sceneNode = root->FirstChildElement();

    XMLElement* sceneElement = sceneNode->FirstChildElement();
    Scene* scene = new Scene();
    while(sceneElement != NULL) {
        std::string elementType = sceneElement->Name();
        if(elementType == "camera") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();
       
            float xFoV = 60.f;
            float yFoV = 60.f;
            float nearClip = 0.1f;
            float farClip = 1000.f;
            float aspect = 4.f/3.f;
            XMFLOAT3 up(0.f, 1.f, 0.f);
            XMFLOAT3 lookAt(0.f, 0.f, 10.f);
            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "persepective") {
                    elementInfo->QueryFloatAttribute("xfov", &xFoV);
                    elementInfo->QueryFloatAttribute("yfov", &yFoV);
                    elementInfo->QueryFloatAttribute("near", &nearClip);
                    elementInfo->QueryFloatAttribute("far", &farClip);
                    elementInfo->QueryFloatAttribute("aspect", &aspect);
                } else if(info == "lookat") { 
                    elementInfo->QueryFloatAttribute("x", &lookAt.x);
                    elementInfo->QueryFloatAttribute("y", &lookAt.y);
                    elementInfo->QueryFloatAttribute("z", &lookAt.z);
                    elementInfo->QueryFloatAttribute("upX", &up.x);
                    elementInfo->QueryFloatAttribute("upY", &up.y);
                    elementInfo->QueryFloatAttribute("upZ", &up.z);
                } else if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "rotate") {
                    elementInfo->QueryFloatAttribute("axisX", &rotateAxis.x);
                    elementInfo->QueryFloatAttribute("axisY", &rotateAxis.y);
                    elementInfo->QueryFloatAttribute("axisZ", &rotateAxis.z);
                    elementInfo->QueryFloatAttribute("degrees", &rotateDegrees);                    
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }
        } else if(elementType == "pointLight") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();
            
            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 color(1.f, 1.f, 1.f);

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "color") {
                    elementInfo->QueryFloatAttribute("r", &color.x);
                    elementInfo->QueryFloatAttribute("g", &color.y);
                    elementInfo->QueryFloatAttribute("b", &color.z);                    
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create pointlight
            PointLight* pointLight = new PointLight();
            pointLight->setPosition(translate);

            //add pointlight to scene
            scene->addLight(pointLight);

        } else if(elementType == "areaLight") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();

            float width = sceneElement->FloatAttribute("width");
            float height = sceneElement->FloatAttribute("height");

            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 color(1.f, 1.f, 1.f);
            XMFLOAT3 target = translate;
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            XMFLOAT3 scale(1.f, 1.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "color") {
                    elementInfo->QueryFloatAttribute("r", &color.x);
                    elementInfo->QueryFloatAttribute("g", &color.y);
                    elementInfo->QueryFloatAttribute("b", &color.z);                    
                } else if(info == "target") {
                    elementInfo->QueryFloatAttribute("x", &target.x);
                    elementInfo->QueryFloatAttribute("y", &target.y);
                    elementInfo->QueryFloatAttribute("z", &target.z);                   
                } else if(info == "rotate") {
                    elementInfo->QueryFloatAttribute("axisX", &rotateAxis.x);
                    elementInfo->QueryFloatAttribute("axisY", &rotateAxis.y);
                    elementInfo->QueryFloatAttribute("axisZ", &rotateAxis.z);
                    elementInfo->QueryFloatAttribute("degrees", &rotateDegrees);                     
                } else if(info == "scale") {
                    elementInfo->QueryFloatAttribute("x", &scale.x);
                    elementInfo->QueryFloatAttribute("y", &scale.y);
                    elementInfo->QueryFloatAttribute("z", &scale.z);                 
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }


            //create arealight
            AreaLight* areaLight = new AreaLight();
            areaLight->width = width;
            areaLight->height = height;
            areaLight->setPosition(translate);
            areaLight->setRotation(rotateAxis, rotateDegrees);
            areaLight->setScale(scale);

            //add arealight to scene as primitive and light
            scene->addPrimitive(areaLight);
            scene->addLight(areaLight);

        } else if(elementType == "sphere") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();

            // get sphere information
            float radius = sceneElement->FloatAttribute("radius");

            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 color(1.f, 1.f, 1.f);
            XMFLOAT3 scale(1.f, 1.f, 1.f);
            XMFLOAT3 target = translate;
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "color") {
                    elementInfo->QueryFloatAttribute("r", &color.x);
                    elementInfo->QueryFloatAttribute("g", &color.y);
                    elementInfo->QueryFloatAttribute("b", &color.z);                  
                } else if(info == "rotate") {
                    elementInfo->QueryFloatAttribute("axisX", &rotateAxis.x);
                    elementInfo->QueryFloatAttribute("axisY", &rotateAxis.y);
                    elementInfo->QueryFloatAttribute("axisZ", &rotateAxis.z);
                    elementInfo->QueryFloatAttribute("degrees", &rotateDegrees);                     
                } else if(info == "scale") {
                    elementInfo->QueryFloatAttribute("x", &scale.x);
                    elementInfo->QueryFloatAttribute("y", &scale.y);
                    elementInfo->QueryFloatAttribute("z", &scale.z);                 
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create sphere
            Sphere* sphere = new Sphere();
            sphere->SetRadius(radius);
            sphere->setPosition(translate);
            sphere->setRotation(rotateAxis, rotateDegrees);
            sphere->setScale(scale);

            //add sphere to scene
            scene->addPrimitive(sphere);

        } else if(elementType == "cube") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();

            float edgelen = sceneElement->FloatAttribute("edgelen");

            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 color(1.f, 1.f, 1.f);
            XMFLOAT3 scale(1.f, 1.f, 1.f);
            XMFLOAT3 target = translate;
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "color") {
                    elementInfo->QueryFloatAttribute("r", &color.x);
                    elementInfo->QueryFloatAttribute("g", &color.y);
                    elementInfo->QueryFloatAttribute("b", &color.z);                   
                } else if(info == "rotate") {
                    elementInfo->QueryFloatAttribute("axisX", &rotateAxis.x);
                    elementInfo->QueryFloatAttribute("axisY", &rotateAxis.y);
                    elementInfo->QueryFloatAttribute("axisZ", &rotateAxis.z);
                    elementInfo->QueryFloatAttribute("degrees", &rotateDegrees);                     
                } else if(info == "scale") {
                    elementInfo->QueryFloatAttribute("x", &scale.x);
                    elementInfo->QueryFloatAttribute("y", &scale.y);
                    elementInfo->QueryFloatAttribute("z", &scale.z);                  
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create cube
            Cube* cube = new Cube();
            cube->SetEdgeLen(edgelen);
            cube->setPosition(translate);
            cube->setRotation(rotateAxis, rotateDegrees);
            cube->setScale(scale);

            //add cube to scene
            scene->addPrimitive(cube);

        } else if(elementType == "square") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();

            float edgelen = sceneElement->FloatAttribute("edgelen");

            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 color(1.f, 1.f, 1.f);
            XMFLOAT3 scale(1.f, 1.f, 1.f);
            XMFLOAT3 target = translate;
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "translate") { 
                    elementInfo->QueryFloatAttribute("x", &translate.x);
                    elementInfo->QueryFloatAttribute("y", &translate.y);
                    elementInfo->QueryFloatAttribute("z", &translate.z);
                } else if(info == "color") {
                    elementInfo->QueryFloatAttribute("r", &color.x);
                    elementInfo->QueryFloatAttribute("g", &color.y);
                    elementInfo->QueryFloatAttribute("b", &color.z);                 
                } else if(info == "rotate") {
                    elementInfo->QueryFloatAttribute("axisX", &rotateAxis.x);
                    elementInfo->QueryFloatAttribute("axisY", &rotateAxis.y);
                    elementInfo->QueryFloatAttribute("axisZ", &rotateAxis.z);
                    elementInfo->QueryFloatAttribute("degrees", &rotateDegrees);                      
                } else if(info == "scale") {
                    elementInfo->QueryFloatAttribute("x", &scale.x);
                    elementInfo->QueryFloatAttribute("y", &scale.y);
                    elementInfo->QueryFloatAttribute("z", &scale.z);                     
                } else {
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create square
            Square* square = new Square();
            square->SetEdgeLen(edgelen);
            square->setPosition(translate);
            square->setRotation(rotateAxis, rotateDegrees);
            square->setScale(scale);

            //add square to scene
            scene->addPrimitive(square);

        } else {

        }
        sceneElement = sceneElement->NextSiblingElement();
    }

    return scene;

}