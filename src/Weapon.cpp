#include "Weapon.h"
Weapon::Weapon() : Actor() {
    // Initialize weapon-specific attributes if needed
    SetPosition({0, 0});
}

Weapon::~Weapon() {
}


void Weapon::CreateWeapon(int _maxAmmoCount, float _maxFireRate, float _maxReloadTime) {
    isReloading = false; // Initially, the weapon is not reloading
    // Set the initial position, speed, color, shape type, and controllability
    currentAmmoCount = _maxAmmoCount; // Start with no ammo
    maxAmmoCount = _maxAmmoCount;

    currentFireRateTime = 0.0f; // Reset the fire rate timer
    maxFireRate = _maxFireRate;

    currentReloadtime = 0.0f; // Reset the reload timer
    maxReloadTime = _maxReloadTime;
}

void Weapon::Update()
{
    // 1. Update the fire rate timer.
    if (currentFireRateTime < maxFireRate) {
        currentFireRateTime += GetFrameTime();
    }

    // 2. If we are currently reloading, update the reload timer.
    if (isReloading) {
        currentReloadtime += GetFrameTime();

        // Check if the reload has finished.
        if (currentReloadtime >= maxReloadTime) {
            // Finish the reload process.
            currentAmmoCount = maxAmmoCount;
            currentReloadtime = 0.0f;
            isReloading = false; // Stop reloading.
        }
    }
}
void Weapon::Render() {

}

void Weapon::Destroy() {

}

bool Weapon::Fire()
{
    // Logic to fire the weapon
    if (!isReloading && currentAmmoCount > 0 && currentFireRateTime >= maxFireRate) 
    {
        // All conditions met, so fire the weapon.
        currentAmmoCount--;         // Decrease ammo count.
        currentFireRateTime = 0.0f; // Reset fire rate timer.
        
        // Add logic here to create a projectile or perform the firing action.
        
        return true; // Indicate that the weapon was fired successfully.
    }
    
    // If any condition was false, the fire attempt fails.
    return false;
}

void Weapon::StartReload()
{
    // We can only start reloading if we aren't already reloading and ammo isn't full.
    if (!isReloading && currentAmmoCount < maxAmmoCount) {
        isReloading = true;
        currentReloadtime = 0.0f; // Reset timer at the start.
    }
}



// Check if the weapon is currently reloading
bool Weapon::IsReloading() const
{
    return isReloading;
}

// Ammo Count
int Weapon::GetCurrentAmmoCount() const
{
    return currentAmmoCount;
}

void Weapon::SetCurrentAmmoCount(int _currentCount)
{
    currentAmmoCount = _currentCount;
}

void Weapon::SetMaxAmmoCount(int _count)
{
    maxAmmoCount = _count;
}

int Weapon::GetMaxAmmoCount() const
{
    return maxAmmoCount;
}


// Fire Rate
float Weapon::GetCurrentFireRate() const
{
    return currentFireRateTime;
}

void Weapon::SetCurrentFireRate(float _fireRate)
{
    currentFireRateTime = _fireRate;
}

float Weapon::GetMaxFireRate() const
{
    return maxFireRate;
}

void Weapon::SetMaxFireRate(float _fireRate)
{
    maxFireRate = _fireRate;
}


// Reload Time
float Weapon::GetCurrentReloadTime() const
{
    return currentReloadtime;
}

void Weapon::SetCurrentReloadTime(float _reloadTime)
{
    currentReloadtime = _reloadTime;
}

float Weapon::GetMaxReloadTime() const
{
    return maxReloadTime;
}

void Weapon::SetMaxReloadTime(float _reloadTime)
{
    maxReloadTime = _reloadTime;
}
