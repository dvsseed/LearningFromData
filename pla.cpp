//
// Perceptron Learning Algorithm for no.15 of quiz 1st
// Created by Davis on 2018/7/7.
// Modified by Davis on 2018/7/13.
// -----------------------------------------------
//   customer features x: points in R4
//   labels y: o(+1), x(-1)
//   hypothesis h: lines, h(x) = sign(w0 + w1x1 + w2x2)
//   perceptrons <=> linear (binary) classifiers
// -----------------------------------------------
//
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <string>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

class PLA {
    // preprocessor
    #define DIMENSION 5  // d-dimensional Euclidean space, problem #15 is 4-dimensions

    // to declare a variable of type
    // unsigned short debug = 0;  // for debug=1(show debug text)
    unsigned short updates = 0;  // the numbers of updates
    unsigned long n = 0;  // the numbers of training examples

    // training examples: (input = x, output = y)
    struct trainingExamples {
        double input[DIMENSION];
        short int output;
    };

    // to store the features(inputs) and outputs to vector, lines by lines from the file of data set
    vector<trainingExamples> dataSet;

public:
    double weight[DIMENSION]{};  // weight

    unsigned short x0;  // to assign the value of x0
    string datFile;  // to assign the file of D: training examples

    // constructor
    PLA(unsigned short x0, string datFile) {
        this->x0 = x0;
        this->datFile = std::move(datFile);
    }

    // destructor
    ~PLA () {
        std::cout << "End of class!" << std::endl;
    }

    // to read the dat file into the training example's vector
    void getData(ifstream &datFile) {
        while (!datFile.eof()) {
            struct trainingExamples currentTraining{};
            currentTraining.input[0] = x0;  // to add x0 = +1 to each xn
            for (unsigned short i = 1; DIMENSION > i; i++) {
                datFile >> currentTraining.input[i];  // space-separated between each line
            }
            datFile >> currentTraining.output;
            /* for debug
            std::cout << "  <dataset> " << currentTraining.output << std::endl;
            */
            dataSet.push_back(currentTraining);  // adds a new element at the end of the vector
        }
        datFile.close();
        n = dataSet.size();
    }

    // sign value
    short int sign(double x) {
        if (x <= 0) {
            return -1;  // sign(0) as -1
        } else {
            return 1;
        }
    }

    // to calculate y*x (vector dot product) and to store result
    void multiply(double *result, const double *x, unsigned short dimension, short int y) {
        for (unsigned short i = 0; i < dimension; i++) {
            result[i] = y * x[i];

            /* for debug
            std::cout << "  <result " << i << "> " << result[i] << std::endl;
            */
        }
    }

    // to add the two vectors, to store the result to the first tuple, to calculate : w(t+1) <- w(t) + y(t) * x(t)
    void add(double *w, const double *yx, unsigned short dimension) {
        for (unsigned short t = 0; t < dimension; t++) {
            w[t] += yx[t];
            /* for debug
            std::cout << "  <w" << t << "> " << w[t] << std::endl;
            */
        }
    }

    // h(x) = sign(wt * x), find a mistake of wt called ( xn(t), yn(t) ) then correct the mistake
    double multiply(const double *w, const double *x, unsigned short dimension) {
        double temp = 0.0;
        for (unsigned short t = 0; t < dimension; t++) {
            temp += w[t] * x[t];
        }

        /* for debug
        std::cout << "  <temp> " << temp << std::endl;
        */
        return temp;
    }

    // to fill the zero
    std::string fillZero(unsigned short n, int number) {
        std::stringstream buffer;
        buffer << setw(n) << setfill('0') << number;
        return buffer.str();
    }

    // standard output stream
    void dupstr() {
        std::cout << "===============================================" << std::endl;
    }

