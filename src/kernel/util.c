#include "kernel/kernel.h"
#include "kernel/util.h"

size_t inttostr(int i, char buf[], size_t buflen) {
	char tmpbuf[buflen-1];
	size_t pos = 0;

	char is_negative = (i < 0) ? 1 : 0;
	if (is_negative) {
		i = i - (2*i); /* make it positive */
	};

	for (; pos < buflen-((is_negative) ? 2 : 1); pos++) {
		tmpbuf[pos] = 48+(i%10);
		if (i < 10) {
			if (is_negative) {
				tmpbuf[++pos] = '-';
			};
			break;
		};		
		i = i/10;
	};
	

	size_t tmpbufend = pos;

	for (pos = 0; pos <= tmpbufend; pos++) {
		buf[pos] = tmpbuf[tmpbufend-pos];
	};
	buf[tmpbufend+1] = 0;

	return tmpbufend+1;	
};
