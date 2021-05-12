#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk * chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    chunk->lines_count = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk * chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
        chunk->lines_count = GROW_ARRAY(char, chunk->lines_count, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    // chunk->lines[chunk->count] = line;
    addLine(chunk, line);
    chunk->count++;
}

void addLine(Chunk * chunk, int line) 
{
    static int lines_cursor = 0;
    static int line_counts_cursor = 0;

    if (chunk->lines[lines_cursor] == line) {
        chunk->lines_count[line_counts_cursor] += 1;
    } else {
        chunk->lines[++lines_cursor] = line;
        chunk->lines_count[++line_counts_cursor] = 1;
    }
}

int getLine(Chunk * chunk, int index)
{
    int running_count = 0;
    int cursor = 0;
    while (running_count <= index) {
        running_count += chunk->lines_count[++cursor]; 
    }
    return chunk->lines[cursor];
}

int addConstant(Chunk * chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void freeChunk(Chunk * chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}
