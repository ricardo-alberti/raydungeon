
void
setup_game(GameMemory* gameMemory)
{
    if (!gameMemory->IsInitialized)
    {
        gameMemory->IsInitialized = true;
        gameMemory->permanentStorage = arena_alloc(MB(5));
        gameMemory->transientStorage = arena_alloc(MB(5));
    }

    arena_reset(&gameMemory->permanentStorage);
    arena_reset(&gameMemory->transientStorage);

    gameMemory->gameContext = arena_push(&gameMemory->permanentStorage, sizeof(GameContext), 16);
    GameContext* gameContext = gameMemory->gameContext;
    zero_memory(gameContext, sizeof(GameContext));

    gameContext->player = arena_push(&gameMemory->permanentStorage, sizeof(Player), 16);
    gameContext->enemies = arena_push(&gameMemory->permanentStorage, sizeof(Enemy) * MAX_ENEMIES, 16);
    gameContext->position = arena_push(&gameMemory->permanentStorage, sizeof(Vector2) * MAX_ENEMIES, 16);
    gameContext->bullets = arena_push(&gameMemory->permanentStorage, sizeof(Bullet) * MAX_BULLETS, 16);
    gameContext->xp = arena_push(&gameMemory->permanentStorage, sizeof(Experience) * MAX_XP, 16);
    gameContext->targets = arena_push(&gameMemory->permanentStorage, sizeof(int) * MAX_BULLETS, 16);

    Image playerImage = LoadImage("data/assets/AdeptNecromancer/AdeptNecromancer.png");
    gameContext->playerTexture = LoadTextureFromImage(playerImage);
    UnloadImage(playerImage);

    Image enemyImage = LoadImage("data/assets/CorruptedTreant/CorruptedTreant.png");
    gameContext->enemyTexture = LoadTextureFromImage(enemyImage);
    UnloadImage(enemyImage);

    Image glowingWisp = LoadImage("data/assets/GlowingWisp/GlowingWisp.png");
    gameContext->glowingWispTexture = LoadTextureFromImage(glowingWisp);
    UnloadImage(glowingWisp);

    // setup player
    Player* player = gameContext->player;
    player->body = (Rectangle){ 0, 0, FRAME_SIZE, FRAME_SIZE };
    player->color = WHITE;
    player->visionRadius = 120;
    player->xpRadius = 70;
    player->speed = 60.0f;
    player->maxHp = 100;
    player->hp = 100;
    player->xpMax = 100;
    player->damage = 20;
    player->invulnerable = false;
    player->invulnerabilityDelay = 1.0f;
    player->invulnerabilityElapsedTime = 0.0f;
    player->animationFrame = (Rectangle){ 0, 0, FRAME_SIZE, FRAME_SIZE };

    // setup bullets
    gameContext->bulletCount = MAX_BULLETS;
    for (int i = 0; i < gameContext->bulletCount; ++i)
    {
        load_bullet(gameContext, i);
    }

    // setup enemies
    gameContext->enemyCount = START_ENEMY_COUNT;
    for (int i = 0; i < gameContext->enemyCount; ++i)
    {
        spawn_enemy(gameContext, i);
    }

    // setup camera
    gameContext->camera = (Camera2D){ 0 };
    gameContext->camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
    gameContext->camera.zoom = 3.0f;
}

