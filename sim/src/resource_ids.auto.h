#pragma once

// AUTOGENERATED BY tools/local/resCompiler
// DO NOT MODIFY
//

#include <stdint.h>
/* Because for some reasons (perhaps SDL?) changing the entry point creates crashes,
 * but because pebble.h includes this file and all your pebble apps include pebble.h
 * I change the name of your "main" function to "pbl_main"
 */
#undef main
#define main pbl_main
typedef enum {
	INVALID_RESOURCE = 0,

} ResourceId;