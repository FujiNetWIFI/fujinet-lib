/**
 * @brief FujiNet Network Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_NETWORK_H
#define FUJINET_NETWORK_H

#include <stdint.h>
#include <stdbool.h>

uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c);
uint8_t network_close(char* devicespec);
uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);
uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len);
uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len);
uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);

#endif /* FUJINET_NETWORK_H */