#ifndef NETWORK_H
#define NETWORK_H

extern void test_network_init(void);
extern void test_network_http_get(void);
extern void test_network_http_get_nonblocking(void);
extern void test_network_http_post(void);
extern void test_network_http_headers(void);
extern void test_network_http_channel_mode(void);
extern void test_network_json(void);
extern void test_network_http_put_delete(void);
extern void test_network_write(void);
extern void test_network_set_eol(void);
extern void test_network_unit(void);
extern void test_network_error_path(void);

#endif /* NETWORK_H */
