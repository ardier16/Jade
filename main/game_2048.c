#include "game_2048.h"

#include <limits.h>
#include <stddef.h>
#include <string.h>

static uint8_t cell_index(const game_2048_direction_t direction, const uint8_t line, const uint8_t offset)
{
    switch (direction) {
    case GAME_2048_LEFT:
        return line * GAME_2048_SIDE + offset;
    case GAME_2048_RIGHT:
        return line * GAME_2048_SIDE + (GAME_2048_SIDE - 1 - offset);
    case GAME_2048_UP:
        return offset * GAME_2048_SIDE + line;
    case GAME_2048_DOWN:
        return (GAME_2048_SIDE - 1 - offset) * GAME_2048_SIDE + line;
    default:
        return GAME_2048_CELL_COUNT;
    }
}

static void add_score(uint64_t* score, const uint8_t exponent)
{
    const uint64_t value = UINT64_C(1) << exponent;
    *score = UINT64_MAX - *score < value ? UINT64_MAX : *score + value;
}

static bool spawn_tile(game_2048_t* game, game_2048_random_fn random_fn, void* random_ctx, uint16_t* spawned_mask)
{
    uint8_t empty_cells[GAME_2048_CELL_COUNT];
    uint8_t num_empty = 0;
    for (uint8_t i = 0; i < GAME_2048_CELL_COUNT; ++i) {
        if (!game->cells[i]) {
            empty_cells[num_empty++] = i;
        }
    }
    if (!num_empty || !random_fn) {
        return false;
    }

    uint8_t empty_index;
    uint8_t tile_roll;
    if (!random_fn(random_ctx, num_empty, &empty_index) || empty_index >= num_empty
        || !random_fn(random_ctx, 10, &tile_roll) || tile_roll >= 10) {
        return false;
    }

    const uint8_t index = empty_cells[empty_index];
    game->cells[index] = tile_roll == 9 ? 2 : 1;
    if (spawned_mask) {
        *spawned_mask |= UINT16_C(1) << index;
    }
    return true;
}

bool game_2048_new(game_2048_t* game, game_2048_random_fn random_fn, void* random_ctx)
{
    if (!game || !random_fn) {
        return false;
    }

    game_2048_t next = { 0 };
    if (!spawn_tile(&next, random_fn, random_ctx, NULL) || !spawn_tile(&next, random_fn, random_ctx, NULL)) {
        return false;
    }

    *game = next;
    return true;
}

bool game_2048_is_game_over(const game_2048_t* game)
{
    if (!game) {
        return true;
    }

    for (uint8_t row = 0; row < GAME_2048_SIDE; ++row) {
        for (uint8_t column = 0; column < GAME_2048_SIDE; ++column) {
            const uint8_t index = row * GAME_2048_SIDE + column;
            const uint8_t exponent = game->cells[index];
            if (!exponent) {
                return false;
            }
            if (column + 1 < GAME_2048_SIDE && exponent < GAME_2048_MAX_EXPONENT
                && exponent == game->cells[index + 1]) {
                return false;
            }
            if (row + 1 < GAME_2048_SIDE && exponent < GAME_2048_MAX_EXPONENT
                && exponent == game->cells[index + GAME_2048_SIDE]) {
                return false;
            }
        }
    }
    return true;
}

bool game_2048_move(game_2048_t* game, const game_2048_direction_t direction, game_2048_random_fn random_fn,
    void* random_ctx, game_2048_move_result_t* result)
{
    if (!game || !result || !random_fn) {
        return false;
    }
    switch (direction) {
    case GAME_2048_LEFT:
    case GAME_2048_RIGHT:
    case GAME_2048_UP:
    case GAME_2048_DOWN:
        break;
    default:
        return false;
    }

    game_2048_t next = *game;
    game_2048_move_result_t next_result = { 0 };

    for (uint8_t line = 0; line < GAME_2048_SIDE; ++line) {
        uint8_t compacted[GAME_2048_SIDE] = { 0 };
        uint8_t output[GAME_2048_SIDE] = { 0 };
        uint8_t compacted_count = 0;

        for (uint8_t offset = 0; offset < GAME_2048_SIDE; ++offset) {
            const uint8_t index = cell_index(direction, line, offset);
            if (next.cells[index]) {
                compacted[compacted_count++] = next.cells[index];
            }
        }

        uint8_t output_count = 0;
        for (uint8_t input = 0; input < compacted_count;) {
            uint8_t exponent = compacted[input];
            if (input + 1 < compacted_count && exponent == compacted[input + 1] && exponent < GAME_2048_MAX_EXPONENT) {
                ++exponent;
                output[output_count] = exponent;
                add_score(&next.score, exponent);
                next_result.merged_mask |= UINT16_C(1) << cell_index(direction, line, output_count);
                next_result.reached_2048 |= exponent == GAME_2048_WIN_EXPONENT;
                input += 2;
            } else {
                output[output_count] = exponent;
                ++input;
            }
            ++output_count;
        }

        for (uint8_t offset = 0; offset < GAME_2048_SIDE; ++offset) {
            next.cells[cell_index(direction, line, offset)] = output[offset];
        }
    }

    next_result.changed = memcmp(game->cells, next.cells, sizeof(game->cells)) != 0;
    if (!next_result.changed) {
        next_result.game_over = game_2048_is_game_over(game);
        *result = next_result;
        return true;
    }

    if (!spawn_tile(&next, random_fn, random_ctx, &next_result.spawned_mask)) {
        return false;
    }
    next_result.game_over = game_2048_is_game_over(&next);
    *game = next;
    *result = next_result;
    return true;
}
