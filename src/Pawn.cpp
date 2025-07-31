/* ===============
Implementation of Pawn
=================*/
#include "Pawn.h"
#include "Weapon.h"


// Constructor for Pawn
Pawn::Pawn() : velocity({0, 0}), maxForce(1.0f), speed(0.0f), rotation(0.0f),
               color(WHITE), shapeType(CIRCLE), size(0.0f), isControllable(true), isAlive(true)
{
    SetPosition({0, 0}); // Initialize position to (0, 0)
    
    // Other attributes are initialized in the member initializer list
}

// Destructor for Pawn
Pawn::~Pawn()
{
}

// Factory method to create a Pawn with specific attributes
// This method initializes the Pawn's position, speed, color, and shape type
void Pawn::CreatePawn(Vector2 _pos, float _speed, Color _color, ShapeType _shapeType, float _size, bool _iscontrollable)
{
    SetPosition(_pos);
    SetSpeed(_speed);
    SetColor(_color);
    SetShapeType(_shapeType);
    SetAlive(true);
    SetSize(_size);
    SetControllable(_iscontrollable);
}

// Update the pawn's position based on its velocity and speed
// This method is called every frame to update the pawn's state
void Pawn::Update()
{
    // 1. Update velocity by adding acceleration
    velocity = Vector2Add(velocity, acceleration);

    // 2. Limit the velocity to the maximum speed
    if (Vector2LengthSqr(velocity) > (speed * speed))
    {
        velocity = Vector2Normalize(velocity);
        velocity = Vector2Scale(velocity, speed);
    }

    // 3. Update position by adding velocity
    position = Vector2Add(position, velocity);

    // 4. Reset acceleration to zero for the next frame
    // This is crucial, as forces are applied fresh each frame.
    acceleration = { 0.0f, 0.0f };
}

// Render the pawn based on its shape type
// This method uses raylib functions to draw the pawn on the screen
void Pawn::Render()
{
    // Render the pawn based on its shape type
    switch (shapeType) {
        case CIRCLE:
            DrawCircleV(position, size, color);
            break;
        case SQUARE:
            DrawRectangleV(position, {size * 2, size * 2}, color);
            break;
        case TRIANGLE:
            DrawTriangle({position.x - size, position.y + size}, {position.x + size, position.y + size}, {position.x, position.y - size}, color);
            break;
    }
}

// Apply a force to the pawn's acceleration
void Pawn::ApplyForce(Vector2 force)
{
    // In a more complex system, you might divide by mass (a = F/m).
    // For now, we'll treat force directly as acceleration.
    acceleration = Vector2Add(acceleration, force);
}

void Pawn::SetWeapon(Weapon* _weapon)
{
    if (_weapon != nullptr) {
        weapon = _weapon; // Assign the weapon to the pawn
    } else {
        weapon = nullptr; // Set to null if no weapon is provided
    }
}

Weapon* Pawn::GetWeapon() const
{
    return weapon;
}

// Destroy method can be used to clean up resources if needed
// Currently, it does not perform any specific actions
void Pawn::Destroy()
{
}

// Setters for Pawn attributes
// These methods allow external code to modify the Pawn's attributes safely

void Pawn::SetVelocity(Vector2 vel)
{
    velocity = vel;
}

Vector2 Pawn::GetVelocity() const
{
    return velocity;
}

void Pawn::SetAcceleration(Vector2 acc)
{
    acceleration = acc;
}

Vector2 Pawn::GetAcceleration() const
{
    return acceleration;
}

void Pawn::SetMaxForce(const float _maxForce)
{
    maxForce = _maxForce;
}

float Pawn::GetMaxForce() const
{
    return maxForce;
}

void Pawn::SetSpeed(float spd)
{
    speed = spd;
}

float Pawn::GetSpeed() const
{
    return speed;
}

void Pawn::SetRotation(float rot)
{
    rotation = rot;
}



void Pawn::SetColor(Color col)
{
    color = col;
}

void Pawn::SetShapeType(ShapeType type)
{
    shapeType = type;
}

void Pawn::SetSize(float _size)
{
    size = _size;
}

float Pawn::GetSize() const
{
    return size;
}

void Pawn::SetControllable(bool controllable)
{
    isControllable = controllable;
}

void Pawn::SetAlive(bool alive)
{
    isAlive = alive;
}
