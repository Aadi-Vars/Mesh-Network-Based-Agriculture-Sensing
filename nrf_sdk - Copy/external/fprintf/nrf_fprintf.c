/**
 * Copyright (c) 2017 - 2022, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(NRF_FPRINTF)
#include <stdarg.h>

#include "nrf_assert.h"
#include "nrf_fprintf_format.h"

void nrf_fprintf_buffer_flush(nrf_fprintf_ctx_t * const p_ctx)
{
    ASSERT(p_ctx != NULL);

    if (p_ctx->io_buffer_cnt == 0)
    {
        return;
    }

    p_ctx->fwrite(p_ctx->p_user_ctx,
                  p_ctx->p_io_buffer,
                  p_ctx->io_buffer_cnt);
    p_ctx->io_buffer_cnt = 0;
}

void nrf_fprintf(nrf_fprintf_ctx_t * const p_ctx,
                 char const *              p_fmt,
                                           ...)
{
    ASSERT(p_ctx != NULL);
    ASSERT(p_ctx->fwrite != NULL);
    ASSERT(p_ctx->p_io_buffer != NULL);
    ASSERT(p_ctx->io_buffer_size > 0);

    if (p_fmt == NULL)
    {
        return;
    }

    va_list args = {0};
    va_start(args, p_fmt);

    nrf_fprintf_fmt(p_ctx, p_fmt, &args);

    va_end(args);
}

#endif // NRF_MODULE_ENABLED(NRF_FPRINTF)

