#ifndef _HTTP_SERVER_IO_H
#define _HTTP_SERVER_IO_H
#include <esp_http_server.h>
typedef void (*http_post_callback_t)(char *data, int len);
typedef void (*http_get_callback_t)(httpd_req_t *req);


void start_webserver(void);
void stop_webserver(void);
void http_set_callback_dht11(void *cb);
void http_set_callback_switch(void *cb);
void http_set_callback_slider(void *cb);
#endif