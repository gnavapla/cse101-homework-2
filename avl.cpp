// Implement only this file. See avl.h and the assignment for contracts.
#include "avl.h"
#include <iostream>
#define R 0
#define L 1
#define RL 2
#define LR 3

using namespace std ;
//These Top 3 are suspicious, but I don't really know...
int nodeHeight(const Node* node) {
    if(node == nullptr){ return 0; }
    return node->height;
} 

void updateHeight(Node* node) {
    if(node== nullptr){return;}
    int left_height = 0,right_height = 0;
    if(node->left != nullptr){
        left_height = node->left->height;
    }
    if(node->right != nullptr){
        right_height = node->right->height;
    }
    if(left_height >= right_height){
        node->height = left_height+1;
    }else {
        node->height = right_height+1;
    }

} 

int balanceFactor(const Node* node) {
    (void)node;
    int turner = 0;
    if(node == nullptr){
        return 0;
    }
    if(node->left != nullptr){
        cout << "Left "<<node->left->height << " Name: "<< node->left->data.departure<< endl;
        turner += node->left->height;
    }
    if(node->right != nullptr){
        cout << "Right "<<node->right->height << endl;
        turner -= node->right->height;
    }
    return turner;// left>right ->bf>0, right>left ->bf<0
} 

Node* rotateLeft(Node* root) {
    if(root->right== nullptr){
        return root;
    }
    Node* turner = root->right;
    root->right = turner->left;
    turner->right = root;
    return turner;
} 

Node* rotateRight(Node* root) {
    if(root->left== nullptr){
        return root;
    }
    Node* turner = root->left;
    root->left = turner->right;
    turner->left = root;
    return turner;
}

Node* rebalance(Node* root) {
    cout<< "The root is: "<< root->data.departure<< " with height "<< root->height<< endl;
    Node* turner = nullptr;
    int bf_case= 0;
    if(balanceFactor(root)<2 && balanceFactor(root)>2){
        return root;
    }
    //Check which balancing case is it.
    if (balanceFactor(root) == 2 && balanceFactor(root->left)>= 0){
        bf_case = R;
    }
    else if (balanceFactor(root) == -2 && balanceFactor(root->right)<=0){
        bf_case = L;
    } else if(balanceFactor(root) == 2 && balanceFactor(root->left)< 0 ){
        bf_case = LR;
    } else if(balanceFactor(root) == -2 && balanceFactor(root->right)>0){
        bf_case = RL;
    }
    cout << "The balance factor case is " << bf_case<< endl;
    switch(bf_case){
        case L:
            turner = rotateLeft(root);
            break;
        case R:
            turner = rotateRight(root);
            break;
        case LR:
             root->left = rotateLeft(root->left);
             turner = rotateRight(root);
             break;
        case RL:
             root->right = rotateRight(root->right);
             turner = rotateLeft(root);
             break;
        default:
            return root;
    }

    return turner;
} 

Node* insertNode(Node* root, Node* newNode) {
    newNode->height = 0;
    if(root==nullptr){
        return newNode;
    }
    if(root->data.departure >= newNode->data.departure){
        if(root->left == nullptr){
             root->height+=1;
             root->left = newNode;
             return rebalance(root);
        }
        root->left = insertNode(root->left,newNode);
    }
    else{
        if(root->right == nullptr){
             root->height+=1;
            root->right = newNode;
            return rebalance(root)  ;
        }
        root->right = insertNode(root->right,newNode);
        
    }
    return rebalance(root);
}

Node* deleteNode(Node* root, int departure) {
    if(root== nullptr){
        return root;
    }
    if(root->data.departure > departure){
        root->left = deleteNode(root->left,departure);
    }
    else if(root->data.departure < departure){
        root->right = deleteNode(root->right,departure);
    }
    else if(root->data.departure == departure){
        Node* turner= nullptr;
        if(root->right != nullptr){
            if(root->left == nullptr){
                turner = root->left;
                delete root;
                return rebalance(turner);
            }
            if(root->right->left == nullptr){
                root->right->left = root->left;
                turner = root->right;
                delete root;
                return rebalance(turner);
            }
            // First Swap
            Node* right,* left;
            right = root->right;
            left = root->left;
            turner = root->right->left;
            // Swap Places
            root->right = turner->right;
            root->left = turner->left;
            turner->right = right;
            turner->left = left;
            // Recursive again until the root is finally deleted
            turner->right->left = deleteNode(root,departure);
        }
    }
    return rebalance(root);
}

const Node* findNode(const Node* root, int departure) {
    if(root== nullptr){
        return nullptr;
    }
    if(root->data.departure  > departure){
        if(root->right!=nullptr){
            return findNode(root->right,departure);
        }
        else {
            return nullptr;
        }
    } else if (root->data.departure < departure){
        if(root->left!=nullptr){
            return findNode(root->left,departure);
        }
        else {
            return nullptr;
        }
    } else if(root->data.departure == departure ){
        return root;
    }
    // Should never, ever happen but just in case.
    return nullptr;
}
// Harder-Ish Parts 
const Node* earliestNode(const Node* root, int departure) {
    //TODO
    if(root == nullptr){
        return nullptr;
    }
    const Node* best_choice = nullptr;
    if(departure > root->data.departure){
        if(root->left==nullptr){
            return best_choice;
        }
        best_choice = earliestNode( root->left,departure);
    }
    else{// We have secured that best_choice->data.departure - departure > 0, we are also know
         // that root->data.departure - departure > 0 so we only need to compare
         //  (best_choice->data.departure - departure) - (root->data.departure - departure) >= 0 
        // I.E. best_choice->data.departure - root->data.departure >= 0 otherwise that implies that
        // root->data.departure is a worse option to best_choice
        if(best_choice != nullptr && best_choice->data.departure - root->data.departure >= 0 ){
            best_choice = root;
        }
    }
    return best_choice;
}

void findRange(const Node* root, int start, int end,
               const Node* results[], int& count) {
    (void)results;
    if(root == nullptr){
        return ;
    }
    if(root->data.departure >= start && root->data.departure <= end){
        count ++;
        if(root->left!=nullptr){
            findRange(root->left,start,end,results,count);
        }
        if(root->right!=nullptr){
            findRange(root->right,start,end,results,count);
        }
    } else if(root->data.departure > end && root->left != nullptr){
        findRange(root->left,start,end,results,count);
        return;
    } else  if(root->data.departure < start && root->right != nullptr){
        findRange(root->right,start,end,results,count);
        return;
    }
}
