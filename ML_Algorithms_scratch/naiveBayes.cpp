#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <chrono>
using namespace std;

double model_accuracy(int numObservations, double probabilities[], vector<int> survived_test);
double model_sensitivity(int numObservations, double probabilities[], vector<int> survived_test);
double model_specificity(int numObservations, double probabilities[], vector<int> survived_test);
double calc_age_lh(double v, double mean_v, double var_v);

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

double calc_age_lh(double v, double mean_v, double var_v) {
    double pi = 3.14;
    //cout << sqrt(2*pi*var_v) << " " << exp(-((v-mean_v) * (v-mean_v)) / (2 * var_v)) << endl;
    //cout << 1 / (sqrt(2*pi*var_v) * exp(-((v-mean_v) * (v-mean_v))) / (2 * var_v)) << endl;
    return 1 / sqrt(2*pi*var_v) * exp(-((v-mean_v) * (v-mean_v)) / (2 * var_v));
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

    //cout << "started training model" << endl;

    auto start = chrono::steady_clock::now();

    // getting count of people who survived and perished in training data
    int survived_0 = 0;
    int survived_1 = 0;
    for (int i=0; i<800; i++) {
        int survived = survived_train.at(i);
        if (survived == 0) {
            survived_0+=1;
        } else {
            survived_1+=1;
        }
    }

    double prior_prob_survived = (double)survived_1/800;
    double prior_prob_perished = (double)survived_0/800;

    //cout << prior_prob_perished << " " << prior_prob_survived << endl;

    // getting likelihood for pclass in training data
    double lh_pclass[2][3];
    int pclass_count[2][3] = {0,0,0,0,0,0};

    for (int i=0; i<800; i++) {
        int pclass = pclass_train.at(i);
        if (survived_train.at(i) == 1) {
            pclass_count[1][pclass-1]+=1;
        } else {
            pclass_count[0][pclass-1]+=1;
        }
    }

    //cout << pclass_count[0] << " " << pclass_count[1] << " " << pclass_count[2] << endl;

    for (int j=0; j<3; j++) {
        lh_pclass[0][j] = (double)pclass_count[0][j] / survived_0;
        //cout << lh_pclass[0][j] << " " << endl;
    }

    for (int j=0; j<3; j++) {
        lh_pclass[1][j] = (double)pclass_count[1][j] / survived_1;
        //cout << lh_pclass[1][j] << " " << endl;
    }


    // getting likelihood of sex in training data
    double lh_sex[2][2];
    int sex_count[2][2] = {0,0,0,0};

    for (int i=0; i<800; i++) {
        int sex = sex_train.at(i);
        if (survived_train.at(i) == 1) {
            sex_count[1][sex]+=1;
        } else {
            sex_count[0][sex]+=1;
        }
    }

    //cout << sex_count[0] << " " << sex_count[1] << endl;

    for (int j=0; j<2; j++) {
        lh_sex[0][j] = (double)sex_count[0][j] / survived_0;
        //cout << lh_sex[0][j] << " " << endl;
    }

    for (int j=0; j<2; j++) {
        lh_sex[1][j] = (double)sex_count[1][j] / survived_1;
//        cout << lh_sex[1][j] << " " << endl;
    }

    // getting likelihood for age
    // calculating mean and variance
    double age_mean[2] = {0,0};
    double age_var[2] = {0,0};
    double age_sum[2] = {0,0};
    double age_num[2] = {0,0};
    int age_survived = 0;

    for (int i=0; i<800; i++) {
        double age = age_train.at(i);
        if (survived_train.at(i)==0) {
            age_sum[0]+= age;
            age_num[0]+=1;
        } else {
            age_sum[1]+= age;
            age_num[1]+=1;
            age_survived+=1;
        }
    }

    age_mean[0] = age_sum[0]/age_num[0];
    age_mean[1] = age_sum[1]/age_num[1];

    //cout << age_mean[0] << " " << age_mean[1] << endl;

    double sum_of_diff[2] = {0,0};

    for (int i = 0; i <800; i++) {
        // calculating difference between age and mean(age) and square it
        double age = age_train.at(i);
        if (survived_train.at(i) == 0) {
            sum_of_diff[0]+= (age - age_mean[0])*(age - age_mean[0]);
        } else {
            sum_of_diff[1]+= (age - age_mean[1])*(age - age_mean[1]);
        }
    }  
    
    age_var[1] = sum_of_diff[1] / (age_survived - 1);
    age_var[0] = sum_of_diff[0] / ((800-age_survived) - 1);

    //cout << age_var[0] << " " << age_var[1] << endl;

    auto end = chrono::steady_clock::now();

    // function to calculate raw probabilities
    double raw[numObservations-800][2];
    for (int j = 800; j < numObservations; j++) {
        int i = j - 800;
        double num_s = lh_pclass[1][pclass_test.at(i)-1] * lh_sex[1][sex_test.at(i)] * prior_prob_survived * calc_age_lh(age_test.at(i), age_mean[1], age_var[1]);
        //cout << lh_pclass[0][pclass_test.at(i)-1] << " " << lh_sex[0][sex_test.at(i)] << " " << prior_prob_survived << " " << calc_age_lh(age_test.at(i), age_mean[0], age_var[0]) << " " << num_s << endl;
        double num_p = lh_pclass[0][pclass_test.at(i)-1] * lh_sex[0][sex_test.at(i)] * prior_prob_perished * calc_age_lh(age_test.at(i), age_mean[0], age_var[0]);
        //cout << num_p  << endl; // << " " << num_p << endl;
        double denominator = num_s + num_p;
        double prob_survived = num_s / denominator;
        double prob_perished = num_p / denominator;
        //cout << pclass_test.at(i) << " " << sex_test.at(i) << " " << age_test.at(i) << " " << prob_survived << " " << prob_perished << " " << i << endl;
        //cout << denominator << endl;
        raw[i][0] = prob_perished;
        raw[i][1] = prob_survived;
        //cout << raw[i][0] << " " << raw[i][1] << endl;
    }

    double probabilities[MAX_LEN-800];    // final probabilities

    // calculating final probabilities
    for (int i=800; i<numObservations; i++){
        if(raw[i-800][0] > raw[i-800][1]) {
            probabilities[i-800] = 0;
        } else {
            probabilities[i-800] = 1;
        }
        //cout << raw[i-800][0] << " " << raw[i-800][1] << " " << probabilities[i-800] << endl;

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
