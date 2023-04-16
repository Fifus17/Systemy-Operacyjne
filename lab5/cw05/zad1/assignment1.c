#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    char mailsBuffer[2048] = "";
    if (argc == 2) {
        if (strcmp(argv[1], "nadawca")) {
            FILE* mails = popen("mail | sort -k2", "r");
            while (fgets(mailsBuffer, sizeof(mailsBuffer), mails) != NULL) {
                fputs(mailsBuffer, stdout);
            }
            pclose(mails);
        }
        else if (strcmp(argv[1], "data")) {
            FILE* mails = popen("mail | sort -k4", "r");
            while (fgets(mailsBuffer, sizeof(mailsBuffer), mails) != NULL) {
                fputs(mailsBuffer, stdout);
            }
            pclose(mails);
        }
        else {
            printf("Wrong argument\n");
            return 1;
        }
    }
    else if (argc == 4) {

        char command[1024];
        sprintf(command, "mail -s \"%s\" %s | sort -k4", argv[2], argv[1]);
        FILE* sendMail = popen(command, "r");
        fputs(argv[3], sendMail);
        pclose(sendMail);
    }
    else {
        printf("Wrong number of arguments\n");
        return 1;
    }
    return 0;
}