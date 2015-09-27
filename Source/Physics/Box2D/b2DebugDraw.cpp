//
//  b2DebugDraw.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "b2DebugDraw.h"
#include "b2Helper.h"
#include "../../Graphics/Primitives/Primitives.h"
#include "../../Graphics/OpenGL.h"

using namespace glm;

namespace GameDev2D
{
    b2DebugDraw::b2DebugDraw() : b2Draw(),
        m_Polygon(nullptr),
        m_Circle(nullptr),
        m_Line(nullptr)
    {
        m_Polygon = new Polygon();
        m_Circle = new Circle();
        m_Line = new Line();
    }
    
    b2DebugDraw::~b2DebugDraw()
    {
        SafeDelete(m_Polygon);
        SafeDelete(m_Circle);
        SafeDelete(m_Line);
    }
    
    void b2DebugDraw::DrawPolygon(const b2Vec2* aVertices, int aVertexCount, const b2Color& aColor)
    {
        //Clear the existing vertices
        m_Polygon->ClearVertices();
        
        float ratio = b2Helper::Ratio();
        vec2 vertex = vec2(0.0f, 0.0f);
        for(int i = 0; i < aVertexCount; i++)
        {
            vertex.x = aVertices[i].x * ratio;
            vertex.y = aVertices[i].y * ratio;
            m_Polygon->AddVertex(vertex);
        }
        
        m_Polygon->SetColor(Color(aColor.r, aColor.g, aColor.b));
        m_Polygon->SetRenderMode(GL_LINE_LOOP);
        m_Polygon->Draw();
    }
    
    void b2DebugDraw::DrawSolidPolygon(const b2Vec2* aVertices, int aVertexCount, const b2Color& aColor)
    {
        //Clear the existing vertices
        m_Polygon->ClearVertices();
        
        float ratio = b2Helper::Ratio();
        vec2 vertex = vec2(0.0f, 0.0f);
        for(int i = 0; i < aVertexCount; i++)
        {
            vertex.x = aVertices[i].x * ratio;
            vertex.y = aVertices[i].y * ratio;
            m_Polygon->AddVertex(vertex);
        }
        
        m_Polygon->SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f));
        m_Polygon->SetRenderMode(GL_TRIANGLE_FAN);
        m_Polygon->Draw();
    
        m_Polygon->SetColor(Color(aColor.r, aColor.g, aColor.b));
        m_Polygon->SetRenderMode(GL_LINE_LOOP);
        m_Polygon->Draw();
    }
    
    void b2DebugDraw::DrawCircle(const b2Vec2& aCenter, float aRadius, const b2Color& aColor)
    {
        float ratio = b2Helper::Ratio();
        m_Circle->SetLocalPosition(aCenter.x * ratio, aCenter.y * ratio);
        m_Circle->SetRadius(aRadius * ratio);
        m_Circle->SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f));
        m_Circle->SetIsFilled(false);
        m_Circle->Draw();
    }
    
    void b2DebugDraw::DrawSolidCircle(const b2Vec2& aCenter, float aRadius, const b2Vec2& aAxis, const b2Color& aColor)
    {
        float ratio = b2Helper::Ratio();
        m_Circle->SetLocalPosition(aCenter.x * ratio, aCenter.y * ratio);
        m_Circle->SetRadius(aRadius * ratio);
        m_Circle->SetColor(Color(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f, 0.5f));
        m_Circle->SetIsFilled(true);
        m_Circle->Draw();
        
        m_Circle->SetColor(Color(aColor.r, aColor.g, aColor.b));
        m_Circle->SetIsFilled(false);
        m_Circle->Draw();
        
        DrawSegment(aCenter,aCenter + aRadius * aAxis, aColor);
    }
    
    void b2DebugDraw::DrawSegment(const b2Vec2& aP1, const b2Vec2& aP2, const b2Color& aColor)
    {
        float ratio = b2Helper::Ratio();
        m_Line->SetLocalPosition(aP1.x * ratio, aP1.y * ratio);
        m_Line->SetPositionB(aP2.x * ratio, aP2.y * ratio);
        m_Line->SetColor(Color(aColor.r, aColor.g, aColor.b));
        m_Line->Draw();
    }
    
    void b2DebugDraw::DrawTransform(const b2Transform& xf)
    {
        b2Vec2 p1 = xf.p, p2;
        const float k_axisScale = 0.4f;
        
        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1,p2,b2Color(1,0,0));
        
        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1,p2,b2Color(0,1,0));
    }
}
