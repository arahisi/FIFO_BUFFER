/*
 Copyright 2024, Shoji Yamamoto

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

#ifndef FIFO_BUFFER_H_
#define FIFO_BUFFER_H_

#include "cs_lib.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FIFO_BUFFER_EMPTY (-1)
#define FIFO_BUFFER_FULL  (-1)

typedef struct S_FIFO_Buffer FIFO_Buffer;

struct S_FIFO_Buffer
{
    int Size;
    int Length;
    int Write;
    int Read;
    uint8_t *Buffer;
    const CS_lib_t *CS;
};

extern void FIFO_Buffer_Init(FIFO_Buffer *fifo, int size, void *buffer, const CS_lib_t *const cs);
extern void FIFO_Buffer_Dispose(FIFO_Buffer *fifo);
extern int FIFO_Buffer_Size(const FIFO_Buffer *fifo);
extern int FIFO_Buffer_Length(const FIFO_Buffer *fifo);
extern int FIFO_Buffer_IPutc(FIFO_Buffer *fifo, uint8_t data);
extern int FIFO_Buffer_Putc(FIFO_Buffer *fifo, uint8_t data);
extern void FIFO_Buffer_Puts(FIFO_Buffer *fifo, const uint8_t *data);
extern int FIFO_Buffer_IGetc(FIFO_Buffer *fifo);
extern int FIFO_Buffer_Getc(FIFO_Buffer *fifo);
extern int FIFO_Buffer_ITest(FIFO_Buffer *fifo);
extern int FIFO_Buffer_Test(FIFO_Buffer *fifo);
extern int FIFO_Buffer_IRead(FIFO_Buffer *fifo, int size, void *in);
extern int FIFO_Buffer_Read(FIFO_Buffer *fifo, int size, void *in);
extern int FIFO_Buffer_IWrite(FIFO_Buffer *fifo, int size, const void *out);
extern int FIFO_Buffer_Write(FIFO_Buffer *fifo, int size, const void *out);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_BUFFER_H_ */
