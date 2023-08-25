#include <iostream>
#include <chrono>

#include <vector>
#include <deque>
#include <list>
#include <array>

#include <algorithm>

#include <functional>

#include <fstream>

using namespace std;
using namespace chrono;

typedef uint64_t u64;
typedef high_resolution_clock hrc;

const bool DEBUG = true;
const string OUTPUT_FILE_PATH = "results.csv";
const int ITERATIONS = 10;

/*
Types: primitive
Operations: push_back, push_front, find, iterate, modify, insert, erase, sort, destroy
Containers: vector, deque, list
Special cases: vector with reserve for push_back and push_front
*/

template <typename T>
u64 push_back_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (int i=0; i<numElems; ++i) {
        container.push_back(i);
    } 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count(); // nanosekunde
}

template <typename T>
u64 push_front_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (int i=0; i<numElems; ++i) {
        container.push_front(i);
    } 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count(); // nanosekunde
}

template <typename T>
u64 push_front_vector_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (int i=0; i<numElems; ++i) {
        container.insert(container.begin(), i);
    } 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

const int TO_FIND = 1000;
template <typename T>
u64 find_bm(T& container, int numElems=1'000'000) {
    array<int, TO_FIND> searchFor;
    for (int i = 0; i < TO_FIND; ++i) {
        searchFor[i] = rand() % container.size();
    }
    hrc::time_point start = hrc::now();
    for (int i=0; i<TO_FIND; ++i) {
        find(container.begin(), container.end(), searchFor[i]);
    } 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

template <typename T>
u64 iterate_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (auto it=container.begin(); it!=container.end(); ++it) {} 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

template <typename T>
u64 iterate2_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (auto it=container.begin(); it!=container.end(); ++it) {*it;} 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

const int INCREMENT_VALUE = 10;
template <typename T>
u64 modify_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    for (auto it=container.begin(); it!=container.end(); ++it) {
        *it += INCREMENT_VALUE;
    } 
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

const int TO_INSERT = 1000;
const int NUM_INSERT = 10;
template <typename T>
u64 insert_bm(T& container, int numElems=1'000'000) {
    array<int, TO_INSERT> insertPositions;
    for (int i = 0; i < TO_INSERT; ++i) {
        insertPositions[i] = rand() % container.size();
    }
    duration<long, nano> duration(0);
    for (int ins: insertPositions) {
        int i = 0;
        hrc::time_point start = hrc::now();
        for (auto it=container.begin(); it!=container.end(); ++it, ++i) {
            if (i == ins) {
                container.insert(it, NUM_INSERT);
                hrc::time_point end = hrc::now();
                duration += end - start;
                break;
            }
        }   
    }
    
    return duration.count();
}

const int TO_ERASE = 1000;
template <typename T>
u64 erase_bm(T& container, int numElems=1'000'000) {
    array<int, TO_ERASE> erasePositions;
    for (int i = 0; i < TO_ERASE; ++i) {
        erasePositions[i] = rand() % numElems;
    }
    duration<long, nano> duration(0);
    for (int ins: erasePositions) {
        int i = 0;
        hrc::time_point start = hrc::now();
        for (auto it=container.begin(); it!=container.end(); ++it, ++i) {
            if (i == ins) {
                container.erase(it);
                hrc::time_point end = hrc::now();
                duration += end - start;
                break;
            }
        }
    }
    
    return duration.count();
}

template <typename T>
u64 sort_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    sort(container.begin(), container.end());
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

template <typename T>
u64 sort_list_bm(T& container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    container.sort();
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

template <typename T>
u64 destroy_bm(T* container, int numElems=1'000'000) {
    hrc::time_point start = hrc::now();
    delete container;
    hrc::time_point end = hrc::now();
    duration<long, nano> duration = end - start;
    return duration.count();
}

template<typename F, typename T>
void benchmark(string operation, string containerName, vector<T>& container, F fun, vector<int> numElements, ofstream& output) {
    cout << "Performance for " << operation << " on " << containerName << ": " << endl;
    string dataType = typeid(*(container[0].begin())).name();
    for (auto elIt = numElements.begin(), cIt = container.begin(); elIt < numElements.end(); ++elIt, ++cIt) {

        u64 measurement = fun(*cIt, *elIt);

        cout << "\t" << *elIt << " elements -> " << measurement / 1'000'000.0 << " ms";
        if (DEBUG) cout << " (" << cIt->size() << ")";
        cout << endl;
        output << containerName << "," << dataType << "," << operation << "," << *elIt << "," << measurement << endl;
    }
}

template<typename F, typename T>
void benchmark_destruction(string containerName, vector<T>& container, F fun, vector<int> numElements, ofstream& output) {
    cout << "Performance for DESTRUCTION on " << containerName << ": " << endl;
    string dataType = typeid(*(container[0]->begin())).name();
    for (auto elIt = numElements.begin(), cIt = container.begin(); elIt < numElements.end(); ++elIt, ++cIt) {

        u64 measurement = fun(*cIt, *elIt);
 
        cout << "\t" << *elIt << " elements -> " << measurement / 1'000'000.0 << " ms\n";
        output << containerName << "," << dataType << ",DESTRUCTION," << *elIt << "," << measurement << endl;
    }
}

template<typename T>
void shuffle(vector<T>& container) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        random_shuffle(it->begin(), it->end());
    }
}   

