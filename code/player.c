
void
update_player(Player* player, float deltaTime)
{
    // key inputs
    if (IsKeyDown(KEY_W)) 
    {
        player->position.y -= player->speed * deltaTime;
    }
    else if (IsKeyDown(KEY_S)) 
    {
        player->position.y += player->speed * deltaTime;
    }

    if (IsKeyDown(KEY_D)) 
    {
        player->position.x += player->speed * deltaTime;
        if (player->animationFrame.width < 0)
        {
            player->animationFrame.width *= -1;
        }
    }
    else if (IsKeyDown(KEY_A)) 
    {
        player->position.x -= player->speed * deltaTime;
        if (player->animationFrame.width > 0)
        {
            player->animationFrame.width *= -1;
        }
    }

    // invulnerability
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

    // level-up
    if (player->xp >= player->xpMax)
    {
        player->xpMax += player->xpMax;
        player->xp = 0;
        player->levelUp = true;
    }

    // centralize
    player->body.x = player->position.x - player->body.width/2;
    player->body.y = player->position.y - player->body.height/2;
}
