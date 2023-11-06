#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std; 
#include "data.h"
#include "decision_tree.h"

 
int main() {
	Dataset myDataSet("Missing_Data_7features.csv", 7);
	myDataSet.knn_imputation();
	cout << "DataSize: " << myDataSet.size() << endl;
	myDataSet.print();
	myDataSet.save_csv("Estimated_Data_7features_final.csv");
	vector<Data> trainDataset = myDataSet.getTable();
	binary_decision_tree model;
	double acc = model.train(trainDataset);
	cout<< "-------------------------------------------------------------"<<endl;
	model.print();
	cout<< "Accuracy: " << acc<<endl;
	system("pause");
	return 0;
}
