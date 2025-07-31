#pragma once
/* ===============
Derived class for player-controlled pawns.
=================*/
#include "Actor.h"
#include "Weapon.h"

enum ShapeType {
    CIRCLE,
    SQUARE,
    TRIANGLE
};

class Pawn : public Actor {

public:
    Pawn();
    ~Pawn() override;

    void CreatePawn(Vector2 pos, float speed, Color color, ShapeType shapeType = CIRCLE, float _size = 10, bool _isControllable = false);   // pawn factory method
    void Update() override;
    void Render() override;
    void Destroy() override;

    // Setters
   
    void SetVelocity(Vector2 vel); 
    Vector2 GetVelocity() const;
    void SetAcceleration(const Vector2 acc);
    Vector2 GetAcceleration() const;
    void SetMaxForce(const float maxForce);
    float GetMaxForce() const;

    void SetSpeed(float spd);
    float GetSpeed() const;
    void SetRotation(float rot);
    void SetColor(Color col);
    void SetShapeType(ShapeType type);
    void SetSize(float _size);
    float GetSize() const;

    void SetControllable(bool controllable);
    void SetAlive(bool alive);

    // Weapon
    void SetWeapon(Weapon* _weapon);
    Weapon* GetWeapon() const;

    // Apply a force to the pawn's acceleration
    void ApplyForce(Vector2 force);

    

private:
    // Pawn specific attributes for Physics
    Vector2 velocity;
    Vector2 acceleration; // Acceleration vector for the pawn
    float maxForce;
    float speed;
    float rotation;
    

    // Visual attributes
    Color color;
    ShapeType shapeType; // Type of shape for rendering
    float size; // Size of the pawn, used for rendering and physics calculations

    // Gameplay attributes
    bool isControllable; // Indicates if the Pawn can be controlled by the player
    bool isAlive;        // Indicates if the Pawn is alive
    Weapon* weapon;

};
