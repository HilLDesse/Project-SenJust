#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "buffer.h"

void copyText(Buffer *buff, int sel_b, int sel_k);
void deleteSelection(Buffer *buff, int sel_b, int sel_k);
void cutText(Buffer *buff, int sel_b, int sel_k);
void pasteText(Buffer *buff);

#endif