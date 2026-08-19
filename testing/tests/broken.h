#ifndef BROKEN_H
#define BROKEN_H

/* =========================================================================
 * Known non-working functions
 *
 * Define a symbol here for each function that is known to be broken.
 * Tests that call a broken function will print SKIP instead of running.
 * Remove the #define once a function is fixed and re-tested.
 *
 * Example:
 *   #define FN_BROKEN_fuji_generate_guid
 *   #define FN_BROKEN_network_http_put
 * ========================================================================= */

// Building for Adam CP/M only defines __COLECOADAM__ but
// fujinet-fuji.h only checks for __ADAM__
#if FNLIB_VERSION_MAJOR < 5 && defined(__COLECOADAM__) && !defined(__ADAM__)
#define __ADAM__
#endif

#include <fujinet-fuji.h>

#if FNLIB_VERSION_MAJOR < 5
#include "broken-v4.h"
#else /* FNLIB_VERSION_MAJOR >= 5 */
#include "broken-v5.h"
#endif /* FNLIB_VERSION_MAJOR */

#endif /* BROKEN_H */
