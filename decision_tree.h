#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_
#include "data.h"
#include <algorithm>
#include <vector>
bool mycompare(vector<double> a, vector<double> b) {return a[1] < b[1];}

class binary_decision_tree
{
public:
    struct tree_node
    {
        bool is_leaf{false};
        tree_node *left_child{nullptr};
        tree_node *right_child{nullptr};
        int feature{0};
        double threshold{0.0};
        double gini{0.0};
        double n1{0.0};
        double n2{0.0};
        int label{0};
    };
    tree_node *build_tree(vector<Data> &T, vector<int> feature_index);
    bool check_purity(vector<Data> &T);
    int getMajorityLabel(vector<Data> &T);
    vector<double> calculate_gini(vector<Data> &T);
    vector<vector<Data>> split_dataset(vector<Data> &T, int feature_index, double threshold);
    vector<double> calculate_split_threshold_ginisplit(vector<Data> &T, int feature_index, int min_group_size);
    double train(vector<Data> &T, vector<int> feature_index = {0, 1, 2, 3, 4, 5, 6});
    void dfs(tree_node* root);
    void print_healper(binary_decision_tree::tree_node* root, int space, int level);
    void print();

private:
    struct tree_node* root;
    double TP, TN;
};

binary_decision_tree::tree_node *binary_decision_tree::build_tree(vector<Data> &T, vector<int> feature_index)
{
    cout << "create node" << endl;
    cout << "sub dataset size: " << T.size() << endl;
    // create new node
    tree_node *node = new tree_node;
    vector<double> gini_n1_n2 = calculate_gini(T);
    node->gini = gini_n1_n2[0];
    cout<<"gini: "<<gini_n1_n2[0]<<endl;
    node->n1 = gini_n1_n2[1];
    cout<<"n1: "<<gini_n1_n2[1]<<endl;
    node->n2 = gini_n1_n2[2];
    cout<<"n2: "<<gini_n1_n2[2]<<endl;

    if (check_purity(T) || feature_index.size() == 0)
    {
        cout<<"end"<<endl;
        node->is_leaf = true;
        node->label = getMajorityLabel(T);
        return node;
    }

    // find best split feature and threshold
    int best_split_feature_index = feature_index[0];
    double best_split_threshold = 0;
    double min_ginisplit = 0.5;
    int erase_index = 0;
    for (int i=0; i<feature_index.size(); i++)
    {
        vector<double> threshold_ginisplit = calculate_split_threshold_ginisplit(T, feature_index[i]);
        if (threshold_ginisplit[1] <= min_ginisplit) 
        {
            best_split_threshold = threshold_ginisplit[0];
            min_ginisplit = threshold_ginisplit[1];
            best_split_feature_index = feature_index[i];
            erase_index = i;
        }
    }

    // cout<<"debug"<<endl;
    // feature_index.erase(feature_index.begin() + erase_index);
    vector<int>copy_feature_index = feature_index;
    copy_feature_index.erase(copy_feature_index.begin() + erase_index);



    // for( int x:feature_index){
    //     cout<<x<<endl;
    // }

    if (gini_n1_n2[0] - min_ginisplit > 0)
    {   
        vector<vector<Data>> leftDataset_rightDataset = split_dataset(T, best_split_feature_index, best_split_threshold);
        cout<<"right branch"<<endl;
        node->right_child = build_tree(leftDataset_rightDataset[1], copy_feature_index);
        cout<<"left branch"<<endl;
        node->left_child = build_tree(leftDataset_rightDataset[0], copy_feature_index);
        node->feature = best_split_feature_index;
        node->threshold = best_split_threshold;
        return node;
        
    }
    else
    {
        node->is_leaf = true;
        node->label = getMajorityLabel(T);
        return node;
    }
}

void binary_decision_tree::dfs(tree_node* root){
    // Base case
    if (root == NULL)
        return;
    // Process right child first
    if (root->is_leaf){
        if (root->label == 1){
            TP += root->n1;
        } else if(root->label == 0){
            TN += root->n2;
        } else{
            cout<<"[dfs]: Error"<<endl;
        }
    }
    dfs(root->right_child);
    dfs(root->left_child);
}

double binary_decision_tree::train(vector<Data> &T, vector<int> feature_index){
    root = build_tree(T, feature_index);

    TP=0;
    TN=0;
    dfs(root);    
    double acc = (TP + TN) / T.size();
    return acc;
}

bool binary_decision_tree::check_purity(vector<Data> &T)
{
    int firstLabel = T[0].getLabel();
    for (Data &x : T)
    {
        if (x.getLabel() != firstLabel)
            return false;
    }
    return true;
}

