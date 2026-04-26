
void
setup_game(GameMemory* memory)
{
    Arena* perm = &memory->permanentStorage;

    if (!memory->IsInitialized) {
        memory->state = (GameState*)arena_push(perm, sizeof(GameState));
        GameState* state = memory->state;

        state->player      = (Player*)     arena_push(perm, sizeof(Player));
        state->enemies     = (Enemy*)      arena_push(perm, sizeof(Enemy) * MAX_ENEMIES);
        state->bullets     = (Bullet*)     arena_push(perm, sizeof(Bullet) * MAX_BULLETS);
        state->xpPositions = (Vector2*)    arena_push(perm, sizeof(Vector2) * MAX_XP);
        state->targets     = (int*)        arena_push(perm, sizeof(int) * MAX_BULLETS);

        memory->IsInitialized = true;
    }

    GameState* state = memory->state;

    // setup player
    Player* player = state->player;
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
    state->bulletCount = MAX_BULLETS;
    for (int i = 0; i < state->bulletCount; ++i)
    {
        load_bullet(state, i);
    }

    // setup enemies
    state->enemyCount = START_ENEMY_COUNT;
    for (int i = 0; i < state->enemyCount; ++i)
    {
        spawn_enemy(state, i);
    }

    // camera
    state->camera = (Camera2D){ 0 };
    state->camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
    state->camera.zoom = 3.0f;

    // assets
    state->playerTexture = LoadTexture("data/assets/AdeptNecromancer/AdeptNecromancer.png");
    state->enemyTexture = LoadTexture("data/assets/CorruptedTreant/CorruptedTreant.png");
}
