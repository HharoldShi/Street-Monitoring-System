#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits>
#include <vector>
#include <cmath>
using namespace std;


int randint(int lnum, int hnum){
    std::ifstream urandom("/dev/urandom");
    unsigned int num;
//    cout << "size of num is " << sizeof(num ) << "\n";
//    cout << "l and h is " << lnum << ", " << hnum <<"\n";
    urandom.read((char *) &num, sizeof(num));
    urandom.close();
    if (hnum == lnum){
        return hnum;
    }else{
        int number = (num%(hnum - lnum + 1));
        number = number + lnum;
        return number;
    }
}


bool zero_len_edge(vector<int> point1, vector<int> point2){
    int x1 = point1[0], y1 = point1[1], x2 = point2[0], y2 = point2[1];
    return x1 == x2 and y1 == y2;
}


bool is_edge_overlap(vector<int> point1, vector<int> point2, vector<int> point3, vector<int> point4){
    int x1, x2, x3, x4, y1, y2, y3, y4;
    if (point1[0] == point2[0] and point3[0] == point4[0] and point1[0] == point3[0]){
        if (point1[1] < point2[1]){
            y1 = point1[1], y2 = point2[1];
        }else{
            y1 = point2[1], y2 = point1[1];
        }
        if (point3[1] < point4[1]){
            y3 = point3[1], y4 = point4[1];
        }else{
            y3 = point4[1], y4 = point3[1];
        }

        if (y1 == y3 and y2 == y4){
            return true;
        }
        if ( (y3 > y1 && y3 < y2) || (y4 > y1 && y4 < y2) || (y1 > y3 && y1 < y4) || (y2 > y3 && y2 < y4) ){
            return true;
        }
    }
    if (point1[0] != point2[0] and point3[0] != point4[0]){
        if (point1[0] < point2[0]){
            x1 = point1[0], y1 = point1[1], x2 = point2[0], y2 = point2[1];
        }else{
            x1 = point2[0], y1 = point2[1], x2 = point1[0], y2 = point1[1];
        }
        if (point3[0] < point4[0]){
            x3 = point3[0], y3 = point3[1],  x4 = point4[0], y4 = point4[1];
        }else{
            x3 = point4[0], y3 = point4[1], x4 = point3[0], y4 = point3[1];
        }
        double slope1 = (y2- y1);
        slope1 = slope1 / (x2 - x1);
        double slope2 = (y4 - y3);
        slope2 = slope2 / (x4 - x3);
        double b1 = (x2 * y1 - x1 * y2);
        b1 = b1 / (x2 - x1);
        double b2 = (x4 * y3 - x3 * y4);
        b2 = b2 / (x4 - x3);
        if (slope1 == slope2 && b1 == b2) {
            if (x1 == x3 and x2 == x4) {
                return true;
            }
            if ( (x3 > x1 && x3 < x2) || (x4 > x1 && x4 < x2) || (x1 > x3 && x1 < x4) || (x2 > x3 && x2 < x4) ){
                return true;
            }
        }
    }
    return false;
}


