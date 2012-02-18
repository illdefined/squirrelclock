#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <termios.h>
#include <unistd.h>

#include <defy/expect>
#include <defy/restrict>

#include <ev.h>

static struct termios pristine;

static void timeString(char *restrict buffer, size_t length) {
	time_t now = time((time_t *) 0);
	strftime(buffer, length, "%T", localtime(&now));
}

static void displayFrame(char const *restrict format) {
	char now[9];
	timeString(now, sizeof now);
	printf(format, "\033[H\033[2J", now);
	fflush(stdout);
}

static void displayFrameA(struct ev_loop *loop, ev_timer *watcher, int events) {
	displayFrame(
		"%s                             O)           \n"
		"                            ( ))_         \n"
		"                           /     \\       \n"
		"   __________             /     *  \\     \n"
		"  /           \\         /      __ _/_    \n"
		" /  %s   \\       /      /___/  \\        \n"
		" |   ____       \\      /      ----\\__/  \n"
		" |__/    \\       \\    /       |         \n"
		"          \\       \\   /       |         \n"
		"           \\       \\V/       /          \n"
		"            \\_____           |           \n"
		"                  \\___ ___ _/            \n"
		"                       `   `              \n"
	);
}

static void displayFrameB(struct ev_loop *loop, ev_timer *watcher, int events) {
	displayFrame(
		"%s                             0)           \n"
		"    ________                ( ))_         \n"
		"   /        \\              /      \\     \n"
		"   (         \\            /      *  \\   \n"
		"   ( %s \\         /       __ _/          \n"
		"    \\___       \\       /      \\    __  \n"
		"        \\       \\      /       \\__/  \\\n"
		"         \\       \\    /        ___\\__/ \n"
		"          \\       \\   /       |         \n"
		"           \\       \\V/       /          \n"
		"            \\_____           |           \n"
		"                  \\___ ___ _/            \n"
		"                       `   `              \n"
	);
}

static void stdinEvent(struct ev_loop *loop, ev_timer *watcher, int events) {
	ev_break(loop, EVBREAK_ALL);
}

static void die(char const *restrict format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	putc('\n', stderr);
	va_end(ap);

	exit(EXIT_FAILURE);
}

static void onExit() {
	tcsetattr(0, TCSANOW, &pristine);
	tcflush(0, TCIFLUSH);
}

int main(int argc, char *argv[]){
	struct ev_loop *loop = ev_default_loop(EVFLAG_AUTO);
	if (unlikely(!loop))
		die("Failed to initialise default event loop");

	struct ev_timer frameAWatcher;
	ev_timer_init(&frameAWatcher, displayFrameA, 0.0, 2.0);
	ev_timer_start(loop, &frameAWatcher);

	struct ev_timer frameBWatcher;
	ev_timer_init(&frameBWatcher, displayFrameB, 1.0, 2.0);
	ev_timer_start(loop, &frameBWatcher);

	struct ev_io stdinWatcher;
	ev_io_init(&stdinWatcher, stdinEvent, 0, EV_READ);
	ev_io_start(loop, &stdinWatcher);

	/* Save pristine terminal parameters */
	if (unlikely(tcgetattr(0, &pristine)))
		die("Failed to retrieve terminal parameters: %s",
			strerror(errno));

	/* Disable canonical mode */
	struct termios tainted;
	memcpy(&tainted, &pristine, sizeof (struct termios));
	tainted.c_lflag &= ~(ICANON | ECHO);

	if (unlikely(tcsetattr(0, TCSANOW, &tainted)))
		die("Failed to set terminal parameters: %s",
			strerror(errno));

	atexit(onExit);

	ev_run(loop, 0);

	return EXIT_SUCCESS;
}
