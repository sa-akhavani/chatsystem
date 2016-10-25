#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

// volatile sig_atomic_t print_flag = false;

void handle_alarm() {
    printf( "Hello\n" );
    alarm(1);
}

int main() {
    signal(SIGALRM, handle_alarm);
    alarm(1);

    while(1) {
    }
}