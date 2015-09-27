//
//  b2DebugDraw.h
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GameDevFramework_b2DebugDraw_h
#define GameDevFramework_b2DebugDraw_h

#include "Box2D.h"

struct b2AABB;

namespace GameDev2D
{
    class Polygon;
    class Circle;
    class Line;

    class b2DebugDraw : public b2Draw
    {
    public:
        b2DebugDraw();
        ~b2DebugDraw();
        
        void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
        void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
        void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
        void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
        void DrawTransform(const b2Transform& xf);
        
    private:
        Polygon* m_Polygon;
        Circle* m_Circle;
        Line* m_Line;
    };
}

#endif
