#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    char* lines_count;
    ValueArray constants;
} Chunk;

void initChunk(Chunk * chunk);
void writeChunk(Chunk * chunk, uint8_t byte, int line);
void addLine(Chunk * chunk, int line);
int getLine(Chunk * chunk, int index);
void freeChunk(Chunk * chunk);
int addConstant(Chunk * chunk, Value value);

#endif
