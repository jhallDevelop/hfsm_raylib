#include "AI_State_Shoot.h"
#include <iostream>
#include "raylib.h"
// Constants for the state
const char* SHOOT_STATE_NAME = "Shoot";

AI_State_Shoot::AI_State_Shoot() : AI_State(e_AI_StateID::Shoot, SHOOT_STATE_NAME)
{
}

void AI_State_Shoot::OnEnter(Pawn &_agent)
{
}

e_AI_StateID AI_State_Shoot::OnUpdate(Pawn &_agent, Pawn &_target)
{
    // animate the shooting effect
    DrawCircleV(_agent.GetPosition(), _agent.GetSize()*2, BLACK);

    return e_AI_StateID::Attack; // Stay in the Shoot state for now
}

void AI_State_Shoot::OnExit(Pawn &_agent)
{

}

