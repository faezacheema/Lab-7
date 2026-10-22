#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Student {
    int studentID;
    string name;
    float GPA;
    Student* left;
    Student* right;
    Student(int id, string n, float g) : studentID(id), name(n), GPA(g), left(nullptr), right(nullptr) {}
};

class StudentBST {
    Student* root = nullptr;

    Student* insert(Student* r, int id, const string& name, float gpa, bool& ok) {
        if (!r) { ok = true; return new Student(id, name, gpa); }
        if (id < r->studentID) r->left = insert(r->left, id, name, gpa, ok);
        else if (id > r->studentID) r->right = insert(r->right, id, name, gpa, ok);
        else ok = false;                       // duplicate ID
        return r;
    }
    Student* search(Student* r, int id) const {
        if (!r || r->studentID == id) return r;
        return id < r->studentID ? search(r->left, id) : search(r->right, id);
    }
    Student* minNode(Student* r) const { while (r && r->left) r = r->left; return r; }

    Student* remove(Student* r, int id, bool& ok) {
        if (!r) return nullptr;
        if (id < r->studentID) r->left = remove(r->left, id, ok);
        else if (id > r->studentID) r->right = remove(r->right, id, ok);
        else {
            ok = true;
            if (!r->left) { Student* t = r->right; delete r; return t; }   // 0 or 1 child
            if (!r->right) { Student* t = r->left;  delete r; return t; }
            Student* s = minNode(r->right);                                  // 2 children: inorder successor
            r->studentID = s->studentID; r->name = s->name; r->GPA = s->GPA;
            bool tmp;
            r->right = remove(r->right, s->studentID, tmp);
        }
        return r;
    }
    static void show(const Student* s) {
        cout << "  ID: " << setw(6) << s->studentID << " | Name: " << left << setw(12) << s->name
            << right << " | GPA: " << fixed << setprecision(2) << s->GPA << '\n';
    }
    void inorder(Student* r) const { if (r) { inorder(r->left); show(r); inorder(r->right); } }
    int count(Student* r) const { return r ? 1 + count(r->left) + count(r->right) : 0; }
    void extremes(Student* r, Student*& hi, Student*& lo) const {
        if (!r) return;
        if (!hi || r->GPA > hi->GPA) hi = r;
        if (!lo || r->GPA < lo->GPA) lo = r;
        extremes(r->left, hi, lo); extremes(r->right, hi, lo);
    }
    void above(Student* r, float t, int& n) const {
        if (!r) return;
        above(r->left, t, n);
        if (r->GPA > t) { show(r); n++; }
        above(r->right, t, n);
    }
    void destroy(Student* r) { if (r) { destroy(r->left); destroy(r->right); delete r; } }

public:
    ~StudentBST() { destroy(root); }

    void insert(int id, const string& name, float gpa) {
        bool ok = false;
        root = insert(root, id, name, gpa, ok);
        if (!ok) cout << "  Student ID " << id << " already exists. Not inserted.\n";
    }
    void search(int id) const {
        Student* s = search(root, id);
        if (s) { cout << "  Found:\n"; show(s); }
        else cout << "  Student with ID " << id << " not found.\n";
    }
    void remove(int id) {
        bool ok = false;
        root = remove(root, id, ok);
        cout << (ok ? "  Deleted student ID " : "  Cannot delete, not found: ") << id << '\n';
    }
    void display() const {
        if (!root) cout << "  (no records)\n"; else inorder(root);
    }
    int count() const { return count(root); }
    void highestLowest() const {
        Student* hi = nullptr, * lo = nullptr;
        extremes(root, hi, lo);
        if (!hi) { cout << "  No records.\n"; return; }
        cout << "  Highest GPA:\n"; show(hi);
        cout << "  Lowest GPA:\n";  show(lo);
    }
    void aboveThreshold(float t) const {
        cout << "  Students with GPA above " << fixed << setprecision(2) << t << ":\n";
        int n = 0; above(root, t, n);
        if (!n) cout << "  (none)\n";
    }
};

void menu() {
    cout << "\n===== Student Record Management (BST) =====\n"
        << "1. Insert student\n2. Search by ID\n3. Delete by ID\n4. Display all (sorted by ID)\n"
        << "5. Total students\n6. Highest & lowest GPA\n7. Students above GPA threshold\n"
        << "8. Load sample data\n0. Exit\nChoice: ";
}

int main() {
    StudentBST t;
    int ch;
    while (true) {
        menu();
        if (!(cin >> ch)) break;
        if (ch == 0) break;
        int id; string name; float gpa;
        switch (ch) {
        case 1:
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin >> name;
            cout << "GPA: "; cin >> gpa;
            t.insert(id, name, gpa); break;
        case 2: cout << "ID: "; cin >> id; t.search(id); break;
        case 3: cout << "ID: "; cin >> id; t.remove(id); break;
        case 4: t.display(); break;
        case 5: cout << "  Total students: " << t.count() << '\n'; break;
        case 6: t.highestLowest(); break;
        case 7: cout << "Threshold: "; cin >> gpa; t.aboveThreshold(gpa); break;
        case 8:
            t.insert(50, "Ali", 3.40f);   t.insert(30, "Sara", 3.90f);
            t.insert(70, "Hamza", 2.60f); t.insert(20, "Ayesha", 3.10f);
            t.insert(40, "Usman", 2.20f); t.insert(60, "Fatima", 3.75f);
            t.insert(80, "Bilal", 2.95f);
            cout << "  Sample data loaded.\n"; break;
        default: cout << "  Invalid choice.\n";
        }
    }
    cout << "Goodbye!\n";
    return 0;
}
