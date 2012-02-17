#include <stdio.h>
#include <time.h>
#include <unistd.h>

static void clearscreen() {
	fputs("\033[H\033[2J", stdout);
}

int main(int argc, char *argv[]){
	char const *frame01 =
		"                             O)           \n"
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
		"                       `   `              \n";

	char const *frame02 =
		"                             0)           \n"
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
		"                       `   `              \n";

	int mode = 1;
	char now[9];
	time_t curtime;

	for (;;) {
		curtime = time((time_t *) 0);
		strftime(now, sizeof now, "%T", localtime(&curtime));

		if (mode == 1) {
			clearscreen();
			printf(frame01, now);
			mode = 2;
		} else {
			clearscreen();
			printf(frame02, now);
			mode = 1;
		}

		sleep(1);
	}

	return(0);
}
