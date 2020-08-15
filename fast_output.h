#ifndef FAST_OUTPUT_H
#define FAST_OUTPUT_H

#include <cstdio>
#include <string>
#include <cassert>
#include <sys/stat.h>

#define outputBuffSize (67108864)

namespace FastOutput {
	char buff[outputBuffSize], * ptr = NULL, * pend = NULL, sep, endl = '\n';
	FILE* stream = NULL;
	int itemsize, maxbytes;

	inline void init(const char* path, const char* mode = "wb", const int element_size = 1, const char& c = '\t') {
		assert(stream == NULL && (stream = fopen(path, mode)) != NULL);
		itemsize = element_size;
		maxbytes = (outputBuffSize / element_size) * element_size;
		ptr = buff;
		pend = buff + maxbytes;
		sep = c;
	}

	inline void init(const std::string & path, const char* mode = "wb", const int element_size = 1, const char& c = '\t') {
		init(path.c_str(), mode, element_size, c);
	}

	inline void flush() {
		fwrite(buff, itemsize, (ptr - buff) / itemsize, stream);
		ptr = buff;
		fflush(stream);
	}

	inline void write_char(const char& c) {
		if (ptr >= pend) flush();
		*ptr++ = c;
	}

	inline void write_sep() {
		if (ptr >= pend) flush();
		*ptr++ = sep;
	}

	inline void endline() {
		if (ptr >= pend) flush();
		*ptr++ = endl;
	}

	inline void write_chars(const char* s) {
		for (; *s; ++s) write_char(*s);
	}

	inline void write_string(const std::string & s) {
		write_chars(s.c_str());
	}

	inline void write_uint(uint32_t x) {
		char buf[20], * p = buf;
		if (x == 0) write_char('0');
		if (x < 0) write_char('-'), x = -x;
		while (x > 0) * p++ = x % 10 + '0', x /= 10;
		while (p > buf) write_char(*--p);
	}

	inline int close() {
		if (ptr > buff) flush();
		int ret = fclose(stream);
		ptr = pend = NULL;
		stream = NULL;
		return ret;
	}
}


#endif
