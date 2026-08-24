#if FNLIB_VERSION_MAJOR >= 5

#ifdef BUILD_APPLE2
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_APPLE2 */

#if defined(BUILD_ADAM) || defined(BUILD_ADAM_CPM)
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_ADAM || BUILD_ADAM_CPM */

#ifdef BUILD_COCO
// Missing from firmware
#define FN_BROKEN_network_set_eol
#endif /* BUILD_COCO */

#endif // FNLIB_VERSION_MAJOR >= 5
