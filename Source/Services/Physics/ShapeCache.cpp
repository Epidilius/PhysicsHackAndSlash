//
//  ShapeCache.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-12-03.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "ShapeCache.h"
#include "../ServiceLocator.h"
#include "../../Physics/PhysicsObject.h"
#include "../../Platforms/PlatformLayer.h"
#include "../../IO/File.h"


namespace GameDev2D
{
    ShapeCache::ShapeCache() : GameService("ShapeCache"),
        m_PixelsToMetersRatio(0)
    {
    
    }
    
    ShapeCache::~ShapeCache()
    {
        //Unload all the shape data objects
        while(m_ShapeCache.size() > 0)
        {
            SafeDelete(m_ShapeCache.begin()->second.first);
            m_ShapeCache.erase(m_ShapeCache.begin());
        }
    }
    
    void ShapeCache::LoadShapesFromFile(const string& aShapesFile)
    {
        //Get the path for the shapes file
        string path = ServiceLocator::GetPlatformLayer()->GetPathForResourceInDirectory(aShapesFile.c_str(), "xml", "Physics");
        
        //Open the file, if the assert is hit, it means there isn't any data in the file
        File file(path);
        assert(file.GetBufferSize() > 0);
        
        //Parse the xml file
        xml_document<> doc;    // character type defaults to char
        doc.parse<0>(const_cast<char*>(file.GetBuffer()));
        
        //Get the root font node
        xml_node<> *rootNode = doc.first_node("bodydef");
        
        //If the root is null, log and error and return
        if(rootNode == nullptr)
        {
            Error(false, "Unable to load shape cache: %s, xml file doesn't have a root node", path.c_str());
            return;
        }
        
        //Get the bodies and metadata node
        xml_node<> *bodiesNode = rootNode->first_node();
        xml_node<> *metaDataNode = bodiesNode->next_sibling();
        
        //Parse the meta data node, we need to know the pixels to meters ratio BEFORE we parse the bodies node
        ParseMetaDataNode(metaDataNode);
        
        //Lastly pase the bodies node
        ParseBodiesNode(bodiesNode);
    }

    void ShapeCache::AddFixtures(b2Body* aBody, const string& aKey)
    {
        //Get the shape data for the key
        pair<ShapeData*, vec2> shapePair = m_ShapeCache[aKey];
        
        //If the assert is hit, then there isn't any shape data for the key
        assert(shapePair.first != nullptr);
        
        //Cycle through the shape data and add the fixtures to the body
        ShapeData* shapeData = shapePair.first;
        while(shapeData != nullptr)
        {
            aBody->CreateFixture(&shapeData->fixture);
            shapeData = shapeData->next;
        }
    }
    
    void ShapeCache::AddFixtures(PhysicsObject* aPhysicsObject, const string& aKey)
    {
        //Get the shape data for the key
        pair<ShapeData*, vec2> shapePair = m_ShapeCache[aKey];
        
        //If the assert is hit, then there isn't any shape data for the key
        assert(shapePair.first != nullptr);
        
        //Cycle through the shape data and add the fixtures to the body
        ShapeData* shapeData = shapePair.first;
        while(shapeData != nullptr)
        {
            aPhysicsObject->CreateFixture(&shapeData->fixture);
            shapeData = shapeData->next;
        }
    }

    vec2 ShapeCache::GetAnchorPoint(const string& aKey)
    {
        //Get the shape data for the key
        pair<ShapeData*, vec2> shapePair = m_ShapeCache[aKey];
        
        //If the assert is hit, then there isn't any shape data for the key
        assert(shapePair.first != nullptr);
        
        //Return the anchor point
        return shapePair.second;
    }
    
    void ShapeCache::ParseMetaDataNode(xml_node<> *aMetaDataNode)
    {
        //Parse the format, ensure it is 1
        xml_node<> *formatNode = aMetaDataNode->first_node();
        int format = atoi(formatNode->value());
        assert(format == 1);
    
        //Parse the pixels to meters ratio
        xml_node<> *ptmNode = formatNode->next_sibling();
        m_PixelsToMetersRatio = (float)atof(ptmNode->value());
    }
    
    void ShapeCache::ParseBodiesNode(xml_node<> *aBodiesNode)
    {
        //Cycle through all the bodies
        for(xml_node<> *bodyNode = aBodiesNode->first_node(); bodyNode; bodyNode = bodyNode->next_sibling())
        {
            //Get the body name
            string bodyName = string(bodyNode->first_attribute()->value());
            
            //Get the anchor point node
            xml_node<> *anchorPointNode = bodyNode->first_node();
            
            //Parse the float string data in the anchor point node value
            vector<float> floats;
            ParseFloatData(anchorPointNode->value(), floats);
            
            //Set the anchor point
            vec2 anchorPoint = vec2(floats.at(0), floats.at(1));
            
            //Parse the fixtures node
            ShapeData* shapeData = nullptr;
            xml_node<> *fixturesNode = anchorPointNode->next_sibling();
            ParseFixturesNode(fixturesNode, &shapeData);
            
            //Set the shape data and the anchor point to the map for the body name
            m_ShapeCache[bodyName] = make_pair(shapeData, anchorPoint);
        }
    }
    
