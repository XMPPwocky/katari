#pragma once

#define MASK_BITAT(bit) 1 << bit
#define MASK_BYTEAT(byte) 0xFF << (byte*8)

typedef uint_fast32_t bitmap_basetype;
#define BITMAP_BASETYPE_BYTES (sizeof (bitmap_basetype))
#define BITMAP_BASETYPE_BITS (BITMAP_BASETYPE_BYTES * 8)

#define BITMAP(size) struct { bitmap_basetype \
	data[(size+BITMAP_BASETYPE_BITS-1)/BITMAP_BASETYPE_BITS]; }

#define BITMAP_GET(map, bit) (map->data[(bit+1)/BITMAP_BASETYPE_BITS] & \
		MASK_BITAT(bit % BITMAP_BASETYPE_BITS))

#define BITMAP_SET(map, bit, value) map->data[(bit+1)/BITMAP_BASETYPE_BITS] \
		= value << (bit % BITMAP_BASETYPE_BITS)
