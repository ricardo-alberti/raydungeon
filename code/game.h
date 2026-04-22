// SIZES
#define kB(value) (value * 1024)
#define MB(value) (kB(value) * 1024)

// ANIMATION
#define ANIMATION_DELAY 0.2f

// SPRITES
#define FRAME_SIZE 16

// SCREEN
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

// STATUS BAR
#define STATUS_BAR_WIDTH 300

// ENEMIES
#define MAX_ENEMIES 100
#define START_ENEMY_COUNT MAX_ENEMIES
#define HORDE_SPAWN_DELAY 5.0f
#define HORDE_ENEMY_NUMBER_INCREASE 10

// XP
#define MAX_XP 100
#define XP_SIZE 2.0f
#define XP_SPEED 180.0f

// BULLETS
#define MAX_BULLETS 100
#define RADIUS 30.0f

typedef struct {
    void* base;
    size_t size;
    size_t used;
} Arena;

typedef struct {
    // STATS
    float speed;
    int hp;
    int maxHp;
    int damage;
    bool invulnerable;
    float invulnerabilityDelay;
    float invulnerabilityElapsedTime;
    float xpRadius;
    float visionRadius;

    // XP
    int xp;
    int xpMax;

    bool levelUp;
    Color color;
    Vector2 position;
    Rectangle body;
    Rectangle animationFrame;
} Player;

typedef struct {
    int type;
    Rectangle body;
    float speed;
} Bullet;

typedef struct {
    int type;
    int hp;
    int damage;
    float speed;
    bool targeted;
    Vector2 position;
    Rectangle body;
    Rectangle animationFrame;
} Enemy;

typedef struct {
    int type;
    float speed;
    Rectangle body;
} Experience;

typedef struct {
    float animationElapsedTime;
    Camera2D camera;

    Player* player;

    Experience* xp;
    int xpCount;
    int ringPointer;

    Enemy* enemies;
    int enemyCount;
    float hordeSpawnElapsedTime;

    Vector2* position;
    Bullet* bullets;
    int* targets;
    int bulletCount;

    Texture2D playerTexture;
    Texture2D enemyTexture;
    Texture2D glowingWispTexture;
} GameContext;

typedef struct {
    bool IsInitialized;
    Arena permanentStorage; 
    Arena transientStorage; 
    GameContext* gameContext;
} GameMemory;
