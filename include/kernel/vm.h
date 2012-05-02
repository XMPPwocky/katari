#pragma once

#include "kernel/kernel.h"

#define	PAGE_SIZE	4096

/* VMSAv7 first-level descriptor formats */
/* Holy bitwise arithmetic, Batman! */
#define TRANSLATION_SECTION_DESCRIPTOR(baseaddr, ns, ng, s, ap, tex, imp, \
		domain, xn, c, b) ( \
			(0b10 & MASK_BITSFROM(0, 1)) | \
			((b << 2) & MASK_BITAT(2)) | \
			((c << 3) & MASK_BITAT(3)) | \
			((xn << 4) & MASK_BITAT(4)) | \
			((domain << 5) & MASK_BITSFROM(5, 8)) | \
			((imp << 9) & MASK_BITAT(9)) | \
			(((ap & MASK_BITSFROM(0, 1)) \
			  << 10) & MASK_BITSFROM(10, 11)) | \
			((tex << 12) & MASK_BITSFROM(12, 14)) | \
			(((ap & MASK_BITAT(2)) << 15) & MASK_BITAT(15)) | \
			((s << 16) & MASK_BITAT(16)) | \
			((ng << 17) & MASK_BITAT(17)) | \
			((0b0 << 18) & MASK_BITAT(18)) | \
			((ns << 19) & MASK_BITAT(19)) | \
			((base_addr & MASK_BITSFROM(20, 31))))