    void ShapeCache::ParseFixturesNode(xml_node<> *aFixturesNode, ShapeData** aShapeData)
    {
        //Cycle through all the fixture nodes
        for(xml_node<> *fixtureNode = aFixturesNode->first_node(); fixtureNode; fixtureNode = fixtureNode->next_sibling())
        {
            b2FixtureDef fixtureData;
        
            //Load the fixture's density
            xml_node<> *densityNode = fixtureNode->first_node();
            fixtureData.density = (float)atof(densityNode->value());
            
            //Load the fixture's friction
            xml_node<> *frictionNode = densityNode->next_sibling();
            fixtureData.friction = (float)atof(frictionNode->value());
            
            //Load the fixture's restitution
            xml_node<> *restitutionNode = frictionNode->next_sibling();
            fixtureData.restitution = (float)atof(restitutionNode->value());
            
            //Load the fixture's filter category bits
            xml_node<> *filterCategoryBitsNode = restitutionNode->next_sibling();
            fixtureData.filter.categoryBits = atoi(filterCategoryBitsNode->value());
            
            //Load the fixture's filter group index
            xml_node<> *filterGroupIndexNode = filterCategoryBitsNode->next_sibling();
            fixtureData.filter.groupIndex = atoi(filterGroupIndexNode->value());
            
            //Load the fixture's filter mask bits
            xml_node<> *filterMaskBitsNode = filterGroupIndexNode->next_sibling();
            fixtureData.filter.maskBits = atoi(filterMaskBitsNode->value());
            
            //Get the fixture type
            xml_node<> *fixtureTypeNode = filterMaskBitsNode->next_sibling();
            const char* fixtureType = fixtureTypeNode->value();
            
            //Is this a sensor node?
            xml_node<> *sensorNode = fixtureTypeNode->next_sibling();
            fixtureData.isSensor = strcmp(sensorNode->name(), "isSensor") == 0;

            //Get the polygons node
            xml_node<> *polygons = fixtureData.isSensor == true ? sensorNode->next_sibling() : sensorNode;
            
            //Ensure that this is polygon data we are trying to load
            if(strcmp(fixtureType, "POLYGON") == 0)
            {
                //Parse the polygons node
                ParsePolygonsNode(polygons, aShapeData, &fixtureData);
            }
            else
            {
                //Circles are not yet supported - re-export the shape data as polygon's
                assert(0);
            }
        }
    }
    
    void ShapeCache::ParsePolygonsNode(xml_node<> *aPolygonsNode, ShapeData** aShapeData, b2FixtureDef* aFixtureDef)
    {
        b2Vec2 vertices[b2_maxPolygonVertices];
    
        //Cycle through all the polygons
        for(xml_node<> *polygon = aPolygonsNode->first_node(); polygon; polygon = polygon->next_sibling())
        {
            //Parse the float data from the polygon node
            vector<float> floats;
            ParseFloatData(polygon->value(), floats);

            //Create the shape data and set the fixture
            ShapeData* shapeData = new ShapeData();
            shapeData->fixture = *aFixtureDef;

            //Create the polygon shape object
            b2PolygonShape* polygonShape = new b2PolygonShape();

            //Cycle through and set the vertices
            int verticesIndex = 0;
            for(unsigned long i = 0; i < floats.size(); i+=2)
            {
                vertices[verticesIndex].x = (floats.at(i) / m_PixelsToMetersRatio);
                vertices[verticesIndex].y = (floats.at(i+1) / m_PixelsToMetersRatio);
                verticesIndex++;
            }
            
            //Set the shape's vertices
            polygonShape->Set(vertices, verticesIndex);
            
            //Set the fixture's shape
            shapeData->fixture.shape = polygonShape;
            
            //Set the shape data pointer
            *aShapeData = shapeData;
            
            //Setup the next pointer (link) in the linked list
            aShapeData = &(shapeData->next);
        }
    }
    
    void ShapeCache::ParseFloatData(const char* aData, vector<float>& aFloats)
    {
        //Does the string contain a comma?
        string s = string(aData);
        
        //Cycle through and parse the string for commas and extract the float values from the string
        long lastComma = -1;
        for(unsigned long i = 0; i < s.size(); i++)
        {
            char c = s.at(i);
            if(c == ',')
            {
                string value = s.substr(lastComma + 1, i - (lastComma + 1));
                lastComma = i;
                aFloats.push_back((float)atof(value.c_str()));
            }
        }
        
        //Get the last value
        string value = s.substr(lastComma + 1, s.size() - (lastComma + 1));
        aFloats.push_back((float)atof(value.c_str()));
    }

    bool ShapeCache::CanUpdate()
    {
        return false;
    }
    
    bool ShapeCache::CanDraw()
    {
        return false;
    }
}