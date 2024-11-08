#include "ql_include.h"

static int test_is_started = 0;

extern int mqtt_client_session_deinit(mqtt_client_session* cs);
extern void extended_app_waiting_for_launch(void);

static mqtt_client_session _mqttc_ses = {0};
struct sub_msg_handlers sub_topic[2] = {0};

char *mqt_strdup(const char *s)
{
    char *res;
    size_t len;

    if (s == NULL)
        return NULL;

    len = os_strlen(s);
    res = os_malloc(len + 1);
    if (res)
        os_memcpy(res, s, len + 1);

    return res;
}

static void mqtt_sub_callback(mqtt_client_session *c, MessageData *msg_data)
{
	TMQTT_LOG("Message arrived on topic %.*s: %.*s\r\n", msg_data->topicName->lenstring.len,
		msg_data->topicName->lenstring.data,
		msg_data->message->payloadlen, msg_data->message->payload);
    return;
}

static void mqtt_default_sub_callback(mqtt_client_session *c, MessageData *msg_data)
{
	TMQTT_LOG("default Message arrived on topic %.*s: %.*s\n", msg_data->topicName->lenstring.len,
		msg_data->topicName->lenstring.data,
		msg_data->message->payloadlen, msg_data->message->payload);
    return;
}

static void mqtt_connect_callback(mqtt_client_session *c)
{
    TMQTT_LOG("inter mqtt_connect_callback!\r\n");
}

static void mqtt_online_callback(mqtt_client_session *c)
{
    TMQTT_LOG("inter mqtt_online_callback!\r\n");
}

static void mqtt_offline_callback(mqtt_client_session *c,MQTT_EVNT_T ev)
{
    TMQTT_LOG("inter mqtt_offline_callback event %d\r\n",ev);
}

static void mqtt_notice_event_callback(mqtt_client_session *c,MQTT_EVNT_T ev)
{
    TMQTT_LOG("mqtt nitice event %d\r\n",ev);
}


static void mqtt_thread_pub(void *parameter)
{
	unsigned int pub_count = 0;
    char payload[60];

    while (1)
    {
    	snprintf(payload,sizeof(payload), "message number %d", pub_count);
        if (!mqtt_client_publish(&_mqttc_ses, QOS1, "7231s/mqtt_test/7231s", payload))
        {
            ++ pub_count;
			os_printf("pub:\"%s\" succeed\r\n",payload);
			if(pub_count > 50)
			{
				os_printf("mqtt client disconnect\r\n");
				mqtt_client_disconnect(&_mqttc_ses);
				ql_rtos_task_sleep_ms(1000);
				mqtt_net_disconnect(&_mqttc_ses);
				break;
			}
        }
		else
		{
			os_printf("pub:\"%s\" failed\r\n",payload);
        }
		ql_rtos_task_sleep_ms(1000);
    }
	mqtt_client_session_deinit(&_mqttc_ses);
	os_printf("[MQTT]exit\r\n\n");
	test_is_started = 0;
	ql_rtos_task_delete( NULL );
}


