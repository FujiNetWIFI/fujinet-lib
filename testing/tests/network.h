#ifndef NETWORK_H
#define NETWORK_H

extern void test_network_init(void);
extern void test_network_write(void);
extern void test_network_write_large(void);
extern void test_network_status_after_close(void);
extern void test_network_status_after_failed_open(void);
extern void test_network_set_eol(void);
extern void test_network_unit(void);
extern void test_network_error_path(void);
extern void test_multiple_network_devices(void);
extern void test_multiple_telnet_devices(void);
extern void test_tcp_peer_close(void);
extern void test_telnet_peer_close(void);
extern void test_network_open_no_n_prefix(void);

#endif /* NETWORK_H */
