/* redirect reads/writes to/from the unix side. */
/* do not compile with msvcrt, as this requires getenv from unix libc and AF_UNIX sockets */

#include "wine/debug.h"

#include <windef.h>
#include <winbase.h>
#include <synchapi.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/* 256 KiB */
#define BUFSIZE 256*1024

extern HANDLE CDECL __wine_make_process_system(void);

WINE_DEFAULT_DEBUG_CHANNEL(discord);

static const char *get_temp_path(void)
{
	const char *result = getenv("XDG_RUNTIME_DIR");
	if (!result)
		result = getenv("TEMP");
	if (!result)
		result = getenv("TMP");
	if (!result)
		result = getenv("TMPDIR");
	if (!result)
		result = "/tmp";
	return result;
}

static void sigpipe_handler(int i)
{
	(void)i;
}

int CDECL main(void)
{
	static const WCHAR discord_ipc_pipe_nameW[] = {'\\','\\','.','\\','p','i','p','e','\\','d','i','s','c','o','r','d','-','i','p','c','-','0',0};
	HANDLE exit_event;
	DWORD bytes_avail, bytes_read;
	ssize_t fd_read;
	char *buf;
	int optval = BUFSIZE;

	/* the interfaces to and from unix side */
	HANDLE pipe = INVALID_HANDLE_VALUE;
	int fd = -1;

	struct sockaddr_un sun;

	WINE_TRACE("starting discord-ipc-0 emulation\n");

	if ((buf = HeapAlloc(GetProcessHeap(), 0, BUFSIZE)) == NULL) {
		WINE_TRACE("exiting, cannot allocate %d byte buffer\n", BUFSIZE);
		return 1;
	}

	exit_event = __wine_make_process_system();

	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;

	signal(SIGPIPE, sigpipe_handler);

	/* there needs to be a better way to do this */
	while (1) {
		if (WaitForSingleObject(exit_event, 20) != WAIT_TIMEOUT)
			break;

		if (fd == -1) {
			if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
				continue;

			fcntl(fd, F_SETFL, O_NONBLOCK);

			setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optval, sizeof(int));
			setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(int));

			strcpy(sun.sun_path, get_temp_path());
			strcat(sun.sun_path, "/discord-ipc-0");

retry_connection:
			if (connect(fd, (const struct sockaddr *)&sun, sizeof(sun)) == -1) {
				SleepEx(1000, 0);
				goto retry_connection;
			}

			if ((pipe = CreateNamedPipeW(discord_ipc_pipe_nameW,
					PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
					PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
					1,
					BUFSIZE,
					BUFSIZE,
					0,
					NULL)) == INVALID_HANDLE_VALUE) {
				close(fd);

				/* another instance exists */
				return 1;
			}
		}

		if (PeekNamedPipe(pipe, NULL, 0, NULL, &bytes_avail, NULL)) {
			if (ReadFile(pipe, buf, BUFSIZE, &bytes_read, NULL)) {
				if (send(fd, buf, bytes_read, 0) == -1) {
disconnected_from_unix:
					WINE_TRACE("disconnected? attempt to reconnect\n");

					CloseHandle(pipe);
					pipe = INVALID_HANDLE_VALUE;

					goto retry_connection;
				}
			}
		}

		if ((fd_read = recv(fd, buf, BUFSIZE, 0)) > 0) {
			WriteFile(pipe, buf, fd_read, &bytes_read, NULL);
		} else {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;

			goto disconnected_from_unix;
		}
	}

	WINE_TRACE("closing up\n");

	HeapFree(GetProcessHeap(), 0, buf);

	if (pipe != INVALID_HANDLE_VALUE)
		CloseHandle(pipe);

	if (fd != -1)
		close(fd);

	CloseHandle(exit_event);

	return 0;
}
