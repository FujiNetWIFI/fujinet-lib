#include <fujinet-qrcode.h>
#include <fujinet-err.h>

int qrcode_create(uint8_t version, qr_ecc_t ecc, bool shorten, qr_output_mode_t mode,
                  void *input, size_t input_len, void *output, size_t output_max)
{
  unsigned long output_len;


  // FIXME - empty any input still in QR code buffer on FujiNet

  if (!fuji_qrcode_input(input, input_len))
    return -NETWORK_ERROR_SERVER_GENERAL;

  // Set the QR code parameters
  if (!fuji_qrcode_encode(version, ecc, shorten))
    return -NETWORK_ERROR_SERVER_GENERAL;

  // Select the output format to be used while getting the length
  if (!fuji_qrcode_length(mode, &output_len))
    return -NETWORK_ERROR_SERVER_GENERAL;

  if (output_len > output_max)
    return -NETWORK_ERROR_NO_SPACE_ON_DEVICE;

  if (!fuji_qrcode_output(output, (uint16_t) output_len))
    return -NETWORK_ERROR_SERVER_GENERAL;

  return (int) output_len;
}