bool are_edges_intersect(vector<int> point1, vector<int> point2, vector<int> point3, vector<int> point4){
    int x1, x2, x3, x4, y1, y2, y3, y4;
    if (point1[0] != point2[0] and point3[0] != point4[0]) {
        if (point1[0] < point2[0]) {
            x1 = point1[0], y1 = point1[1], x2 = point2[0], y2 = point2[1];
        } else {
            x1 = point2[0], y1 = point2[1], x2 = point1[0], y2 = point1[1];
        }
        if (point3[0] < point4[0]) {
            x3 = point3[0], y3 = point3[1], x4 = point4[0], y4 = point4[1];
        } else {
            x3 = point4[0], y3 = point4[1], x4 = point3[0], y4 = point3[1];
        }
        double k1 = (y2 - y1);
        k1 = k1 / (x2 - x1);
        double k2 = (y4 - y3);
        k2 = k2 / (x4 - x3);
        double b1 = (x2 * y1 - x1 * y2);
        b1 = b1 / (x2 - x1);
        double b2 = (x4 * y3 - x3 * y4);
        b2 = b2 / (x4 - x3);
        if (k1 != k2){
            double ix = (b2 - b1);
            ix = ix / (k1 - k2);
            double iy = (k1 * b2 - k2 * b1);
            iy = iy / (k1 - k2);
            if (ix >= x1 && ix <= x2 && iy >= y1 && iy <= y2){
                return true;
            }
        }
    }
    if (point1[0] != point2[0] and point3[0] == point4[0]) {
        if (point3[1] < point4[1]) {
            y3 = point3[1], y4 = point4[1];
        } else {
            y3 = point4[1], y4 = point3[1];
        }
        x1 = point1[0], y1 = point1[1], x2 = point2[0], y2 = point2[1], x3 = point3[0];
        double k1 = (y2 - y1);
        k1 = k1 / (x2 - x1);
        double b1 = (x2 * y1 - x1 * y2);
        b1 = b1 / (x2 - x1);
        double y_int;
        y_int = k1 * x3 + b1;
        if (y_int <= y4 and y_int >= y3){
            return true;
        }
    }
    if (point1[0] == point2[0] and point3[0] != point4[0]) {
        if (point1[1] < point2[1]) {
            y1 = point1[1], y2 = point2[1];
        } else {
            y1 = point2[1], y2 = point1[1];
        }
        x3 = point3[0], y3 = point3[1], x4 = point4[0], y4 = point4[1], x1 = point1[0];
        double k2 = (y4 - y3);
        k2 = k2 / (x4 - x3);
        double b2 = (x4 * y3 - x3 * y4);
        b2 = b2 / (x4 - x3);
        double y_int;
        y_int = k2 * x1 + b2;
        if (y_int <= y2 and y_int >= y1){
            return true;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    int k;
//    const char *optstring = "s:n:l:c:";
    string s_string, n_string, l_string, c_string;
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    while ((k = getopt(argc, argv, "s:n:l:c:")) != -1) {
        switch (k) {
            case 's':
                s_string = optarg;
                s = atoi(s_string.c_str());
//                cout << "opt is s " << s << "\n";
                break;
            case 'n':
                n_string = optarg;
                n = atoi(n_string.c_str());
//                cout << "opt is n " << n << "\n";
                break;
            case 'l':
                l_string = optarg;
                l = atoi(l_string.c_str());
//                cout << "opt is l " << l << "\n";
                break;
            case 'c':
                c_string = optarg;
                c = atoi(c_string.c_str());
//                cout << "opt is c " << c << "\n";
                break;
        }
    }

    vector<string> streetList;
    while(1){
        int st = 1;
        int sf = randint(2, s); // # of streets
        int lf = randint(5, l); // # of seconds to wait
        vector< vector< vector<int> > > graph;
        int flag5 = 0;
        // build graph
        int count2 = 0;
        while (flag5 == 0) {
            count2 += 1;
            if (count2 > 50) {
                cerr << "Error: 2failed to generate valid input for 50 simultaneous attempts. \n";
                exit(1);
            }
            graph.clear();
            while (st <= sf) {
                int nf = randint(1, n);
                vector<vector<int> > street;
                //            street.clear();
                int nt = 1; // nf + 1 is # of points in a street
                // build street
                while (nt <= nf + 1) {
                    vector<int> point;
                    //                point.clear();
                    point.push_back(randint(-c, c));
                    point.push_back(randint(-c, c));
                    if (!street.empty()) {
                        int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;
                        int count = 0;
                        while (flag1 == 0 || flag2 == 0 || flag3 == 0 || flag4 == 0) {
                            count += 1;
                            if (count > 50) {
                                cerr << "Error: 1failed to generate valid input for 50 simultaneous attempts. \n";
                                exit(1);
                            }
                            point[0] = randint(-c, c);
                            point[1] = randint(-c, c);

                            //check zero length edge
                            if (!zero_len_edge(point, street.back())) {
                                flag1 = 1;
                            } else {
                                flag1 = 0;
                            }

                            int flag22 = 1;
                            //check overlap with previous streets
                            if (graph.size() > 1) {
                                for (unsigned int i = 0; i < graph.size() - 1; i++) {
                                    for (unsigned int j = 0; j < graph[i].size() - 1; j++) {
                                        if (is_edge_overlap(graph[i][j], graph[i][j + 1], street.back(), point)) {
//                                            int x1 = graph[i][j][0], y1 = graph[i][j][1];
//                                            int x2 = graph[i][j + 1][0], y2 = graph[i][j + 1][1];
//                                            int x3 = street.back()[0], y3 = street.back()[1];
//                                            int x4 = point[0], y4 = point[1];
//
//                                            cout << x1 << ", " << y1 << "\n";
//                                            cout << x2 << ", " << y2 << "\n";
//                                            cout << x3 << ", " << y3 << "\n";
//                                            cout << x4 << ", " << y4 << "\n";
                                            flag22 = 0;
                                        }
                                    }
                                }
                            }
                            if (flag22 != 0) {
                                flag2 = 1;
                            } else {
                                flag2 = 0;
                            }

                            int flag33 = 0;
                            //check overlap with its own street
                            if (street.size() > 1) {
                                for (unsigned int i = 0; i < street.size() - 1; i++) {
                                    if (is_edge_overlap(street[i], street[i + 1], street.back(), point)) {
                                        flag33 = 1;
                                    }
                                }
                            }
                            if (flag33 == 1) {
                                flag3 = 0;
                            } else {
                                flag3 = 1;
                            }

                            int flag44 = 0;
                            //check intersect with its own street
                            if (street.size() > 2) {
                                for (unsigned int i = 0; i < street.size() - 2; i++) {
                                    if (are_edges_intersect(street[i], street[i + 1], street.back(), point)) {
                                        //                                    int x1 = street[i][0], y1 = street[i][1];
                                        //                                    int x2 = street[i+1][0], y2 = street[i+1][1];
                                        //                                    int x3 = street.back()[0], y3 = street.back()[1];
                                        //                                    int x4 = point[0], y4 = point[1];
                                        //
                                        //                                    cout << x1 <<", " << y1 <<"\n";
                                        //                                    cout << x2 <<", " << y2 <<"\n";
                                        //                                    cout << x3 <<", " << y3 <<"\n";
                                        //                                    cout << x4 <<", " << y4 <<"\n";
                                        flag44 = 1;
                                    }
                                }
                            }
                            if (flag44 == 1) {
                                flag4 = 0;
                            } else {
                                flag4 = 1;
                            }
                        }

                        //check intersection with previous streets
                        if (flag5 == 0) {
                            if (graph.size() > 1) {
                                for (unsigned int i = 0; i < graph.size() - 1; i++) {
                                    for (unsigned int j = 0; j < graph[i].size() - 1; j++) {
                                        if (are_edges_intersect(graph[i][j], graph[i][j + 1], street.back(), point)) {
                                            flag5 = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //                cout << point[0] << ", " << point[1]<<"\n";
                    street.push_back(point);
                    nt++;
                }
                graph.push_back(street);
                st++;
            }
        }

        for (unsigned int i = 0; i < graph.size(); i++){
            streetList.push_back("street " + to_string(i+1));
            cout << "a \"street " << i + 1 << "\"";
            for (unsigned int j = 0; j < graph[i].size(); j++){
                cout << " (" << graph[i][j][0] << "," << graph[i][j][1] << ")";
            }
            cout << endl;
        }
        cout << "g" << endl;
        sleep(lf);
        if (!streetList.empty()){
            for (unsigned int i = 0; i < streetList.size(); i++){
                cout << "r \"" << streetList[i] <<"\"" << endl;
            }
        }
        streetList.clear();
    }
    return 0;
}
