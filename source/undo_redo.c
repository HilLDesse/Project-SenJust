#include <string.h>
#include "../header/undo_redo.h"

static Buffer undoStack[MAX_HISTORY];
static Buffer redoStack[MAX_HISTORY];
static int undoTop = -1;
static int redoTop = -1;

void  initHistory() {
    undoTop = -1;
    redoTop = -1;
}

void recordState(Buffer *buff) {
    if (undoTop < MAX_HISTORY - 1) {
        undoTop++;
        memcpy(&undoStack[undoTop], buff, sizeof(Buffer));
        redoTop = -1;
    } 
    else
    {
        for(int i = 0; i < MAX_HISTORY - 1; i++) {
            memcpy(&undoStack[i], &undoStack[i + 1], sizeof(Buffer));
        }
        memcpy(&undoStack[MAX_HISTORY - 1], buff, sizeof(Buffer));
        redoTop = -1;
    }
}

void undo(Buffer *buff) {
    if (undoTop >= 0) {
        if (redoTop < MAX_HISTORY - 1) {
            redoTop++;
            memcpy(&redoStack[redoTop], buff, sizeof(Buffer));
        }
        memcpy(buff, &undoStack[undoTop], sizeof(Buffer));
        undoTop--;
    }
}

void redo(Buffer *buff) {
    if (redoTop >= 0) {
        if (undoTop < MAX_HISTORY - 1) {
            undoTop++;
            memcpy(&undoStack[undoTop], buff, sizeof(Buffer));
        }
        memcpy(buff, &redoStack[redoTop], sizeof(Buffer));
        redoTop--;
    }
}