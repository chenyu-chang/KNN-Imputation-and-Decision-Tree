#ifndef DATA_H_
#define DATA_H_
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include<cmath>
#include <limits>
using namespace std;


vector<pair<double, int>> sort_with_index(vector<double> &arr)
{
 
    // Vector to store element
    // with respective present index
    vector<pair<double, int> > vp;
 
    // Inserting element in pair vector
    // to keep track of previous indexes
    for (int i = 0; i < arr.size(); ++i) {
        vp.push_back(make_pair(arr[i], i));
    }
 
    // Sorting pair vector
    sort(vp.begin(), vp.end());
 
    return vp;
}


double nan_euclidean_distances(vector<double> a, vector<double> b, double missing_value = -99.0){
	double distance=0;
	int missing_num = 0;
	for(int i=0; i<a.size(); i++){
		if (a[i] == missing_value || b[i] == missing_value){
			missing_num ++;
		} else {
			distance += pow((a[i] - b[i]), 2);
		}
	}
	return pow(distance * ((double)a.size()/(a.size() - missing_num+0.00000001)), 0.5);
}

//Object to store one data
class Data {
public:
	Data(string primary_key, vector<double> features, int label) {
		this->primary_key = primary_key;
		this->label = label;
		this->features = features;
	}
	string getPrimaryKey() {return primary_key;}	
	int getLabel() {return label;}
	vector<double> getFeatures() {return features;}
	void setFeatures(vector<double> features) {this->features = features;}	

private:
	string primary_key;
	int label;
	vector<double> features;
};

//Object to store dataset
class Dataset {
public:
	Dataset(string csv_path, int num_feature); // Default constructor: read csv file to vector and store missing data
    int size() {return table.size();}
	void knn_imputation(double missing_value, int k);
	void save_csv(string file_path); // Save dataset to csv file
	void print();
	vector<Data> getTable(){return table;}
	vector<vector<double>> distance_matrix(double missing_value);
	vector<vector<bool>> missing_value_mask(double missing_value);
private:
    int num_feature;
	vector<Data> table; 
};

Dataset::Dataset(string csv_path, int num_feature) {
	this->num_feature = num_feature;
	ifstream input_file;
	input_file.open(csv_path);

	string line = "";
	getline(input_file, line); // Skip first row of csv file

	while (getline(input_file, line)) {
		stringstream input_string(line);
		string temp_string;
		getline(input_string, temp_string, ',');
		string primary_key = temp_string;

		vector<double> features(num_feature);

		for (int i = 0; i < num_feature; i++) {
			getline(input_string, temp_string, ',');
			features[i] = atof(temp_string.c_str());
		}
		getline(input_string, temp_string, ',');
		int label = atoi(temp_string.c_str());

		Data data{ primary_key, features, label };
		table.push_back(data);
	}
}


vector<vector<double>> Dataset::distance_matrix(double missing_value = -99.0){
	vector<vector<double>> d(size(), vector<double>(size(), numeric_limits<double>::infinity()));

	for (int i=0; i<size(); i++){
		for(int j=i; j<size(); j++){
			d[i][j] = d[j][i] = nan_euclidean_distances(table[i].getFeatures(), table[j].getFeatures(), missing_value);
		}
	}
	return d;
}


vector<vector<bool>> Dataset::missing_value_mask(double missing_value = -99.0){
	vector<vector<bool>> mask(num_feature, vector<bool>(size(), false));
	for(int i=0; i<num_feature; i++){
		for (int j=0; j<size(); j++){
			if (table[j].getFeatures()[i] == missing_value) mask[i][j] = true;
		}
	}
	return mask;
}


void Dataset::knn_imputation(double missing_value=-99.0, int knn=5) {
	vector<vector<double>> d = distance_matrix(missing_value);
	for(int x=0; x<d.size(); x++){
		for(int y=0; y<d.size(); y++){
			// cout<< d[x][y] <<'\t';
		}
		// cout<<endl;
	}
	vector<vector<bool>> missing_mask(size(), vector<bool>(num_feature, false));

    for(int i=0; i<size(); i++){
		Data &data = table[i];
		
		vector<double> f = data.getFeatures();

		vector<pair<double, int>> sorted_distance_with_index = sort_with_index(d[i]);
		for (int x=0; x<sorted_distance_with_index.size();x++){
			// cout << sorted_distance_with_index[x].second << '\t';
		}
		for(int j=0; j< num_feature; j++){
			if (f[j]==missing_value){ //j feature 有missing value
				missing_mask[i][j] = true;

				double kmean=0;
				int k=0;
				int count = 1;
				while (k<knn)
				{	
					if (!missing_mask[sorted_distance_with_index[count].second][j]){
						cout<<sorted_distance_with_index[count].second<<'\t';
						kmean += table[sorted_distance_with_index[count].second].getFeatures()[j];
						k++;
					}
					count++;
				}
				cout<<endl;
				kmean /=knn;
				f[j] = kmean;
				data.setFeatures(f);
			}
		}
	}

}


void Dataset::save_csv(string file_path){
	ofstream file;
	file.open(file_path);
	file << "primary key" << ',';
	for (int i = 0; i < num_feature; i++) {
		file << 'f' << i + 1 << ',';
	}
	file << "class" << endl;
	for (Data& element : table) {
		file << fixed << setprecision(3) << element.getPrimaryKey() << ',' ;
		for (int i = 0; i < num_feature; i++) {
			file << element.getFeatures()[i] << ',';
		}
		file << to_string(element.getLabel()) << endl;
	}
	file.close();
}


void Dataset::print() {
	cout << left << setw(10) << "primary key" << '\t';
	for (int i = 0; i < num_feature; i++) {
		cout << 'f' << i + 1 << '\t';
	}
	cout << "class" << endl;
	for (Data& element : table) {
		cout << fixed << setprecision(3) << left << setw(10) << element.getPrimaryKey() << '\t';
		for (int i = 0; i < num_feature; i++) {
			cout << element.getFeatures()[i] << '\t';
		}
		cout << to_string(element.getLabel()) << endl;
	}
}

#endif