//
//  CharacterManager.cpp
//  GameDev2D
//
//  Created by Joel Cright on 2015-03-19.
//  Copyright (c) 2015 Algonquin College. All rights reserved.
//

#include"../../Source/Physics/Box2D/b2Helper.h"
#include "CharacterManager.h"
#include "../Character.h"
#include "../BaseObjects/Limb.h"
#include "../BaseObjects/Weapon.h"
#include "../../Source/Animation/Random.h"
#include "../../Source/Events/Input/TouchEvent.h"
#include "../../Source/Services/ServiceLocator.h"
#include "../../Source/Physics/PhysicsObject.h"
#include "PhysicsWorld.h"

namespace GameDev2D
{
    CharacterManager::CharacterManager(float aWidth, float aHeight, PhysicsWorld* aPhysicsWorld)
    {
        m_SwipeTimer = nullptr;
        m_MoveEnemies = nullptr;

        m_IsSwiping = false;
        m_DestroyingCharacter = false;
        
        m_CurrentSpawn = SpawnPointOne;

        m_WidthOfScreen = aWidth;
        m_HeightOfScreen = aHeight;
        
        m_CurrentTouches = 0;
        
        m_PhysicsWorld = aPhysicsWorld;
        
        m_Random = new Random;
        for(int i = 0; i < MAX_NUMBER_OF_CHARACTERS; i++)
        {
            m_Random->RandomizeSeed();
            
            if (i == 0)
                CreateCharacter(CharacterTypePlayer);
            else
                CreateCharacter();
        }
    }
    CharacterManager::~CharacterManager()
    {
        for (int i = 0; i < m_Characters.size(); i++)
        {
            SafeDelete(m_Characters.at(i));
        }

        if (m_Random != nullptr)
        {
            SafeDelete(m_Random);
            m_Random = nullptr;
        }
        if (m_SwipeTimer != nullptr)
        {
            SafeDelete(m_SwipeTimer);
            m_SwipeTimer = nullptr;
        }
        
        if(m_MoveEnemies != nullptr)
        {
            SafeDelete(m_MoveEnemies);
            m_MoveEnemies = nullptr;
        }
        
        for(int j = 0; j < m_FloatingLimbs.size(); j++) SafeDelete(m_FloatingLimbs.at(j));
    }
    
    
    
    void CharacterManager::Update(double aDelta)
    {
        if(m_DestroyingCharacter == false)
        {
            for (int i = 0; i < m_Characters.size(); i++)
            {
                if(m_Characters.at(i) != nullptr)
                    m_Characters.at(i)->Update(aDelta); //TODO: This is where the crash happens. It happens only AFTER the player has died.
            }
        }
        
        //Swipe timer
        if (m_SwipeTimer != nullptr)
        {
            m_SwipeTimer->Update(aDelta);
        }
        
        //Enemy movement
        if(m_MoveEnemies == nullptr)
        {
            m_MoveEnemies = new Timer(SECONDS_TO_WAIT_FOR_ENEMY_MOVEMENT);
            m_MoveEnemies->Start();
        }
        else if(m_MoveEnemies != nullptr)
        {
            m_MoveEnemies->Update(aDelta);
            if (m_MoveEnemies->IsRunning() == false)
            {
                MoveEnemies();
                SafeDelete(m_MoveEnemies);
                m_MoveEnemies = nullptr;
            }
        }
        
        typedef std::map<Character*, Timer*>::iterator it_type;
        for(it_type iterator = m_Timers.begin(); iterator != m_Timers.end(); ++iterator) {
            iterator->second->Update(aDelta);
            
            if (iterator->second->IsRunning() == false)
            {
                Character* character = iterator->first;
                
                DestroyCharacter(character);
                
                break;
            }
        }
    }
    void CharacterManager::Draw()
    {
        if(m_DestroyingCharacter == false)
        {
            for (int i = 0; i < m_Characters.size(); i++)
            {
                m_Characters.at(i)->Draw();
            }
        }
    }




