#ifndef FUJINET_QRCODE_H
#define FUJINET_QRCODE_H

#include <fujinet-bus.h>
#include <fujinet-commands.h>

// QR Code
// ALL RETURN VALUES ARE SUCCESS STATUS VALUE, i.e. true == success

// Error correction level, passed to fuji_qrcode_encode
typedef enum QRCodeEcc
{
    QR_ECC_LOW,
    QR_ECC_MEDIUM,
    QR_ECC_QUARTILE,
    QR_ECC_HIGH
} qr_ecc_t;

// Output format requested via fuji_qrcode_length, determines the bytes returned by fuji_qrcode_output
typedef enum QRCodeOutputMode
{
    QR_OUTPUT_MODE_BINARY,
    QR_OUTPUT_MODE_ANSI,
    QR_OUTPUT_MODE_BITMAP,
    QR_OUTPUT_MODE_SVG,
    QR_OUTPUT_MODE_ATASCII,
    QR_OUTPUT_MODE_PETSCII
} qr_output_mode_t;

// Add data to be encoded. Call one or more times before fuji_qrcode_encode.
#define fuji_qrcode_input(data, len) FUJICALL_B12_D(FUJICMD_QRCODE_INPUT, len, data, len)

// Generate the QR code from the data sent with fuji_qrcode_input.
// version is 1-40 (0 auto), ecc is a qr_ecc_t, shorten runs the url through a shortener first.
#ifdef __ATARI__
#define fuji_qrcode_encode(version, ecc, shorten) \
  FUJICALL_A1_A2(FUJICMD_QRCODE_ENCODE, version, (shorten << 4) | (ecc))
#else
#define fuji_qrcode_encode(version, ecc, shorten) \
  FUJICALL_A1_A2_A3(FUJICMD_QRCODE_ENCODE, version, ecc, shorten)
#endif /* __ATARI__ */

// Re-render the code in output_mode (a qr_output_mode_t) and return its byte length in len.
#define fuji_qrcode_length(mode, len) FUJICALL_A1_RV(FUJICMD_QRCODE_LENGTH, mode, \
                                                     len, sizeof(unsigned long))

// Read len bytes of the rendered code into s.
#define fuji_qrcode_output(buf, len) FUJICALL_B12_RV(FUJICMD_QRCODE_OUTPUT, len, buf, len)

// Creates encoded QR code using the specified parameters. Returns the
// final encoded length, or a negative error code if calls failed or
// not enough space in buffer.
extern int qrcode_create(uint8_t version, qr_ecc_t ecc, bool shorten, qr_output_mode_t mode,
                         void *input, size_t input_len, void *output, size_t output_max);

#endif /* FUJINET_QRCODE_H */
