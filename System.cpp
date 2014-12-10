//
//  System.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "System.h"
#include "Engine.h"
#include <math.h>
#include <GLFW/glfw3.h>
#include "Terrain.h"

Engine *System::gameEngine;

System::System()
{
    
}

void System::setGameEngine(Engine *gEngine)
{
    gameEngine = gEngine;
}

void System::removeComponent(gMath::componentID cid)
{
    components.erase(components.begin() + cid); //this method will cause problems because all componentID's larger than cid in the table are now invalid. Need to either update all cid's higher than cid OR modify component creation method and update mehtod so they handle a sparse component array
}

System::~System()
{
    
}

//---------------------------------------------------------------------------------------------------------------------



Physics::Physics() : System()
{
    std::vector<PhysicsComponent> components;
}

void Physics::computeAlpha(PhysicsComponent *comp)
{
    
}
void Physics::computeAccel(PhysicsComponent *comp)
{
    if (comp->gravityEnabled)
    {
        comp->setAcceleration(gMath::vect {0,0,-10});
    }
}

void Physics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        
        gMath::state *s = components.at(i).getState();
        
        if (components.at(i).mouseControl)
        {
            double altitude = Input::altitude;
            double azimuth = Input::azimuth;
            
            s->orientation.s =  cos(altitude/2)*cos(azimuth/2);
            s->orientation.i =  sin(altitude/2)*sin(azimuth/2);
            s->orientation.j = -sin(altitude/2)*cos(azimuth/2);
            s->orientation.k =  cos(altitude/2)*sin(azimuth/2);
        }
        else
        {
            computeAlpha(&(components.at(i)));
            s->orientation = scalarMultiply(s->orientation,1/gMath::magnitude(s->orientation)); //normalizes quaternion
            s->orientation = hMultiply(padVector(gMath::scalarMultiply(s->omega,dt/2), 1),s->orientation);
            s->omega = addVect(s->omega, gMath::scalarMultiply(components.at(i).getAlpha(), dt));
        }
        
        computeAccel(&(components.at(i)));
        
        if (components.at(i).terrainEnabled) //TERRAIN COLLISION DETECTION
        {
            collisionDetect(s, i, dt);
        }
        else
        {
            s->pos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
            s->vel = addVect(s->vel, scalarMultiply(components.at(i).getAcceleration(), dt));
        }
    }
}

