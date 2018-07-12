//
// Perceptron Learning Algorithm for no.15 of quiz 1
// Created by Davis on 2018/7/7.
// Modified by Davis on 2018/7/12.
// -----------------------------------------------
//   customer features x: points in R4
//   labels y: o(+1), x(-1)
//   hypothesis h: lines, h(x) = sign(w0 + w1x1 + w2x2)
//   perceptrons <=> linear (binary) classifiers
// -----------------------------------------------
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#define DIMENSION 5  // d-dimensional Euclidean space

using namespace std;

unsigned short debug = 0;  // for debug=1(show debug)
double weight [ DIMENSION ];  // weight
int updates = 0;  // the numbers of updates
unsigned long n = 0;  // the numbers of training examples
auto datFile = "../hw1_15_train.dat";  // the file of D: training examples
// The URL is ==> https://www.csie.ntu.edu.tw/~htlin/mooc/datasets/mlfound_math/hw1_15_train.dat

// training examples: (input = x, output = y)
struct trainingExamples {
    double input [ DIMENSION ];
    int output;
};

// to store the features(inputs) and outputs to vector, lines by lines from the file of data set
vector < trainingExamples > dataSet;

// to read the dat file into the training example's vector
void getData ( ifstream &datFile ) {
    while ( !datFile.eof ( ) ) {
        struct trainingExamples currentTraining { };
        currentTraining.input [ 0 ] = 1;  // to add x0 = +1 to each xn
        for ( int i = 1; DIMENSION > i; i++ ) {
            datFile >> currentTraining.input [ i ];  // space-separated between each line
        }
        datFile >> currentTraining.output;
        /* for debug
        std::cout << "  <dataset> " << currentTraining.output << std::endl;
        */
        dataSet.push_back ( currentTraining );  // adds a new element at the end of the vector
    }
    datFile.close ( );
    n = dataSet.size ( );
}

// sign value
int sign ( double x ) {
    if ( x <= 0 ) {
        return -1;  // sign(0) as -1
    } else {
        return 1;
    }
}

// to calculate y*x (vector dot product) and to store result
void multiply ( double *result, const double *x, int dimension, int y ) {
    for ( int i = 0; i < dimension; i++ ) {
        result [ i ] = y * x [ i ];

        /* for debug
        std::cout << "  <result " << i << "> " << result[i] << std::endl;
        */
    }
}

// to add the two vectors, to store the result to the first tuple, to calculate : w(t+1) <- w(t) + y(t) * x(t)
void add ( double *w, const double *yx, int dimension ) {
    for ( int t = 0; t < dimension; t++ ) {
        w [ t ] += yx [ t ];
        /* for debug
        std::cout << "  <w" << t << "> " << w[t] << std::endl;
        */
    }
}

// h(x) = sign(wt * x), find a mistake of wt called ( xn(t), yn(t) ) then correct the mistake
double multiply ( const double *w, const double *x, int dimension ) {
    double temp = 0.0;
    for ( int t = 0; t < dimension; t++ ) {
        temp += w [ t ] * x [ t ];
    }

    /* for debug
    std::cout << "  <temp> " << temp << std::endl;
    */
    return temp;
}

// Perceptron Learning Algorithm
void PLA ( ) {
    int correctNum = 0;  // correct counter
    int index = 0;  // example counter
    bool isFinished = false;  // =true means no more mistakes

    std::cout << "====================================" << std::endl;
    std::cout << "*** To execute the PLA (n = " << n << ") ***" << std::endl;
    std::cout << "====================================" << std::endl;

    while ( !isFinished ) {
        // h(x) = sign(w0 + w1x1 + w2x2 + w3x3 + w4x4)
        if ( sign ( multiply ( weight, dataSet [ index ].input, DIMENSION ) ) == dataSet [ index ].output ) {
            /* for debug
            std::cout << "  <weight> " << weight[0] << "," << weight[1] << "," << weight[2] << "," << weight[3] << "," << weight[4] << std::endl;
            std::cout << "  <input> " << dataSet[index].input[0] << "," << dataSet[index].input[1] << "," << dataSet[index].input[2] << "," << dataSet[index].input[3] << "," << dataSet[index].input[4] << std::endl;
            std::cout << "  <output> " << dataSet[index].output << std::endl;
            std::cout << "  <sign> " << sign( multiply( weight, dataSet[ index ].input, DIMENSION ) ) << std::endl;
            */

            correctNum++;  // correct add 1
        } else {  // find a mistake of wt, sign( wt * xn(t) ) != yn(t)
            double temp [ DIMENSION ];
            multiply ( temp, dataSet [ index ].input, DIMENSION, dataSet [ index ].output );  // to calculate : y * x
            add ( weight, temp, DIMENSION );  // (try to) correct the mistake by w(t+1) <- w(t) + yn(t) * xn(t)

            /* for debug
            std::cout << "  <temp> " << temp[0] << "," << temp[1] << "," << temp[2] << "," << temp[3] << "," << temp[4] << std::endl;
            std::cout << "  <weight> " << weight[0] << "," << weight[1] << "," << weight[2] << "," << weight[3] << "," << weight[4] << std::endl;
            std::cout << "  <input> " << dataSet[index].input[0] << "," << dataSet[index].input[1] << "," << dataSet[index].input[2] << "," << dataSet[index].input[3] << "," << dataSet[index].input[4] << std::endl;
            std::cout << "  <output> " << dataSet[index].output << std::endl;
            */

            updates++;  // the number of updates
            correctNum = 0;  // find a mistake so return to 0
            std::cout << " Update: " << updates << "  ( Index = " << index + 1 << " is wrong! )" << std::endl;
        }
        if ( index == n - 1 ) {
            index = 0;
        } else {
            index++;
        }
        // until no more mistakes
        if ( correctNum == n ) {  // =n means no more mistakes
            isFinished = true;  // the algorithm halts
        }
    }

    std::cout << "=======================================" << std::endl;
    std::cout << "***       Final update: [ " << updates << " ]      ***" << std::endl;
    std::cout << "=======================================" << std::endl;
}

// to get the current working directory(path)
std::string GetCurrentWorkingDir ( ) {
    char buff [ FILENAME_MAX ];
    GetCurrentDir ( buff, FILENAME_MAX );
    std::string currentWorkingDir ( buff );
    return currentWorkingDir;
}

int main ( ) {
    // std::cout << "Current directory is " << GetCurrentWorkingDir() << std::endl;

    ifstream dataFile ( datFile );
    if ( dataFile.is_open ( ) ) {
        getData ( dataFile );
    } else {
        std::cout << "Error opening file!" << std::endl;
        exit ( 1 );
    }

    for ( double &i : weight ) {
        i = 0.0;
    }
    PLA ( );

    return 0;
}