#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <chrono>
using namespace std;

double sigmoid(double x);
double model_accuracy(int numObservations, double probabilities[], vector<int> survived_test);
double model_sensitivity(int numObservations, double probabilities[], vector<int> survived_test);
double model_specificity(int numObservations, double probabilities[], vector<int> survived_test);

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double model_accuracy(int numObservations, double probabilities[], vector<int> survived_test) {
    int sum = 0;
    for (int i=800; i< numObservations; i++) {
        if (probabilities[i-800] == survived_test[i-800]) {
            //cout << sum << endl;
            sum += 1;
        }
    }

    //cout << sum << " " << sum*1.0/(numObservations-800) << endl;
    return sum*1.0 / (numObservations-800);
}

double model_specificity(int numObservations, double probabilities[], vector<int> survived_test) {
    int tp_sum = 0;
    int fn_sum = 0;
    //int tp2 = 0;
    //int fn2 = 0;
    for (int i=800; i< numObservations; i++) {
        if (probabilities[i-800] == 0 && probabilities[i-800] == survived_test[i-800]) {
            //cout << sum << endl;
            tp_sum += 1;
        } else if (probabilities[i-800] == 1 && survived_test[i-800] == 0) {
            fn_sum += 1;
        }
    }

    //cout << tp_sum << " " << fn_sum << " " << tp_sum*1.0000000 / (tp_sum+fn_sum) << endl;
    //cout << sum << " " << sum*1.0000000/(numObservations-800) << endl;
    return (double)tp_sum / (tp_sum+fn_sum);
}

double model_sensitivity(int numObservations, double probabilities[], vector<int> survived_test) {
    int tn_sum = 0;
    int fp_sum = 0;
    for (int i=800; i< numObservations; i++) {
        if (probabilities[i-800] == 1 && probabilities[i-800] == survived_test[i-800]) {
            //cout << sum << endl;
            tn_sum += 1;
        } else if (probabilities[i-800] == 0 && survived_test[i-800] == 1) {
            fp_sum += 1;
        }
    }

    //cout << tn_sum << " " << fp_sum << endl; // " " << tp_sum*1.0000000 / (tp_sum+fn_sum) << endl;
    //cout << sum << " " << sum*1.0000000/(numObservations-800) << endl;
    return (double)tn_sum / (tn_sum+fp_sum);
}

