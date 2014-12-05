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
        
        if (components.at(i).terrainEnabled) //TERRAIN COLLISION DETECTION
        {
            
            gMath::vect tempPos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt));
            
            float posX = (int)s->pos.x;
            float posY = (int)s->pos.y;
            float posZ = (int)s->pos.z;
            
            
            float posXf = s->pos.x - posX;
            float delX = tempPos.x - s->pos.x;
            
            float posYf = s->pos.y - posY;
            float delY = tempPos.y - s->pos.y;
            
            float posZf = s->pos.z - posZ;
            float delZ = tempPos.z - s->pos.z;
            
            gMath::vect sf = (gMath::vect){posXf,posYf,posZf};
            gMath::vect delS = (gMath::vect){delX,delY,delZ};
            
            gMath::vect iPls = (gMath::vect){.25,.25,.25};
            gMath::vect iNeg = (gMath::vect){.25,.25,1.6};
            
            bool moveX = true;
            bool moveY = true;
            bool moveZ = true;
            
            gMath::vect initialMin = (gMath::vect){sf.x - iNeg.x, sf.y - iNeg.y, sf.z - iNeg.z};
            gMath::vect initialMax = (gMath::vect){sf.x + iPls.x, sf.y + iPls.y, sf.z + iPls.z};
            
            gMath::vect finalMin = (gMath::vect){sf.x - iNeg.x + delS.x, sf.y - iNeg.y + delS.y, sf.z - iNeg.z + delS.z};
            gMath::vect finalMax = (gMath::vect){sf.x + iPls.x + delS.x, sf.y + iPls.y + delS.y, sf.z + iPls.z + delS.z};
            
            std::cout << "posX:  " << posX << '\n';
            std::cout << "posY:  " << posY << '\n';
            std::cout << "posZ:  " << posZ << '\n';
            
            std::cout << "posXf: " << posXf << '\n';
            std::cout << "posYf: " << posYf << '\n';
            std::cout << "posZf: " << posZf << '\n';
            
            std::cout << "delX:  " << delX << '\n';
            std::cout << "delY:  " << delY << '\n';
            std::cout << "delZ:  " << delZ << '\n';
            
            std::cout << "initialMinX: " << initialMin.x << '\n';
            std::cout << "initialMinY: " << initialMin.y << '\n';
            std::cout << "initialMinZ: " << initialMin.z << '\n';
            
            std::cout << "initialMaxX: " << initialMax.x << '\n';
            std::cout << "initialMaxY: " << initialMax.y << '\n';
            std::cout << "initialMaxZ: " << initialMax.z << '\n';
            
            std::cout << "X For Condition 1: GREATER THAN: " << finalMin.x - 1 << '\n';
            std::cout << "X For Condition 2: LESS THAN:    " << finalMax.x << '\n';
            std::cout << "Sign:  " << (delX >= 0) - (delX < 0) << '\n';
            
            std::cout << "Y For Condition 1: GREATER THAN: " << finalMin.y - 1 << '\n';
            std::cout << "Y For Condition 2: LESS THAN:    " << finalMax.y << '\n';
            std::cout << "Sign:  " << (delY >= 0) - (delY < 0) << "\n\n";
            
            
            /*
            {
                
                 float xNeg = .25;
                 float xPls = .75;
                 
                 float yNeg = .25;
                 float yPls = .75;
                 
                 float zNeg = .60;
                 float zPls = .75;
                 
                
                
                
                
                
                 
                 bool checkXPls = (posXf <= xPls) && (posXf + delX > xPls);
                 bool checkXNeg = (posXf >= xNeg) && (posXf + delX < xNeg);
                 
                 bool checkYPls = (posYf <= yPls) && (posYf + delY > yPls);
                 bool checkYNeg = (posYf >= yNeg) && (posYf + delY < yNeg);
                 
                 bool checkZPls = (posZf <= zPls) && (posZf + delZ > zPls);
                 bool checkZNeg = (posZf >= zNeg) && (posZf + delZ < zNeg);
                 
                 std::cout << checkXPls + 2 * checkXNeg + 4 * checkYPls + 8 * checkYNeg + 16 * checkZPls + 32 * checkZNeg << '\n';
                 
                 bool moveX = true;
                 bool moveY = true;
                 bool moveZ = true;
                 
                 
                 
                 //CHECK IMMEDIATELY ADJACENT BLOCKS
                 
                 if (checkXPls && (gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ) or gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ - 1)))
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 checkXPls = checkXNeg = false;
                 }
                 
                 else if (checkXNeg && (gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ) or gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ - 1)))
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 checkXPls = checkXNeg = false;
                 }
                 
                 if (checkYPls && (gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ) or gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ - 1)))
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 checkYPls = checkYNeg = false;
                 }
                 
                 else if (checkYNeg && (gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ) or gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ - 1)))
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 checkYPls = checkYNeg = false;
                 }
                 
                 if (checkZPls && (gameEngine->gameTerrain.getBlock(posX, posY, posZ + 1) or gameEngine->gameTerrain.getBlock(posX, posY, posZ)))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkZNeg && (gameEngine->gameTerrain.getBlock(posX, posY, posZ - 1) or gameEngine->gameTerrain.getBlock(posX, posY, posZ - 2)))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 checkZPls = checkZNeg = false;
                 }
                 
                 std::cout << checkXPls + 2 * checkXNeg + 4 * checkYPls + 8 * checkYNeg + 16 * checkZPls + 32 * checkZNeg << '\n';
                 std::cout << moveZ << "\n\n";
                 
                 //CHECK BLOCKS THAT ARE ADJACENT TO AN EDGE
                 
                 //X & Y
                 
                 if (checkXNeg or checkXPls or checkYNeg or checkYPls or checkZNeg or checkZPls)
                 {
                 
                 if (checkXPls && checkYPls && (gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ) or gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ - 1)))
                 {
                 if (delX/delY > (xPls - posXf)/(yPls - posYf + .0001))
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 }
                 else
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 }
                 checkXPls = checkXNeg = checkYPls = checkYNeg = false;
                 }
                 
                 else if (checkXPls && checkYNeg && (gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ) or gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ - 1)))
                 {
                 if (delX/delY > (xPls - posXf)/(yNeg - posYf - .0001))
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 }
                 else
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 }
                 checkXPls = checkXNeg = checkYPls = checkYNeg = false;
                 }
                 
                 else if (checkXNeg && checkYNeg && (gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ) or gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ - 1)))
                 {
                 if (delX/delY > (xNeg - posXf)/(yNeg - posYf - .0001))
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 }
                 else
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 }
                 checkXPls = checkXNeg = checkYPls = checkYNeg = false;
                 }
                 
                 else if (checkXNeg && checkYPls && (gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ) or gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ - 1)))
                 {
                 if (delX/delY > (xNeg - posXf)/(yPls - posYf + .0001))
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 }
                 else
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 }
                 checkXPls = checkXNeg = checkYPls = checkYNeg = false;
                 }
                 
                 //X & Z
                 
                 if (checkXPls && checkZPls && (gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ + 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ)))
                 {
                 if (delX/delZ > (xPls - posXf)/(zPls - posZf + .0001))
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 }
                 else
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 }
                 checkXPls = checkXNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkXPls && checkZNeg && (gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ - 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY, posZ - 2)))
                 {
                 if (delX/delZ > (xPls - posXf)/(zNeg - posZf - .0001))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 }
                 else
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 }
                 checkXPls = checkXNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkXNeg && checkZNeg && (gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ - 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ - 2)))
                 {
                 if (delX/delZ > (xNeg - posXf)/(zNeg - posZf - .0001))
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 }
                 else
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 }
                 checkXPls = checkXNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkXNeg && checkZPls && (gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ + 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY, posZ)))
                 {
                 if (delX/delZ > (xNeg - posXf)/(zPls - posZf + .0001))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 }
                 else
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 }
                 checkXPls = checkXNeg = checkZPls = checkZNeg = false;
                 }
                 
                 //Y & Z
                 
                 if (checkYPls && checkZPls && (gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ)))
                 {
                 if (delY/delZ > (yPls - posYf)/(zPls - posZf + .0001))
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 }
                 else
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 }
                 checkYPls = checkYNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkYPls && checkZNeg && (gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX, posY + 1, posZ - 2)))
                 {
                 if (delY/delZ > (yPls - posYf)/(zNeg - posZf - .0001))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 }
                 else
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 }
                 checkYPls = checkYNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkYNeg && checkZNeg && (gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ - 2)))
                 {
                 if (delY/delZ > (yNeg - posYf)/(zNeg - posZf - .0001))
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 }
                 else
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 }
                 checkYPls = checkYNeg = checkZPls = checkZNeg = false;
                 }
                 
                 else if (checkYNeg && checkZPls && (gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX, posY - 1, posZ)))
                 {
                 if (delY/delZ > (yNeg - posYf)/(zPls - posZf + .0001))
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 }
                 else
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 }
                 checkYPls = checkYNeg = checkZPls = checkZNeg = false;
                 }
                 
                 }
                 //CHECK BLOCKS THAT ARE ADJACENT TO A CORNER
                 
                 if (checkXNeg or checkXPls or checkYNeg or checkYPls or checkZNeg or checkZPls)
                 {
                 if (checkXNeg && checkYNeg && checkZNeg && (gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ -2)))
                 {
                 if ((xNeg - posXf)/delX < (yNeg - posYf)/delY and (xNeg - posXf)/delX < (zNeg - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yNeg - posYf)/delY < (xNeg - posXf)/delX and (yNeg - posYf)/delY < (zNeg - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXNeg && checkYNeg && checkZPls && (gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY - 1, posZ)))
                 {
                 if ((xNeg - posXf)/delX < (yNeg - posYf)/delY and (xNeg - posXf)/delX < (zPls - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yNeg - posYf)/delY < (xNeg - posXf)/delX and (yNeg - posYf)/delY < (zPls - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXNeg && checkYPls && checkZNeg && (gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ -2)))
                 {
                 if ((xNeg - posXf)/delX < (yPls - posYf)/delY and (xNeg - posXf)/delX < (zNeg - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yPls - posYf)/delY < (xNeg - posXf)/delX and (yPls - posYf)/delY < (zNeg - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXNeg && checkYPls && checkZPls && (gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX - 1, posY + 1, posZ)))
                 {
                 if ((xNeg - posXf)/delX < (yPls - posYf)/delY and (xNeg - posXf)/delX < (zPls - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xNeg;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yPls - posYf)/delY < (xNeg - posXf)/delX and (yPls - posYf)/delY < (zPls - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXPls && checkYNeg && checkZNeg && (gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ - 2)))
                 {
                 if ((xPls - posXf)/delX < (yNeg - posYf)/delY and (xPls - posXf)/delX < (zNeg - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yNeg - posYf)/delY < (xPls - posXf)/delX and (yNeg - posYf)/delY < (zNeg - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXPls && checkYNeg && checkZPls && (gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY - 1, posZ)))
                 {
                 if ((xPls - posXf)/delX < (yNeg - posYf)/delY and (xPls - posXf)/delX < (zPls - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yNeg - posYf)/delY < (xPls - posXf)/delX and (yNeg - posYf)/delY < (zPls - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yNeg;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXPls && checkYPls && checkZNeg && (gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ - 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ - 2)))
                 {
                 if ((xPls - posXf)/delX < (yPls - posYf)/delY and (xPls - posXf)/delX < (zNeg - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yPls - posYf)/delY < (xPls - posXf)/delX and (yPls - posYf)/delY < (zNeg - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zNeg;
                 s->vel.z = Input::isJumping*6;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 
                 if (checkXPls && checkYPls && checkZPls && (gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ + 1) or gameEngine->gameTerrain.getBlock(posX + 1, posY + 1, posZ)))
                 {
                 if ((xPls - posXf)/delX < (yPls - posYf)/delY and (xPls - posXf)/delX < (zPls - posZf)/delZ) //intersects x = .25 first
                 {
                 moveX = false;
                 s->pos.x = posX + xPls;
                 s->vel.x = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else if ((yPls - posYf)/delY < (xPls - posXf)/delX and (yPls - posYf)/delY < (zPls - posZf)/delZ) //intersects y = .25 first
                 {
                 moveY = false;
                 s->pos.y = posY + yPls;
                 s->vel.y = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 else //intersects z = .25 first
                 {
                 moveZ = false;
                 s->pos.z = posZ + zPls;
                 s->vel.z = 0.0f;
                 checkXNeg = checkXPls = checkYNeg = checkYPls = checkZNeg = checkZPls = false;
                 }
                 }
                 }
                
                
            }
             */
            for (int xIndex = 0; (xIndex > finalMin.x - 1) and (xIndex < finalMax.x) /*and (!xIndex or moveX)*/; xIndex += (delX >= 0) - (delX < 0))
            {
                for (int yIndex = 0; (yIndex > finalMin.y - 1) and (yIndex < finalMax.y) /*and (!yIndex or moveY)*/; yIndex += (delY >= 0) - (delY < 0))
                {
                    int zIndex = 0;
                    int blockID = gameEngine->gameTerrain.getBlock(posX + xIndex, posY + yIndex, posZ + zIndex);
                    int condition = Physics::testBlock(xIndex, yIndex, zIndex, initialMin, initialMax, delS, blockID,moveX,moveY,moveZ);
                    Physics::impact(condition, &moveX, &moveY, &moveZ, s, iNeg, iPls);
                    
                    std::cout << "xIndex: " << xIndex << '\n';
                    std::cout << "yIndex: " << yIndex << '\n';
                    std::cout << "zIndex: 0\n";
                    std::cout << "BlockID: " << blockID << '\n';
                    std::cout << "Condition: " << condition << '\n';
                    
                    zIndex = -1;
                    blockID = gameEngine->gameTerrain.getBlock(posX + xIndex, posY + yIndex, posZ + zIndex);
                    condition = Physics::testBlock(xIndex, yIndex, zIndex, initialMin, initialMax, delS, blockID,moveX,moveY,moveZ);
                    Physics::impact(condition, &moveX, &moveY, &moveZ, s, iNeg, iPls);
                    
                    std::cout << "xIndex: " << xIndex << '\n';
                    std::cout << "yIndex: " << yIndex << '\n';
                    std::cout << "zIndex: -1\n";
                    std::cout << "BlockID: " << blockID << '\n';
                    std::cout << "Condition: " << condition << '\n';
                    
                    if (finalMax.z > 1 /*and moveZ*/)
                    {
                        zIndex = 1;
                        blockID = gameEngine->gameTerrain.getBlock(posX + xIndex, posY + yIndex, posZ + zIndex);
                        condition = Physics::testBlock(xIndex, yIndex, zIndex, initialMin, initialMax, delS, blockID,moveX,moveY,moveZ);
                        Physics::impact(condition, &moveX, &moveY, &moveZ, s, iNeg, iPls);
                        
                        std::cout << "xIndex: " << xIndex << '\n';
                        std::cout << "yIndex: " << yIndex << '\n';
                        std::cout << "zIndex: 1\n";
                        std::cout << "BlockID: " << blockID << '\n';
                        std::cout << "Condition: " << condition << '\n';
                    }
                    else if (finalMin.z < - 1 /*and moveZ*/)
                    {
                        zIndex = -2;
                        blockID = gameEngine->gameTerrain.getBlock(posX + xIndex, posY + yIndex, posZ + zIndex);
                        condition = Physics::testBlock(xIndex, yIndex, zIndex, initialMin, initialMax, delS, blockID, moveX,moveY,moveZ);
                        Physics::impact(condition, &moveX, &moveY, &moveZ, s, iNeg, iPls);
                        
                        std::cout << "xIndex: " << xIndex << '\n';
                        std::cout << "yIndex: " << yIndex << '\n';
                        std::cout << "zIndex: -2\n";
                        std::cout << "BlockID: " << blockID << '\n';
                        std::cout << "Condition: " << condition << '\n';
                    }
                }
            }
            
            std::cout << "MOVE X: " << moveX << '\n';
            std::cout << "MOVE Y: " << moveY << '\n';
            std::cout << "MOVE Z: " << moveZ << "\n\n\n";
            
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
            else
            {
                s->vel.z = Input::isJumping*6.0;
            }
            
            if (gameEngine->gameTerrain.getBlock(s->pos.x, s->pos.y, s->pos.z) or gameEngine->gameTerrain.getBlock(s->pos.x, s->pos.y, s->pos.z - 1))
            {
                std::cout << "\n\n\n--------ENTERED BLOCK------------\n\n\n";
            }
        }
        else
        {
            s->pos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
            s->vel = addVect(s->vel, scalarMultiply(components.at(i).getAcceleration(), dt));
        }
        
    }
}