int binary_decision_tree::getMajorityLabel(vector<Data> &T)
{
    int positive = 0, negative = 0;
    for (auto &x : T)
    {
        if (x.getLabel() == 0)
        {
            negative += 1;
        }
        else if (x.getLabel() == 1)
        {
            positive += 1;
        }
        else
        {
            cout << "[getMajorityLabel]: Error" << endl;
        }
    }
    return (positive >= negative) ? 1 : 0;
}

vector<double> binary_decision_tree::calculate_gini(vector<Data> &T)
{
    // return GINI, positive, negative
    double positive = 0, negative = 0, size = T.size();
    for (Data &element : T)
    {
        if (element.getLabel() == 0)
        {
            negative += 1;
        }
        else if (element.getLabel() == 1)
        {
            positive += 1;
        }
        else
        {
            cout << "[calculate_gini]: Error" << endl;
        }
    }

    double GINI = 1 - pow(positive / size, 2) - pow(negative / size, 2);
    return {GINI, positive, negative};
}

vector<vector<Data>> binary_decision_tree::split_dataset(vector<Data> &T, int feature_index, double threshold)
{
    vector<Data> left_dataset;
    vector<Data> right_dataset;
    for (Data &element : T)
    {
        if (element.getFeatures()[feature_index] <= threshold)
        {
            left_dataset.push_back(element);
        }
        else
        {
            right_dataset.push_back(element);
        }
    }
    return {left_dataset, right_dataset};
}

vector<double> binary_decision_tree::calculate_split_threshold_ginisplit(vector<Data> &T, int feature_index, int min_group_size=5)
{
    vector<vector<double>> featureand_label(T.size(), vector<double>(2, 0));
    for (int i = 0; i < T.size(); i++)
    {
        featureand_label[i][0] = T[i].getLabel();
        featureand_label[i][1] = T[i].getFeatures()[feature_index];
    }
    sort(featureand_label.begin(), featureand_label.end(), mycompare);
    // for(auto &x: featureand_label){
    //     cout<<x[0]<<'\t'<<x[1]<<endl;
    // }
    vector<double> thresholds(T.size(), 0);
    for (int i=0; i< T.size(); i++){
        thresholds[i] = featureand_label[i][1];
    }
    // double minFeature = featureand_label[0][1];
    // double maxFeature = featureand_label[featureand_label.size() - 1][1];
    // double stepThreshold = (maxFeature - minFeature) / (step - 1);

    double threshold = thresholds[0];
    double min_ginisplit = 0.5;

    for (int i = 0; i < T.size(); i++)
    {
        // cout << "min_ginisplit" << min_ginisplit << endl;
        double temp_threshold = thresholds[i];
        if (i == T.size() - 1)
            temp_threshold = temp_threshold + 0.000000001;

        vector<vector<Data>> left_right_group = split_dataset(T, feature_index, temp_threshold);

        double ginisplit = calculate_gini(left_right_group[0])[0] * left_right_group[0].size() / T.size() + calculate_gini(left_right_group[1])[0] * left_right_group[1].size() / T.size();
        // cout << "ginisplit" << ginisplit << endl;
        if (ginisplit < min_ginisplit && left_right_group[0].size() >= min_group_size && left_right_group[1].size() >= min_group_size)
        {
            min_ginisplit = ginisplit;
            threshold = temp_threshold;
        }
    }
    return {threshold, min_ginisplit};
}

#define COUNT 10
void binary_decision_tree::print_healper(binary_decision_tree::tree_node* root, int space, int level)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print_healper(root->right_child, space, level+1);
 
    // Print current node after space
    // count
    cout << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout<<"is leaf: " << root->is_leaf<< endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout<<"level: " << level << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    if (!root->is_leaf){
        cout << "attribute: "<< "f" <<root->feature<< endl;
        for (int i = COUNT; i < space; i++)
            cout << " ";
        cout<<"threshold: " <<root->threshold << endl;
        for (int i = COUNT; i < space; i++)
            cout << " ";
    }
    cout<<"n1: " <<root->n1 << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout<<"n2: " <<root->n2 << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout<<"gini: " <<root->gini << endl;
    if (root->is_leaf){
        for (int i = COUNT; i < space; i++)
            cout << " ";
        cout<<"class: " <<root->label << endl;
    }
    

 
    // Process left child
    print_healper(root->left_child, space, level+1);
}

void binary_decision_tree::print()
{
    print_healper(this->root, 0, 0);
}

#endif