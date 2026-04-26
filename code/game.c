
void
update_game(GameMemory* memory)
{
    GameState* state = memory->state;
    Player* player = state->player;
    float deltaTime = GetFrameTime();

    /*
    if (player->levelUp)
    {
        return;
    }
    */

    if (player->hp <= 0)
    {
        setup_game(memory);
    }

    state->camera.target = (Vector2) { player->position.x, player->position.y };

    update_bullets(state, deltaTime);
    update_player(player, deltaTime);
    update_xps(player, &player->xp, state->xpPositions, &state->xpCount, deltaTime);
    update_enemies(player, state->enemies, &state->enemyCount, state->targets, deltaTime);
    update_horde(&state->enemyCount, &state->hordeSpawnElapsedTime, deltaTime);
}

void
render_game(const GameMemory* memory)
{
    GameState* state = memory->state;
    if (state->animationElapsedTime >= ANIMATION_DELAY)
    {
        state->animationElapsedTime = 0.0f;
    }

    state->animationElapsedTime += GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(state->camera);

    // bullets
    for (int b = 0; b < state->bulletCount; ++b)
    {
        int targetIndex = state->targets[b];
        if (targetIndex == -1)
        {
            continue;
        }

        Bullet* bullet = &state->bullets[b];
        DrawRectangleRec(bullet->body, RED);
    }

    // player
    Player *player = state->player;
    const Vector2 playerTexturePos = { 
        player->position.x - player->body.width/2, 
        player->position.y - player->body.height/2 
    };

    update_animation_frame(state->animationElapsedTime, &(player->animationFrame.x));
    DrawTextureRec(state->playerTexture, player->animationFrame, playerTexturePos, player->color);       

    // xps
    for (int x = 0; x < state->xpCount; ++x)
    {
        DrawRectangleRec(get_xp_body(&state->xpPositions[x]), YELLOW);
    }

    // enemy
    for (int e = 0; e < state->enemyCount; ++e)
    {
        Enemy* enemy = &state->enemies[e];
        update_animation_frame(state->animationElapsedTime, &(enemy->animationFrame.x));

        Vector2 enemyTexturePos = {
            enemy->position.x - enemy->body.width/2,
            enemy->position.y - enemy->body.height/2,
        };

        DrawTextureRec(state->enemyTexture, enemy->animationFrame, enemyTexturePos, WHITE);       
    }

    EndMode2D();

    DrawRectangle(20, 30, STATUS_BAR_WIDTH, 20, GRAY);
    DrawRectangle(20, 30, (player->hp * STATUS_BAR_WIDTH) / player->maxHp, 20, RED);
    DrawRectangle(20, 50, STATUS_BAR_WIDTH, 20, GRAY);
    DrawRectangle(20, 50, (player->xp * STATUS_BAR_WIDTH) / player->xpMax, 20, YELLOW);

    /*
    if (player->levelUp)
    {
        update_level_up_menu(state);
    }
    */

    DrawFPS(20, 80);
    EndDrawing();
}