static int mqtt_demo_start_connect(char *host)
{
	char* link_name = "broker.mqttdashboard.com";
	int ret;
	
	if(host != NULL)
	{
		link_name = host;
	}
	
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	if(mqtt_client_session_init(&_mqttc_ses) != MQTT_OK)
	{
		goto _exit;
	}
#if 1
	tcp_mqtt_client_api_register( &_mqttc_ses.netport );

	if(mqtt_net_connect(&_mqttc_ses,link_name,1883) != MQTT_OK)
	{
		goto _exit;
	}
#else
	ssl_mqtt_client_api_register( &_mqttc_ses.netport );

	if(mqtt_net_connect(&_mqttc_ses,link_name,8883) != MQTT_OK)
	{
		goto _exit;
	}

#endif
	
	connectData.MQTTVersion = 3;
	connectData.clientID.cstring = "my_client_os72xx_mqtt";
	connectData.username.cstring = "mqtt_user1";
	connectData.password.cstring = "123";
	connectData.keepAliveInterval = 30000;
	connectData.cleansession = 1;

	_mqttc_ses.buf_size = MQTT_DEFUALT_BUF_SIZE;
	_mqttc_ses.buf = os_malloc(_mqttc_ses.buf_size);
	_mqttc_ses.readbuf_size = MQTT_DEFUALT_BUF_SIZE;
	_mqttc_ses.readbuf = os_malloc(_mqttc_ses.readbuf_size);

	if (!(_mqttc_ses.readbuf && _mqttc_ses.buf))
    {
        os_printf("no memory for MQTT client buffer!\n");
        goto _exit;
    }
	
	/* set event callback function */
    _mqttc_ses.connect_callback = mqtt_connect_callback;
    _mqttc_ses.online_callback = mqtt_online_callback;
    _mqttc_ses.offline_callback = mqtt_offline_callback;
	_mqttc_ses.mqtt_notice_cb = mqtt_notice_event_callback;
	
	/* set subscribe table and event callback */
	_mqttc_ses.sub.messageHandlers = sub_topic;
	if(sub_topic[0].topicFilter == NULL)
	{
    	sub_topic[0].topicFilter = mqt_strdup("7231s/mqtt_test_sub/#");
	}
    sub_topic[0].callback = mqtt_sub_callback;
	sub_topic[0].qos = QOS0;
	if(sub_topic[1].topicFilter == NULL)
	{
		sub_topic[1].topicFilter = mqt_strdup("7231s/mqtt_win_sub/windows10");
	}
    sub_topic[1].callback = mqtt_sub_callback;
	sub_topic[1].qos = QOS1;
	_mqttc_ses.sub.sub_topic_num = sizeof(sub_topic)/sizeof(struct sub_msg_handlers);
	_mqttc_ses.defaultMessageHandler =  mqtt_default_sub_callback;
	
	ret = mqtt_client_connect(&_mqttc_ses,&connectData);
	if(MQTT_OK != ret)
	{
		os_printf("[MQTT]connect failed\r\n");
		goto _exit;
	}
	os_printf("[MQTT]connect succeed\r\n");

	return 0;
_exit:
	if(_mqttc_ses.readbuf)
	{
		os_free(_mqttc_ses.readbuf);
	}
	if(_mqttc_ses.buf)
	{
		os_free(_mqttc_ses.buf);
	}
	mqtt_client_session_deinit(&_mqttc_ses);
	return -1;
}

static void mqtt_test_start(int argc, char **argv)
{
	if (test_is_started)
	{
		return;
	}

  //  if(mqtt_demo_start_connect("192.168.19.24") != 0)
	os_printf("url:%s\n" , argv[2]);

    if(mqtt_demo_start_connect(argv[2]) != 0)
	{
		return;
    }

    if (!_mqttc_ses.is_connected)
    {
        os_printf("Waiting for mqtt connection...\n");
		return;
    }

    ql_rtos_task_create(NULL,
					 3 * 1024,
                     THD_EXTENDED_APP_PRIORITY,
                     "MQTT_pub",
                     (beken_thread_function_t)mqtt_thread_pub,
			         NULL);
	
	test_is_started = 1;

    return;
}


static void mqtt_demo_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

	if(argc >= 2)
	{
		if(strcmp(argv[1], "create_echo") == 0)
		{
			///echo_start_tasks(ECHO_STACK_SIZE, ECHO_THD_PRORITY);
		}
		else if(strcmp(argv[1], "con") == 0)
		{
			mqtt_test_start(argc, argv);
		}
		else if(strcmp(argv[1], "stop") == 0)
		{
			
		}
	}

	
}

ql_task_t	mqtt_thread_handle = NULL;
static const struct cli_command mqtt_cli_cmd[] =
{
    {"MQTT", "MQTT", mqtt_demo_command},
};

void mqtt_app_demo_init(void *arg)
{
    ql_rtos_task_sleep_ms(3000);
	cli_register_commands(mqtt_cli_cmd, sizeof(mqtt_cli_cmd) / sizeof(struct cli_command));
	mqtt_core_handler_thread_init();
	tcp_mqtt_client_init();
	os_printf("[MQTT]%s:%d\r\n",__FUNCTION__,__LINE__);
	os_printf("!!!!!!!!!!!wait wifi connect \r\n");
	while (!ql_sta_ip_is_start())
	{
		ql_rtos_task_sleep_ms(1000);
		os_printf("connecting \r\n");
	}
	if( mqtt_thread_handle != NULL ) {
		ql_rtos_task_delete(mqtt_thread_handle);
	}
}


// void user_main(void)
// {
// 	extended_app_waiting_for_launch();
// 	mqtt_app_demo_init();
// }
void ql_mqtt_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&mqtt_thread_handle,
		               (unsigned short)1024*4,
					   THD_EXTENDED_APP_PRIORITY,
					   "mqtt_test",
					   mqtt_app_demo_init,	   
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create mqtt test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( mqtt_thread_handle != NULL ) {
		   ql_rtos_task_delete(mqtt_thread_handle);
	   }

}








