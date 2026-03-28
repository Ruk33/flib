#include <stdarg.h>
#include <string.h>

// c = extract unsigned char
// C = match unsigned char
// s = extract unsigned short
// S = match unsigned short
// i = extract unsigned int
// I = match unsigned int
// l = extract unsigned long long
// L = match unsigned long long
// t = extract text/string
// T = match text/string
// [ = repeat, must have how many fields can hold
// ] = end fields to repeat
int extract(void *_src, unsigned int size, char *fmt, ...)
{
	unsigned char *src = (unsigned char *) _src;

	va_list args;

	va_start(args, fmt);

	unsigned long long max_repeat = 0;
	unsigned long long repeat_count = 0;
	void *repeat_fields[32] = {0};
	int repeat_field_index = 0;
	char *repeat_fmt = 0;

	while (*fmt) {
		if (*fmt == '[') {
			max_repeat = va_arg(args, unsigned long long);
			if (repeat_count > max_repeat) {
				return 0;
			}
			fmt++;
			repeat_fmt = fmt;
			continue;
		}
		if (*fmt == ']') {
			repeat_count--;
			repeat_field_index = 0;
			if (repeat_count) {
				fmt = repeat_fmt;
				continue;
			}
			memset(repeat_fields, 0, sizeof(repeat_fields));
			repeat_fmt = 0;
			max_repeat = 0;
			fmt++;
			continue;
		}
		if (*fmt == 'c') {
			if (size < 1) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned char *);
				}
				unsigned char *dest = (unsigned char *) repeat_fields[repeat_field_index];
				*dest = *src;
				repeat_fields[repeat_field_index]++;
				repeat_field_index++;
			} else {
				unsigned char *dest = va_arg(args, unsigned char *);
				*dest = *src;
				repeat_count = *dest;
			}
			fmt++;
			src++;
			size--;
			continue;
		}
		if (*fmt == 'C') {
			if (size < 1) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned char *);
				}
				unsigned char *expected = (unsigned char *) repeat_fields[repeat_field_index];
				if (*expected != *src) {
					return 0;
				}
				repeat_fields[repeat_field_index]++;
				repeat_field_index++;
			} else {
				unsigned char expected = va_arg(args, unsigned int);
				if (*src != expected) {
					return 0;
				}
				repeat_count = expected;
			}
			fmt++;
			src++;
			size--;
			continue;
		}
		if (*fmt == 's') {
			if (size < 2) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned short *);
				}
				unsigned short *dest = (unsigned short *) repeat_fields[repeat_field_index];
				*dest = *((unsigned short *) src);
				repeat_fields[repeat_field_index] += 2;
				repeat_field_index++;
			} else {
				unsigned short *dest = va_arg(args, unsigned short *);
				*dest = *((unsigned short *) src);
				repeat_count = *dest;
			}
			fmt++;
			src += 2;
			size -= 2;
			continue;
		}
		if (*fmt == 'S') {
			if (size < 2) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned short *);
				}
				unsigned short *expected = (unsigned short *) repeat_fields[repeat_field_index];
				if (*((unsigned short *) src) != *expected) {
					return 0;
				}
				repeat_fields[repeat_field_index] += 2;
				repeat_field_index++;
			} else {
				unsigned short expected = va_arg(args, unsigned int);
				if (*((unsigned short *) src) != expected) {
					return 0;
				}
				repeat_count = expected;
			}
			fmt++;
			src += 2;
			size -= 2;
			continue;
		}
		if (*fmt == 'i') {
			if (size < 4) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned int *);
				}
				unsigned int *dest = (unsigned int *) repeat_fields[repeat_field_index];
				*dest = *((unsigned int *) src);
				repeat_fields[repeat_field_index] += 4;
				repeat_field_index++;
			} else {
				unsigned int *dest = va_arg(args, unsigned int *);
				*dest = *((unsigned int *) src);
				repeat_count = *dest;
			}
			fmt++;
			src += 4;
			size -= 4;
			continue;
		}
		if (*fmt == 'I') {
			if (size < 4) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned int *);
				}
				unsigned int *expected = (unsigned int *) repeat_fields[repeat_field_index];
				if (*((unsigned int *) src) != *expected) {
					return 0;
				}
				repeat_fields[repeat_field_index] += 4;
				repeat_field_index++;
			} else {
				unsigned int expected = va_arg(args, unsigned int);
				if (*((unsigned int *) src) != expected) {
					return 0;
				}
				repeat_count = expected;
			}
			fmt++;
			src += 4;
			size -= 4;
			continue;
		}
		if (*fmt == 'l') {
			if (size < 8) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned long long *);
				}
				unsigned long long *dest = (unsigned long long *) repeat_fields[repeat_field_index];
				*dest = *((unsigned long long *) src);
				repeat_fields[repeat_field_index] += 8;
				repeat_field_index++;
			} else {
				unsigned long long *dest = va_arg(args, unsigned long long *);
				*dest = *((unsigned long long *) src);
				repeat_count = *dest;
			}
			fmt++;
			src += 8;
			size -= 8;
			continue;
		}
		if (*fmt == 'L') {
			if (size < 8) {
				return 0;
			}
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, unsigned long long *);
				}
				unsigned long long *expected = (unsigned long long *) repeat_fields[repeat_field_index];
				if (*((unsigned long long *) src) != *expected) {
					return 0;
				}
				repeat_fields[repeat_field_index] += 8;
				repeat_field_index++;
			} else {
				unsigned long long expected = va_arg(args, unsigned long long);
				if (*((unsigned long long *) src) != expected) {
					return 0;
				}
				repeat_count = expected;
			}
			fmt++;
			src += 8;
			size -= 8;
			continue;
		}
		if (*fmt == 't') {
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, char **);
				}
				char *dest = *((char **) repeat_fields[repeat_field_index]);
				repeat_fields[repeat_field_index] += sizeof(void *);
				repeat_field_index++;
				while (*src) {
					if (size < 1) {
						return 0;
					}
					*dest++ = *src++;
					size--;
				}
				if (size < 1) {
					return 0;
				}
				*dest++ = *src++;
				size--;
			} else {
				char *dest = va_arg(args, char *);
				while (*src) {
					if (size < 1) {
						return 0;
					}
					*dest++ = *src++;
					size--;
				}
				if (size < 1) {
					return 0;
				}
				*dest++ = *src++;
				size--;
			}
			fmt++;
			continue;
		}
		if (*fmt == 'T') {
			if (repeat_fmt) {
				if (!repeat_fields[repeat_field_index]) {
					repeat_fields[repeat_field_index] = (void *) va_arg(args, char *);
				}
				char *expected = *((char **) repeat_fields[repeat_field_index]);
				repeat_fields[repeat_field_index] += sizeof(void *);
				repeat_field_index++;
				while (*expected) {
					if (size < 1) {
						return 0;
					}
					if (*expected != *src) {
						return 0;
					}
					expected++;
					src++;
					size--;
				}
				if (size < 1) {
					return 0;
				}
				if (*src != 0) {
					return 0;
				}
				src++;
				size--;
			} else {
				char *expected = va_arg(args, char *);
				while (*expected) {
					if (size < 1) {
						return 0;
					}
					if (*expected != *src) {
						return 0;
					}
					expected++;
					src++;
					size--;
				}
				if (size < 1) {
					return 0;
				}
				src++;
				size--;
			}
			fmt++;
			continue;
		}
		fmt++;
	}

	va_end(args);

	return 1;
}