    void CharacterManager::CreateCharacter(CharacterTypes aType)
    {
        m_Random->RandomizeSeed();

        switch (aType)
        {
        case CharacterTypePlayer:
        {
            //int numberOfLimbs, glm::vec2 rangeOfLimbSize, glm::vec2 rangeOfStats, glm::vec2 position, bool isPlayer = true
            m_Characters.insert(m_Characters.begin(), new Character(6, glm::vec2(0.2f, 1.5f), glm::vec2(0.2f, 1.5f), PLAYER_SPAWN, true));
            m_Characters.at(0)->CreateJoints(m_PhysicsWorld);
            break;
        }
                
        case CharacterTypeEnemy:
        {
            int numberOfLimbs = m_Random->RandomRange(MIN_NUMBER_OF_LIMBS, MAX_NUMBER_OF_LIMBS);
            glm::vec2 rangeOfLimbSize = glm::vec2(m_Random->RandomRange(MIN_LIMB_RANGE_SIZE.x, MIN_LIMB_RANGE_SIZE.y),
                m_Random->RandomRange(MAX_LIMB_RANGE_SIZE.x, MAX_LIMB_RANGE_SIZE.y));
            
            glm::vec2 rangeOfStats = glm::vec2(m_Random->RandomRange(MIN_STAT_RANGE.x, MIN_STAT_RANGE.y),
                m_Random->RandomRange(MAX_STAT_RANGE.x, MAX_STAT_RANGE.y));
            
            glm::vec2 position;
            
            switch (m_CurrentSpawn/*m_Random->RandomRange(0, 4)*/) {
                case SpawnPointOne:
                    position = glm::vec2( m_Random->RandomRange(SPAWN_LOCATION_X_RANGE.x, SPAWN_LOCATION_X_RANGE.y) + SPAWN_LOCATION_ONE.x,
                                                   m_Random->RandomRange(SPAWN_LOCATION_Y_RANGE.x, SPAWN_LOCATION_Y_RANGE.y) + SPAWN_LOCATION_ONE.y);

                    break;
                case SpawnPointTwo:
                    position = glm::vec2( m_Random->RandomRange(SPAWN_LOCATION_X_RANGE.x, SPAWN_LOCATION_X_RANGE.y) + SPAWN_LOCATION_TWO.x,
                                         m_Random->RandomRange(SPAWN_LOCATION_Y_RANGE.x, SPAWN_LOCATION_Y_RANGE.y) + SPAWN_LOCATION_TWO.y);
                    break;
                case SPawnPointThree:
                    position = glm::vec2( m_Random->RandomRange(SPAWN_LOCATION_X_RANGE.x, SPAWN_LOCATION_X_RANGE.y) + SPAWN_LOCATION_THREE.x,
                                         m_Random->RandomRange(SPAWN_LOCATION_Y_RANGE.x, SPAWN_LOCATION_Y_RANGE.y) + SPAWN_LOCATION_THREE.y);
                    break;
                case SpawnPointFour:
                    position = glm::vec2( m_Random->RandomRange(SPAWN_LOCATION_X_RANGE.x, SPAWN_LOCATION_X_RANGE.y) + SPAWN_LOCATION_FOUR.x,
                                         m_Random->RandomRange(SPAWN_LOCATION_Y_RANGE.x, SPAWN_LOCATION_Y_RANGE.y) + SPAWN_LOCATION_FOUR.y);
                    break;
                    
                default:
                    position = glm::vec2( m_Random->RandomRange(SPAWN_LOCATION_X_RANGE.x, SPAWN_LOCATION_X_RANGE.y) + SPAWN_LOCATION_ONE.x,
                                         m_Random->RandomRange(SPAWN_LOCATION_Y_RANGE.x, SPAWN_LOCATION_Y_RANGE.y) + SPAWN_LOCATION_ONE.y);
                    break;
            }
            
            bool isPlayer = false;

            m_Characters.push_back(new Character(numberOfLimbs, rangeOfLimbSize, rangeOfStats, position, isPlayer));
            m_Characters.back()->CreateJoints(m_PhysicsWorld);
            break;
        }

        default:
            break;
        }
        
        m_CurrentSpawn++;
        
        if(m_CurrentSpawn == SpawnPointFour)
            m_CurrentSpawn = SpawnPointOne;
        
        m_DestroyingCharacter = false;
    }
    
    void CharacterManager::RemoveCharacterAtIndex(int aIndex)
    {
        SafeDelete(m_Characters.at(aIndex));
        m_Characters.erase(m_Characters.begin() + aIndex);
        m_Characters.shrink_to_fit();
    }
    
    void CharacterManager::RemoveCharacter(Character* aCharacter)
    {
        for(int i = 0; i < m_Characters.size(); i++)
        {
            if(m_Characters.at(i) == aCharacter)
            {
                RemoveCharacterAtIndex(i);
                break;
            }
        }
    }
    
    
    
