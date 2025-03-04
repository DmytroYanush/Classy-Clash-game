#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter ()
{}
void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}
Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale};
}
void BaseCharacter::tick(float deltaTime) {
    worldPosLastFrame = worldPos;
    // update animation frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame >= maxFrames)
            frame = 0;
    }
   
    if (Vector2Length(velocity) != 0.0)
    {
        // The speed will remain 1 pixel per frame, even if a key for movement along the x-axis and y-axis is pressed simultaneously.
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        texture = run;
    }
    else
    {
        texture = idle;
    }
    velocity = {};
    // draw the character
    Rectangle source{
        texture.width / 6.f * frame, // frame is controlling possition in sprite sheet
        0.0f,
        rightLeft * width, // rightLeft is used for turning the animation
        height};
    Rectangle dest{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale,
    };
    DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
}