#if 0
#include <stdio.h>

#define array_length(x) (sizeof(x)/sizeof(*(x)))

#define parse_packet_or_return(packet, protocol, type, fmt, ...) \
	{ \
		unsigned short len = 0; \
		if (!extract(packet, packet_size(packet), "C C s " fmt, protocol, type, &len, __VA_ARGS__)) \
			return 0; \
	}

struct packet {
	unsigned char data[8192];
};

unsigned short packet_size(struct packet *packet)
{
	unsigned short size = ((unsigned short *) packet->data)[1];
	return size;
}

int handle_auth_login(struct packet *request)
{
	// these are the fields we want to extract from the packet
	// if these are not present, then we don't deal with this packet :)
	unsigned char protocol = 255;
	unsigned char type = 69;
	unsigned int foo = 0;
	unsigned char repeat = 0;

	#define max_users 2
	// here, you can see how we only care if we get these
	// names. we can also extract the names with the format "t"
	// instead of "T"
	char names[max_users][32] = {0};
	// char names[max_users][32] = {"ruke", "roberto"};
	char *tmp[max_users] = {names[0], names[1]};
	int bars[max_users] = {0};
	#undef max_users
	int baz = 0;

	parse_packet_or_return(
		request,
		protocol,
		type,
		"i c [t i] i",
		&foo,
		&repeat,
		array_length(names),
		tmp,
		bars,
		&baz
	);

	printf(
		"foo: %d, repeat: %d, name[0] = %s, bar[0] = %d, name[1] = %s, bar[1] = %d, baz = %d\n",
		foo,
		repeat,
		names[0],
		bars[0],
		names[1],
		bars[1],
		baz
	);
}

