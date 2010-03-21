/*
 * split.c
 * fsf - 2003-08-02
 * last modified: 2003-08-23
 *
 * Copyright (c) 2003, Frank S. Fejes III
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the author nor the names of contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "lifstat.h"

int split(const char *line, char **cols, int cols_max, char split_char)
{
    int n = 0;
    char *tmp;
    char *index;

    if (! line) {
        return(0);
    }

    tmp = (char *) malloc(strlen(line) + 1);
    if (! tmp) {
        fprintf(stderr, "Cannot allocate memory for split()!\n");
        return(0);
    }
    strcpy(tmp, line);

    for (n = 0; n < cols_max; ++n) {
        /* skip over any number of leading split_chars */
        while (*tmp == split_char) {
            ++tmp;
        }

        /* find the split_char and, if found, null-terminate the tmp string */
        index = strchr(tmp, split_char);
        if (index) {
            *index = 0;
        }

        cols[n] = (char *) malloc(strlen(tmp) + 1);
        if (! cols[n]) {
            fprintf(stderr, "Cannot allocate memory for split()!\n");
            return(n);
        }
        strcpy(cols[n], tmp);
        //printf("%d: %s\n", n, cols[n]);

        if (! index) {
            ++n;
            break;
        }
        tmp = index + 1;
    }
    
    return(n);
}