    // Perceptron Learning Algorithm
    void PerceptronLearningAlgorithm() {
        unsigned short correctNum = 0;  // correct counter
        unsigned short index = 0;  // example counter
        unsigned short runno = 0;
        bool isFinished = false;  // =true means no more mistakes

        // header
        dupstr();
        std::cout << "******  To execute the PLA  ( n = " << n << " )  ******" << std::endl;
        dupstr();

        // learning algorithm
        while (!isFinished) {
            // h(x) = sign(w0 + w1x1 + w2x2 + w3x3 + w4x4)
            if (sign(multiply(weight, dataSet[index].input, DIMENSION)) == dataSet[index].output) {
                /* for debug
                std::cout << "  <weight> " << weight[0] << "," << weight[1] << "," << weight[2] << "," << weight[3] << "," << weight[4] << std::endl;
                std::cout << "  <input> " << dataSet[index].input[0] << "," << dataSet[index].input[1] << "," << dataSet[index].input[2] << "," << dataSet[index].input[3] << "," << dataSet[index].input[4] << std::endl;
                std::cout << "  <output> " << dataSet[index].output << std::endl;
                std::cout << "  <sign> " << sign( multiply( weight, dataSet[ index ].input, DIMENSION ) ) << std::endl;
                */

                correctNum++;  // correct add 1
            } else {  // find a mistake of wt, sign( wt * xn(t) ) != yn(t)
                double temp[DIMENSION]{};
                multiply(temp, dataSet[index].input, DIMENSION, dataSet[index].output);  // to calculate : y * x
                add(weight, temp, DIMENSION);  // (try to) correct the mistake by w(t+1) <- w(t) + yn(t) * xn(t)

                /* for debug
                std::cout << "  <temp> " << temp[0] << "," << temp[1] << "," << temp[2] << "," << temp[3] << "," << temp[4] << std::endl;
                std::cout << "  <weight> " << weight[0] << "," << weight[1] << "," << weight[2] << "," << weight[3] << "," << weight[4] << std::endl;
                std::cout << "  <input> " << dataSet[index].input[0] << "," << dataSet[index].input[1] << "," << dataSet[index].input[2] << "," << dataSet[index].input[3] << "," << dataSet[index].input[4] << std::endl;
                std::cout << "  <output> " << dataSet[index].output << std::endl;
                */

                updates++;  // the number of updates
                correctNum = 0;  // find a mistake so return to 0
                std::cout << " Update: " << fillZero(2, updates) << "  ( Index = " << fillZero(3, index + 1) << ", y = " << dataSet[index].output << " is wrong! )" << std::endl;
            }
            if (index == n - 1) {
                index = 0;
                runno++;
                std::cout << " >>Times: " << runno << "<<" << std::endl;
            } else {
                index++;
            }
            // until no more mistakes
            if (correctNum == n) {  // =n means no more mistakes
                isFinished = true;  // the algorithm halts
            }
        }

        // footer
        dupstr();
        std::cout << "******       Final update: [  " << updates << "  ]      ******" << std::endl;
        dupstr();
    }

    // to get the current working directory(path)
    std::string GetCurrentWorkingDir() {
        char buff[FILENAME_MAX]{};
        GetCurrentDir(buff, FILENAME_MAX);
        std::string currentWorkingDir(buff);
        return currentWorkingDir;
    }

};  // class

int main() {
    // std::cout << "Current directory is " << GetCurrentWorkingDir() << std::endl;

    unsigned short cx0 = 1;                   // to assign the value of x0 = 1
    string cdatFile = "../hw1_15_train.dat";  // to assign the file of D: training examples D: (x1, y1), ... , (xn, yn)
    // The Data set's URL is ==> https://www.csie.ntu.edu.tw/~htlin/mooc/datasets/mlfound_math/hw1_15_train.dat
    PLA cPLA(cx0, cdatFile);

    // tranining exmples
    ifstream dataFile(cPLA.datFile);
    if (dataFile.is_open()) {
        cPLA.getData(dataFile);
    } else {
        std::cout << "Error opening file!" << std::endl;
        exit(1);
    }

    // start from some w0 and correct its mistakes on D
    // initialize with w = 0 and take sign(0) as -1
    for (double &i : cPLA.weight) {
        i = 0.0;
    }

    // learning algorithm
    cPLA.PerceptronLearningAlgorithm();

    return 0;
}