int handle_cancel_dagger(struct packet *request)
{
	unsigned char protocol = 255;
	unsigned char type = 68;
	unsigned char cancel = 4;

	parse_packet_or_return(
		request,
		protocol,
		type,
		"C",
		cancel
	);

	printf("the player clicked cancel\n");
}

int main()
{
	// play arround with this possible bytes we could have from the internet
	unsigned char raw_request1[] = {
		255,
		69,
		0, // size
		0, // size
		4,
		6,
		7,
		8,
		2,
		'r',
		'u',
		'k',
		'e',
		0,
		4,
		6,
		7,
		8,
		'r',
		'o',
		'b',
		'e',
		'r',
		't',
		'o',
		0,
		3,
		2,
		1,
		4,
		1,
		1,
		1,
		2,
	};

	// assume we get these bytes from the network
	// im choosing the warcraft 3 protocol which basically:
	// - [x]: protocol (255) - byte
	// - [x]: packet type - byte
	// - [x, x]: packet size (including header) - short
	// - [...]: packet body - dynamic
	unsigned short len = 0;

	len = sizeof(raw_request1);
	memcpy(raw_request1 + 2, &len, 2);

	unsigned char raw_request2[] = {
		255,
		68,
		0, // size
		0, // size
		4,
	};

	len = sizeof(raw_request2);
	memcpy(raw_request2 + 2, &len, 2);

	// when getting bytes from the internet, we may
	// or may not recieve the entire thing at once.
	// sometimes packets/requests arrive in chunks.
	// we could check for sizes to make sure
	// we are only dealing with complete packets.
	// but here, i want to take a differnt approach,
	// i want to say: take whatever we have received
	// and put it on a larger buffer. this way, we can
	// more easily deal with it and parse it. if
	// the packet is not complete, no worries, we will
	// be dealing with bytes set as 0.
	struct packet safe_request = {0};
	memcpy(safe_request.data, raw_request1, sizeof(raw_request1));

	int handled = 0;

	handled =
		handle_auth_login(&safe_request) ||
		handle_cancel_dagger(&safe_request) /* || ... */;

	if (handled) {
		memset(safe_request.data, 0, packet_size(&safe_request));
		memcpy(safe_request.data, raw_request2, sizeof(raw_request2));
	}

	handled =
		handle_auth_login(&safe_request) ||
		handle_cancel_dagger(&safe_request) /* || ... */;

	return 0;
}
#endif
