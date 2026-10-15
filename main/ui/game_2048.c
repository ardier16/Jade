#include "../button_events.h"
#include "../jade_assert.h"
#include "../ui.h"

#include <string.h>

#define GAME_2048_CONTENT_HEIGHT ((CONFIG_DISPLAY_HEIGHT * 80) / 100)
#define GAME_2048_MIN_SIDE_PANEL 96
#define GAME_2048_TILE_GAP 1

#if CONFIG_DISPLAY_WIDTH - GAME_2048_CONTENT_HEIGHT >= GAME_2048_MIN_SIDE_PANEL
#define GAME_2048_LANDSCAPE_LAYOUT 1
#define GAME_2048_BOARD_SIZE GAME_2048_CONTENT_HEIGHT
#else
#define GAME_2048_LANDSCAPE_LAYOUT 0
#define GAME_2048_HUD_HEIGHT (CONFIG_DISPLAY_HEIGHT >= 200 ? 40 : 32)
#if CONFIG_DISPLAY_WIDTH < GAME_2048_CONTENT_HEIGHT - GAME_2048_HUD_HEIGHT
#define GAME_2048_BOARD_SIZE CONFIG_DISPLAY_WIDTH
#else
#define GAME_2048_BOARD_SIZE (GAME_2048_CONTENT_HEIGHT - GAME_2048_HUD_HEIGHT)
#endif
#endif

#if GAME_2048_BOARD_SIZE / 4 < 32
#define GAME_2048_TILE_FONT SMALL_FONT
#else
#define GAME_2048_TILE_FONT GUI_DEFAULT_FONT
#endif

static color_t tile_background(const uint8_t exponent)
{
    switch (exponent) {
    case 0:
        return TFT_BLACK;
    case 1:
        return TFT_LIGHTGREY;
    case 2:
        return TFT_YELLOW;
    case 3:
        return TFT_ORANGE;
    case 4:
        return TFT_GREENYELLOW;
    case 5:
        return TFT_PINK;
    case 6:
        return TFT_RED;
    case 7:
        return TFT_MAGENTA;
    case 8:
        return TFT_PURPLE;
    case 9:
        return TFT_BLUE;
    case 10:
        return TFT_DARKCYAN;
    default:
        return GUI_BLOCKSTREAM_JADE_GREEN;
    }
}

static color_t tile_foreground(const uint8_t exponent) { return exponent > 5 ? TFT_WHITE : TFT_BLACK; }

static void format_u64_decimal(char* output, const size_t output_len, uint64_t value)
{
    JADE_ASSERT(output);
    JADE_ASSERT(output_len);

    char reversed[20];
    size_t num_digits = 0;
    do {
        JADE_ASSERT(num_digits < sizeof(reversed));
        reversed[num_digits++] = '0' + value % 10;
        value /= 10;
    } while (value);

    JADE_ASSERT(num_digits < output_len);
    for (size_t i = 0; i < num_digits; ++i) {
        output[i] = reversed[num_digits - 1 - i];
    }
    output[num_digits] = '\0';
}

static void format_tile(char* output, const size_t output_len, const uint8_t exponent)
{
    JADE_ASSERT(output);
    JADE_ASSERT(output_len);

    if (!exponent) {
        output[0] = '\0';
        return;
    }
    if (exponent <= 9) {
        format_u64_decimal(output, output_len, UINT64_C(1) << exponent);
    } else {
        static const char suffixes[] = "kMGTPE";
        const uint8_t suffix_index = (exponent / 10) - 1;
        const uint8_t remainder = exponent % 10;
        JADE_ASSERT(suffix_index < sizeof(suffixes) - 1);
        format_u64_decimal(output, output_len, UINT64_C(1) << remainder);
        const size_t len = strlen(output);
        JADE_ASSERT(len + 1 < output_len);
        output[len] = suffixes[suffix_index];
        output[len + 1] = '\0';
    }
}

static void format_score(char* output, const size_t output_len, const uint64_t score)
{
    static const char prefix[] = "Score: ";
    JADE_ASSERT(output);
    JADE_ASSERT(output_len > sizeof(prefix));

    memcpy(output, prefix, sizeof(prefix) - 1);
    format_u64_decimal(output + sizeof(prefix) - 1, output_len - (sizeof(prefix) - 1), score);
}

