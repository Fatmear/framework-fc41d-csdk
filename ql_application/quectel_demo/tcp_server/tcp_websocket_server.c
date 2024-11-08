#include "mongoose.h"
#include "error.h"
#include "rtos_pub.h"

#if CFG_SUPPORT_MONGOOSE
beken_thread_t tcp_thread_handle = NULL;

static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_web_root = ".";

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_READ)
    {
        // bk_printf("recv:[%d]->%s\r\n", c->recv.len, c->recv.buf);
        // mg_send(c, c->recv.buf, c->recv.len); // Echo received data back

        // mg_iobuf_del(&c->recv, 0, c->recv.len); // And discard it
    }
    else if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/websocket"))
        {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm, NULL);
        }
        else if (mg_http_match_uri(hm, "/rest"))
        {
            // Serve REST response
            mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
        }
        else
        {
            // Serve static files
            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
    else if (ev == MG_EV_WS_MSG)
    {
        // Got websocket frame. Received data is wm->data. Echo it back!
        struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
        mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
    }
}

void tcp_server_entry(void *arg)
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                               // Init manager
    mg_listen(&mgr, "tcp://0.0.0.0:1234", cb, &mgr); // Setup listener
    mg_http_listen(&mgr, s_listen_on, cb, NULL);  // Create HTTP listener
    for (;;)
    {
        mg_mgr_poll(&mgr, 500); // Event loop
        rtos_delay_milliseconds(500);
    }
    mg_mgr_free(&mgr); // Cleanup
    tcp_thread_handle = NULL;
    rtos_delete_thread(NULL);
}

static int tcp_server_init(void)
{

    OSStatus err = kNoErr;

    if (tcp_thread_handle == NULL)
    {
        // todo  create new thread;
        err = rtos_create_thread(&tcp_thread_handle,
                                 BEKEN_APPLICATION_PRIORITY,
                                 "tcpwebsocket_demo",
                                 tcp_server_entry,
                                 8*1024,
                                 0);

        if (err != kNoErr)
        {
            return -1;
        }

        
    }

    return 0;
}

void tcp_websocket_server_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (tcp_server_init() != 0)
    {
        bk_printf("tcp server start error \r\n");
    }
    else
    {
        bk_printf("tcp server start success \r\n");
    }
}

#endif
