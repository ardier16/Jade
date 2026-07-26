#ifndef AMALGAMATED_BUILD
#include "../button_events.h"
#include "../jade_assert.h"
#include "../ui.h"

gui_activity_t* make_tools_games_activity(void)
{
    btn_data_t hdrbtns[] = { { .txt = "=", .font = JADE_SYMBOLS_16x16_FONT, .ev_id = BTN_TOOLS_GAMES_EXIT },
        { .txt = NULL, .font = GUI_DEFAULT_FONT, .ev_id = GUI_BUTTON_EVENT_NONE } };

    btn_data_t menubtns[] = { { .txt = "Stopwatch", .font = GUI_DEFAULT_FONT, .ev_id = BTN_TOOLS_STOPWATCH },
        { .txt = "Timer", .font = GUI_DEFAULT_FONT, .ev_id = BTN_TOOLS_TIMER },
        { .txt = "2048", .font = GUI_DEFAULT_FONT, .ev_id = BTN_TOOLS_2048 } };

    return make_menu_activity("Tools & Games", hdrbtns, 2, menubtns, sizeof(menubtns) / sizeof(btn_data_t));
}

static gui_activity_t* make_timed_activity(const char* title, const char* primary_label, const uint32_t primary_event,
    const char* secondary_label, const uint32_t secondary_event, const uint32_t exit_event, gui_view_node_t** time_text,
    gui_view_node_t** primary_text)
{
    JADE_ASSERT(title);
    JADE_ASSERT(primary_label);
    JADE_ASSERT(secondary_label);
    JADE_INIT_OUT_PPTR(time_text);
    JADE_INIT_OUT_PPTR(primary_text);

    gui_activity_t* const act = gui_make_activity();
    btn_data_t hdrbtns[] = { { .txt = "=", .font = JADE_SYMBOLS_16x16_FONT, .ev_id = exit_event },
        { .txt = NULL, .font = GUI_DEFAULT_FONT, .ev_id = GUI_BUTTON_EVENT_NONE } };
    gui_view_node_t* const parent = add_title_bar(act, title, hdrbtns, 2, NULL);

    gui_view_node_t* vsplit;
    gui_make_vsplit(&vsplit, GUI_SPLIT_RELATIVE, 2, 70, 30);
    gui_set_parent(vsplit, parent);

    gui_view_node_t* time_container;
    gui_make_fill(&time_container, TFT_BLACK, FILL_PLAIN, vsplit);

    gui_make_text_font(time_text, "00:00:00", TFT_WHITE, DEJAVU24_FONT);
    gui_set_align(*time_text, GUI_ALIGN_CENTER, GUI_ALIGN_MIDDLE);
    gui_set_parent(*time_text, time_container);

    gui_make_text(primary_text, primary_label, TFT_WHITE);
    gui_set_align(*primary_text, GUI_ALIGN_CENTER, GUI_ALIGN_MIDDLE);
    btn_data_t ftrbtns[] = { { .content = *primary_text, .ev_id = primary_event, .borders = GUI_BORDER_TOPRIGHT },
        { .txt = secondary_label, .font = GUI_DEFAULT_FONT, .ev_id = secondary_event, .borders = GUI_BORDER_TOP } };
    add_buttons(vsplit, UI_ROW, ftrbtns, 2);
    gui_set_activity_initial_selection(ftrbtns[0].btn);

    return act;
}

gui_activity_t* make_stopwatch_activity(gui_view_node_t** time_text, gui_view_node_t** action_text)
{
    return make_timed_activity("Stopwatch", "Start", BTN_STOPWATCH_TOGGLE, "Reset", BTN_STOPWATCH_RESET,
        BTN_STOPWATCH_EXIT, time_text, action_text);
}

gui_activity_t* make_timer_activity(gui_view_node_t** time_text, gui_view_node_t** action_text)
{
    return make_timed_activity(
        "Timer", "Pause", BTN_TIMER_TOGGLE, "Reset", BTN_TIMER_RESET, BTN_TIMER_EXIT, time_text, action_text);
}

gui_activity_t* make_timer_complete_activity(void)
{
    gui_activity_t* const act = gui_make_activity();
    btn_data_t hdrbtns[] = { { .txt = "=", .font = JADE_SYMBOLS_16x16_FONT, .ev_id = BTN_TIMER_EXIT },
        { .txt = NULL, .font = GUI_DEFAULT_FONT, .ev_id = GUI_BUTTON_EVENT_NONE } };
    gui_view_node_t* const parent = add_title_bar(act, "Timer", hdrbtns, 2, NULL);

    gui_view_node_t* vsplit;
    gui_make_vsplit(&vsplit, GUI_SPLIT_RELATIVE, 2, 70, 30);
    gui_set_parent(vsplit, parent);

    gui_view_node_t* message_split;
    gui_make_vsplit(&message_split, GUI_SPLIT_RELATIVE, 2, 60, 40);
    gui_set_parent(message_split, vsplit);

    gui_view_node_t* node;
    gui_make_text_font(&node, "00:00:00", TFT_WHITE, DEJAVU24_FONT);
    gui_set_align(node, GUI_ALIGN_CENTER, GUI_ALIGN_BOTTOM);
    gui_set_parent(node, message_split);

    gui_make_text(&node, "Time's up!", TFT_WHITE);
    gui_set_align(node, GUI_ALIGN_CENTER, GUI_ALIGN_TOP);
    gui_set_parent(node, message_split);

    btn_data_t ftrbtns[]
        = { { .txt = "Restart", .font = GUI_DEFAULT_FONT, .ev_id = BTN_TIMER_RESTART, .borders = GUI_BORDER_TOPRIGHT },
              { .txt = "Change", .font = GUI_DEFAULT_FONT, .ev_id = BTN_TIMER_CHANGE, .borders = GUI_BORDER_TOP } };
    add_buttons(vsplit, UI_ROW, ftrbtns, 2);
    gui_set_activity_initial_selection(ftrbtns[0].btn);

    return act;
}
#endif // AMALGAMATED_BUILD
