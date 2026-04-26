void
drop_xp(GameState* state, Vector2 position)
{
    Vector2* xp;

    if (state->ringPointer >= MAX_XP || state->ringPointer >= state->xpCount)
        state->ringPointer = 0;

    if (state->xpCount < MAX_XP)
    {
         xp = &state->xpPositions[state->xpCount++];
    }
    else 
    {
        xp = &state->xpPositions[state->ringPointer++];
    }

    xp->x = position.x;
    xp->y = position.y;
}

void
load_bullet(GameState* state, int index) 
{
    const Player* player = state->player;
    Bullet* bullet = &state->bullets[index];
    Rectangle* body = &bullet->body;

    bullet->type = 1;
    bullet->speed = 180.0f;

    body->width = 5;
    body->height = 5;

    body->x = player->position.x - body->width / 2;
    body->y = player->position.y - body->height / 2;
                
    state->targets[index] = -1;
}

void
update_homing_missile(GameState *state, int b, float deltaTime)
{
    Bullet *bullet = &state->bullets[b];
    int targetIndex = state->targets[b];
    if (targetIndex == -1) {
        load_bullet(state, b);
        return;
    }

    Enemy* target = &state->enemies[targetIndex];
    float dx = target->position.x - bullet->body.x;
    float dy = target->position.y - bullet->body.y;
    float len = sqrtf(dx*dx + dy*dy);

    if (len > 0.0f) {
        bullet->body.x += (dx / len) * bullet->speed * deltaTime;
        bullet->body.y += (dy / len) * bullet->speed * deltaTime;
    }

    if (CheckCollisionRecs(target->body, bullet->body))
    {
        load_bullet(state, b);
        drop_xp(state, target->position);
        spawn_enemy(state, targetIndex);
    }
}

void
update_bullets(GameState* state, float deltaTime)
{
    for (int b = 0; b < state->bulletCount; ++b)
    {
        update_homing_missile(state, b, deltaTime);
    }
}
