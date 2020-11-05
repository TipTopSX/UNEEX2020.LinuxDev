#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int (*orig_unlinkat)(int, const char *, int) = NULL;

int unlinkat(int dirfd, const char *pathname, int flags) {
	if (strstr(pathname, "FIX")) {
		errno = EPERM;
		return -1;
	}
	if (!orig_unlinkat) {
		orig_unlinkat = dlsym(RTLD_NEXT, "unlinkat");
	}
	return orig_unlinkat(dirfd, pathname, flags);
}