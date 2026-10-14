#ifndef HTTP_H
#define HTTP_H

extern void test_network_http_get(void);
extern void test_network_http_get_nonblocking(void);
extern void test_network_http_post(void);
extern void test_network_http_post_large(void);
extern void test_network_json_query_large(void);
extern void test_network_http_headers(void);
extern void test_network_http_channel_mode(void);
extern void test_network_json(void);
extern void test_network_http_put_plain(void);
extern void test_network_http_put_delete(void);

#endif /* HTTP_H */
