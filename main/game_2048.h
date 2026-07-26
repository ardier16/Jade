#ifndef GAME_2048_H_
#define GAME_2048_H_

#include <stdbool.h>
#include <stdint.h>

#define GAME_2048_SIDE 4
#define GAME_2048_CELL_COUNT (GAME_2048_SIDE * GAME_2048_SIDE)
#define GAME_2048_WIN_EXPONENT 11
#define GAME_2048_MAX_EXPONENT 63

typedef enum {
    GAME_2048_LEFT,
    GAME_2048_RIGHT,
    GAME_2048_UP,
    GAME_2048_DOWN,
} game_2048_direction_t;

typedef struct {
    uint8_t cells[GAME_2048_CELL_COUNT];
    uint64_t score;
} game_2048_t;

typedef struct {
    bool changed;
    bool reached_2048;
    bool game_over;
    uint16_t merged_mask;
    uint16_t spawned_mask;
} game_2048_move_result_t;

// Writes a uniformly distributed value in [0, upper_bound) to value.
typedef bool (*game_2048_random_fn)(void* ctx, uint8_t upper_bound, uint8_t* value);

bool game_2048_new(game_2048_t* game, game_2048_random_fn random_fn, void* random_ctx);
bool game_2048_move(game_2048_t* game, game_2048_direction_t direction, game_2048_random_fn random_fn, void* random_ctx,
    game_2048_move_result_t* result);
bool game_2048_is_game_over(const game_2048_t* game);

#endif /* GAME_2048_H_ */
