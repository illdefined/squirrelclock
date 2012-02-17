#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <defy/expect>
#include <defy/restrict>

#include <ev.h>

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

static void die(char const *restrict format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	putc('\n', stderr);
	va_end(ap);

	exit(EXIT_FAILURE);
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

	ev_run(loop, 0);

	return EXIT_SUCCESS;
}
