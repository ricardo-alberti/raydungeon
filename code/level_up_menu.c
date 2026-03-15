#define BTN_WIDTH  350
#define BTN_HEIGHT 100
#define OFFSET     (BTN_HEIGHT + 10)
#define FONT_SIZE  30
#define TEXT_Y_OFFSET 10

void
update_level_up_menu(GameContext* gameContext)
{
    const Vector2 center = { SCREEN_WIDTH/2 - BTN_WIDTH/2, SCREEN_HEIGHT/2 - BTN_HEIGHT*2};

    // button text
    const char* option1Text = "+ 100 MAX HP";
    const char* option2Text = "+ 100 DAMAGE";
    const char* option3Text = "NEW WEAPON";

    // button dimensions/position
    Rectangle mouse = { GetMouseX() - 5, GetMouseY() - 5 , 10, 10 };
    const Rectangle option1 = { center.x, center.y, BTN_WIDTH, BTN_HEIGHT };
    const Rectangle option2 = { center.x, center.y + OFFSET, BTN_WIDTH, BTN_HEIGHT };
    const Rectangle option3 = { center.x, center.y + OFFSET*2, BTN_WIDTH, BTN_HEIGHT };

    // draw rectangle
    DrawRectangleRec(option1, CheckCollisionRecs(mouse, option1) ? WHITE : BLUE);
    DrawRectangleRec(option2, CheckCollisionRecs(mouse, option2) ? WHITE : BLUE);
    DrawRectangleRec(option3, CheckCollisionRecs(mouse, option3) ? WHITE : BLUE);
    DrawRectangleRec(mouse, BLUE);

    // draw button texts
    DrawText(option1Text, option1.x + BTN_WIDTH/2 - MeasureText(option1Text, FONT_SIZE)/2, option1.y + TEXT_Y_OFFSET, FONT_SIZE, BLACK);
    DrawText(option2Text, option2.x + BTN_WIDTH/2 - MeasureText(option2Text, FONT_SIZE)/2, option2.y + TEXT_Y_OFFSET, FONT_SIZE, BLACK);
    DrawText(option3Text, option3.x + BTN_WIDTH/2 - MeasureText(option3Text, FONT_SIZE)/2, option3.y + TEXT_Y_OFFSET, FONT_SIZE, BLACK);

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 pos = GetMousePosition();
    if (CheckCollisionPointRec(pos, option1))
    {
        gameContext->player->maxHp += 100;
        gameContext->player->hp = gameContext->player->maxHp;
    }
    else if (CheckCollisionPointRec(pos, option2))
    {
        gameContext->player->damage += FONT_SIZE;
    }
    else if (CheckCollisionPointRec(pos, option3))
    {

    }

    gameContext->player->levelUp = false;
}
