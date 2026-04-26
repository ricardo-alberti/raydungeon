static void
attack_player(Player* player, int damage)
{ 
    if (player->invulnerable) return;

    player->hp -= damage;
    player->invulnerable = true;
}

static void
spawn_enemy(GameState* state, int index)
{
    Enemy *enemy = &state->enemies[index];
    enemy->type = 1;  
    enemy->hp = 10;
    enemy->speed = 80.0f;
    enemy->damage = 10;
    enemy->targeted = false;
    enemy->animationFrame = (Rectangle){0, 0, FRAME_SIZE, FRAME_SIZE};

    int side = GetRandomValue(0, 3);
    Vector2 spawnPos = { 
        (int)state->player->position.x, 
        (int)state->player->position.y
    };

    if (side < 2)
    {
        spawnPos.x += (side == 0 ? -1 : 1) * (SCREEN_WIDTH/2 + FRAME_SIZE);
        spawnPos.y += GetRandomValue(-SCREEN_HEIGHT/2, SCREEN_HEIGHT/2);
    } 
    else
    {
        spawnPos.x += GetRandomValue(-SCREEN_WIDTH/2, SCREEN_WIDTH/2);
        spawnPos.y += (side == 2 ? -1 : 1) * (SCREEN_HEIGHT/2 + FRAME_SIZE);
    }

    enemy->position = spawnPos;
    enemy->body = (Rectangle){ enemy->position.x, enemy->position.y, FRAME_SIZE, FRAME_SIZE };
}

void
update_enemies(Player* player,
        Enemy* enemies,
        int* enemyCount,
        int* targets,
        float deltaTime)
{
    for (int e = 0; e < *enemyCount; ++e)
    {
        Enemy* enemy = &enemies[e];

        float dx = player->position.x - enemy->position.x;
        float dy = player->position.y - enemy->position.y;
        float len = sqrtf(dx*dx + dy*dy);
        if (len > 0.0f) {
            enemy->position.x += (dx / len) * enemy->speed * deltaTime;
            enemy->position.y += (dy / len) * enemy->speed * deltaTime;

            enemy->body.x = enemy->position.x - enemy->body.width / 2;
            enemy->body.y = enemy->position.y - enemy->body.height / 2;
        }

        if (CheckCollisionRecs(enemy->body, player->body))
        {
            attack_player(player, enemy->damage);
        }

        if (CheckCollisionCircleRec(player->position, player->visionRadius, enemy->body))
        {
            if (targets[e] == -1) 
            {
                targets[e] = e;
                enemy->targeted = true;
            }
        }

        if (player->position.x < enemy->position.x)
        {
            if (enemy->animationFrame.width > 0) 
                enemy->animationFrame.width *= -1;
        }
        else
        {
            if (enemy->animationFrame.width < 0) 
                enemy->animationFrame.width *= -1;
        }
    }
}