int Physics::testBlock(int xIndex, int yIndex, int zIndex, gMath::vect iMin, gMath::vect iMax, gMath::vect delS, int blockID, bool moveX, bool moveY, bool moveZ)
{
    if ((iMin.x - 1 >= xIndex or iMax.x <= xIndex or iMin.y - 1 >= yIndex or iMax.y <= yIndex or iMin.z - 1 >= zIndex or iMax.z <= zIndex) and blockID)
    {
        //Since bounding blocks intersect after time dt, and do not initially intersect, there must be a time in between in which they share a face (intermediate value theorem!). At this time, the index corresponding to the shared face is on the borderline between intersecting and not intersecting, and the other two indices are intersected. Thus, if the time of intersection of all three indices is computed, the impact face corresonds to the index with the highest intersection time. Of course, there must be two such times for both indices because as time goes to infinity, the bounding blocks pass entirely through each other. Thus, the correct time to use for each index is the lower intersection time.
        
        float tx = -1; //intersection times, parameterized from t = 0 initially to t = 1 at time dt.
        float ty = -1; //Since intersections times are at least 0, values that are not modified from
        float tz = -1; //the initial -1 are not considered.
        
        if (xIndex and (!moveX or ((!yIndex or (yIndex and moveY)) and (!(zIndex == -2 or zIndex == 1) or ((zIndex == -2 or zIndex == 1) and moveZ))))) // if xIndex is 0, it is not possible to intersect its x-faces
        {
            tx = std::min((xIndex - iMax.x)/delS.x,(xIndex + 1 - iMin.x)/delS.x);
        }
        if (yIndex and (!moveY or ((!xIndex or (xIndex and moveX)) and (!(zIndex == -2 or zIndex == 1) or ((zIndex == -2 or zIndex == 1) and moveZ)))))
        {
            ty = std::min((yIndex - iMax.y)/delS.y,(yIndex + 1 - iMin.y)/delS.y);
        }
        if ((zIndex == -2 or zIndex == 1) and (!moveZ or ((!xIndex or (xIndex and moveX)) and (!yIndex or (yIndex and moveY))))) //player is two blocks tall, thus for zIndex = -1 it is not possible to intersect z-faces
        {
            tz = std::min((zIndex - iMax.z)/delS.z,(zIndex + 1 - iMin.z)/delS.z);
        }
        
        //Find maximum intersection time
        if (tx > ty and tx > tz) //Player will impact block on its x face
        {
            return 1 * ((delS.x >= 0) - (delS.x < 0));
        }
        else if (ty > tz) //Player will impact block on its y face
        {
            return 2 * ((delS.y >= 0) - (delS.y < 0));
        }
        else //Player will impact block on its z face
        {
            return 3 * ((delS.z >= 0) - (delS.z < 0));
        }
    }
    return 0;
}

void Physics::impact(int condition, bool *moveX, bool *moveY, bool *moveZ, gMath::state *s, gMath::vect iNeg, gMath::vect iPls)
{
    switch (condition)
    {
        case 0:
            break;
        case -1:
            *moveX = false;
            s->pos.x = floorf(s->pos.x) + iNeg.x;
            s->vel.x = 0.0f;
            break;
        case 1:
            *moveX = false;
            s->pos.x = floorf(s->pos.x) + 1 - iPls.x;
            s->vel.x = 0.0f;
            break;
        case -2:
            *moveY = false;
            s->pos.y = floorf(s->pos.y) + iNeg.y;
            s->vel.y = 0.0f;
            break;
        case 2:
            *moveY = false;
            s->pos.y = floorf(s->pos.y) + 1 - iPls.y;
            s->vel.y = 0.0f;
            break;
        case -3:
            *moveZ = false;
            s->pos.z = floorf(s->pos.z) + iNeg.z - 1;
            s->vel.z = 0.0f;
            break;
        case 3:
            *moveZ = false;
            s->pos.z = floorf(s->pos.z) + 1 - iPls.z;
            s->vel.z = 0.0f;
            break;
        default:
            break;
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