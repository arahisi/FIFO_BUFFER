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

#include <stdio.h>
#include "fifo_buffer.h"

static void Default_EnterCS(void const *Context)
{
    (void) Context;
}

static void Default_LeaveCS(void const *Context)
{
    (void) Context;
}

static const CS_lib_t Default_FIFO_Buffer_CS =
{ NULL, Default_EnterCS, Default_LeaveCS };

void FIFO_Buffer_Init(FIFO_Buffer *fifo, int size, void *buffer, const CS_lib_t *const cs)
{
    if (NULL == cs)
    {
        fifo->CS = &Default_FIFO_Buffer_CS;
    }
    else
    {
        fifo->CS = cs;
    }
    fifo->CS->Enter (fifo->CS->Handle);
    fifo->Buffer = (uint8_t*) buffer;
    fifo->Size = size;
    fifo->Length = 0;
    fifo->Read = 0;
    fifo->Write = 0;
    fifo->CS->Leave (fifo->CS->Handle);
}

void FIFO_Buffer_Dispose(FIFO_Buffer *fifo)
{
    fifo->CS->Enter (fifo->CS->Handle);
    fifo->Length = 0;
    fifo->Read = 0;
    fifo->Write = 0;
    fifo->CS->Leave (fifo->CS->Handle);
}

int FIFO_Buffer_Size(const FIFO_Buffer *fifo)
{
    return fifo->Size;
}

int FIFO_Buffer_Length(const FIFO_Buffer *fifo)
{
    return fifo->Length;
}

int FIFO_Buffer_IPutc(FIFO_Buffer *fifo, uint8_t data)
{
    int ret;
    if (fifo->Length < fifo->Size)
    {
        fifo->Buffer[fifo->Write] = data;
        fifo->Write++;
        if (fifo->Write == fifo->Size)
        {
            fifo->Write = 0;
        }
        fifo->Length++;
        ret = data;
    }
    else
    {
        ret = FIFO_BUFFER_FULL;
    }
    return ret;
}

int FIFO_Buffer_Putc(FIFO_Buffer *fifo, uint8_t data)
{
    int ret;
    fifo->CS->Enter (fifo->CS->Handle);
    ret = FIFO_Buffer_IPutc (fifo, data);
    fifo->CS->Leave (fifo->CS->Handle);
    return ret;
}

void FIFO_Buffer_Puts(FIFO_Buffer *fifo, const uint8_t *data)
{
    while ('\0' != *data)
    {
        if (FIFO_BUFFER_FULL != FIFO_Buffer_Putc (fifo, *data))
        {
            data++;
        }
    }
}

int FIFO_Buffer_IGetc(FIFO_Buffer *fifo)
{
    int ret;
    if (fifo->Length)
    {
        ret = fifo->Buffer[fifo->Read];
        fifo->Read++;
        if (fifo->Read == fifo->Size)
        {
            fifo->Read = 0;
        }
        fifo->Length--;
    }
    else
    {
        ret = FIFO_BUFFER_EMPTY;
    }
    return ret;
}

int FIFO_Buffer_Getc(FIFO_Buffer *fifo)
{
    int ret;
    fifo->CS->Enter (fifo->CS->Handle);
    ret = FIFO_Buffer_IGetc (fifo);
    fifo->CS->Leave (fifo->CS->Handle);
    return ret;
}

int FIFO_Buffer_ITest(FIFO_Buffer *fifo)
{
    int ret;
    if (fifo->Length)
    {
        ret = fifo->Buffer[fifo->Read];
    }
    else
    {
        ret = FIFO_BUFFER_EMPTY;
    }
    return ret;
}

int FIFO_Buffer_Test(FIFO_Buffer *fifo)
{
    int ret;
    fifo->CS->Enter (fifo->CS->Handle);
    ret = FIFO_Buffer_ITest (fifo);
    fifo->CS->Leave (fifo->CS->Handle);
    return ret;
}

int FIFO_Buffer_IRead(FIFO_Buffer *fifo, int size, void *in)
{
    int ret = 0;
    int idx = 0;
    uint8_t *byte = (uint8_t*) in;
    while (size && ((ret = FIFO_Buffer_IGetc (fifo)) != FIFO_BUFFER_EMPTY))
    {
        size--;
        byte[idx] = (uint8_t) ret;
        idx++;
    }
    return idx;
}

int FIFO_Buffer_Read(FIFO_Buffer *fifo, int size, void *in)
{
    int ret;
    fifo->CS->Enter (fifo->CS->Handle);
    ret = FIFO_Buffer_IRead (fifo, size, in);
    fifo->CS->Leave (fifo->CS->Handle);
    return ret;
}

int FIFO_Buffer_IWrite(FIFO_Buffer *fifo, int size, const void *out)
{
    int idx = 0;
    uint8_t *byte = (uint8_t*) out;
    while (size && (FIFO_Buffer_IPutc (fifo, byte[idx]) != FIFO_BUFFER_FULL))
    {
        size--;
        idx++;
    }
    return idx;
}

int FIFO_Buffer_Write(FIFO_Buffer *fifo, int size, const void *out)
{
    int ret;
    fifo->CS->Enter (fifo->CS->Handle);
    ret = FIFO_Buffer_IWrite (fifo, size, out);
    fifo->CS->Leave (fifo->CS->Handle);
    return ret;
}
