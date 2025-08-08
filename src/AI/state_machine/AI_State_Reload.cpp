#include "AI_State_Reload.h"
#include <iostream>
#include "raylib.h"

// Constants for the state
const char* RELOAD_STATE_NAME = "Reload";

AI_State_Reload::AI_State_Reload() : AI_State(e_AI_StateID::Reload, RELOAD_STATE_NAME)
{
    
}

void AI_State_Reload::OnEnter(Pawn &_agent)
{
    // Reset the reload timer when entering the state
    _agent.GetWeapon()->StartReload();
}

e_AI_StateID AI_State_Reload::OnUpdate(Pawn &_agent, Actor &_target)
{
   // While reloading, the agent should stop moving.
   _agent.SetVelocity({0, 0});

   Weapon* agentWeapon = _agent.GetWeapon();

   agentWeapon->Update(); // Update the weapon's internal timers
   
   // The weapon's own Update() method handles the timer. We don't need to touch it.
   // The state's only job is to wait until the weapon is no longer busy.
   if (agentWeapon->IsReloading()) {
       // The weapon is still busy. Draw a visual indicator and wait.
       DrawCircleV(_agent.GetPosition(), _agent.GetSize() * 1.5f, PURPLE);
       return e_AI_StateID::None; // Stay in this state.
   } else {
       // The weapon has finished its internal reload process.
       // Time to go back to the Attack state to make a new decision.
       std::cout << "Reload complete. Transitioning back to Attack." << std::endl;
       return e_AI_StateID::Attack;
   }
}

void AI_State_Reload::OnExit(Pawn &_agent)
{

}

