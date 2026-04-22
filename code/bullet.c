void
drop_xp(GameContext* gameContext, Vector2 position)
{
    Experience* xp;

    if (gameContext->ringPointer >= MAX_XP || gameContext->ringPointer >= gameContext->xpCount)
        gameContext->ringPointer = 0;

    if (gameContext->xpCount < MAX_XP)
         xp = &gameContext->xp[gameContext->xpCount++];
    else 
        xp = &gameContext->xp[gameContext->ringPointer++];

    xp->body.width = XP_SIZE;
    xp->body.height = XP_SIZE;
    xp->body.x = position.x - xp->body.width / 2;
    xp->body.y = position.y - xp->body.height / 2;
}

void
load_bullet(GameContext* gameContext, int index) 
{
    const Player* player = gameContext->player;
    Bullet* bullet = &gameContext->bullets[index];
    Rectangle* body = &bullet->body;

    bullet->type = 1;
    bullet->speed = 180.0f;

    body->width = 5;
    body->height = 5;

    body->x = player->position.x - body->width / 2;
    body->y = player->position.y - body->height / 2;
                
    gameContext->targets[index] = -1;
}

void
update_homing_missile(GameContext *gameContext, int b, float deltaTime)
{
    Bullet *bullet = &gameContext->bullets[b];
    int targetIndex = gameContext->targets[b];
    if (targetIndex == -1) {
        load_bullet(gameContext, b);
        return;
    }

    Enemy* target = &gameContext->enemies[targetIndex];
    float dx = target->position.x - bullet->body.x;
    float dy = target->position.y - bullet->body.y;
    float len = sqrtf(dx*dx + dy*dy);

    if (len > 0.0f) {
        bullet->body.x += (dx / len) * bullet->speed * deltaTime;
        bullet->body.y += (dy / len) * bullet->speed * deltaTime;
    }

    if (CheckCollisionRecs(target->body, bullet->body))
    {
        load_bullet(gameContext, b);
        drop_xp(gameContext, target->position);
        spawn_enemy(gameContext, targetIndex);
    }
}

void
update_bullets(GameContext* gameContext, float deltaTime)
{
    for (int b = 0; b < gameContext->bulletCount; ++b)
    {
        update_homing_missile(gameContext, b, deltaTime);
    }
}
