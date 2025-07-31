/* ===============
Pure virtual class for Actors in the game
=================*/
#pragma once
#include "raylib.h"
#include "raymath.h"

class Actor {
public:
    Actor();
    virtual ~Actor() = default;

    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;

    // Setters
    void SetPosition(Vector2 pos) { position = pos; }
    Vector2 GetPosition() const { return position; }

protected:
    Vector2 position;
    
};