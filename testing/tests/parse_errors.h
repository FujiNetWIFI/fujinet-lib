#ifndef PARSE_ERRORS_H
#define PARSE_ERRORS_H

extern void test_parse_no_parser(void);
extern void test_parse_after_close(void);
extern void test_parse_parser_none(void);
extern void test_json_parse_malformed(void);
extern void test_json_parse_empty_body(void);
extern void test_sgml_parse_empty_body(void);
extern void test_parse_error_not_sticky(void);
extern void test_query_after_failed_parse(void);
extern void test_query_no_channel(void);
extern void test_set_parser_invalid_mode(void);
extern void test_set_parameter_errors(void);

#endif /* PARSE_ERRORS_H */
