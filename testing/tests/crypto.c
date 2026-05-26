#include "broken.h"

#include "harness.h"
#include "globals.h"
#include <fujinet-fuji.h>

#ifndef _CMOC_VERSION_
#include <stdio.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#define BASE64_IN  "Hello, FujiNet!"
#define BASE64_OUT "SGVsbG8sIEZ1amlOZXQh\012"
#define MD5_OUT    "98d6da021c1a0b081b52de3b8207a823"
#define SHA1_OUT   "954382c7aa0a0d3252faf234ba4911f2aed22e39"
#define SHA256_OUT "3631e38122de15b2e5b4ae209ba1eb7c79b0e756" \
                   "fd9bc7149ce83da8aba6ddc6"
#define SHA512_OUT "8230a7a465c8ef21e6d89c423e5cb06f1f8b183b" \
                   "7928189736e00e81d269484d5a5d8a40723a157e" \
                   "f55d1992fd2929b213770eada5501434a592d75a" \
                   "d1f935fd"

void test_fuji_base64(void)
{
  char input[] = BASE64_IN;
  uint16_t in_len;
  bool ok;
  unsigned long enc_len;
  unsigned long dec_len;

  SECTION("fuji base64 encode/decode");

    /*
     * The firmware has no way to clear base64 state other than calling
     * output. Drain both encode and decode pipelines before starting so
     * we know we're in a clean state regardless of what ran before.
     */
#ifdef FN_BROKEN_fuji_base64_encode_length
  SKIP(fuji_base64_encode_length);
#endif
  enc_len = 0;
  ok = fuji_base64_encode_length(&enc_len);
#ifdef FN_BROKEN_fuji_base64_encode_output
  SKIP(fuji_base64_encode_output);
#endif
  if (ok && enc_len)
    fuji_base64_encode_output(g.b64.enc, (uint16_t) enc_len);

#ifdef FN_BROKEN_fuji_base64_decode_length
  SKIP(fuji_base64_decode_length);
#endif
  dec_len = 0;
  ok = fuji_base64_decode_length(&dec_len);
#ifdef FN_BROKEN_fuji_base64_decode_output
  SKIP(fuji_base64_decode_output);
#endif
  if (ok && dec_len)
    fuji_base64_decode_output(g.b64.dec, (uint16_t) dec_len);

  in_len = (uint16_t)strlen(input);

#ifdef FN_BROKEN_fuji_base64_encode_input
  SKIP(fuji_base64_encode_input);
#endif
  ok = fuji_base64_encode_input(input, in_len);
  TEST("fuji_base64_encode_input succeeds", ok);

#ifdef FN_BROKEN_fuji_base64_encode_compute
  SKIP(fuji_base64_encode_compute);
#endif
  ok = fuji_base64_encode_compute();
  TEST("fuji_base64_encode_compute succeeds", ok);

#ifdef FN_BROKEN_fuji_base64_encode_length
  SKIP(fuji_base64_encode_length);
#endif
  enc_len = 0;
  ok = fuji_base64_encode_length(&enc_len);
  TEST("fuji_base64_encode_length succeeds", ok);
  TEST("Encoded length is positive", enc_len > 0);
  TEST("Encoded length fits in g.b64.enc", enc_len < sizeof(g.b64.enc));

#ifdef FN_BROKEN_fuji_base64_encode_output
  SKIP(fuji_base64_encode_output);
#endif
  ok = fuji_base64_encode_output(g.b64.enc, (uint16_t)enc_len);
  TEST("fuji_base64_encode_output succeeds", ok);
  g.b64.enc[(uint16_t) enc_len] = '\0';
  printf("  Base64 encoded: %s\n", g.b64.enc);
  TEST("Base64 output matches expected value", strcmp(g.b64.enc, BASE64_OUT) == 0);

#ifdef FN_BROKEN_fuji_base64_decode_input
  SKIP(fuji_base64_decode_input);
#endif
  ok = fuji_base64_decode_input(g.b64.enc, (uint16_t)enc_len);
  TEST("fuji_base64_decode_input succeeds", ok);

#ifdef FN_BROKEN_fuji_base64_decode_compute
  SKIP(fuji_base64_decode_compute);
#endif
  ok = fuji_base64_decode_compute();
  TEST("fuji_base64_decode_compute succeeds", ok);

#ifdef FN_BROKEN_fuji_base64_decode_length
  SKIP(fuji_base64_decode_length);
#endif
  dec_len = 0;
  ok = fuji_base64_decode_length(&dec_len);
  TEST("fuji_base64_decode_length succeeds", ok);
  TEST("Decoded length fits in g.b64.dec", dec_len < sizeof(g.b64.dec));

#ifdef FN_BROKEN_fuji_base64_decode_output
  SKIP(fuji_base64_decode_output);
#endif
  ok = fuji_base64_decode_output(g.b64.dec, (uint16_t)dec_len);
  TEST("fuji_base64_decode_output succeeds", ok);
  g.b64.dec[(uint16_t) dec_len] = '\0';
  printf("  Base64 decoded: %s\n", g.b64.dec);
  TEST("Decoded output matches original input", strcmp(g.b64.dec, input) == 0);

  END_OF_TEST();
}