int main(int argc, char** argv) {
    ifstream inFS;  // Input file stream
    string line;
    string id_in, pclass_in, survived_in, sex_in, age_in;
    const int MAX_LEN = 1046;
    vector<int> id(MAX_LEN);
    vector<int> pclass(MAX_LEN);
    vector<int> survived(MAX_LEN);
    vector<int> sex(MAX_LEN);
    vector<int> age(MAX_LEN);

    // Try to open file
    cout << "Opening file titanic_project.csv" << endl;

    inFS.open("titanic_project.csv");
    if (!inFS.is_open()) {
        cout << "Could not open file titanic_project.csv." << endl;
        return 1;
    }

    //cout << "Reading line 1" << endl;
    getline(inFS, line);

    // echo heading
    //cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {
        //cout << numObservations ;

        getline(inFS, id_in, ',');
        getline(inFS, pclass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');

        //cout << id_in << " " << pclass_in << " survived: " << survived_in << " sex:" << sex_in << " age: " << age_in << endl;

        //cout << stoi(id_in.substr(1, id_in.length()))<<endl;
        
        id.at(numObservations) = stoi(id_in.substr(1, id_in.length()));
        pclass.at(numObservations) = stoi(pclass_in);
        survived.at(numObservations) = stoi(survived_in);
        sex.at(numObservations) = stoi(sex_in);
        age.at(numObservations) = stoi(age_in);

        numObservations++;
    }

    id.resize(numObservations);
    pclass.resize(numObservations);
    survived.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);

    //cout << "new length " << rm.size() << endl;
    
    // Dividing data into train and test data
    vector<int> id_train(800);
    vector<int> pclass_train(800);
    vector<int> survived_train(800);
    vector<int> sex_train(800);
    vector<int> age_train(800);

    vector<int> id_test(MAX_LEN-800);
    vector<int> pclass_test(MAX_LEN-800);
    vector<int> survived_test(MAX_LEN-800);
    vector<int> sex_test(MAX_LEN-800);
    vector<int> age_test(MAX_LEN-800);

    for (int i=0; i<800; i++) {
        id_train.at(i) = id.at(i);
        pclass_train.at(i) = pclass.at(i);
        survived_train.at(i) = survived.at(i);
        sex_train.at(i) = sex.at(i);
        age_train.at(i) = age.at(i);
    }

    /*for (int i: id_train) {
        cout << i << endl;
    }*/
    for (int i=800; i<numObservations; i++){
        id_test.at(i-800) = id.at(i);
        pclass_test.at(i-800) = pclass.at(i);
        survived_test.at(i-800) = survived.at(i);
        sex_test.at(i-800) = sex.at(i);
        age_test.at(i-800) = age.at(i);
    }

    //cout << "started making model" << endl;
    double weights[2] = {1, 1};    // set up weight array

    int data_matrix[800][2];

    for (int i=0; i<800; i++) {
        data_matrix[i][0] = 1;
        data_matrix[i][1] = sex_train.at(i);
    }

    //cout << "data_matrix initialized" << endl;

    // Since survival is already 0,1 no need to subtract 1 from the labels. The survival column is already an integer.
    // calculating coefficients for model
    double learning_rate = 0.001;
    auto start = chrono::steady_clock::now();

    for (int j=0; j<50000; j++){
        // matrix multiplication
        vector<double> prob_vector(800);
        vector<double> errors(800);
        for (int i=0; i<800; i++){
            prob_vector.at(i) = sigmoid(data_matrix[i][0]*weights[0] + data_matrix[i][1]*weights[1]);
            //cout << prob_vector.at(i) << " " << i << endl;
            errors.at(i) = survived_train.at(i) - prob_vector.at(i);
            //cout << errors.at(i) << " " << prob_vector.at(i) << " " << i << endl;
        }
        //cout << errors.at(1) << endl;
        double product[2] = {1, 1};
        for (int i=0; i<800; i++){
            product[0] += errors.at(i) * data_matrix[i][0];
            product[1] += errors.at(i) * data_matrix[i][1];
        }
        //cout << product[0] << " " << product[1] << endl;
        weights[0] = weights[0] + learning_rate * product[0];
        weights[1] = weights[1] + learning_rate * product[1];

        //cout << "The coefficients are " << weights[0] << " and " << weights[1] << endl;

    }

    auto end = chrono::steady_clock::now();

    cout << "The coefficients for the logistic regression model are " << weights[0] << " and " << weights[1] << endl;

    // prediction with test data
    int test_matrix[MAX_LEN-800][2];
    double predicted[MAX_LEN-800];    // predicted values

    for (int i=800; i<numObservations; i++) {
        test_matrix[i-800][0] = 1;
        test_matrix[i-800][1] = sex_test.at(i-800);
    }

    //cout << "test_matrix initialized" << endl;

    for (int i=800; i<numObservations; i++){
        predicted[i-800] = test_matrix[i-800][0]*weights[0] + test_matrix[i-800][1]*weights[1];
        //cout << predicted[i-800] << endl;
    }

    // calculating final probabilities
    //double e = 2.718;
    double probabilities[MAX_LEN-800];    // probabilities
    //double sum = 0;

    //cout << exp(1) << endl;
    for (int i=800; i<numObservations; i++){
        probabilities[i-800] = exp(predicted[i-800]) / (1 + exp(predicted[1-800]));
        //cout << exp(predicted[i-800]) << " " << predicted[i-800] << " " << probabilities[i-800] << " " << test_matrix[i-800][0] << " " << test_matrix[i-800][1] << endl;
        if(probabilities[i-800] > 0.5) {
            probabilities[i-800] = 1;
        } else {
            probabilities[i-800] = 0;
        }
        //cout << probabilities[i-800] << endl;

    }

    double accuracy = model_accuracy(numObservations, probabilities, survived_test);
    double sensitivity = model_sensitivity(numObservations, probabilities, survived_test);
    double specificity = model_specificity(numObservations, probabilities, survived_test);

    cout << "Accuracy: " << accuracy << endl;
    cout << "Sensitivity: " << sensitivity << endl;
    cout << "Specificity: " << specificity << endl;

    cout << "Runtime for the algorithm (in nanoseconds): " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;

    cout << "Closing file titanic_project.csv.\n" << endl;

    inFS.close();
}

/*int sigmoid(int x) {
    return 1 / (1 + exp(-x));
}*/