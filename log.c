/*Write your won logfile in c.*/
#include <stdio.h>
#define LOG(X, Y) fprintf (fp, #X ": Time:%s, File:%s(%d) " #Y  "\n", __TIMESTAMP__, __FILE__, __LINE__)

int main(int args, char **argv)
{
    FILE *fp= fopen("logfile.txt", "w"); //("c:\\temp\\logfile.txt")

    LOG(INFO, "File open success.");
    LOG(WARN, "File path missing.");
    LOG(ERROR, "File close faild.");

    fclose(fp);
return 0;
}
