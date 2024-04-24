/**
 * @brief   Set channel mode to add headers.
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#define HTTP_CHAN_MODE_BODY 0

extern unsigned char network_http_set_channel_mode(char *devicespec, unsigned char mode);

unsigned char network_http_end_add_headers(char *devicespec) {
    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}
