
void
update_xps(GameContext* gameContext, float deltaTime)
{
    Player* player = gameContext->player;

    for (int x = 0; x < gameContext->xpCount; ++x)
    {
        Experience* xp = &gameContext->xp[x];

        if (!CheckCollisionCircleRec(player->position, player->xpRadius, xp->body))
        {
            continue;
        }

        float dx = player->position.x - xp->body.x;
        float dy = player->position.y - xp->body.y;
        float len = sqrtf(dx*dx + dy*dy);

        if (len > 0.0f)
        {
            xp->body.x += (dx / len) * XP_SPEED * deltaTime;
            xp->body.y += (dy / len) * XP_SPEED * deltaTime;
        }

        if (CheckCollisionRecs(xp->body, player->body))
        {
            player->xp++;

            gameContext->xp[x] = gameContext->xp[gameContext->xpCount - 1];
            gameContext->xpCount--;

            if (player->xp >= player->xpMax)
            {
                player->xpMax += player->xpMax;
                player->xp = 0;
                player->levelUp = true;
            }
        }
    }
}