static void make_board(gui_view_node_t* parent, ui_2048_view_t* view)
{
    gui_make_vsplit(&view->board, GUI_SPLIT_RELATIVE, 4, 25, 25, 25, 25);
    gui_set_parent(view->board, parent);

    for (uint8_t row = 0; row < 4; ++row) {
        gui_view_node_t* columns;
        gui_make_hsplit(&columns, GUI_SPLIT_RELATIVE, 4, 25, 25, 25, 25);
        gui_set_parent(columns, view->board);

        for (uint8_t column = 0; column < 4; ++column) {
            const uint8_t index = row * 4 + column;
            gui_make_fill(&view->tile_fills[index], TFT_BLACK, FILL_PLAIN, columns);
            gui_set_margins(view->tile_fills[index], GUI_MARGIN_ALL_EQUAL, GAME_2048_TILE_GAP);
            gui_set_borders(view->tile_fills[index], TFT_DARKGREY, 1, GUI_BORDER_ALL);

            gui_make_text_font(&view->tile_texts[index], "", TFT_WHITE, GAME_2048_TILE_FONT);
            gui_set_align(view->tile_texts[index], GUI_ALIGN_CENTER, GUI_ALIGN_MIDDLE);
            gui_set_parent(view->tile_texts[index], view->tile_fills[index]);
        }
    }
}

static gui_view_node_t* make_text(const char* value, const uint32_t font)
{
    gui_view_node_t* text;
    gui_make_text_font(&text, value, TFT_WHITE, font);
    gui_set_align(text, GUI_ALIGN_CENTER, GUI_ALIGN_MIDDLE);
    return text;
}

gui_activity_t* make_2048_activity(ui_2048_view_t* view)
{
    JADE_ASSERT(view);
    memset(view, 0, sizeof(*view));

    gui_activity_t* const act = gui_make_activity();
    view->activity = act;
    gui_view_node_t* const content = add_title_bar(act, "2048", NULL, 0, NULL);

#if GAME_2048_LANDSCAPE_LAYOUT
    gui_view_node_t* layout;
    gui_make_hsplit(&layout, GUI_SPLIT_ABSOLUTE, 2, GAME_2048_BOARD_SIZE, GUI_SPLIT_FILL_REMAINING);
    gui_set_parent(layout, content);
    make_board(layout, view);

    gui_make_fill(&view->hud, TFT_BLACK, FILL_PLAIN, layout);

    gui_view_node_t* hud;
    gui_make_vsplit(&hud, GUI_SPLIT_RELATIVE, 5, 20, 20, 20, 20, 20);
    gui_set_padding(hud, GUI_MARGIN_TWO_VALUES, 2, 4);
    gui_set_parent(hud, view->hud);

    view->score_text = make_text("Score: 0", SMALL_FONT);
    gui_set_parent(view->score_text, hud);
    view->axis_text = make_text("Axis: Horizontal", GUI_DEFAULT_FONT);
    gui_set_parent(view->axis_text, hud);
    gui_view_node_t* directions = make_text("<  Move  >", GUI_DEFAULT_FONT);
    gui_set_parent(directions, hud);
    view->select_text = make_text("Select: Axis", SMALL_FONT);
    gui_set_parent(view->select_text, hud);
    gui_view_node_t* menu_text = make_text("Double: Menu", SMALL_FONT);
    gui_set_parent(menu_text, hud);
#else
    gui_view_node_t* layout;
    gui_make_vsplit(&layout, GUI_SPLIT_ABSOLUTE, 2, GAME_2048_HUD_HEIGHT, GAME_2048_BOARD_SIZE);
    gui_set_parent(layout, content);

    gui_make_fill(&view->hud, TFT_BLACK, FILL_PLAIN, layout);

    gui_view_node_t* hud;
    gui_make_vsplit(&hud, GUI_SPLIT_RELATIVE, 2, 50, 50);
    gui_set_parent(hud, view->hud);

    gui_view_node_t* status;
    gui_make_hsplit(&status, GUI_SPLIT_RELATIVE, 2, 50, 50);
    gui_set_parent(status, hud);
    view->score_text = make_text("Score: 0", SMALL_FONT);
    gui_set_parent(view->score_text, status);
    view->axis_text = make_text("Horizontal", SMALL_FONT);
    gui_set_parent(view->axis_text, status);

    gui_view_node_t* help;
    gui_make_hsplit(&help, GUI_SPLIT_RELATIVE, 2, 50, 50);
    gui_set_parent(help, hud);
    view->select_text = make_text("Select: Axis", SMALL_FONT);
    gui_set_parent(view->select_text, help);
    gui_view_node_t* menu_text = make_text("Double: Menu", SMALL_FONT);
    gui_set_parent(menu_text, help);

    const uint16_t side_padding = (CONFIG_DISPLAY_WIDTH - GAME_2048_BOARD_SIZE) / 2;
    gui_view_node_t* board_row;
    gui_make_hsplit(&board_row, GUI_SPLIT_ABSOLUTE, 3, side_padding, GAME_2048_BOARD_SIZE, GUI_SPLIT_FILL_REMAINING);
    gui_set_parent(board_row, layout);
    gui_view_node_t* spacer;
    gui_make_fill(&spacer, TFT_BLACK, FILL_PLAIN, board_row);
    make_board(board_row, view);
    gui_make_fill(&spacer, TFT_BLACK, FILL_PLAIN, board_row);
#endif

    return act;
}

