#include "debug.hpp"

void debugPrint(int x, int y, const char* format, ...) {
	va_list args;
	va_start(args, format);

	printf("\x1b[%d;%dH", y + 1, x + 1);
	vprintf(format, args);

	va_end(args);
}
