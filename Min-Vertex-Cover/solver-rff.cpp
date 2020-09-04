#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <signal.h>
#include "graph.h"
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"


// Define a semaphore as SAT function timeout flag
static sem_t to_flag;

// Define a struct to store thread data and the return value of subthreads
struct thread_data {
    int vertex_num;
    float time;
    std::vector<std::vector<int>> vertex;
    std::vector<int> cover;
};

// This function is used to parse the input start with "V": 
// Return the total vertex numbers;
int ParseInputV(std::vector<std::string> command) {
    if(command.size() == 2) {
        int v_n = atoi(command[1].c_str());
        if(v_n >= 0) {
            return v_n;
        }
        else {
            std::cerr << "Error: Input error!\n";
            return 0;
        }
    }
    else {
       std::cerr << "Error: Input error!\n";
        return 0; 
    }
}

// This function is used to parse the input start with "E": 
// Parse the edges and store vertices into "vertex" as pairs and return;
std::vector<std::vector<int>> ParseInputE(std::vector<std::string> command) {
    std::vector<std::vector<int>> vertex;
    if(command.size() == 2) {
        std::string edge = command[1];
        // try to parse the edges to vertices and store into "vertex"
        try {
            for(unsigned i = 0; i < edge.length(); ++i) {
                if(edge.at(i) == '<') {
                    int n1, n2;
                    unsigned j = i + 1;
                    while(edge.at(j) != ',') j++;
                    n1 = atoi(edge.substr(i+1, j++).c_str());
                    i = j;
                    while(edge.at(j) != '>') j++;
                    n2 = atoi(edge.substr(i, j).c_str());
                    i = j;
                    std::vector<int> pair(2);                       
                    pair[0] = n1;
                    pair[1] = n2;
                    vertex.push_back(pair);
                }
            }
        }catch(std::exception& e) {
            std::cerr << "Error: Parsing input error:" << e.what() << "\n";
        }
    }
    else {
       std::cerr << "Error: Input error!\n";
    }
    return vertex;
}

// This function is used to generate real CPU time from a clockid
int Pclock(clockid_t cid) {
    struct timespec ts;
    if (clock_gettime(cid, &ts) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    int time = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    return time;
}

// Signal handler for timeout
void TimeOut(int n) {
    sem_post(&to_flag);
}

// CNF-SAT subthread function
void *CNF_SAT_VC(void *arg) {
    int retcode;
    struct thread_data *data;
    // Decode thread data
    data = (struct thread_data *) arg;
    auto graph = new Graph<int>;
    // Initialize the graph object
    graph->vert_num = data->vertex_num;
    graph->Generate(data->vertex);
    // Invoke the CNF_SAT_VC function
    data->cover = graph->CNF_SAT_VC();
    // Set the timeout flag and inform the main thread that the function is done
    sem_post(&to_flag);
    graph->~Graph();
    // Print the result
    std::cout << "CNF_SAT_VC: ";
    for(int i = 0; i < data->cover.size(); ++i) {
        if(i == data->cover.size() - 1) {
            std::cout << data->cover[i] << "\n";
        }
        else {
            std::cout << data->cover[i] << ",";
        }
    }
    // Compute the running time of the function
    clockid_t cid;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode) {
	    perror("pthread_getcpuclockid");
    }
    else {
	    data->time = Pclock(cid);
    }
}

// APPROX_VC_1 subthread function
void *APPROX_VC_1(void *arg) {
    struct thread_data *data;
    // Decode thread data
    data = (struct thread_data *) arg;
    auto graph = new Graph<int>;
    // Initialize the graph object
    graph->vert_num = data->vertex_num;
    graph->Generate(data->vertex);
    // Invoke the APPROX_VC_1 function
    data->cover = graph->APPROX_VC_1();
    graph->~Graph();
    // Print the result
    std::cout << "APPROX_VC_1: ";
    for(int i = 0; i < data->cover.size(); ++i) {
        if(i == data->cover.size() - 1) {
            std::cout << data->cover[i] << "\n";
        }
        else {
            std::cout << data->cover[i] << ",";
        }
    }
    // Compute the running time of the function
    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode) {
	    perror("pthread_getcpuclockid");
    }
    else {
	    data->time = Pclock(cid);
    }
}

// APPROX_VC_2 subthread function
void *APPROX_VC_2(void *arg) {
    struct thread_data *data;
    // Decode thread data
    data = (struct thread_data *) arg;
    auto graph = new Graph<int>;
    // Initialize the graph object
    graph->vert_num = data->vertex_num;
    graph->Generate(data->vertex);
    // Invoke the APPROX_VC_2 function
    data->cover = graph->APPROX_VC_2();
    graph->~Graph();
    // Print the result
    std::cout << "APPROX_VC_2: ";
    for(int i = 0; i < data->cover.size(); ++i) {
        if(i == data->cover.size() - 1) {
            std::cout << data->cover[i] << "\n";
        }
        else {
            std::cout << data->cover[i] << ",";
        }
    }
    // Compute the running time of the function
    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode) {
	    perror("pthread_getcpuclockid");
    }
    else {
	    data->time = Pclock(cid);
    }
}


