#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

struct Node {
    char data;
    Node* left, * right;
    Node(char d) : data(d), left(nullptr), right(nullptr) {}
};

struct BSTNode {
    int data;
    BSTNode* left, * right;
    BSTNode(int d) : data(d), left(nullptr), right(nullptr) {}
};

// -1. Build tree from string 
// Grammar: node := letter [ '(' [node] ',' [node] ')' ]
Node* build(const string& s, int& i) {
    if (i >= (int)s.size() || !isalnum(s[i])) return nullptr;   // empty child
    Node* root = new Node(s[i++]);
    if (i < (int)s.size() && s[i] == '(') {
        i++;                          // skip '('
        root->left = build(s, i);
        if (i < (int)s.size() && s[i] == ',') i++;   // skip ','
        root->right = build(s, i);
        if (i < (int)s.size() && s[i] == ')') i++;   // skip ')'
    }
    return root;
}
Node* buildTree(const string& s) { int i = 0; return build(s, i); }

// 2. Preorder string form (recursive) 
// Prints in the same format as the input: root(left,right)
void preorderString(Node* r) {
    if (!r) return;
    cout << r->data;
    if (r->left || r->right) {
        cout << '(';
        preorderString(r->left);
        cout << ',';
        preorderString(r->right);
        cout << ')';
    }
}
// Plain preorder sequence (Root Left Right), recursive
void preorder(Node* r) {
    if (!r) return;
    cout << r->data << ' ';
    preorder(r->left);
    preorder(r->right);
}

// 3. Inorder (iterative, using stack) 
void inorderIter(Node* root) {
    stack<Node*> st;
    Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        cout << cur->data << ' ';
        cur = cur->right;
    }
}

// 4. Postorder (iterative, using stack) 
void postorderIter(Node* root) {
    stack<Node*> st;
    Node* cur = root, * lastVisited = nullptr;
    while (cur || !st.empty()) {
        if (cur) { st.push(cur); cur = cur->left; }
        else {
            Node* top = st.top();
            if (top->right && lastVisited != top->right) cur = top->right;
            else { cout << top->data << ' '; lastVisited = top; st.pop(); }
        }
    }
}

// 5. Validate string without building the tree 
bool validNode(const string& s, int& i) {
    if (i >= (int)s.size() || !isalnum(s[i])) return false;
    i++;
    if (i < (int)s.size() && s[i] == '(') {
        i++;
        if (i < (int)s.size() && isalnum(s[i]) && !validNode(s, i)) return false; // optional left
        if (i >= (int)s.size() || s[i] != ',') return false;
        i++;
        if (i < (int)s.size() && isalnum(s[i]) && !validNode(s, i)) return false; // optional right
        if (i >= (int)s.size() || s[i] != ')') return false;
        i++;
    }
    return true;
}
bool isValid(const string& s) {
    // quick balanced-parentheses check
    int bal = 0;
    for (char c : s) {
        if (c == '(') bal++;
        else if (c == ')' && --bal < 0) return false;
    }
    if (bal != 0) return false;
    int i = 0;
    return validNode(s, i) && i == (int)s.size();   // whole string consumed
}

// 6. Height and depth 
int height(Node* r) {                 // height of a node (leaf = 0, empty = -1)
    if (!r) return -1;
    return 1 + max(height(r->left), height(r->right));
}
void printDepths(Node* r, int d = 0) {   // depth of each node (root = 0)
    if (!r) return;
    cout << "  Node " << r->data << " -> depth " << d << '\n';
    printDepths(r->left, d + 1);
    printDepths(r->right, d + 1);
}

// 7. BST from integer sequence
BSTNode* insertBST(BSTNode* r, int v) {
    if (!r) return new BSTNode(v);
    if (v < r->data) r->left = insertBST(r->left, v);
    else if (v > r->data) r->right = insertBST(r->right, v);
    return r;
}
BSTNode* buildBST(const vector<int>& v) {
    BSTNode* root = nullptr;            // first element becomes the root
    for (int x : v) root = insertBST(root, x);
    return root;
}
void inorderBST(BSTNode* r) {
    if (!r) return;
    inorderBST(r->left);
    cout << r->data << ' ';
    inorderBST(r->right);
}

void freeTree(Node* r) { if (r) { freeTree(r->left); freeTree(r->right); delete r; } }
void freeBST(BSTNode* r) { if (r) { freeBST(r->left); freeBST(r->right); delete r; } }

int main() {
    string s = "a(b(d,e),c(,f(g,)))";
    cout << "Given string: " << s << "\n\n";

    cout << "[5] Valid string? " << (isValid(s) ? "YES" : "NO") << '\n';
    string bad[] = { "a(b(d,e),c(,f(g,))", "a(b,c))", "a(b c)", "a(," };
    for (auto& b : bad) cout << "    \"" << b << "\" -> " << (isValid(b) ? "valid" : "invalid") << '\n';

    Node* root = buildTree(s);
    cout << "\n[1] Tree built.\n";

    cout << "\n[2] Preorder (string form): "; preorderString(root);
    cout << "\n    Preorder (sequence)   : "; preorder(root);
    cout << "\n\n[3] Inorder (iterative, stack)  : "; inorderIter(root);
    cout << "\n\n[4] Postorder (iterative, stack): "; postorderIter(root);

    cout << "\n\n[6] Height of tree: " << height(root) << '\n';
    cout << "    Depth of each node:\n";
    printDepths(root);

    vector<int> v = { 50, 30, 70, 20, 40, 60, 80 };
    BSTNode* bst = buildBST(v);
    cout << "\n[7] BST inorder from [50,30,70,20,40,60,80]: ";
    inorderBST(bst);
    cout << '\n';

    freeTree(root); freeBST(bst);
    return 0;
}