void test_fuji_guid(void)
{
  char guid1[MAX_GUID_LEN];
  char guid2[MAX_GUID_LEN];
  bool ok1;
  bool ok2;

  SECTION("fuji_generate_guid");

#ifdef FN_BROKEN_fuji_generate_guid
  SKIP(fuji_generate_guid);
#endif
  memset(guid1, 0, sizeof(guid1));
  memset(guid2, 0, sizeof(guid2));

  ok1 = fuji_generate_guid(guid1);
  ok2 = fuji_generate_guid(guid2);
  TEST("First fuji_generate_guid succeeds", ok1);
  TEST("Second fuji_generate_guid succeeds", ok2);
  TEST("GUID1 length is 36", strlen(guid1) == 36);
  TEST("GUID2 length is 36", strlen(guid2) == 36);
  TEST("GUIDs are unique", strcmp(guid1, guid2) != 0);
  printf("  GUID1: %s\n  GUID2: %s\n", guid1, guid2);

  END_OF_TEST();
}

void test_fuji_hashing(void)
{
  uint8_t input[] = BASE64_IN;
  uint16_t in_len;
  uint16_t sz;
  bool ok;

  SECTION("fuji hashing (fuji_hash_data single-shot)");

  in_len = (uint16_t)strlen((char *)input);

#ifdef FN_BROKEN_fuji_hash_size
  SKIP(fuji_hash_size);
#endif
#ifdef FN_BROKEN_fuji_hash_data
  SKIP(fuji_hash_data);
#else

  sz = fuji_hash_size(MD5, true);
  TEST("fuji_hash_size(MD5, hex) returns 32", sz == 32);
  ok = fuji_hash_data(MD5, input, in_len, true, g.hash.md5);
  TEST("fuji_hash_data MD5 succeeds", ok);
  g.hash.md5[sz] = '\0';
  printf("  MD5 hex: %s\n", g.hash.md5);
  TEST("MD5 output length is 32 hex chars", strlen((char *)g.hash.md5) == 32);
  TEST("MD5 output matches expected value", strcmp((char *)g.hash.md5, MD5_OUT) == 0);

  sz = fuji_hash_size(SHA1, true);
  TEST("fuji_hash_size(SHA1, hex) returns 40", sz == 40);
  ok = fuji_hash_data(SHA1, input, in_len, true, g.hash.sha1);
  TEST("fuji_hash_data SHA1 succeeds", ok);
  g.hash.sha1[sz] = '\0';
  printf("  SHA1 hex: %s\n", g.hash.sha1);
  TEST("SHA1 output length is 40 hex chars", strlen((char *)g.hash.sha1) == 40);
  TEST("SHA1 output matches expected value", strcmp((char *)g.hash.sha1, SHA1_OUT) == 0);

  sz = fuji_hash_size(SHA256, true);
  TEST("fuji_hash_size(SHA256, hex) returns 64", sz == 64);
  ok = fuji_hash_data(SHA256, input, in_len, true, g.hash.sha256);
  TEST("fuji_hash_data SHA256 succeeds", ok);
  g.hash.sha256[sz] = '\0';
  printf("  SHA256 hex: %s\n", g.hash.sha256);
  TEST("SHA256 output length is 64 hex chars", strlen((char *)g.hash.sha256) == 64);
  TEST("SHA256 output matches expected value", strcmp((char *)g.hash.sha256, SHA256_OUT) == 0);

  sz = fuji_hash_size(SHA512, true);
  TEST("fuji_hash_size(SHA512, hex) returns 128", sz == 128);
  ok = fuji_hash_data(SHA512, input, in_len, true, g.hash.sha512);
  TEST("fuji_hash_data SHA512 succeeds", ok);
  g.hash.sha512[sz] = '\0';
  printf("  SHA512 hex: %s\n", g.hash.sha512);
  TEST("SHA512 output length is 128 hex chars", strlen((char *)g.hash.sha512) == 128);
  TEST("SHA512 output matches expected value", strcmp((char *)g.hash.sha512, SHA512_OUT) == 0);

  SECTION("fuji hashing (incremental interface)");
#endif

#ifdef FN_BROKEN_fuji_hash_clear
  SKIP(fuji_hash_clear);
#else
  ok = fuji_hash_clear();
  TEST("fuji_hash_clear succeeds", ok);
#endif

#ifdef FN_BROKEN_fuji_hash_add
  SKIP(fuji_hash_add);
#else
  ok = fuji_hash_add(input, in_len);
  TEST("fuji_hash_add succeeds", ok);
#endif

#ifdef FN_BROKEN_fuji_hash_calculate
  SKIP(fuji_hash_calculate);
#else
  memset(g.hash_verify.inc1, 0, sizeof(g.hash_verify.inc1));
  ok = fuji_hash_calculate(SHA256, true, false, g.hash_verify.inc1);
  TEST("fuji_hash_calculate(SHA256, keep) succeeds", ok);
  TEST("Incremental SHA256 length is 64 hex chars", strlen((char *)g.hash_verify.inc1) == 64);

  memset(g.hash_verify.inc2, 0, sizeof(g.hash_verify.inc2));
  ok = fuji_hash_calculate(SHA256, true, true, g.hash_verify.inc2);
  TEST("fuji_hash_calculate(SHA256, discard) succeeds", ok);
  TEST("Second incremental calculation matches first",
       memcmp(g.hash_verify.inc1, g.hash_verify.inc2, 64) == 0);
  TEST("Incremental SHA256 matches single-shot result",
       memcmp(g.hash_verify.inc1, g.hash.sha256, 64) == 0);
#endif

  END_OF_TEST();
}
