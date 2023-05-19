#ifndef _HTTP_SERVER_IO_H
#define _HTTP_SERVER_IO_H

typedef void (*http_post_callback_t)(char *data, int len);
typedef void (*http_get_callback_t)(void);


void start_webserver(void);
void stop_webserver(void);
void http_set_callback_dht11(void *cb);
void http_set_callback_switch(void *cb);
#endif