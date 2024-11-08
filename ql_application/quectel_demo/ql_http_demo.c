/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-05     Bernard      the first version
 * 2013-06-10     Bernard      fix the slow speed issue when download file.
 * 2015-11-14     aozima       add content_length_remainder.
 * 2017-12-23     aozima       update gethostbyname to getaddrinfo.
 * 2018-01-04     aozima       add ipv6 address support.
 * 2018-07-26     chenyong     modify log information
 * 2018-08-07     chenyong     modify header processing
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_wlan.h"
#include "ql_https.h"


#define HTTP_POST_DEMO 1
#define HTTP_GET_DEMO 0
#define HTTP_SHARD_DOWNLOAD 0


const char *post_data = "RT-Thread is an open source IoT operating system from China!";

#if HTTP_SHARD_DOWNLOAD

#define GET_LOCAL_URI                  "http://www.rt-thread.com/service/rt-thread.txt"
#define CHARACTER_LENGTH 60

/* handle function, you can store data and so on */
static int shard_download_handle(char *buffer, size_t length)
{
    int outindex, inindex = 0;
    int boundary;

    /* print the receive data */
    for (outindex = 0; outindex < length; outindex = outindex + inindex)
    {
        char print_buffer[CHARACTER_LENGTH + 1] = {0};
        char *point = NULL;
        point = print_buffer;

        if(length - outindex > CHARACTER_LENGTH)
        {
            boundary = CHARACTER_LENGTH;
        }
        else
        {
            boundary = length - outindex;
        }

        for (inindex = 0; inindex < boundary; inindex++)
        {
            *point++ = buffer[outindex + inindex];
        }
        *point = 0;
        ql_http_log("%04d - %04d: %s\n", outindex, outindex + boundary - 1, print_buffer);
    }

    /* release this buffer if we have handled data */
    web_free(buffer);

    return 0;
}

int webclient_shard_download_test(char *uri, int size,http_call_back fn)
{
    struct webclient_session* session = NULL;
    int result = 0;
    int length = 0;
    int usage_flag = 0;
    char *url=NULL;
    if(size==0)
    {
        size =200;
    }
    if(uri == NULL)
    {
        url = web_strdup(GET_LOCAL_URI);
    }
    else
    {
            url=web_strdup(uri);
    }


    /* sometime, the header bufsz can set more smaller */
    session = webclient_session_create(WEBCLIENT_HEADER_BUFSZ / 4);
    if (session == NULL)
    {
        result = -WEBCLIENT_NOMEM;
        goto __exit;
    }

    /* get the real data length */
    webclient_shard_head_function(session, url, &length);

    /* register the handle function, you can handle data in the function */
    webclient_register_shard_position_function(session, fn);

    /* the "memory size" that you can provide in the project and url */
    result = webclient_shard_position_function(session, url, 0, length, size);
    if(result != WEBCLIENT_OK)
    {
        ql_http_log("web shard download, test failed!\n");
    }

    /* clear the handle function */
    webclient_register_shard_position_function(session, fn);

__exit:
    if (url)
    {
        web_free(url);
    }

    if (session)
    {
        webclient_close(session);
        session = NULL;
    }

    return result;
}


#endif



#if HTTP_POST_DEMO

#define POST_RESP_BUFSZ                1024
#define POST_HEADER_BUFSZ              1024


void post_cb(char *data,size_t len)
{
    int index = 0;
    ql_http_log("webclient send post request by simplify request interface.\n");
    ql_http_log("webclient post response data: \n");
    for (index = 0; index < len; index++)
    {
        ql_http_log("%c", data[index]);
    }

}

