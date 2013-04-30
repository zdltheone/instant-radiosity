#include "xmlLoader.h"
using namespace tinyxml2;

Scene* XMLLoader::parseSceneXML(std::string fpath) {
    tinyxml2::XMLDocument document;
    XMLError error = document.LoadFile(fpath.c_str());
  
    if (error != XML_SUCCESS) {
	    return NULL;
    }
    XMLElement* root = document.RootElement();
   

    XMLElement* sceneElement = root->FirstChildElement();
    Scene* scene = new Scene();
    while(sceneElement != NULL) {
        std::string elementType = sceneElement->Name();
        if(elementType == "camera") {
            XMLElement* elementInfo = sceneElement->FirstChildElement();
       
            float xFoV = 60.f;
            float yFoV = 60.f;
            float nearClip = 0.1f;
            float farClip = 1000.f;
            float width = 800.f;
            float height = 600.f;
            XMFLOAT3 up(0.f, 1.f, 0.f);
            XMFLOAT3 lookAt(0.f, 0.f, 10.f);
            XMFLOAT3 translate(0.f, 0.f, 0.f);
            XMFLOAT3 rotateAxis(0.f, 0.f, 1.f);
            float rotateDegrees = 0.f;

            while(elementInfo != NULL) {
                std::string info = elementInfo->Name();
                if(info == "perspective") {
                    elementInfo->QueryFloatAttribute("xfov", &xFoV);
                    elementInfo->QueryFloatAttribute("yfov", &yFoV);
                    elementInfo->QueryFloatAttribute("near", &nearClip);
                    elementInfo->QueryFloatAttribute("far", &farClip);
                    elementInfo->QueryFloatAttribute("width", &width);
                    elementInfo->QueryFloatAttribute("height", &height);
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
                    std::cout << __LINE__ << " -- boom\n";
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create camera
            Camera* camera = new Camera(width, height, xFoV, yFoV, nearClip, farClip);
            camera->setPosition(translate);
            camera->setTarget(lookAt, up);
            

            //set camera in scene
            scene->setCamera(camera);

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
                    std::cout << __LINE__ << " -- boom\n";
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
                    std::cout << __LINE__ << " -- boom\n";
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

            XMVECTOR pos = XMLoadFloat3(&translate);
            XMVECTOR tar = XMLoadFloat3(&target);
            XMStoreFloat3(&areaLight->direction, XMVector3Normalize(tar - pos));

            //add arealight to scene
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
                    std::cout << __LINE__ << " -- boom\n";
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create sphere
            Sphere* sphere = new Sphere();
            sphere->SetRadius(radius);
            sphere->setPosition(translate);
            sphere->setRotation(rotateAxis, rotateDegrees);
            sphere->setScale(scale);
            
            sphere->color = color;

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
                    std::cout << __LINE__ << " -- boom\n";
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create cube
            Cube* cube = new Cube();
            cube->SetEdgeLen(edgelen);
            cube->setPosition(translate);
            cube->setRotation(rotateAxis, rotateDegrees);
            cube->setScale(scale);

            cube->color = color;

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
                    std::cout << __LINE__ << " -- boom\n";
                }

                elementInfo = elementInfo->NextSiblingElement();
            }

            //create square
            Square* square = new Square();
            square->SetEdgeLen(edgelen);
            square->setPosition(translate);
            square->setRotation(rotateAxis, rotateDegrees);
            square->setScale(scale);

            square->color = color;

            //add square to scene
            scene->addPrimitive(square);

        } else {
            std::cout << __LINE__ << " -- boom\n";
        }
        sceneElement = sceneElement->NextSiblingElement();
    }

    return scene;

}