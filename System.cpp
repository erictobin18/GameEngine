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
            //orient object so "up" is in the z direction by rolling the object around its local x axis
            //First, find altitude and azimuth:
            /*
            double arg = -2 * s->orientation.s * s->orientation.j + 2 * s->orientation.i * s->orientation.k;
            while (arg < -1.0)
            {
                arg += 1.0;
            }
            while (arg > 1.0)
            {
                arg -= 1.0;
            }
            
            double altitude = asin(arg);
            double azimuth = atan((2*s->orientation.i*s->orientation.j+2*s->orientation.s*s->orientation.k)/(s->orientation.s*s->orientation.s+s->orientation.i*s->orientation.i-s->orientation.j*s->orientation.j-s->orientation.k*s->orientation.k));
            if ((s->orientation.s*s->orientation.s+s->orientation.i*s->orientation.i-s->orientation.j*s->orientation.j-s->orientation.k*s->orientation.k) < 0)
            {
                azimuth = azimuth + M_PI;
            }
            
             */
            //std::cout << "ALTITUDE: " << altitude << " AZIMUTH: " << azimuth << '\n';
            
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
        
        if (components.at(i).terrainEnabled)
        {
            std::cout << "pos.x: " << s->pos.x << '\n';
            std::cout << "vel.x: " << s->vel.x << '\n';
            
            gMath::vect tempPos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt));
            
            std::cout << "tempX: " << tempPos.x << '\n';
            
            float posX = (int)s->pos.x;
            float posY = (int)s->pos.y;
            float posZ = (int)s->pos.z;
            float posXf = s->pos.x - posX;
            float delX = tempPos.x - s->pos.x;
            
            std::cout << "posX:  " << posX << '\n';
            std::cout << "posXf: " << posXf << '\n';
            std::cout << "delX:  " << delX << '\n';
            
            
            if (posXf < .75 && posXf + delX > .75 && (gameEngine->gameTerrain).getBlock(posX + 1,posY,posZ))
            {
                s->pos.x = posX + .75;
                s->vel.x = 0.0f;
                
                std::cout << "pos.x: " << s->pos.x << '\n';
                std::cout << "vel.x: " << s->vel.x << "\n\n";
            }
            else if (posXf > .25 && posXf + delX < .25 && (gameEngine->gameTerrain).getBlock(posX - 1,posY,posZ))
            {
                s->pos.x = posX + .25;
                s->vel.x = 0.0f;
                std::cout << "pos.x: " << s->pos.x << '\n';
                std::cout << "vel.x: " << s->vel.x << "\n\n";
            }
            else
            {
                s->pos.x = s->pos.x + delX;
                s->vel.x = s->vel.x + components.at(i).getAcceleration().x * dt;
                std::cout << "pos.x: " << s->pos.x << '\n';
                std::cout << "vel.x: " << s->vel.x << "\n\n";
            }
            
            float posYf = s->pos.y - posY;
            float delY = tempPos.y - s->pos.y;
            
            if (posYf < .75 && posYf + delY > .75 && (gameEngine->gameTerrain).getBlock(posX,posY + 1,posZ))
            {
                s->pos.y = posY + .75;
                s->vel.y = 0.0f;
            }
            else if (posYf > .25 && posYf + delY < .25 && (gameEngine->gameTerrain).getBlock(posX,posY - 1,posZ))
            {
                s->pos.y = posY + .25;
                s->vel.y = 0.0f;
            }
            else
            {
                s->pos.y = s->pos.y + delY;
                s->vel.y = s->vel.y + components.at(i).getAcceleration().y * dt;
            }
            
            float posZf = s->pos.z - posZ;
            float delZ = tempPos.z - s->pos.z;
            
            
            if (posZf < .75 && posZf + delZ > .75 && (gameEngine->gameTerrain).getBlock(posX,posY,posZ + 1))
            {
                s->pos.z = posZ + .75;
                s->vel.z = 0.0f;
            }
            else if (posZf >= .60 && posZf + delZ < .60 && (gameEngine->gameTerrain).getBlock(posX,posY,posZ - 2))
            {
                s->pos.z = posZ + .60;
                s->vel.z = 0.0f + components.at(i).mouseControl*Input::isJumping*5.5f;
            }
            else
            {
                s->pos.z = s->pos.z + delZ;
                s->vel.z = s->vel.z + components.at(i).getAcceleration().z * dt;
            }

        }
        else
        {
            s->pos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
            s->vel = addVect(s->vel, scalarMultiply(components.at(i).getAcceleration(), dt));
        }
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
    
    if (physComp->getState()->vel.x*physComp->getState()->vel.x + physComp->getState()->vel.y*physComp->getState()->vel.y < 32.1)
    {
        physComp->getState()->vel.x = 4*(xVel*cos(azimuth) - yVel*sin(azimuth));
        physComp->getState()->vel.y = 4*(xVel*sin(azimuth) + yVel*cos(azimuth));
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