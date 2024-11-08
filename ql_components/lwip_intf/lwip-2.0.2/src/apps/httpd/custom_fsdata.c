#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "lwip/def.h"
#include "fsdata.h"
#include "lwip_netif_address.h"
#include "net.h"
// #include "customer_confg.h"
// #include "user_flash.h"


#define CUSTOM_FSDATA_STA_CONNECTED "Connected"
#define CUSTOM_FSDATA_STA_DISCONNECTED "Disconnected"

#define  CUSTOM_FSDATA_INDEX_HTML_FILE_NAME "/index.html"

#define CUSTOM_FSDATA_INDEX_HTML_HEADER \
"HTTP/1.0 200 OK\r\n" \
"Server: lwIP/1.3.1\r\n" \
"Content-type: text/html\r\n\r\n"

# if 0
#define CUSTOM_FSDATA_INDEX_HTML_FILE_FMT "%s<!DOCTYPE html>\r\n<html> \r\n"\
"<head>\r\n"\
"    <meta charset=\"UTF-8\">\r\n"\
"    <title>FC41D smart config</title>\r\n"\
"    <script>\r\n"\
"        function serializeElement(element) {\r\n"\
"            var method = element.tagName.toLowerCase();\r\n"\
"            var parameter;\r\n"\
"            if (method == 'select') {\r\n"\
"                parameter = [element.name, element.value];\r\n"\
"            }\r\n"\
"            switch (element.type.toLowerCase()) {\r\n"\
"                case 'submit':\r\n"\
"                case 'hidden':\r\n"\
"                case 'password':\r\n"\
"                case 'text':\r\n"\
"                case 'number':\r\n"\
"                case 'date':\r\n"\
"                case 'textarea':\r\n"\
"                    parameter = [element.name, element.value];\r\n"\
"                    break;\r\n"\
"                case 'checkbox':\r\n"\
"                case 'radio':\r\n"\
"                    if (element.checked) {\r\n"\
"                        parameter = [element.name, element.value];\r\n"\
"                    }\r\n"\
"                    break;\r\n"\
"            }\r\n"\
"            if (parameter) {\r\n"\
"                var key = encodeURIComponent(parameter[0]);\r\n"\
"                if (key.length == 0)\r\n"\
"                    return;\r\n"\
"                if (parameter[1].constructor != Array)\r\n"\
"                    parameter[1] = [parameter[1]];\r\n"\
"                var values = parameter[1];\r\n"\
"                var results = [];\r\n"\
"                for (var i = 0; i < values.length; i++) {\r\n"\
"                    results.push(key + '=' + encodeURIComponent(values[i]));\r\n"\
"                }\r\n"\
"                return results.join('&');\r\n"\
"            }\r\n"\
"        }\r\n"\
"        function serializeForm(form) {\r\n"\
"            var elements = form.elements;\r\n"\
"            var queryComponents = new Array();\r\n"\
"            for (var i = 0; i < elements.length; i++) {\r\n"\
"                var queryComponent = serializeElement(elements[i]);\r\n"\
"                if (queryComponent) {\r\n"\
"                    queryComponents.push(queryComponent);\r\n"\
"                }\r\n"\
"            }\r\n"\
"            return queryComponents.join('&');\r\n"\
"        }\r\n"\
"        function getXmlHttpObj() {\r\n"\
"            var xmlhttp = null;\r\n"\
"            if (window.XMLHttpRequest) {\r\n"\
"                xmlhttp = new XMLHttpRequest();\r\n"\
"            } else {\r\n"\
"                xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\r\n"\
"            }\r\n"\
"            return xmlhttp;\r\n"\
"        }\r\n"\
"        function ajaxPost(formId) {\r\n"\
"            var xmlhttp = getXmlHttpObj();\r\n"\
"            if (xmlhttp != null) {\r\n"\
"                xmlhttp.onreadystatechange = function parasing() {\r\n"\
"                    if (xmlhttp.readyState == 4 && (xmlhttp.status == 200 || xmlhttp.status == 204)) {\r\n"\
"                        location.href = '/';\r\n"\
"                    }\r\n"\
"                }\r\n"\
"                var form = document.getElementById(formId);\r\n"\
"                var formatData = serializeForm(form);\r\n"\
"                var url = form.action;\r\n"\
"                xmlhttp.open('POST', url, true);\r\n"\
"                xmlhttp.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\r\n"\
"                xmlhttp.send(formatData);\r\n"\
"            }\r\n"\
"        }\r\n"\
"    </script>\r\n"\
"</head>\r\n"\
"<body bgcolor=\"white\" text=\"black\">\r\n"\
"    <h1>Network Connection Setting</h1>\r\n"\
"    <form id=\"networkForm\" action=\"index.html\" method=\"post\">\r\n"\
"        SSID:<br />\r\n"\
"        <input type=\"text\" name=\"ssid\" placeholder=\"%s\" />&nbsp;&nbsp;<span>%s</span><br />\r\n"\
"        PassWord:<br />\r\n"\
"        <input type=\"password\" name=\"pwd\" /><br /><br />\r\n"\
"        <input type=\"button\" value=\"Save\" onclick=\"ajaxPost('networkForm')\" />\r\n"\
"    </form>\r\n"\
"    <form action=\"index.html\" method=\"post\">\r\n"\
"        Uart Baud Rate:<br />\r\n"\
"        <input type=\"number\" name=\"uart_band\" min=\"4800\" max=\"921600\" placeholder=\"%d\" /><br /><br />\r\n"\
"        <input type=\"submit\" value=\"Save\" />\r\n"\
"    </form>\r\n"\
"    <br />\r\n"\
"    <button onclick=\"location.href='/'\">Refresh</button>\r\n"\
"</body>\r\n"\
"</html>"
#endif