void Physics::collisionDetect(gMath::state *s, int i, float dt)
{
    gMath::vect iPls = (gMath::vect){.25,.25,.25};
    gMath::vect iNeg = (gMath::vect){.25,.25,1.6};
    
    bool moveX = true;
    bool moveY = true;
    bool moveZ = true;
    
    bool intersection;
    
    gMath::vect tempPos;
    
    float x0;
    float y0;
    float z0;
    
    float delX;
    float delY;
    float delZ;
    
    do {
        tempPos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt));
        
        x0 = s->pos.x;
        y0 = s->pos.y;
        z0 = s->pos.z;
        
        delX = tempPos.x - x0;
        delY = tempPos.y - y0;
        delZ = tempPos.z - z0;
        
        intersection = false;
        
        float xMin = x0 - iNeg.x - 1;
        float xMax = x0 + iPls.x;
        float yMin = y0 - iNeg.y - 1;
        float yMax = y0 + iPls.y;
        float zMin = z0 - iNeg.z - 1;
        float zMax = z0 + iPls.z;
        
        if (delX < 0)
            xMin += delX;
        else if (delX > 0)
            xMax += delX;
        
        if (delY < 0)
            yMin += delY;
        else if (delY > 0)
            yMax += delY;
        
        if (delZ < 0)
            zMin += delZ;
        else if (delZ > 0)
            zMax += delZ;
        
        if (xMin == ceil(xMin))
            xMin += .001;
        if (yMin == ceil(yMin))
            yMin += .001;
        if (zMin == ceil(zMin))
            zMin += .001;
        
        
        float intersectionTime = 1.0f;
        int intersectionFace = 0;
        
        int intersectionBlockX = 0,intersectionBlockY = 0,intersectionBlockZ = 0;
        
        for (int x = (int)ceil(xMin); x < xMax and intersectionTime > 0; x++)
        {
            for (int y = (int)ceil(yMin); y < yMax and intersectionTime > 0; y++)
            {
                for (int z = (int)ceil(zMin); z < zMax and intersectionTime > 0; z++)
                {
                    //ALL POSSIBLE BLOCKS:
                    
                    float tminX = std::min((-x + x0 - iNeg.x - 1)/(-delX), (-x + x0 + iPls.x)/(-delX));
                    float tmaxX = std::max((-x + x0 - iNeg.x - 1)/(-delX), (-x + x0 + iPls.x)/(-delX));
                    float tminY = std::min((-y + y0 - iNeg.y - 1)/(-delY), (-y + y0 + iPls.y)/(-delY));
                    float tmaxY = std::max((-y + y0 - iNeg.y - 1)/(-delY), (-y + y0 + iPls.y)/(-delY));
                    float tminZ = std::min((-z + z0 - iNeg.z - 1)/(-delZ), (-z + z0 + iPls.z)/(-delZ));
                    float tmaxZ = std::max((-z + z0 - iNeg.z - 1)/(-delZ), (-z + z0 + iPls.z)/(-delZ));
                    
                    if (!moveX)
                        tminX = -1; tmaxX = 1.0f/0.0f; //Infinity
                    if (!moveY)
                        tminY = -1; tmaxY = 1.0f/0.0f;
                    if (!moveZ)
                        tminZ = -1; tmaxZ = 1.0f/0.0f;
                    
                    
                    float tempIntersectionTime = std::max(std::max(tminX, tminY),tminZ);
                    float intersectionCheck = std::min(std::min(tmaxX,tmaxY),tmaxZ);
                    if (gameEngine->gameTerrain.getBlock(x, y, z) and (tempIntersectionTime >= 0) and (tempIntersectionTime < intersectionTime) and (tempIntersectionTime < intersectionCheck))
                    {
                        //SOLID BLOCKS THAT ARE ACTUALLY INTERSECTED:
                        if (tminX > tminY and tminX > tminZ and moveX)
                        {
                            intersectionTime = tempIntersectionTime;
                            intersectionFace = 1*((delX >= 0) - (delX < 0));
                            intersectionBlockX = x;
                            intersectionBlockY = y;
                            intersectionBlockZ = z;
                            intersection = true;
                        }
                        else if (tminY > tminZ and moveY)
                        {
                            intersectionTime = tempIntersectionTime;
                            intersectionFace = 2*((delY >= 0) - (delY < 0));
                            intersectionBlockX = x;
                            intersectionBlockY = y;
                            intersectionBlockZ = z;
                            intersection = true;
                        }
                        else if (moveZ)
                        {
                            intersectionTime = tempIntersectionTime;
                            intersectionFace = 3*((delZ >= 0) - (delZ < 0));
                            intersectionBlockX = x;
                            intersectionBlockY = y;
                            intersectionBlockZ = z;
                            intersection = true;
                        }
                    }
                }
            }
        }
        
        switch (intersectionFace)
        {
            case 0:
                break;
            case -1:
                moveX = false;
                s->pos.x = intersectionBlockX + 1 + iNeg.x;
                s->vel.x = 0.0f;
                break;
            case 1:
                moveX = false;
                s->pos.x = intersectionBlockX - iPls.x;
                s->vel.x = 0.0f;
                break;
            case -2:
                moveY = false;
                s->pos.y = intersectionBlockY + 1 + iNeg.y;
                s->vel.y = 0.0f;
                break;
            case 2:
                moveY = false;
                s->pos.y = intersectionBlockY - iPls.y;
                s->vel.y = 0.0f;
                break;
            case -3:
                moveZ = false;
                components.at(i).onGround = true;
                s->pos.z = intersectionBlockZ + 1 + iNeg.z;
                s->vel.z = 0.0f;
                break;
            case 3:
                moveZ = false;
                s->pos.z = intersectionBlockZ - iPls.z;
                s->vel.z = 0.0f;
                break;
            default:
                break;
                
            
        }
    } while (intersection);
    
    
    //UPDATE POSITION AND VELOCITY (FINALLY)
    
    if (moveX)
    {
        s->pos.x = s->pos.x + delX;
        s->vel.x = s->vel.x + components.at(i).getAcceleration().x * dt;
    }
    if (moveY)
    {
        s->pos.y = s->pos.y + delY;
        s->vel.y = s->vel.y + components.at(i).getAcceleration().y * dt;
    }
    if (moveZ)
    {
        s->pos.z = s->pos.z + delZ;
        s->vel.z = s->vel.z + components.at(i).getAcceleration().z * dt;
    }
    
    
}

gMath::componentID Physics::newComponent(gMath::entityID eid)
{
    gMath::state s;
    return newComponent(eid, s);
}

gMath::componentID Physics::newComponent(gMath::entityID eid, gMath::state s)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    PhysicsComponent newComp = PhysicsComponent(cid, eid, s);
    components.push_back(newComp);
    std::cout << "PhysicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Physics components list.\n";
    return cid;
}
PhysicsComponent *Physics::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Physics::~Physics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Graphics::Graphics() : System()
{
    std::vector<GraphicsComponent> components;
}

void Graphics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        gMath::componentID phys = gameEngine->getPhysicsComponent(components.at(i).getEntity());
        PhysicsComponent physComp = *Engine::gamePhysics.getComponent(phys);
        gMath::vect p = physComp.getState()->pos;
        gMath::quaternion q = physComp.getState()->orientation;
        
        components.at(i).draw(p, q);
    }
}

