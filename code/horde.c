
void
update_horde(int* enemyCount, float* hordeSpawnElapsedTime, float deltaTime)
{
    *hordeSpawnElapsedTime += deltaTime;
    if (*enemyCount < MAX_ENEMIES
        && *hordeSpawnElapsedTime >= HORDE_SPAWN_DELAY)
    {
        *enemyCount += HORDE_ENEMY_NUMBER_INCREASE;
        *hordeSpawnElapsedTime = 0;

        if (*enemyCount > MAX_ENEMIES)
        {
            *enemyCount = MAX_ENEMIES;
        }
    }
}
