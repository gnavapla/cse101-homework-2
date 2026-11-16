// Implement only this file. See avl.h and the assignment for contracts.
#include "avl.h"
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
    // // cout << "Updating Height for " << node->data.departure << endl;
    int left_height = 0,right_height = 0;
    if(node->left == nullptr && node->right == nullptr){
        node->height = 0;
        return;
    }
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
        turner += node->left->height + 1;
    }
    if(node->right != nullptr){
        turner -= node->right->height + 1;
    }
    return turner;// left>right ->bf>0, right>left ->bf<0
} 

Node* rotateLeft(Node* root) {
    if(root->right== nullptr){
        return root;
    }
    Node* turner = root->right;
    root->right = turner->left;
    turner->left = root;
    updateHeight(root);
    updateHeight(turner);
    return turner;
} 

Node* rotateRight(Node* root) {
    if(root->left== nullptr){
        return root;
    }
    Node* turner = root->left;
    root->left = turner->right;
    turner->right= root;
    updateHeight(root);

    updateHeight(turner);
    return turner;
}
void print_childs(Node* turner){
    // // // cout << "First Height " << turner->height << endl;
    // // cout << "New Root: " << turner->data.departure << " HEIGHT: " << turner->height << endl;
    // // cout << "WITH CHILDREN: LEFT | ";
    if(turner->left == nullptr){
      // // cout << "NONE";
    }else{
        // // cout << turner->left->data.departure << " Height " << turner->left->height;
    }
    // // cout << " | RIGHT | ";
    if(turner->right== nullptr){
        // // cout << "NONE";
    }else{
       // // cout << turner->right->data.departure << " Height " << turner->right->height;
    }
    // // cout << endl;
    updateHeight(turner);
    // // cout << "Last Height " << turner->height << endl;
}

Node* rebalance(Node* root) {
    // // // cout<< "The root is: "<< root->data.departure<< " with height "<< root->height<< endl;
    Node* turner = nullptr;
    int bf_case= 0;
    // // // cout << "Balance factor of " << root->data.departure << " :" << balanceFactor(root) << endl;
    if(balanceFactor(root)<2 && balanceFactor(root)> -2){
        return root;
    }
    // // cout << "The balance is wrong by " << balanceFactor(root)<< endl;
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
    // // cout << "The balance factor case is " << bf_case << " With old Root " << root->data.departure << endl;
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
             // // cout << "NODE: " << root->data.departure << " HAS HEIGHT " << root->height << endl;
             print_childs(root);
             // // cout << "NODE: " << turner->data.departure<< " HAS HEIGHT " << turner->height << endl;
             break;
        case RL:
             root->right = rotateRight(root->right);
             turner = rotateLeft(root);
             // // cout << "NODE: " << root->data.departure<< " HAS HEIGHT " << root->height << endl;
             // // cout << "NODE: " << turner->data.departure<< " HAS HEIGHT " << turner->height << endl;
             break;
        default:
            return root;
    }
    print_childs(turner);
    return turner;
} 

Node* insertNode(Node* root, Node* newNode) {
    newNode->height = 0;
        if(root==nullptr){
        return newNode;
    }
    // // // cout<< "BEING " << root->data.departure << " " << newNode->data.departure << endl;

    if(root->data.departure >= newNode->data.departure){
        if(root->left == nullptr){
            // // // cout << "INSERTING " << newNode->data.departure << " IN LEFT "<< "WITH PARENT " << root->data.departure<< endl;
             root->left = newNode;
             updateHeight(root);
              // // cout << "THE LEFT CHILD OF " << root->data.departure << " IS NOW " << root->left->data.departure  << " WITH HEIGHT " << root->height << endl;
             return rebalance(root);
        }
        root->left = insertNode(root->left,newNode);
    }
    else{
        if(root->right == nullptr){
             // // // cout << "INSERTING " << newNode->data.departure << " IN RIGHT "<< "WITH PARENT " << root->data.departure<< endl;
            root->right = newNode;
            updateHeight(root);
             // // cout << "THE RIGHT CHILD OF " << root->data.departure << " IS NOW " << root->right->data.departure << " WITH HEIGHT " << root->height << endl;

            return rebalance(root);
        }


        root->right = insertNode(root->right,newNode);
        
    }

    updateHeight(root);
    // // cout << "THE CHILDREN OF " << root->data.departure << " WHERE UNCHANGED, BUT NOW HAS HEIGHT " << root->height << " AND BL "<< balanceFactor(root)<< endl;
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
    // Found the rat
    else if(root->data.departure == departure){
        Node* turner= nullptr;
        if(root->right != nullptr && root->left!=nullptr){
            // First Swap
            Node* next=  root->right;
            while(next->left != nullptr){
                // Go to the right most right
                next = next-> left;
            }
            // Root data does not need to be saved (will be deleted anyways)
            root->data = next->data;
            root->right = deleteNode(root->right, next->data.departure);

        }
        else if(root->right != nullptr){
                turner = root->right;
                // We know that root->left = nullptr
                delete root;
                return turner;

        } else if(root->left!= nullptr){
                turner = root->left;
                delete root;
                return turner;
        } else{
                delete root;
                return nullptr;
        }
    }
    updateHeight(root);
    return rebalance(root);
}

const Node* findNode(const Node* root, int departure) {
    if(root== nullptr){
        return nullptr;
    }
    if(root->data.departure  < departure){
        if(root->right!=nullptr){
            return findNode(root->right,departure);
        }
        else {
            return nullptr;
        }
    } else if (root->data.departure > departure){
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
    Node* next = (Node*)root;
    while(next != nullptr){
        if(next->data.departure >= departure ){
            best_choice = next;
            next = next ->left;
        }else{
            next = next ->right;
        }
    }
    return best_choice;
}

void findRange(const Node* root, int start, int end,
               const Node* results[], int& count) {
    if(root == nullptr){
        return ;
    }
    if(root->data.departure > start && root->left!= nullptr){
        findRange(root->left,start,end,results,count);
    }
    if(root->data.departure >= start && root->data.departure <= end){
        results[count] = root;
        count ++;
    } 
    if(root->data.departure < end && root->right != nullptr){
        findRange(root->right,start,end,results,count);
    } 
}