    int CharacterManager::GetIndexOfCharacter(Character* aCharacter)
    {
        for(int i = 0; i < m_Characters.size(); i++)
        {
            if(m_Characters.at(i) == aCharacter)
                return i;
        }
        
        return -1;
    }
    
    Character* CharacterManager::GetCharacterAtIndex(int aIndex)
    {
        return m_Characters.at(aIndex);
    }
    
    std::vector<Character*> CharacterManager::GetCharacters()
    {
        return m_Characters;
    }
    
    
    
    void CharacterManager::PrepareToDestroyCharacterAtIndex(int aIndex)
    {
        m_Characters.at(aIndex)->SetDestroyCharacter(true);
        m_Characters.at(aIndex)->SetDestructionSlating(true);
        
        Timer* timer = new Timer();
        timer = new Timer(SECONDS_TO_WAIT_FOR_CHARACTER_DESTRUCTION);
        timer->Start();
        
        m_Timers.insert(std::make_pair(m_Characters.at(aIndex), timer));
    }
    
    void CharacterManager::DestroyCharacter(Character* aCharacter)
    {
        m_DestroyingCharacter = true;
        
        CharacterTypes type;
        if(aCharacter->GetIsPlayer() == true)
            type = CharacterTypePlayer;
        else
            type = CharacterTypeEnemy;
        
        RemoveCharacter(aCharacter);
        
        m_Timers.at(aCharacter)->Stop();
        m_Timers.erase(aCharacter);
        
        CreateCharacter(type);
    }


    
    void CharacterManager::HandleTouchEvent(TouchEvent* aTouchEvent)
    {
        switch (aTouchEvent->GetTouchType())
        {
            case TouchBegan:
            {
                if(m_Characters.at(0)->GetIsPlayer() == true && m_Characters.at(0)->GetSlatedToDestroy() == false)
                {
                    m_Characters.at(0)->MoveCharacter(aTouchEvent->GetTouchLocation());

                    m_IsSwiping = false;
                    
                    m_CurrentTouchLocations.push_back(aTouchEvent->GetTouchLocation());
                    m_CurrentTouches++;
                    
                    switch(m_CurrentTouches)
                    {
                            
                    }
                }
                
                break;
            }
            case TouchMoved:
            {
                //TODO: Fine tune
                m_IsSwiping = true;

                if (m_SwipeTimer == nullptr)
                {
                    m_BeginSwipeLocation = aTouchEvent->GetPreviousLocation();
                    m_SwipeTimer = new Timer(SECONDS_TO_WAIT_FOR_SWIPE_EVENT);
                    m_SwipeTimer->Start();
                }
                else
                {
                    if (m_SwipeTimer->IsRunning() == false)
                    {
                        for (int i = 0; i < m_Characters.size(); i++)
                        {
                            if (m_Characters.at(i)->GetIsPlayer() == true)
                            {
                                m_Characters.at(i)->SwingInDirection(m_BeginSwipeLocation, aTouchEvent->GetTouchLocation());
                                break;
                            }
                        }
                        
                        SafeDelete(m_SwipeTimer);
                        m_SwipeTimer = nullptr;
                    }
                }
                break;
            }
            case TouchEnded:
            {
                m_CurrentTouches--;
                m_CurrentTouchLocations.clear();
                
                if (m_IsSwiping == true)
                {
                    if(m_SwipeTimer != nullptr)
                    {
                        m_SwipeTimer->Stop();
                        SafeDelete(m_SwipeTimer);
                        m_SwipeTimer = nullptr;
                    }
                    m_IsSwiping = false;
                    
                    break;
                }

                if (aTouchEvent->GetTouchLocation().x >= m_WidthOfScreen * .5 && aTouchEvent->GetTouchLocation().y >= m_HeightOfScreen * .5)    //Top Right
                {

                }
                if (aTouchEvent->GetTouchLocation().x >= m_WidthOfScreen * .5 && aTouchEvent->GetTouchLocation().y < m_HeightOfScreen * .5)     //Bottom Right
                {

                }
                if (aTouchEvent->GetTouchLocation().x < m_WidthOfScreen * .5 && aTouchEvent->GetTouchLocation().y >= m_HeightOfScreen * .5)     //Top Left
                {

                }
                if (aTouchEvent->GetTouchLocation().x < m_WidthOfScreen * .5 && aTouchEvent->GetTouchLocation().y < m_HeightOfScreen * .5)      //Bottom Left
                {

                }
                if (aTouchEvent->GetDuration() > SECONDS_TO_WAIT_FOR_SWIPE_EVENT)   //Held a touch
                {

                }

                break;
            }
            case TouchCancelled:
            {
                m_CurrentTouchLocations.clear();

                if (m_SwipeTimer != nullptr)
                {
                    m_SwipeTimer->Stop();
                    SafeDelete(m_SwipeTimer);
                    m_SwipeTimer = nullptr;
                }

                m_IsSwiping = false;
                m_CurrentTouches--;
                break;
            }
        }
    }




