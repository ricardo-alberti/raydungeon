
void
update_animation_frame(float elapsedTime,
                       float* animationFrameX)
{
    if (elapsedTime < ANIMATION_DELAY) return;

    *animationFrameX += FRAME_SIZE;
    if (*animationFrameX >= FRAME_SIZE * 3)
    {
        *animationFrameX = 0;
    }
}
