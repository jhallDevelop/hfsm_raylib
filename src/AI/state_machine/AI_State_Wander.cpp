#include "AI_State_Wander.h"
#include <cmath>
#include "raymath.h"
#include <iostream>

// Constants for the state
const char* WANDER_STATE_NAME = "Wander";

AI_State_Wander::AI_State_Wander() : AI_State(e_AI_StateID::Wander, WANDER_STATE_NAME)
{
}


void AI_State_Wander::OnEnter(Pawn &_agent)
{
}

e_AI_StateID AI_State_Wander::OnUpdate(Pawn& _agent, Actor& _target)
{
    // --- Wander Behavior Implementation ---
    // if the target is close enough, we stop wandering.
    
    float distanceToTarget = Vector2Distance(_agent.GetPosition(), _target.GetPosition());
    // Draw the debug circle for the wander target
    DrawCircleLines(_agent.GetPosition().x, _agent.GetPosition().y, minDistanceToTarget / 2, GREEN);
    if (distanceToTarget <= minDistanceToTarget) {
        // Transition to Seek State
        //std::cout << "Switching to Seek State" << std::endl;
        return e_AI_StateID::Seek; // Stop wandering if we are close enough to the target
    }

    // create a vector towards the agents current position and its goal.
    // 1. Calculate the center of the wander circle projected in front of the agent.
    Vector2 circleCenter = _agent.GetVelocity();
    if (Vector2LengthSqr(circleCenter) == 0) {
        // If agent is not moving, pick a default forward direction.
        circleCenter = { 1.0f, 0.0f };
    }
    circleCenter = Vector2Normalize(circleCenter);
    // Let's use a distance of 100 units for the projection.
    circleCenter = Vector2Scale(circleCenter, _agent.GetSize()); 
    // This point is relative to the agent, so we add the agent's position.
    circleCenter = Vector2Add(circleCenter, _agent.GetPosition());

    // 2. Calculate the displacement on the circle's edge.
    // Randomly adjust the wander angle for the next frame.
    // GetRandomValue with floats is not standard; we'll simulate it.
    float randomJitter = (((float)GetRandomValue(0, 20000) / 10000.0f) - 1.0f); // Result is [-1.0, 1.0]
    wanderAngle += randomJitter * maxJitter;

    // The radius of our wander circle is 50.
    Vector2 displacement = { wanderRadius * cosf(wanderAngle), wanderRadius * sinf(wanderAngle) };

    // 3. The final wander target is the circle center plus the displacement.
    Vector2 wanderTarget = Vector2Add(circleCenter, displacement);

    // 4. Steer towards the wander target (seek behavior).
    Vector2 desired = Vector2Subtract(wanderTarget, _agent.GetPosition());
    desired = Vector2Normalize(desired);
    desired = Vector2Scale(desired, _agent.GetSpeed());

    Vector2 steer = Vector2Subtract(desired, _agent.GetVelocity());
    // Limit the steering force to the maximum force.
    if (Vector2LengthSqr(steer) > (_agent.GetMaxForce() * _agent.GetMaxForce())) {
        steer = Vector2Normalize(steer);
        steer = Vector2Scale(steer, _agent.GetMaxForce());
    }

    // Debugging: Draw the wander target and the steering force
    DrawLineEx(_agent.GetPosition(), Vector2Add(_agent.GetPosition(), steer*100.0f), 2.0f, GREEN);
    // 5. Apply the steering force to the agent.
    // This requires an ApplyForce method on your Pawn.
    _agent.ApplyForce(steer);

    return e_AI_StateID::None; // Continue wandering
}

void AI_State_Wander::OnExit(Pawn &_agent)
{
}