#define CUSTOM_FSDATA_INDEX_HTML_FILE_FMT "%s<!DOCTYPE html>\r\n<html><head><meta charset=\"UTF-8\"><title>FC41D smart config</title></head><body bgcolor=\"white\" text=\"black\">" \
    "<h1>Network Connection Setting</h1><form action=\"index.html\" method=\"post\">   SSID:<br /><input type=\"text\" name=\"ssid\" placeholder=\"%s\" />" \
    "&nbsp;&nbsp;<span>%s</span><br />PassWord:<br /><input type=\"password\" name=\"pwd\"/>" \
    "<br /><br /><input type=\"submit\" value=\"Save\" /></form><form action=\"index.html\" method=\"post\">   Uart Baud Rate:<br />" \
    "<input type=\"number\" name=\"uart_band\"  min=\"4800\" max=\"2000000\" placeholder=\"%d\" />" \
    "<br /><br /><input type=\"submit\" value=\"Save\" /></form><br /><button onclick=\"location.href='/'\">Refresh</button></body></html>"

extern uint32_t sta_ip_is_start(void);
extern void *net_get_sta_handle(void);

extern uint8_t wifi_config_ssid[32];
bk_uart_config_t web_uart_config;


static struct fsdata_file g_file_index_html[] = {{
    NULL,
    NULL,
    NULL,
    0,
    1,
}};


static int custom_fsdata_sta_state(void)
{
    struct wlan_ip_config local_ip = {0};

    if (sta_ip_is_start())
    {
        net_get_if_addr(&local_ip, net_get_sta_handle());
    }

    if (local_ip.ipv4.address == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



struct fsdata_file * custom_fsdata_gen_index_html(void)
{
    char *file_data = NULL;
    int file_data_len = 0;
    char *sta_state_data = NULL;
    // uint32 uart_band = 9600;
    // uint8 ssid[32] = {0};
    struct fsdata_file *f = NULL;

    if (custom_fsdata_sta_state() == 1)
    {
        sta_state_data = CUSTOM_FSDATA_STA_CONNECTED;
    }
    else
    {
        sta_state_data = CUSTOM_FSDATA_STA_DISCONNECTED;
    }

    // usr_config_read_id_flash(&uart_band, USERID_BAND);
    // usr_config_read_id_flash(ssid, USERID_APSSID);

    file_data_len = strlen((char *)CUSTOM_FSDATA_INDEX_HTML_HEADER) + strlen((char *)CUSTOM_FSDATA_INDEX_HTML_FILE_FMT) + 10 /* for uart band */ + strlen((char *)wifi_config_ssid) + 1 /* \0 */;
    file_data = malloc(file_data_len);

        if (file_data == NULL)
    {
        // no mem
        return NULL;
    }

    memset(file_data, 0, file_data_len);

    file_data_len = snprintf(file_data, file_data_len - 1, (char *)CUSTOM_FSDATA_INDEX_HTML_FILE_FMT, (char *)CUSTOM_FSDATA_INDEX_HTML_HEADER, (char *)wifi_config_ssid, sta_state_data, web_uart_config.baud_rate);
    f = g_file_index_html;

    if (f->data)
    {
        free((void *)f->data);
        f->data = NULL;
    }
    f->flags = 1;
    f->data = (const unsigned char *)file_data;
    f->len = file_data_len;

    f->next = NULL;
    f->name = (const unsigned char *)CUSTOM_FSDATA_INDEX_HTML_FILE_NAME;

    os_printf("file_data_len:%d, data:\r\n", file_data_len);
    // custom_printf_string_buffer(file_data, file_data_len);

    return f;
}

