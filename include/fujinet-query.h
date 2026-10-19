#ifndef FUJINET_QUERY_H
#define FUJINET_QUERY_H

/* network_set_query_param() flags: any remap bits, ORed with one output mode */
typedef enum QueryFlags
{
  QUERY_REMAP_CHARS                 = 0x01,
  QUERY_REMAP_ATASCII_INTERNATIONAL = 0x02,
  QUERY_DELETE_SGML_TAGS            = 0x04, /* JSON only */
  QUERY_OUTPUT_VERBATIM             = 0x00,
  QUERY_OUTPUT_ASCII                = 0x10, /* decode entities, strip non-ASCII */
  QUERY_OUTPUT_RESERVED_0X20        = 0x20, /* rejected by the firmware */
  QUERY_OUTPUT_RESERVED_0X30        = 0x30  /* rejected by the firmware */
} query_flags_t;

#endif /* FUJINET_QUERY_H */
