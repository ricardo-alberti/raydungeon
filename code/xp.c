static inline Rectangle
get_xp_body(Vector2* xp_position)
{
    return (Rectangle){
        xp_position->x - XP_SIZE/2,
        xp_position->y - XP_SIZE/2,
        XP_SIZE,
        XP_SIZE
    };
}

void
update_xps(const Player* player, int* player_xp, Vector2* xp_positions, int* xp_count, float delta_time)
{
    for (int x = 0; x < *xp_count; ++x)
    {
        // if not in players range skip
        Vector2* xp_position = &xp_positions[x];
        Rectangle xp_body = get_xp_body(xp_position);
        if (!CheckCollisionCircleRec(player->position, player->xpRadius, xp_body))
        {
            continue;
        }

        // move to player's direction
        float dx = player->position.x - xp_body.x;
        float dy = player->position.y - xp_body.y;
        float len = sqrtf(dx*dx + dy*dy);
        if (len > 0.0f)
        {
            xp_position->x += (dx / len) * XP_SPEED * delta_time;
            xp_position->y += (dy / len) * XP_SPEED * delta_time;
        }

        // collidde with player
        if (CheckCollisionRecs(xp_body, player->body))
        {
            xp_positions[x] = xp_positions[(*xp_count) - 1];
            (*xp_count)--;
            (*player_xp)++;
        }
    }
}
