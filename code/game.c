void
update_game(GameMemory* gameMemory)
{
    GameContext* gameContext = gameMemory->gameContext;
    Player* player = gameContext->player;
    float deltaTime = GetFrameTime();

    if (player->levelUp)
    {
        return;
    }

    if (player->hp < 0)
    {
        setup_game(gameMemory);
    }

    gameContext->camera.target = (Vector2) { player->position.x, player->position.y};

    update_bullets(gameContext, deltaTime);
    update_player(gameContext->player, deltaTime);
    update_xps(gameContext, deltaTime);
    update_enemies(gameContext, deltaTime);
}

void
render_game(const GameMemory* gameMemory)
{
    GameContext* gameContext = gameMemory->gameContext;

    if (gameContext->animationElapsedTime >= ANIMATION_DELAY)
    {
        gameContext->animationElapsedTime = 0.0f;
    }

    gameContext->animationElapsedTime += GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(gameContext->camera);

    // bullets
    for (int b = 0; b < gameContext->bulletCount; ++b)
    {
        int targetIndex = gameContext->targets[b];
        if (targetIndex == -1)
        {
            continue;
        }

        Bullet* bullet = &gameContext->bullets[b];
        DrawRectangleRec(bullet->body, RED);
    }

    // player
    Player *player = gameContext->player;
    const Vector2 playerTexturePos = { 
        player->position.x - player->body.width/2, 
        player->position.y - player->body.height/2 
    };

    update_animation_frame(gameContext->animationElapsedTime, &(player->animationFrame.x));
    DrawTextureRec(gameContext->playerTexture, player->animationFrame, playerTexturePos, player->color);       

    // xps
    for (int x = 0; x < gameContext->xpCount; ++x)
    {
        Experience* xp = &gameContext->xp[x];
        DrawRectangleRec(xp->body, YELLOW);
    }

    // enemy
    for (int e = 0; e < gameContext->enemyCount; ++e)
    {
        Enemy* enemy = &gameContext->enemies[e];
        update_animation_frame(gameContext->animationElapsedTime, &(enemy->animationFrame.x));

        Vector2 enemyTexturePos = {
            enemy->position.x - enemy->body.width/2,
            enemy->position.y - enemy->body.height/2,
        };

        DrawTextureRec(gameContext->enemyTexture, enemy->animationFrame, enemyTexturePos, WHITE);       
    }

    EndMode2D();

    DrawRectangle(20, 30, STATUS_BAR_WIDTH, 20, GRAY);
    DrawRectangle(20, 30, (player->hp * STATUS_BAR_WIDTH) / player->maxHp, 20, RED);
    DrawRectangle(20, 50, STATUS_BAR_WIDTH, 20, GRAY);
    DrawRectangle(20, 50, (player->xp * STATUS_BAR_WIDTH) / player->xpMax, 20, YELLOW);

    if (player->levelUp)
    {
        update_level_up_menu(gameContext);
    }

    DrawFPS(20, 80);
    EndDrawing();
}