int main(int argc, char **argv) {
    std::ofstream fout("output.csv", std::ios::app);
    fout << "V" << "," << "Time(SAT)" << "," << "Time(A1)" << "," << "Time(A2)" << "," << "Approx1" << "Approx2" << std::endl;
    fout.close();
    std::string line;
    std::fstream fin("input_graph.txt", std::ios::in);
    // Read each line of data from input file.
    while(getline(fin, line)) {
        // Define 3 thread data structs to store data from the input.
        struct thread_data td1, td2, td3;
        std::istringstream input(line);
        std::vector<std::string> command;
        // Read the input string by stream.
        while (!input.eof()) {
            std::string s;
            input >> s;
            if (input.fail()) {
                break;
            }
            else {
                command.push_back(s);
            }
            if (input.eof()) {
                break;
            }
        }
        // Parse inputs in different situations.
        if(command.size() == 0) {
            continue;
        }
        if(command[0] == "V") {
            // Store the number of vertices
            td1.vertex_num = ParseInputV(command);
            td2.vertex_num = ParseInputV(command);
            td3.vertex_num = ParseInputV(command);
            continue;
        }
        if(command[0] == "E") {
            std::ofstream fout("output.csv", std::ios::app);
            fout << td1.vertex_num << ",";
            // Store the coordinates of vertices
            td1.vertex = ParseInputE(command);
            td2.vertex = ParseInputE(command);
            td3.vertex = ParseInputE(command);

            // Initialize the timeout semaphore
            sem_init(&to_flag, 0, -1);
            int retcode;
            // Register the handler function to SIGALRM
            signal(SIGALRM, TimeOut);
            struct itimerval tick;
            // Initialize itimerval struct
            memset(&tick, 0, sizeof(tick));
            // Set the timeout value
            tick.it_value.tv_sec = 15;
            tick.it_value.tv_usec = 0;
            // Start the timer
            retcode = setitimer(ITIMER_REAL, &tick, NULL);
            // If setting timer failed print error message
            if(retcode) {
                perror("setitimer");
            }

            // Start 3 subthreads to run 3 functions
            pthread_t t1, t2, t3;
            retcode = pthread_create(&t1, NULL, CNF_SAT_VC, (void *)&td1);
            if(retcode != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            retcode = pthread_create(&t2, NULL, APPROX_VC_1, (void *)&td2);
            if(retcode != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            retcode = pthread_create(&t3, NULL, APPROX_VC_2, (void *)&td3);
            if(retcode != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
            // Wait for timeout or the complete of t1
            sem_wait(&to_flag);
            // If t1 times out then cancel the thread
            if(td1.cover.size() == 0) {
                solver->interrupt();
                retcode = pthread_cancel(t1);
                if(retcode) {
                    perror("pthread_cancel");
                }
                retcode = pthread_detach(t1);
                if(retcode) {
                    perror("pthread_detach");
                }
                std::cout << "CNF_SAT_VC: Time out\n"; 
                retcode = pthread_join(t2, NULL);
                if(retcode) {
	                perror("pthread_join");
                }
                retcode = pthread_join(t3, NULL);
                if(retcode) {
	                perror("pthread_join");
                }
                fout << "TO" << "," << td2.time << "," << td3.time << "," << "N/A" << "," << "N/A" << std::endl;
            }
            // If t1 completes in time then cancel the timer and output the approximation ratio information
            else {
                tick.it_value.tv_sec = 0;
                tick.it_value.tv_usec = 0;
                retcode = setitimer(ITIMER_REAL, &tick, NULL);
                if(retcode) {
                    perror("setitimer");
                }
                retcode = pthread_join(t1, NULL);
                if(retcode) {
	                perror("pthread_join");
                }
                retcode = pthread_join(t2, NULL);
                if(retcode) {
	                perror("pthread_join");
                }
                retcode = pthread_join(t3, NULL);
                if(retcode) {
	                perror("pthread_join");
                }
                float a1 = (float)td2.cover.size() / (float)td1.cover.size();
                float a2 = (float)td3.cover.size() / (float)td1.cover.size();
                fout << td1.time << "," << td2.time << "," << td3.time << "," << a1 << "," << a2 << std::endl;
            }
            fout.close();
            continue;
        }
        std::cerr << "Error: Input error!\n";
    }
    fin.close();
    return 0;
}
