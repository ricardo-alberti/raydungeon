#define DEBUG 1

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
#define MAX_ENEMIES 10000
#define START_ENEMY_COUNT MAX_ENEMIES
#define HORDE_SPAWN_DELAY 5.0f
#define HORDE_ENEMY_NUMBER_INCREASE 10

// XP
#define MAX_XP MAX_ENEMIES
#define XP_SIZE 2.0f
#define XP_SPEED 180.0f

// BULLETS
#define MAX_BULLETS MAX_ENEMIES
#define RADIUS 30.0f

typedef struct {
    void* base;
    size_t size;
    size_t used;
} Arena;

typedef struct {
    // stats
    float speed;
    int hp;
    int maxHp;
    int damage;
    bool invulnerable;
    float invulnerabilityDelay;
    float invulnerabilityElapsedTime;
    float xpRadius;
    float visionRadius;

    // xp
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
    // timers
    float animationElapsedTime;
    float hordeSpawnElapsedTime;
    Camera2D camera;

    // textures
    Texture2D playerTexture;
    Texture2D enemyTexture;

    // entities
    Player* player;

    Vector2* xpPositions;
    int xpCount;
    int ringPointer;

    Enemy* enemies;
    int enemyCount;

    Bullet* bullets;
    int* targets;
    int bulletCount;
} GameState;

typedef struct {
    bool IsInitialized;
    Arena permanentStorage; 
    Arena transientStorage; 
    GameState* state;
} GameMemory;