gMath::componentID Graphics::newComponent(gMath::entityID eid)
{
    gMath::mesh m;
    return newComponent(eid, m);
}
gMath::componentID Graphics::newComponent(gMath::entityID eid, gMath::mesh m)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    GraphicsComponent newComp(cid, eid, m);
    components.push_back(newComp);
    std::cout << "GraphicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Graphics components list.\n";
    return cid;
}
GraphicsComponent *Graphics::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Graphics::~Graphics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Logic::Logic() : System()
{
    std::vector<LogicComponent> components;
}
void Logic::update(float dt)
{
    //no gamelogic
}

gMath::componentID Logic::newComponent(gMath::entityID eid)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    LogicComponent newComp = LogicComponent(cid, eid);
    components.push_back(newComp);
    std::cout << "LogicComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Logic components list.\n";
    return cid;
}
LogicComponent *Logic::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Logic::~Logic()
{
    
}


//---------------------------------------------------------------------------------------------------------------------
char Input::xVel = 0;
char Input::yVel = 0;

double Input::altitude = 0.0;
double Input::azimuth = 0.0;

bool Input::isJumping = false;

Input::Input() : System(), inComp(InputComponent(0,0))
{
    
}
void Input::update(float dt)
{
    //Set camera updates here
    PhysicsComponent *physComp =gameEngine->gamePhysics.getComponent( gameEngine->getPhysicsComponent(gameEngine->cameraEntity));
    
    
    //gMath::quaternion oC = physComp->getState()->orientation;
    //float scale = (1.0f - 4 * oC.s*oC.s*oC.j*oC.j + 8*oC.s*oC.i*oC.j*oC.k - 4*oC.i*oC.i*oC.k*oC.k)/4.5;
    
    //std::cout << "scale: " << scale << '\n';
    
    if (physComp->getState()->vel.x*physComp->getState()->vel.x + physComp->getState()->vel.y*physComp->getState()->vel.y < 50.1)
    {
        physComp->getState()->vel.x = 5*(xVel*cos(azimuth) - yVel*sin(azimuth));
        physComp->getState()->vel.y = 5*(xVel*sin(azimuth) + yVel*cos(azimuth));
    }
    if (physComp->onGround and isJumping)
    {
        physComp->getState()->vel.z = 5.0;
        physComp->onGround = false;
    }
    //physComp->isJumping = isJumping;
}

gMath::componentID Input::newComponent(gMath::entityID eid)
{
    //set callback functions here
    glfwSetCursorPosCallback(gameEngine->openGLServer.window, Input::mouseFunction);
    glfwSetKeyCallback(gameEngine->openGLServer.window, Input::keyFunction);
    return 0;
}
InputComponent *Input::getComponent()
{
    return &inComp;
}

void Input::mouseFunction(GLFWwindow * window, double xpos, double ypos)
{
    altitude = -(ypos)/400.0;
    if (altitude < -M_PI/2)
    {
        altitude = -M_PI/2;
    }
    else if (altitude > M_PI/2)
    {
        altitude = M_PI/2;
    }
    
    azimuth = -xpos/400.0;
    while (azimuth < -M_PI)
    {
        azimuth += 2*M_PI;
    }
    while (azimuth > M_PI)
    {
        azimuth -= 2*M_PI;
    }
    
    //PhysicsComponent *cameraComponent = gameEngine->gamePhysics.getComponent( gameEngine->getPhysicsComponent(gameEngine->cameraEntity));
    
    //gMath::quaternion oC = cameraComponent->getState()->orientation;
    
    //float scale = 100.0f*(1.0f - 4 * oC.s*oC.s*oC.j*oC.j + 8*oC.s*oC.i*oC.j*oC.k - 4*oC.i*oC.i*oC.k*oC.k);
    
    /*
     
     gMath::vect omega = {0.0f,0.0f,0.0f};
     omega.x =  (ypos-300.0f)*(-2*oC.i*oC.j - 2*oC.k*oC.s)/scale;
     omega.y =  (ypos-300.0f)*(oC.s*oC.s + oC.i*oC.i - oC.j*oC.j-oC.k*oC.k)/scale;
     omega.z = -(xpos-300.0f)/100.0f;
     
     omegaUpdate = omega;
     */
    //std::cout << "MOUSE X: " << floor(xpos) << " MOUSE Y: " << floor(ypos) << '\n';
    //glfwSetCursorPos(window, 300, 300);
}

void Input::keyFunction(GLFWwindow * window, int key, int scancode, int action, int modifierKeys)
{
    switch (key)
    {
        case 262: //RIGHT ARROW
        case 68:
            yVel = -(bool)action; //Super fancy. Converts presses (1) and repeats (2) to 1, and releases (0) to 0.
            break;
        case 263: //LEFT ARROW
        case 65:
            yVel = (bool)action;
            break;
        case 264: //DOWN ARROW
        case 83:
            xVel = -(bool)action;
            break;
        case 265: //UP ARROW
        case 87:
            xVel = (bool)action;
            break;
        case 32: //SPACEBAR
            isJumping = (bool)action;
            break;
        default:
            break;
    }
}

Input::~Input()
{
    
}