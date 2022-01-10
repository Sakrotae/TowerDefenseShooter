#include "game/Target.h"
#include "game/Game.h"

bool Target::processHit()
{
    health--;
    float relativeHealth = (float)health / (float)maxHealth;
    target.color = glm::vec4{1 - relativeHealth, relativeHealth, 0, 1};
    target.scale = {2, 2, 2};
    return health <= 0;
}

void Target::setRotation(float newSpeed, float newRadius)
{
    speed = newSpeed;
    radius = newRadius;
}

Cube Target::getCube(float deltaTime)
{
    rotation += speed * deltaTime;
    target.position = position;
    // target.position.x += speed * deltaTime;
    target.position.z -= rotation;
    // std::cos(rotation);
    // target.position.y += radius * std::sin(rotation);
    target.update();
    return target;
}