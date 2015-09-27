//
//  ShapeCache.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-03.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__ShapeCache__
#define __GameDev2D__ShapeCache__

#include "../GameService.h"
#include "rapidxml.hpp"
#include "Box2D.h"


using namespace glm;
using namespace rapidxml;


namespace GameDev2D
{
    //Forward declarations
    class PhysicsObject;

    //The ShapeCache class will load and cache Box2D shapes generated with PhysicsEditor
    class ShapeCache : public GameService
    {
    public:
        ShapeCache();
        ~ShapeCache();
    
        //Loads the Shapes cache from an xml file exported from Physics Editor
        void LoadShapesFromFile(const string& shapesFile);

        //Adds the fixtures to a b2Body using the shape key
        void AddFixtures(b2Body* body, const string& key);
        
        //Adds the fixtures to a PhysicsObject using the shape key
        void AddFixtures(PhysicsObject* physicsObject, const string& key);

        //Gets the anchor point associated with a shape for the shape key
        vec2 GetAnchorPoint(const string& key);
    
        //Tells the ServiceLocator wether to Update and Draw this Game Service
        bool CanUpdate();
        bool CanDraw();
        
    private:
        //Forward declarations
        struct ShapeData;
    
        //Conveniance xml parsing methods
        void ParseMetaDataNode(xml_node<> *metaDataNode);
        void ParseBodiesNode(xml_node<> *bodiesNode);
        void ParseFixturesNode(xml_node<> *fixturesNode, ShapeData** shapeData);
        void ParsePolygonsNode(xml_node<> *polygonsNode, ShapeData** shapeData, b2FixtureDef* fixtureDef);
        
        //Conveniance string parsing methods, extracts a vector of floats contained with the string
        void ParseFloatData(const char* data, vector<float>& floats);
                
        //ShapeData struct to manage the fixture data
        struct ShapeData
        {
        public:
            ShapeData()
                : next(nullptr)
            {}

            ~ShapeData()
            {
                SafeDelete(next);
                SafeDelete(fixture.shape);
            }

            ShapeData* next;
            b2FixtureDef fixture;
        };

        //Member variables
        map<string, pair<ShapeData*, vec2>> m_ShapeCache;
        float m_PixelsToMetersRatio;
    };
}

#endif /* defined(__GameDev2D__ShapeCache__) */