/* send HTTP POST request by common request interface, it used to receive longer data */
static int webclient_post_comm(const char *uri, const void *post_data, size_t data_len,http_call_back fn)
{
    struct webclient_session* session = NULL;
    unsigned char *buffer = NULL;
    int index, ret = 0;
    int bytes_read, resp_status;

    buffer = (unsigned char *) web_malloc(POST_RESP_BUFSZ);
    if (buffer == NULL)
    {
        ql_http_log("no memory for receive response buffer.\n");
        ret = -WEBCLIENT_NOMEM;
        goto __exit;
    }

    /* create webclient session and set header response size */
    session = webclient_session_create(POST_HEADER_BUFSZ);
    if (session == NULL)
    {
        ret = -WEBCLIENT_NOMEM;
        goto __exit;
    }

    /* build header for upload */
    webclient_request_header_add(&session->header->buffer, "Content-Length: %d\r\n", strlen(post_data));
    webclient_request_header_add(&session->header->buffer, "Content-Type: application/json\r\n");

    /* send POST request by default header */
    if ((resp_status = webclient_post(session, uri, post_data, data_len)) != 200)
    {
        ql_http_log("webclient POST request failed, response(%d) error.\n", resp_status);
        ret = -WEBCLIENT_ERROR;
        goto __exit;
    }

    ql_http_log("webclient post response data: \n");
    do
    {
        bytes_read = webclient_read(session, buffer, POST_RESP_BUFSZ);
        if (bytes_read <= 0)
        {
            break;
        }

        // for (index = 0; index < bytes_read; index++)
        // {
        //     ql_http_log("%c", buffer[index]);
        // }
        fn(buffer,bytes_read);
    } while (1);

    ql_http_log("\n");

__exit:
    if (session)
    {
        webclient_close(session);
        session = NULL;
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}

static int webclient_post_smpl(const char *uri, const char *post_data, size_t data_len,http_call_back fn)
{
    char *response = NULL;
    char *header = NULL;
    size_t resp_len = 0;

    webclient_request_header_add(&header, "Content-Length: %d\r\n", strlen(post_data));
    webclient_request_header_add(&header, "Content-Type: application/json\r\n");

    if (webclient_request(uri, header, post_data, data_len, (void **)&response, &resp_len,fn) < 0)
    {
        ql_http_log("webclient send post request failed.");
        web_free(header);
        return -WEBCLIENT_ERROR;
    }
    if (header)
    {
        web_free(header);
    }

    if (response)
    {
        web_free(response);
    }
    return 0;
}




#endif


#if HTTP_GET_DEMO

#define GET_HEADER_BUFSZ               1024
#define GET_RESP_BUFSZ                 1024


void get_cb(char *data,size_t len)
{
    int index = 0;
    ql_http_log("webclient send get request by simplify request interface.\n");
    ql_http_log("webclient get response data: \n");
    for (index = 0; index < len; index++)
    {
        ql_http_log("%c", data[index]);
    }

}

/* send HTTP GET request by simplify request interface, it used to received shorter data */
static int webclient_get_smpl(const char *uri,http_call_back fn)
{
    char *response = NULL;
    size_t resp_len = 0;
    int index;

    if (webclient_request(uri, NULL, NULL, 0, (void **)&response, &resp_len,fn) < 0)
    {
        ql_http_log("webclient send get request failed.");
        return -WEBCLIENT_ERROR;
    }

    ql_http_log("webclient send get request by simplify request interface.\n");
    ql_http_log("webclient get response data: \n");
    for (index = 0; index < strlen(response); index++)
    {
        ql_http_log("%c", response[index]);
    }
    ql_http_log("\n");

    if (response)
    {
        web_free(response);
    }

    return 0;
}


/* send HTTP GET request by common request interface, it used to receive longer data */
static int webclient_get_comm(const char *uri,http_call_back fn)
{
    struct webclient_session* session = NULL;
    unsigned char *buffer = NULL;
    int index, ret = 0;
    int bytes_read, resp_status;
    int content_length = -1;

    buffer = (unsigned char *) web_malloc(GET_RESP_BUFSZ);
    if (buffer == NULL)
    {
        ql_http_log("no memory for receive buffer.\n");
        ret = -1;
        goto __exit;

    }

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (session == NULL)
    {
        ret = -1;
        goto __exit;
    }

    /* send GET request by default header */
    if ((resp_status = webclient_get(session, uri)) != 200)
    {
        ql_http_log("webclient GET request failed, response(%d) error.\n", resp_status);
        ret = -WEBCLIENT_ERROR;
        goto __exit;
    }

    ql_http_log("webclient get response data: \n");

    content_length = webclient_content_length_get(session);
    if (content_length < 0)
    {
        ql_http_log("webclient GET request type is chunked.\n");
        do
        {
            bytes_read = webclient_read(session, (void *)buffer, GET_RESP_BUFSZ);
            if (bytes_read <= 0)
            {
                break;
            }

            // for (index = 0; index < bytes_read; index++)
            // {
            //     ql_http_log("%c", buffer[index]);
            // }
            fn(buffer,bytes_read);
        } while (1);

        ql_http_log("\n");
    }
    else
    {
        int content_pos = 0;

        do
        {
            bytes_read = webclient_read(session, (void *)buffer,
                    content_length - content_pos > GET_RESP_BUFSZ ?
                            GET_RESP_BUFSZ : content_length - content_pos);
            if (bytes_read <= 0)
            {
                break;
            }

            // for (index = 0; index < bytes_read; index++)
            // {
            //     ql_http_log("%c", buffer[index]);
            // }
            fn(buffer,bytes_read);

            content_pos += bytes_read;
        } while (content_pos < content_length);

        ql_http_log("\n");
    }

__exit:
    if (session)
    {
        webclient_close(session);
        session = NULL;
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}

#endif


static void ql_https_demo_thread(void * arg)
{

       ql_demo_sta_app_init("quectel", "12345678");
   do
   {
      ql_rtos_task_sleep_ms(1000);
      ql_http_log("connectting");
   } while (0 == sta_ip_is_start());
   ql_rtos_task_sleep_ms(2000);

#if HTTP_POST_DEMO
    webclient_post_comm("http://www.rt-thread.com/service/echo",post_data,strlen(post_data),post_cb);
#endif

#if HTTP_SHARD_DOWNLOAD
    webclient_shard_download_test("http://www.rt-thread.com/service/rt-thread.txt",200,shard_download_handle);

#endif

#if HTTP_GET_DEMO
webclient_get_smpl("http://www.rt-thread.com/service/rt-thread.txt",get_cb);

#endif

while (1)
{
    ql_rtos_task_sleep_ms(1000);
}
    
}



ql_task_t http_test_thread_handle = NULL;


void ql_https_demo_thread_creat(void)
{
    int ret;
    ret = ql_rtos_task_create(&http_test_thread_handle,
                              (unsigned short)1024*3,
                              6,
                              "http_test",
                              ql_https_demo_thread,
                              0);

    if (ret != QL_OSI_SUCCESS)
    {
        ql_http_log("Error: Failed to create http test thread: %d\r\n", ret);
        return;
    }

    return;
}