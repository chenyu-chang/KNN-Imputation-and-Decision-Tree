
# KNN Imputation and Decision Tree Classifier

## Overview

This project aims to address the challenge of missing data in clinical datasets and to develop a classifier that can accurately categorize subjects into positive or negative cases. The project is implemented in C++ and consists of two main parts:

1. **KNN Imputation**: Estimating missing data using the K-Nearest Neighbors algorithm.
2. **Decision Tree Classifier**: Constructing a binary decision tree based on the Gini index for classification.

## Dataset

The dataset, titled `Missing_Data_7features.csv`, includes anonymous clinical data for 640 subjects, each with a primary key, seven clinical features, and a class label (1 for positive, 0 for negative).

## Prerequisites

To compile and run this project, you need a C++ compiler that supports C++11 (or later) standards. Examples of such compilers include GCC, Clang, and MSVC.

## Compilation

Compile the project using the following command (adjust based on your compiler):

```bash
g++ -std=c++11 main.cpp data.cpp decision_tree.cpp -o classifier
```

## Running the Application

Run the compiled application with:

```bash
./classifier
```

Upon running the application, the KNN imputation process will fill the missing values in the dataset, and the decision tree model will be trained and evaluated.

## Output

After the execution, the program will:

- Print the size of the dataset and the imputed dataset to the console.
- Save the imputed dataset to `Estimated_Data_7features_final.csv`.
- Train a binary decision tree on the imputed dataset and print its structure.
- Print the accuracy of the decision tree classifier.

## Source Files

- `main.cpp`: Contains the main function to run the KNN imputation and decision tree classifier.
- `data.h`: Contains the `Data` and `Dataset` class definitions for handling the data.
- `decision_tree.h`: Contains the `binary_decision_tree` class definition for the decision tree model.

## Results

After successful execution, you will find:

- The complete dataset with estimated missing values in `Estimated_Data_7features_final.csv`.
- The accuracy of the decision tree model printed on the console=0.722.

## Notes

- Make sure that the dataset `Missing_Data_7features.csv` is located in the same directory as the executable or adjust the path accordingly in `main.cpp`.
- The `system("pause")` command in `main.cpp` is meant for Windows systems. If you are on a different system, you might need to remove or modify this line.