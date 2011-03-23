
#include "ucParser.h"

static const char* gPtr;

static uint8_t getByte()
{
	return (uint8_t) *gPtr++;
}

int main()
{
	gPtr = "123.456e-12";
	UC::Parser parser(getByte);
	parser.program();
}
