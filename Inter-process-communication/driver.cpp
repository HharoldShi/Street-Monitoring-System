#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main (int argc, char **argv) {
    std::vector<pid_t> kids;
    // create a pipe
    int rgentoa1[2], a1a3toa2[2];
    pipe(rgentoa1);
    pipe(a1a3toa2);

    pid_t rgen_pid, a1_pid, a2_pid;
    rgen_pid = fork();
    if (rgen_pid == 0){
        dup2(rgentoa1[1], STDOUT_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        close(a1a3toa2[0]);
        close(a1a3toa2[1]);
        char *arg1[] = {(char *)"./rgen", reinterpret_cast<char *>(argv), nullptr};
        execv(arg1[0], arg1);
        if (execv(arg1[0], arg1) == -1){
            cerr << strerror(int errno) << "\n";
        }
    }

    kids.push_back(rgen_pid);

    a1_pid = fork();
    if (a1_pid == 0){
        dup2(rgentoa1[0],STDIN_FILENO);
        dup2(a1a3toa2[1],STDOUT_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        close(a1a3toa2[0]);
        close(a1a3toa2[1]);
        char* arg2[] = {(char *)"./ece650-a1.py", nullptr};
        execv(arg2[0], arg2);
        if (execv(arg2[0],arg2) == -1){
            cerr << strerror(int errno) << "\n";
            abort();
        }
    }
    kids.push_back(a1_pid);
    a2_pid = fork();
    if(a2_pid == 0){
        dup2(a1a3toa2[0],STDIN_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        close(a1a3toa2[0]);
        close(a1a3toa2[1]);
        char* arg3[] = {(char*)"./ece650-a2",nullptr};
        execv(arg3[0], arg3);
        if (execv(arg3[0],arg3) == -1){
            cerr << strerror(int errno) << "\n";
            abort();
        }
    }
    kids.push_back(a2_pid);
    dup2(a1a3toa2[1],STDOUT_FILENO);
    close(rgentoa1[0]);
    close(rgentoa1[1]);
    close(a1a3toa2[0]);
    close(a1a3toa2[1]);
    while(!cin.eof()){
        string input;
        getline(cin, input);
        cout << input << endl;
    }
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}
