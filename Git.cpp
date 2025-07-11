#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// Commit node representing a snapshot in the version history
class Commit {
public:
    string commitId;
    string message;
    time_t timestamp;
    unordered_map<string, string> files;  // filename -> content
    Commit* parent;
    vector<Commit*> children;             // Tree: multiple child commits possible

    Commit(string id, string msg, unordered_map<string, string> filesSnapshot, Commit* prev)
        : commitId(id), message(msg), timestamp(time(0)), files(filesSnapshot), parent(prev) {}
};

class Repository {
private:
    unordered_map<string, string> stagingArea;       // files staged for commit
    Commit* HEAD;                                    // current commit pointer
    unordered_map<string, Commit*> allCommits;       // commitId -> Commit node
    int commitCounter;

    string generateCommitId() {
        return "commit_" + to_string(++commitCounter);
    }

public:
    Repository() : HEAD(nullptr), commitCounter(0) {}

    void init() {
        cout << "Repository initialized.\n";
    }

    void addFile(const string& filename, const string& content) {
        stagingArea[filename] = content;
        cout << "\nFile '" << filename << "' staged for commit.\n";
    }

    void commit(const string& message) {
        if (stagingArea.empty() && HEAD != nullptr) {
            cout << "\nNo files staged. Nothing to commit.\n";
            return;
        }

        unordered_map<string, string> snapshot;
        if (HEAD != nullptr) {
            snapshot = HEAD->files;  // inherit previous state
        }

        for (const auto& file : stagingArea) {
            snapshot[file.first] = file.second;
        }

        string commitId = generateCommitId();
        Commit* newCommit = new Commit(commitId, message, snapshot, HEAD);
        allCommits[commitId] = newCommit;

        if (HEAD != nullptr) {
            HEAD->children.push_back(newCommit);  // Tree structure
        }

        HEAD = newCommit;
        stagingArea.clear();

        cout << "\nCommitted as " << commitId << ": " << message << endl;
    }

    void logHelper(Commit* node, int depth = 0) const {
        if (node == nullptr) return;

        for (int i = 0; i < depth; i++) cout << "  ";  // Indent for tree view
        cout << "- " << node->commitId << " : " << node->message << " @ " << ctime(&node->timestamp);

        for (Commit* child : node->children) {
            logHelper(child, depth + 1);
        }
    }

    void log() const {
        if (HEAD == nullptr) {
            cout << "No commits yet.\n";
            return;
        }

        cout << "\n--- Commit History Tree ---\n";
        Commit* root = HEAD;
        while (root && root->parent) root = root->parent;  // Move to initial commit
        logHelper(root);
        cout << "---------------------------\n";
    }

    void checkout(const string& commitId) {
        if (allCommits.find(commitId) == allCommits.end()) {
            cout << "\nCommit ID not found.\n";
            return;
        }
        HEAD = allCommits[commitId];
        cout << "\nChecked out to " << HEAD->commitId << ": " << HEAD->message << endl;
    }

    void showCurrentFiles() const {
        if (HEAD == nullptr) {
            cout << "\nNo commits yet. No files to display.\n";
            return;
        }

        cout << "\n--- Files at HEAD (" << HEAD->commitId << ") ---\n";
        for (const auto& file : HEAD->files) {
            cout << file.first << ": " << file.second << endl;
        }
    }
};

int main() {
    Repository repo;
    int choice;
    string filename, content, message, commitId;

    repo.init();

    do {
        cout << "\n===== MiniGit Menu =====\n";
        cout << "1. Add File\n";
        cout << "2. Commit\n";
        cout << "3. Log History (Tree)\n";
        cout << "4. Checkout Commit\n";
        cout << "5. Show Current Files\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter filename: ";
                getline(cin, filename);
                cout << "Enter file content: ";
                getline(cin, content);
                repo.addFile(filename, content);
                break;

            case 2:
                cout << "Enter commit message: ";
                getline(cin, message);
                repo.commit(message);
                break;

            case 3:
                repo.log();
                break;

            case 4:
                cout << "Enter commit ID to checkout: ";
                getline(cin, commitId);
                repo.checkout(commitId);
                break;

            case 5:
                repo.showCurrentFiles();
                break;

            case 6:
                cout << "Exiting MiniGit.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
