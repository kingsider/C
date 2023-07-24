#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "signal.h"

void signal_handler(int code) {
        switch (code) {
                case 1:
                printf ("\nSignal SIGHUP processed\n");
                break;
                case 2:
                printf ("\nSignal SIGINT processed\n");
                break;
                case 3:
                printf ("\nSignal SIGQUIT processed\n");
                break;
                case 6:
                printf ("\nSignal SIGABRT processed\n");
                break;
                case 9:
                printf ("\nSignal SIGKILL processed\n"); //Cannot be processed
                break;
                case 15:
                printf ("\nSignal SIGTERM processed\n");
                break;
                case 19:
                printf ("\nSignal SIGSTOP processed\n"); //Cannot be processed
                break;
                case 20:
                printf ("\nSignal SIGTSTP processed\n");
                break;
        }
  exit(EXIT_SUCCESS);
}
int main() {

        printf("Use \"kill 'option' 'pid'\" to send me a signal\n");
        printf("Available options:\n");
        printf("-1 - hang up (SIGHUP)\n");
        printf("-2 or Ctrl+C - interrupt (SIGINT)\n");
        printf("-3 - quit (SIGQUIT)\n");
        printf("-6 - abort (SIGABRT)\n");
        printf("-9 - kill (SIGKILL)\n");
        printf("-15 - terminate (SIGTERM)\n");
        printf("-19 - stop (SIGSTOP)\n");
        printf("-20 or Ctrl+Z - terminal stop (SIGTSTP)\n");

        struct sigaction signal;
        signal.sa_handler = signal_handler;
        sigaction(SIGHUP, &signal, 0);
        sigaction(SIGINT, &signal, 0);
        sigaction(SIGQUIT, &signal, 0);
        sigaction(SIGABRT, &signal, 0);
        sigaction(SIGKILL, &signal, 0);
        sigaction(SIGTERM, &signal, 0);
        sigaction(SIGSTOP, &signal, 0);
        sigaction(SIGTSTP, &signal, 0);
        printf("My pid is %i\n", getpid());
        printf("Waiting for signal...\n");
        while(1);
        return 0;
}