    //b2Contact methods
    void CharacterManager::OnContactStart(b2Contact* aContact)
    {
        b2Fixture* fixtureA = aContact->GetFixtureA();
        b2Fixture* fixtureB = aContact->GetFixtureB();

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        PhysicsObject* physicsObjectA = (PhysicsObject*)bodyA->GetUserData();
        PhysicsObject* physicsObjectB = (PhysicsObject*)bodyB->GetUserData();

        int indexOfFirstCharacter = -1;
        Limb* hurtLimb = nullptr;
        Weapon* weapon;
        bool foundWeapon = false;
        std::vector<Limb*> limbs;

        //TODO: Make these into a function
        //Check if first object is a weapon
        if (physicsObjectA->GetType() == WEAPON_TYPE)
        {
            for (int i = 0; i < m_Characters.size(); i++)
            {
                limbs = m_Characters.at(i)->GetLimbs();
                weapon = m_Characters.at(i)->GetWeapon();

                for (int j = 0; j < limbs.size(); j++)
                {
                    if (weapon->GetWeapon() == physicsObjectA)   //TOOD: Make this a weapon array
                    {
                        indexOfFirstCharacter = i;
                        foundWeapon = true;
                    }
                    if (limbs.at(j)->GetLimb() == physicsObjectB)
                    {
                        hurtLimb = limbs.at(j);
                    }
                }

                if (hurtLimb != nullptr && foundWeapon == true)
                {
                    if(m_Characters.at(i)->DealDamage(hurtLimb, m_Characters.at(indexOfFirstCharacter)->GetDamage()) == true)
                    {
                        m_FloatingLimbs.push_back(hurtLimb);
                    }
                    
                    if(m_Characters.at(i)->GetHealth() <= 0 && m_Characters.at(i)->GetSlatedToDestroy() == false)
                        PrepareToDestroyCharacterAtIndex(i);

                    break;
                }
            }
        }
        
        foundWeapon = false;
        
        //Check if second object is a weapon
        if (physicsObjectB->GetType() == WEAPON_TYPE)
        {
            for (int i = 0; i < m_Characters.size(); i++)
            {
                limbs = m_Characters.at(i)->GetLimbs();
                weapon = m_Characters.at(i)->GetWeapon();

                for (int j = 0; j < limbs.size(); j++)
                {
                    if (limbs.at(j)->GetLimb() == physicsObjectA)
                    {
                        indexOfFirstCharacter = i;
                        foundWeapon = true;
                    }
                    if (limbs.at(j)->GetLimb() == physicsObjectB)
                    {
                        hurtLimb = limbs.at(j);
                    }
                }
                

                if (hurtLimb != nullptr && foundWeapon == true)
                {
                    if(m_Characters.at(i)->DealDamage(hurtLimb, m_Characters.at(indexOfFirstCharacter)->GetDamage()))
                    {
                        m_FloatingLimbs.push_back(hurtLimb);
                    }
                    
                    if(m_Characters.at(i)->GetHealth() <= 0)
                        PrepareToDestroyCharacterAtIndex(i);

                    break;
                }
            }
        }
    }
    void CharacterManager::OnContactEnd(b2Contact* aContact)
    {

    }
    
    //Private Functions
    void CharacterManager::MoveEnemies()
    {
        for(int i = 1; i < m_Characters.size(); i++)
        {
            vec2 newPosition = m_Characters.at(i)->GetCharacterPosition();
            m_Random->RandomizeSeed();//TODO: Fix this always downward movement
            switch (m_Random->RandomRange(0, 3)) {  //TODO: Enum this?
                case 0:
                    newPosition += vec2(0, 1);
                    break;
                case 1:
                    newPosition += vec2(1, 0);
                    break;
                case 2:
                    newPosition += vec2(0, -1);
                    break;
                case 3:
                    newPosition += vec2(-1, 0);
                    break;
                    
                default:
                    break;
            }
            
            //m_Characters.at(i)->MoveCharacter(newPosition);
            //m_Characters.at(i)->MoveCharacter(m_Characters.at(i)->GetCharacterPosition());
        }
    }
}