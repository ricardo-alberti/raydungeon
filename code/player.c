
void
update_player(Player* player, float deltaTime)
{
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