template<typename D>
void benchmark_all_num(vector<int> numElems, ofstream& output_file) {

    // Push back
    vector<vector<D>> vecNum(numElems.size());
    vector<deque<D>> deqNum(numElems.size());
    vector<list<D>> listNum(numElems.size());
    vector<vector<D>> vecNumReserve(numElems.size());

    auto elIt = numElems.begin();
    for (auto vecIt = vecNumReserve.begin(); vecIt != vecNumReserve.end(); ++vecIt, ++elIt) {
        vecIt->reserve(*elIt);
    }

    benchmark("PUSH BACK", "VECTOR", vecNum, push_back_bm<vector<D>>, numElems, output_file);
    benchmark("PUSH BACK", "DEQUE", deqNum, push_back_bm<deque<D>>, numElems, output_file);
    benchmark("PUSH BACK", "LIST", listNum, push_back_bm<list<D>>, numElems, output_file);
    benchmark("PUSH BACK", "VECTOR RESERVE", vecNumReserve, push_back_bm<vector<D>>, numElems, output_file);

    cout << "Shuffling..." << endl;
    shuffle(vecNum);
    shuffle(deqNum);
    // List can't be shuffled directly because it doesn't support random access iterators
    for (auto& list: listNum) {
        vector<D> vecTemp(list.begin(), list.end());
        random_shuffle(vecTemp.begin(), vecTemp.end());
        copy(vecTemp.begin(), vecTemp.end(), list.begin());
    }

    // Push front
    vector<vector<D>> vecNum2(numElems.size());
    vector<deque<D>> deqNum2(numElems.size());
    vector<list<D>> listNum2(numElems.size());
    vector<vector<D>> vecNumReserve2(numElems.size());

    elIt = numElems.begin();
    for (auto vecIt = vecNumReserve2.begin(); vecIt != vecNumReserve2.end(); ++vecIt, ++elIt) {
        vecIt->reserve(*elIt);
    }

    benchmark("PUSH FRONT", "VECTOR", vecNum2, push_front_vector_bm<vector<D>>, numElems, output_file);
    benchmark("PUSH FRONT", "DEQUE", deqNum2, push_front_bm<deque<D>>, numElems, output_file);
    benchmark("PUSH FRONT", "LIST", listNum2, push_front_bm<list<D>>, numElems, output_file);
    benchmark("PUSH FRONT", "VECTOR RESERVE", vecNumReserve2, push_front_vector_bm<vector<D>>, numElems, output_file);

    // Find
    benchmark("FIND", "VECTOR", vecNum, find_bm<vector<D>>, numElems, output_file);
    benchmark("FIND", "DEQUE", deqNum, find_bm<deque<D>>, numElems, output_file);
    benchmark("FIND", "LIST", listNum, find_bm<list<D>>, numElems, output_file);

    // Iterate
    benchmark("ITERATE", "VECTOR", vecNum, iterate_bm<vector<D>>, numElems, output_file);
    benchmark("ITERATE", "DEQUE", deqNum, iterate_bm<deque<D>>, numElems, output_file);
    benchmark("ITERATE", "LIST", listNum, iterate_bm<list<D>>, numElems, output_file);

    benchmark("ITERATE2", "VECTOR", vecNum, iterate2_bm<vector<D>>, numElems, output_file);
    benchmark("ITERATE2", "DEQUE", deqNum, iterate2_bm<deque<D>>, numElems, output_file);
    benchmark("ITERATE2", "LIST", listNum, iterate2_bm<list<D>>, numElems, output_file);

    // Iterate and modify
    benchmark("ITERATE AND MODIFY", "VECTOR", vecNum, modify_bm<vector<D>>, numElems, output_file);
    benchmark("ITERATE AND MODIFY", "DEQUE", deqNum, modify_bm<deque<D>>, numElems, output_file);
    benchmark("ITERATE AND MODIFY", "LIST", listNum, modify_bm<list<D>>, numElems, output_file);

    // Insert
    benchmark("INSERT", "VECTOR", vecNum, insert_bm<vector<D>>, numElems, output_file);
    benchmark("INSERT", "DEQUE", deqNum, insert_bm<deque<D>>, numElems, output_file);
    benchmark("INSERT", "LIST", listNum, insert_bm<list<D>>, numElems, output_file);

    // Erase
    benchmark("ERASE", "VECTOR", vecNum, erase_bm<vector<D>>, numElems, output_file);
    benchmark("ERASE", "DEQUE", deqNum, erase_bm<deque<D>>, numElems, output_file);
    benchmark("ERASE", "LIST", listNum, erase_bm<list<D>>, numElems, output_file);

    // Sort
    benchmark("SORT", "VECTOR", vecNum, sort_bm<vector<D>>, numElems, output_file);
    benchmark("SORT", "DEQUE", deqNum, sort_bm<deque<D>>, numElems, output_file);
    benchmark("SORT", "LIST", listNum, sort_list_bm<list<D>>, numElems, output_file);

    // Destruction
    vector<vector<D>*> vecNumDyn(numElems.size());
    vector<deque<D>*> deqNumDyn(numElems.size());
    vector<list<D>*> listNumDyn(numElems.size());

    for (int i = 0; i < numElems.size(); ++i) {
        vecNumDyn[i] = new vector<D>();
        deqNumDyn[i] = new deque<D>();
        listNumDyn[i] = new list<D>();

        for (int j = 0; j < numElems[i]; ++j) {
            vecNumDyn[i]->push_back(j);
            deqNumDyn[i]->push_back(j);
            listNumDyn[i]->push_back(j);
        }

        if (DEBUG) {
            cout << "Sizes: " << vecNumDyn[i]->size() << " " << deqNumDyn[i]->size() << " " << listNumDyn[i]->size() << endl;
        }
    }

    benchmark_destruction("VECTOR", vecNumDyn, destroy_bm<vector<D>>, numElems, output_file);
    benchmark_destruction("DEQUE", deqNumDyn, destroy_bm<deque<D>>, numElems, output_file);
    benchmark_destruction("LIST", listNumDyn, destroy_bm<list<D>>, numElems, output_file);

}

int main() {

    srand(time(NULL));

    vector<int> numElems = {100'000, 200'000, 300'000, 400'000, 500'000, 
                600'000, 700'000, 800'000, 900'000, 1'000'000};

    ofstream output_file(OUTPUT_FILE_PATH, ios_base::trunc);
    output_file << "Container,Data type,Operation,Elements,Time" << endl;

    for (int i = 0; i < ITERATIONS; ++i) {
        benchmark_all_num<u64>(numElems, output_file);
    }

    output_file.close();
    
    return 0;
}