void update_2048_activity(const ui_2048_view_t* view, const uint8_t tiles[UI_2048_CELL_COUNT], const uint64_t score,
    const bool vertical, const uint16_t accent_mask)
{
    JADE_ASSERT(view);
    JADE_ASSERT(tiles);

    for (uint8_t i = 0; i < UI_2048_CELL_COUNT; ++i) {
        char value[8];
        format_tile(value, sizeof(value), tiles[i]);
        const bool accent = accent_mask & (UINT16_C(1) << i);
        const color_t background = accent ? TFT_WHITE : tile_background(tiles[i]);
        const color_t foreground = accent ? TFT_BLACK : tile_foreground(tiles[i]);
        const uint32_t font = strlen(value) >= 3 ? SMALL_FONT : GAME_2048_TILE_FONT;
        gui_set_color(view->tile_fills[i], background);
        gui_set_color(view->tile_texts[i], foreground);
        gui_set_text_font(view->tile_texts[i], font);
        gui_update_text(view->tile_texts[i], value);
        gui_repaint(view->tile_fills[i]);
    }

    char score_value[32];
    format_score(score_value, sizeof(score_value), score);
    gui_update_text(view->score_text, score_value);

#if GAME_2048_LANDSCAPE_LAYOUT
    const char* const axis = vertical ? "Axis: Vertical" : "Axis: Horizontal";
#else
    const char* const axis = vertical ? "Vertical" : "Horizontal";
#endif
    gui_update_text(view->axis_text, axis);

    // Split nodes do not paint a background, so repaint the owning fill after
    // batched text changes to clear the previous HUD glyphs.
    gui_repaint(view->hud);
}

gui_activity_t* make_2048_pause_activity(void)
{
    btn_data_t hdrbtns[] = {
        { .txt = NULL, .font = GUI_DEFAULT_FONT, .ev_id = GUI_BUTTON_EVENT_NONE },
        { .txt = NULL, .font = GUI_DEFAULT_FONT, .ev_id = GUI_BUTTON_EVENT_NONE },
    };
    btn_data_t buttons[] = {
        { .txt = "Resume", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_RESUME },
        { .txt = "New Game", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_NEW },
        { .txt = "Exit", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_EXIT },
    };
    gui_activity_t* const act = make_menu_activity("2048 Paused", hdrbtns, 2, buttons, 3);
    gui_set_activity_initial_selection(buttons[0].btn);
    return act;
}

gui_activity_t* make_2048_confirm_activity(const char* title, const char* message)
{
    JADE_ASSERT(title);
    JADE_ASSERT(message);

    const char* messages[] = { message };
    btn_data_t buttons[] = {
        { .txt = "No", .font = GUI_DEFAULT_FONT, .ev_id = BTN_NO, .borders = GUI_BORDER_TOPRIGHT },
        { .txt = "Yes", .font = GUI_DEFAULT_FONT, .ev_id = BTN_YES, .borders = GUI_BORDER_TOPLEFT },
    };
    gui_activity_t* const act = make_show_message_activity(messages, 1, title, NULL, 0, buttons, 2);
    gui_set_activity_initial_selection(buttons[0].btn);
    return act;
}

static gui_activity_t* make_result_activity(
    const char* title, const char* result, const uint64_t score, btn_data_t buttons[2])
{
    char score_value[32];
    format_score(score_value, sizeof(score_value), score);
    const char* messages[] = { result, score_value };
    gui_activity_t* const act = make_show_message_activity(messages, 2, title, NULL, 0, buttons, 2);
    gui_set_activity_initial_selection(buttons[0].btn);
    return act;
}

gui_activity_t* make_2048_win_activity(const uint64_t score)
{
    btn_data_t buttons[] = {
        { .txt = "Continue", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_CONTINUE, .borders = GUI_BORDER_TOPRIGHT },
        { .txt = "New Game", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_NEW, .borders = GUI_BORDER_TOPLEFT },
    };
    return make_result_activity("2048!", "You made 2048!", score, buttons);
}

gui_activity_t* make_2048_game_over_activity(const uint64_t score)
{
    btn_data_t buttons[] = {
        { .txt = "Restart", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_RESTART, .borders = GUI_BORDER_TOPRIGHT },
        { .txt = "Exit", .font = GUI_DEFAULT_FONT, .ev_id = BTN_2048_EXIT, .borders = GUI_BORDER_TOPLEFT },
    };
    return make_result_activity("Game Over", "No moves left", score, buttons);
}
