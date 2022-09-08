#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

void print(std::vector<double> &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}

// 1. a function to find the sum of a numeric vector
double find_sum(std::vector<double> &input)
{
    double sum = 0;
    for (int i = 0; i < input.size(); i++) {
        sum += input.at(i);
    }
    return sum;
}

// 2. a function to find the mean of a numeric vector
double find_mean(std::vector<double> &input) {
    double sum = find_sum(input);
    double mean = sum / input.size();
    return mean;
}

// 3. a function to find the median of a numeric vector
double find_median(std::vector<double> &input)
{
    sort(input.begin(), input.end());
    int numObservations = input.size();
    if (numObservations % 2 == 1) { // if number of vector elements is odd
        //cout << input[numObservations/2] << endl;
        return input[numObservations/2];
    } else {
        double median = ( input[numObservations/2] + input[numObservations/2 + 1] ) / 2;
        return median;
    }
}

// 4. a function to find the range of a numeric vector
double find_range(std::vector<double> &input) {
    sort(input.begin(), input.end());
    int numObservations = input.size();
    // cout << input[numObservations-1] << " " << input[0] << endl;
    return input[numObservations-1] - input[0];
} 

// 5. a function to find the covariance between rm and medv
double find_covariance(std::vector<double> &rm, std::vector<double> &medv) {
    // finding mean for rm and medv
    double mean_rm = find_mean(rm);
    double mean_medv = find_mean(medv);

    double product = 1;
    for (int i = 0; i < rm.size(); i++) {
        // calculating difference between x and mean(x) AND y and mean(y)
        double diff_rm = rm.at(i) - mean_rm;
        double diff_medv = medv.at(i) - mean_medv;

        // calculating sum of products of differences
        product += diff_rm* diff_medv;
    }   
    return product / (rm.size() - 1);
}

double find_correlation(std::vector<double> &rm, std::vector<double> &medv) {
    
    double covar = find_covariance(rm, medv);

    // finding mean for rm and medv
    double mean_rm = find_mean(rm);
    double mean_medv = find_mean(medv);

    double sum_rm = 0, sum_medv = 0;
    for (int i = 0; i < rm.size(); i++) {
        // calculating difference between x and mean(x) AND y and mean(y)
        double diff_rm = rm.at(i) - mean_rm;
        double diff_medv = medv.at(i) - mean_medv;

        // calculating sum of products of differences
        sum_medv += diff_medv * diff_medv;
        sum_rm += diff_rm * diff_rm;
    } 

    double var_squared_rm = sum_rm / (rm.size() - 1);  
    double var_squared_medv = sum_medv / (medv.size() - 1);   

    double var_rm = sqrt(var_squared_rm);
    double var_medv = sqrt(var_squared_medv);

    return covar / (var_rm * var_medv);
}

int main(int argc, char** argv) {
    ifstream inFS;  // Input file stream
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    // Try to open file
    cout << "Opening file Boston.csv" << endl;

    inFS.open("Boston.csv");
    if (!inFS.is_open()) {
        cout << "Could not open file Boston.csv." << endl;
        return 1;
    }

    //cout << "Reading line 1" << endl;
    getline(inFS, line);

    // echo heading
    //cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    //cout << "new length " << rm.size() << endl;

    cout << "Closing file Boston.csv.\n" << endl;

    inFS.close();

    //cout << "Number of records: " << numObservations << endl;
    
    //debug
    //print(rm);
    //print(medv);

    // Calling functions described in 1-4 for rm
    cout << "Calling functions for rm:" << endl;
    double sum_rm = find_sum(rm);
    cout << "Sum: " << sum_rm << endl;

    double mean_rm = find_mean(rm);
    cout << "Mean: " << mean_rm << endl;

    double median_rm = find_median(rm);
    cout << "Median: " << median_rm << endl;

    double range_rm = find_range(rm);
    cout << "Range: " << range_rm << endl;

    // Calling functions described in 1-4 for medv
    cout << "\nCalling functions for medv:" << endl;
    double sum_medv = find_sum(medv);
    cout << "Sum: " << sum_medv << endl;

    double mean_medv = find_mean(medv);
    cout << "Mean: " << mean_medv << endl;

    double median_medv = find_median(medv);
    cout << "Median: " << median_medv << endl;

    double range_medv = find_range(medv);
    cout << "Range: " << range_medv << endl;

    // Calling covariance function for both
    cout << "\nCalling functions for both:" << endl;
    double covar = find_covariance(rm, medv);
    cout << "Covariance: " << covar << endl;

    // Calling correlation function for both
    double corre = find_correlation(rm, medv);
    cout << "Correlation: " << corre << endl;

   /* cout << "\nStats for rm" << endl;
    print_stats(rm);

    cout << "\nStats for medv" << endl;
    print_stats(medv);

    cout << "\n Covariance = " << covar(rm, medv) << endl;
    cout << "\n Correlation = " << cor(rm, medv) << endl; */
    cout << "\n Program terminated.";
    //std::cout << "Hello World!";
    return 0;
}

