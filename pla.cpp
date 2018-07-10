//
// Perceptron Learning Algorithm for no.15 of quiz 1
// Created by Davis on 2018/7/7.
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

double weight[ DIMENSION ];  // weight
int updates = 0;  // the numbers of updates
int n = 0;  // the numbers of training examples
auto datFile = "../hw1_15_train.dat";  // the file of D: training examples
// The URL is ==> https://d396qusza40orc.cloudfront.net/ntumlone%2Fhw1%2Fhw1_15_train.dat

// training examples: (input = x, output = y)
struct trainingExamples {
    double input[ DIMENSION ];
    int output;
};

// 把记录存在向量里而不是存在结构体数组内，这样可以根据实际一项项添加
vector<trainingExamples> dataSet;

// to read the dat file into the training example's vector
void getData( ifstream &datFile ) {
    while ( !datFile.eof() ) {
        struct trainingExamples currentTraining{};
        currentTraining.input[ 0 ] = 1;  // to add x0 = 1 to each xn
        for ( int i = 1; i < DIMENSION; i++ ) {
            datFile >> currentTraining.input[ i ];
        }
        datFile >> currentTraining.output;
        dataSet.push_back( currentTraining );  // 添加元素至vector容器
    }
    datFile.close();
    n = dataSet.size();
}

// sign value
int sign( double x ) {
    if ( x <= 0 ) {
        return -1;  // sign(0) as -1
    } else {
        return 1;
    }
}

// to add the two vectors, to store the result to the first tuple, to calculate : w(t+1) <- w(t) + y(t) * x(t)
void add( double *vec1, const double *vec2, int dimension ) {
    for ( int i = 0; i < dimension; i++ ) {
        vec1[ i ] += vec2[ i ];
    }
}

// 计算两数值相乘值，用于判断 w*x 是否小于0，若小于0要执行修正算法
double multiply( const double *vec1, const double *vec2, int dimension ) {
    double temp = 0.0;
    for ( int i = 0; i < dimension; i++ ) {
        temp += vec1[ i ] * vec2[ i ];
    }

    return temp;
}

// 计算实数 num 与 向量乘积 放在 result 中，用于计算 y*x
void multiply( double *result, const double *vec, int dimension, int num ) {
    for ( int i = 0; i < dimension; i++ ) {
        result[ i ] = num * vec[ i ];
    }
}

// Perceptron Learning Algorithm
void PLA() {
    int correctNum = 0;  // 当前连续正确样本数，当等于n则表明轮完一圈，则表示全部正确，算法结束
    int index = 0;  // 当前正在计算第几个样本
    bool isFinished = false;  // 算法是否全部完成的表示，=true表示算法结束

    std::cout << "====================================" << std::endl;
    std::cout << "*** To execute the PLA (n = " << n << ") ***" << std::endl;
    std::cout << "====================================" << std::endl;

    while ( !isFinished ) {
        if ( dataSet[ index ].output == sign( multiply( weight, dataSet[ index ].input, DIMENSION ) ) )
            correctNum++;  // 当前样本无错，连续正确样本数+1
        else {  // 出错，执行修正算法
            double temp[ DIMENSION ];
            multiply( temp, dataSet[ index ].input, DIMENSION, dataSet[ index ].output );  // to calculate : y * x
            add( weight, temp, DIMENSION );  // to calculate : w(t+1) = w(t) + y(t) * x(t)
            updates++;  // the number of updates
            correctNum = 0;  // 由于出错了，连续正确样本数归0
            std::cout << " Update: " << updates << "  ( Index = " << index << " is wrong! )" << std::endl;
        }
        if ( index == n - 1 ) {
            index = 0;
        } else {
            index++;
        }
        if ( correctNum == n ) {
            isFinished = true;  // the algorithm halts
        }
    }

    std::cout << "=======================================" << std::endl;
    std::cout << "***       Final update: [ " << updates << " ]      ***" << std::endl;
    std::cout << "=======================================" << std::endl;
}

// To get the current working directory(path)
std::string GetCurrentWorkingDir() {
    char buff[ FILENAME_MAX ];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string currentWorkingDir( buff );
    return currentWorkingDir;
}

int main() {
    // std::cout << "Current directory is " << GetCurrentWorkingDir() << std::endl;

    ifstream dataFile( datFile );
    if ( dataFile.is_open() ) {
        getData( dataFile );
    } else {
        std::cout << "Error opening file!" << std::endl;
        exit(1);
    }

    for ( double &i : weight ) {
        i = 0.0;
    }
    PLA();

    return 0;
}