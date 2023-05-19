/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <esp_http_server.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "http_server_iot.h"
/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */
static const char *TAG = "HTTP_SERVER";
static httpd_handle_t server = NULL;

static http_post_callback_t http_post_switch_callback = NULL;
static http_get_callback_t http_get_dht11_callback = NULL;

// extern const uint8_t index_html_start[] asm("_binary_ronaldo_jpg_start"); // Anh coi nhu gio se thanh mot cai chuoi
// extern const uint8_t index_html_end[] asm("_binary_ronaldo_jpg_end");
extern const uint8_t index_html_start[] asm("_binary_index_html_start"); // Anh coi nhu gio se thanh mot cai chuoi
extern const uint8_t index_html_end[] asm("_binary_index_html_end");


// ========================================================TEST GET & GET TRANG HTML====================================================
/* An HTTP GET handler */
static esp_err_t hello_get_handler1(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    // const char* resp_str = (const char*) "Xin Chaoooooo";
    // httpd_resp_send(req, resp_str, strlen(resp_str));         // Gửi phản hồi xin chào
    httpd_resp_set_type(req, "text/html");                       // Set type cho HTML
    // httpd_resp_set_type(req, "image/jpg");                    // Set type cho image
    httpd_resp_send(req,(const char *)index_html_start, index_html_end - index_html_start);  // Tham số truyền vào: 1 phản hồi; 2 là địa chỉ chuỗi; 3 là độ dài chuỗi
    return ESP_OK;
}

static const httpd_uri_t get_pageHTML_dht11 = {
    .uri       = "/testGET",
    .method    = HTTP_GET,
    .handler   = hello_get_handler1,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};
// ======================================================= GET DATA  ==================================================================
/* An HTTP GET handler */
static esp_err_t hello_get_handler2(httpd_req_t *req)
{
    // const char* resp_str = (const char*) "{\"temperature\" : \"25.10\", \"humidity\" : \"10.25\"}"; // Gửi chuỗi JSON: KEY : value
    // const char* resp_str = (const char*)DHT11_read().temperature;
    http_get_dht11_callback();

    // // Lấy dữ liệu nhiệt độ và độ ẩm từ cảm biến DHT11
    // float T = DHT11_read().temperature;
    // float H = DHT11_read().humidity;
    // T = esp_random() % 50;
    // H = esp_random() % 100;

    // // Tạo đối tượng JSON
    // cJSON *root = cJSON_CreateObject();

    // // Thêm trường dữ liệu nhiệt độ và độ ẩm vào đối tượng JSON
    // // Thêm trường dữ liệu nhiệt độ và độ ẩm vào đối tượng JSON
    // cJSON_AddNumberToObject(root, "temperature", T);
    // cJSON_AddNumberToObject(root, "humidity", H);

    // // Chuyển đổi đối tượng JSON thành chuỗi JSON
    // const char * resp_str = cJSON_Print(root);

    // // Gửi phản hồi HTTP chứa chuỗi JSON
    // httpd_resp_set_type(req, "application/json");
    // httpd_resp_send(req, resp_str, strlen(resp_str)); 
    return ESP_OK;
}

    static const httpd_uri_t get_data_dht11 = {
    .uri       = "/getDATA",
    .method    = HTTP_GET,
    .handler   = hello_get_handler2,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


// ==============================================================================================================================
// /* An HTTP POST handler */
static esp_err_t data_post_handler1(httpd_req_t *req)    // Struct truyền tới nó đã có cả data và len_data
{
    char buf1[100];                                      // Tạo buffer chứa dữ liệu truyền tới
    /* Read the data for the request */
    httpd_req_recv(req, buf1, req->content_len);         // Hàm nhận dữ liệu các tham số truyền vào" 1 là reqq; 2 là buffer để đọc ra, 3 là data đọc ra
    printf("DATA: %s", buf1);                            // In ra Data nhận được 
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_data_dht11 = {
    .uri       = "/post_data_dht11",
    .method    = HTTP_POST,
    .handler   = data_post_handler1,
    .user_ctx  = NULL
};
// =================================================POST BUTTTON=========================================================================
static esp_err_t data_post_handler2(httpd_req_t *req)    // Struct truyền tới nó đã có cả data và len_data
{
                                 // Tạo buffer chứa dữ liệu truyền tới
    /* Read the data for the request */
    char buf2[100];     
    httpd_req_recv(req, buf2, req->content_len);         // Hàm nhận dữ liệu các tham số truyền vào" 1 là reqq; 2 là buffer để đọc ra, 3 là data đọc ra
    printf("DATA: %s\n", buf2);                            // In ra Data nhận được 
    http_post_switch_callback(buf2, req->content_len);
    
    // char* result = strstr(buf2, "1");
    // if (result != NULL) {
    //      // Nếu chuỗi "1" được tìm thấy trong chuỗi nhận được, bật LED
    //      output_io_set_level(2, 1);
    // } else {
    //      // Nếu không tìm thấy chuỗi "1" trong chuỗi nhận được, tắt LED
    //     output_io_set_level(2, 0);
    // }
    
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_state_Switch = {
    .uri       = "/switch1",
    .method    = HTTP_POST,
    .handler   = data_post_handler2,
    .user_ctx  = NULL
};
/* This handler allows the custom error handling functionality to be
 * tested from client side. For that, when a PUT request 0 is sent to
 * URI /ctrl, the /hello and /echo URIs are unregistered and following
 * custom error handler http_404_error_handler() is registered.
 * Afterwards, when /hello or /echo is requested, this custom error
 * handler is invoked which, after sending an error message to client,
 * either closes the underlying socket (when requested URI is /echo)
 * or keeps it open (when requested URI is /hello). This allows the
 * client to infer if the custom error handler is functioning as expected
 * by observing the socket state.
 */
// ==============================================================================================================================
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/dht11", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } else if (strcmp("/echo", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}


// ==============================================================================================================================
// Bắt đầu nghe
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG(); // HTTPD là thư viện nhẹ giúp esp32 chạy đc http
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");       // đăng kí các handler/ resource
        httpd_register_uri_handler(server, &get_pageHTML_dht11);      // clience sẽ GET/POST data vào các resource này
        httpd_register_uri_handler(server, &get_data_dht11);      // clience sẽ GET/POST data vào các resource này
        httpd_register_uri_handler(server, &post_data_dht11);      // clience sẽ GET/POST data vào các resource này 
        httpd_register_uri_handler(server, &post_state_Switch); 
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
        // httpd_register_uri_handler(server, &echo);       // request đúng resource sẽ trả về đc dữ liệu
    }else{
        ESP_LOGI(TAG, "Error starting server!");
    }
}
// ==============================================================================================================================
void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_set_callback_switch(void *cb)
{
    http_post_switch_callback = cb;
}

void http_set_callback_dht11(void *cb)
{
    http_get_dht11_callback = cb;
}

