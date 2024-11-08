/*
 * SPDX-FileCopyrightText: 2020-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _DYNAMIC_IMPL_H_
#define _DYNAMIC_IMPL_H_

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/platform.h"

#include <stdio.h>  
#include <stdlib.h>

#define CONFIG_MBEDTLS_DYNAMIC_BUFFER_DEBUG 0
#define CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA 
#define CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT


#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({          \
        const typeof( ((type *)0)->member ) *__mptr = (const typeof( ((type *)0)->member ) *)(ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#ifndef __containerof
#define __containerof(ptr, type, member) container_of(ptr, type, member)
#endif

#define TRACE_ERR_PRINTF bk_printf

#if CONFIG_MBEDTLS_DYNAMIC_BUFFER_DEBUG

#define TRACE_PRINTF dbg

#define TRACE_CHECK(_fn, _state) \
({ \
    dbg("%d " _state " to do \"%s\"\r\n", __LINE__, # _fn); \
})

#define CHECK_OK(_fn) \
({ \
    int _ret; \
 \
    TRACE_CHECK(_fn, "state"); \
 \
    if ((_ret = _fn) != 0) { \
        bk_printf("\"%s\" result is %d\r\n", # _fn, -_ret); \
        TRACE_CHECK(_fn, "fail"); \
        return _ret; \
    } \
 \
    TRACE_CHECK(_fn, "end"); \
 \
})

#else

#define TRACE_PRINTF(...) do {} while (0)
#define CHECK_OK(_fn) \
({ \
    int _ret; \
 \
 \
    if ((_ret = _fn) != 0) { \
        return _ret; \
    } \
 \
 \
})

#define TRACE_CHECK(_fn, _state) do {} while (0)

#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef enum {
    ESP_MBEDTLS_SSL_BUF_CACHED,
    ESP_MBEDTLS_SSL_BUF_NO_CACHED,
} esp_mbedtls_ssl_buf_states;

struct esp_mbedtls_ssl_buf {
    esp_mbedtls_ssl_buf_states state;
    unsigned int len;
    unsigned char buf[];
};

#define SSL_BUF_HEAD_OFFSET_SIZE ((int)offsetof(struct esp_mbedtls_ssl_buf, buf))

void esp_mbedtls_free_buf(unsigned char *buf);

int esp_mbedtls_setup_tx_buffer(mbedtls_ssl_context *ssl);

void esp_mbedtls_setup_rx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_reset_add_tx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_reset_add_rx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_reset_free_tx_buffer(mbedtls_ssl_context *ssl);

void esp_mbedtls_reset_free_rx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_add_tx_buffer(mbedtls_ssl_context *ssl, size_t buffer_len);

int esp_mbedtls_add_rx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_free_tx_buffer(mbedtls_ssl_context *ssl);

int esp_mbedtls_free_rx_buffer(mbedtls_ssl_context *ssl);

size_t esp_mbedtls_get_crt_size(mbedtls_x509_crt *cert, size_t *num);

#ifdef CONFIG_MBEDTLS_DYNAMIC_FREE_CONFIG_DATA
void esp_mbedtls_free_dhm(mbedtls_ssl_context *ssl);

void esp_mbedtls_free_keycert(mbedtls_ssl_context *ssl);

void esp_mbedtls_free_keycert_cert(mbedtls_ssl_context *ssl);

void esp_mbedtls_free_keycert_key(mbedtls_ssl_context *ssl);
#endif

#ifdef CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT
void esp_mbedtls_free_cacert(mbedtls_ssl_context *ssl);
#endif

#endif /* _DYNAMIC_IMPL_H_ */
