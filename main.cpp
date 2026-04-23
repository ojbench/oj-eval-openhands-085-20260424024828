

#include <iostream>
#include <vector>
#include <memory>

// Definition for a binary tree node
struct TreeNode {
    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    
    // Custom deleter to help prevent stack overflow during destruction
    ~TreeNode() {
        // Clear children iteratively to avoid deep recursion
        std::vector<TreeNode*> stack;
        if (left) stack.push_back(left.release());
        if (right) stack.push_back(right.release());
        
        while (!stack.empty()) {
            TreeNode* node = stack.back();
            stack.pop_back();
            
            if (node->left) stack.push_back(node->left.release());
            if (node->right) stack.push_back(node->right.release());
            
            delete node;
        }
    }
};

class Solution {
public:
    // Function to prune the binary tree
    std::unique_ptr<TreeNode> pruneTree(std::unique_ptr<TreeNode> root) {
        if (!root) {
            return nullptr;
        }
        
        // Recursively prune left and right subtrees
        root->left = pruneTree(std::move(root->left));
        root->right = pruneTree(std::move(root->right));
        
        // If current node is 0 and has no children, remove it
        if (root->val == 0 && !root->left && !root->right) {
            return nullptr;
        }
        
        return root;
    }
};

// Function to build tree from extended preorder traversal
std::unique_ptr<TreeNode> buildTree(const std::vector<int>& data, int& index) {
    if (index >= data.size() || data[index] == -1) {
        index++;
        return nullptr;
    }
    
    if (data[index] == -2) {
        return nullptr; // End of input
    }
    
    auto node = std::make_unique<TreeNode>(data[index]);
    index++;
    
    node->left = buildTree(data, index);
    node->right = buildTree(data, index);
    
    return node;
}

// Function to perform preorder traversal and output the result
void preorderTraversal(const TreeNode* node, std::vector<int>& result) {
    if (!node) {
        result.push_back(-1);
        return;
    }
    
    result.push_back(node->val);
    preorderTraversal(node->left.get(), result);
    preorderTraversal(node->right.get(), result);
}

int main() {
    // Read input
    std::vector<int> data;
    int val;
    
    while (std::cin >> val) {
        data.push_back(val);
        if (val == -2) {
            break;
        }
    }
    
    // Build the tree
    int index = 0;
    auto root = buildTree(data, index);
    
    // Prune the tree
    Solution solution;
    root = solution.pruneTree(std::move(root));
    
    // Get the preorder traversal of the pruned tree
    std::vector<int> result;
    preorderTraversal(root.get(), result);
    
    // Output the result
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << result[i];
    }
    std::cout << std::endl;
    
    return 0;
}

