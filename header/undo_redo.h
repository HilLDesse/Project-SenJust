#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include "buffer.h"

#define MAX_HISTORY 50

void initHistory();
void recordState(Buffer *buffer);
void undo(Buffer *buffer);
void redo(Buffer *buffer);

#endif