void
zero_memory(void* ptr, size_t size)
{
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        p[i] = 0;
    }
}

void
update_animation_frame(AnimationSettings animation,
                       float* animationFrameX)
{
    if (animation.elapsedTime < animation.delay) return;

    *animationFrameX += FRAME_SIZE;
    if (*animationFrameX >= FRAME_SIZE * 3)
    {
        *animationFrameX = 0;
    }
}

void
spawn_enemy(GameContext* gameContext, int index)
{
    Enemy *enemy = &gameContext->enemies[index];
    enemy->type = 1;
    enemy->hp = 10;
    enemy->speed = 80.0f;
    enemy->damage = 10;
    enemy->targeted = false;
    enemy->animationFrame = (Rectangle){0, 0, FRAME_SIZE, FRAME_SIZE};

    int side = GetRandomValue(0, 3);
    Vector2 spawnPos = { 
        (int)gameContext->player->position.x, 
        (int)gameContext->player->position.y
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
setup_game(GameMemory* gameMemory)
{
    // alloc memory arena if not allocated yet
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

    // setup effects
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        gameContext->position[i] = (Vector2) { 
            GetRandomValue(0, SCREEN_WIDTH), 
            GetRandomValue(0, -100000)
        };
    }

    // setup camera
    gameContext->camera = (Camera2D){ 0 };
    gameContext->camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
    gameContext->camera.zoom = 3.0f;

    // setup animation
    gameContext->animation.delay = 0.2f;
}

void
player_take_damage(Player* player, int damage)
{ 
    if (player->invulnerable)
        return;

    player->hp -= damage;
    player->invulnerable = true;
}

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
update_player(GameContext* gameContext, float deltaTime)
{
    Player* player = gameContext->player;

    if (IsKeyDown(KEY_SPACE))
    {
        for (int x = 0; x < gameContext->xpCount; ++x)
        {
            Experience* xp = &gameContext->xp[x];

            float dx = player->position.x - xp->body.x;
            float dy = player->position.y - xp->body.y;
            float len = sqrtf(dx*dx + dy*dy);

            if (len > 0.0f)
            {
                xp->body.x += (dx / len) * XP_SPEED * deltaTime;
                xp->body.y += (dy / len) * XP_SPEED * deltaTime;
            }
        }
    }
    
    if (IsKeyDown(KEY_W)) player->position.y -= player->speed * deltaTime;
    else if (IsKeyDown(KEY_S)) player->position.y += player->speed * deltaTime;

    if (IsKeyDown(KEY_D)) 
    {
        player->position.x += player->speed * deltaTime;
        if (player->animationFrame.width < 0) player->animationFrame.width *= -1;
    }
    else if (IsKeyDown(KEY_A)) 
    {
        player->position.x -= player->speed * deltaTime;
        if (player->animationFrame.width > 0) player->animationFrame.width *= -1;
    }

    player->body.x = player->position.x - player->body.width/2;
    player->body.y = player->position.y - player->body.height/2;

    if (player->invulnerable) 
    {
        player->color = RED;
        player->invulnerabilityElapsedTime += deltaTime;
        if (player->invulnerabilityElapsedTime >= player->invulnerabilityDelay)
        {
            player->invulnerable = false;
            player->invulnerabilityElapsedTime = 0.0f;
            player->color = WHITE;
        }
    }
}

void
update_xps(GameContext* gameContext, float deltaTime)
{
    Player* player = gameContext->player;

    for (int x = 0; x < gameContext->xpCount; ++x)
    {
        Experience* xp = &gameContext->xp[x];

        if (!CheckCollisionCircleRec(player->position, player->xpRadius, xp->body))
            continue;

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

void update_enemies(GameContext* gameContext, float deltaTime)
{
    const Player* player = gameContext->player;

    gameContext->hordeSpawnElapsedTime += deltaTime;
    if (gameContext->enemyCount < MAX_ENEMIES
        && gameContext->hordeSpawnElapsedTime >= HORDE_SPAWN_DELAY)
    {
        gameContext->enemyCount += HORDE_ENEMY_NUMBER_INCREASE;
        gameContext->hordeSpawnElapsedTime = 0;

        if (gameContext->enemyCount > MAX_ENEMIES)
            gameContext->enemyCount = MAX_ENEMIES;
    }

    for (int e = 0; e < gameContext->enemyCount; ++e)
    {
        Enemy* enemy = &gameContext->enemies[e];

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
            player_take_damage(gameContext->player, enemy->damage);

        if (CheckCollisionCircleRec(player->position, player->visionRadius, enemy->body))
        {
            for (int b = 0; b < gameContext->bulletCount; ++b)
            {
                if (enemy->targeted)
                {
                    break;
                }

                if (gameContext->targets[b] == -1) 
                {
                    gameContext->targets[b] = e;
                    enemy->targeted = true;
                    break;
                }
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
    update_player(gameContext, deltaTime);
    update_xps(gameContext, deltaTime);
    update_enemies(gameContext, deltaTime);
}

void
render_game(const GameMemory* gameMemory)
{
    GameContext* gameContext = gameMemory->gameContext;

    if (gameContext->animation.elapsedTime >= gameContext->animation.delay)
    {
        gameContext->animation.elapsedTime = 0.0f;
    }

    gameContext->animation.elapsedTime += GetFrameTime();

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

    update_animation_frame(gameContext->animation, &(player->animationFrame.x));
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
        update_animation_frame(gameContext->animation, &(enemy->animationFrame.x));

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

    //const Vector2 center = { SCREEN_WIDTH/2, 0 };
    //const char* timer = "10:00";
    //DrawText(timer, center.x - MeasureText(timer, FONT_SIZE)/2, center.y + TEXT_Y_OFFSET, FONT_SIZE, WHITE);

    if (player->levelUp)
    {
        update_level_up_menu(gameContext);
    }

    DrawFPS(20, 80);
    EndDrawing();
}
