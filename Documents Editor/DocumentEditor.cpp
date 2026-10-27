#include <bits/stdc++.h>
using namespace std;

/*
    Document Editor LLD Example
    --------------------------
    This code follows a simple layered design:
    1. DocumentElement represents the content blocks inside a document.
    2. Document aggregates all content blocks and renders them together.
    3. Persistence defines how the final document is stored.
    4. DocumentsEditor acts as the high-level controller coordinating all actions.

    The design is intentionally modular so that new content types or storage backends
    can be added without changing the existing core logic.
*/

// Base class for all document content types.
// Each concrete element must know how to render itself into a string.
class DocumentElement {
public:
    virtual ~DocumentElement() = default;
    virtual string render() const = 0;
};

// A text block inside the document.
class TextElement : public DocumentElement {
private:
    string text;

public:
    TextElement(string text) : text(text) {}

    string render() const override {
        return text;
    }
};

// An image block represented as a placeholder string.
class ImageElement : public DocumentElement {
private:
    string imagePath;

public:
    ImageElement(string imagePath) : imagePath(imagePath) {}

    string render() const override {
        return "[Image:" + imagePath + "]";
    }
};

// Adds a newline in the rendered output.
class NewLineElement : public DocumentElement {
public:
    string render() const override {
        return "\n";
    }
};

// Adds a tab spacing in the rendered output.
class TabSpaceElement : public DocumentElement {
public:
    string render() const override {
        return "\t";
    }
};

// Represents the entire document as a collection of elements.
// It acts as a composition root that can render the complete document.
class Document {
private:
    vector<shared_ptr<DocumentElement>> elements;
    vector<vector<shared_ptr<DocumentElement>>> undoHistory;
    vector<vector<shared_ptr<DocumentElement>>> redoHistory;

public:
    // Add a new content element to the document.
    void addElement(shared_ptr<DocumentElement> element) {
        undoHistory.push_back(elements);
        redoHistory.clear();
        elements.push_back(move(element));
    }

    // Restore the previous document state when one is available.
    bool undo() {
        if (undoHistory.empty()) {
            return false;
        }

        redoHistory.push_back(elements);
        elements = move(undoHistory.back());
        undoHistory.pop_back();
        return true;
    }

    // Reapply a state that was previously undone.
    bool redo() {
        if (redoHistory.empty()) {
            return false;
        }

        undoHistory.push_back(elements);
        elements = move(redoHistory.back());
        redoHistory.pop_back();
        return true;
    }

    // Renders all elements in order to produce the final document body.
    string render() const {
        string result = "";
        for (const auto& element : elements) {
            result += element->render();
        }
        return result;
    }
};

// Strategy interface for saving the rendered document.
// Any storage type (file, database, cloud, etc.) can implement this contract.
class Persistence {
public:
    virtual ~Persistence() = default;
    virtual void save(string data) = 0;
};

// Saves the document data to a local text file.
class FileStorage : public Persistence {
public:
    void save(string data) override {
        ofstream outFile("document.txt");
        if (outFile) {
            outFile << data;
            outFile.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error" << endl;
        }
    }
};

// A sample storage provider that represents database-backed persistence.
class DBstorage : public Persistence {
public:
    void save(string data) override {
        cout << "Documents saved to database" << endl;
    }
};

// Main editor class that coordinates document editing and persistence.
// It depends on abstractions instead of concrete storage implementations.
class DocumentsEditor {
private:
    Document* document;
    Persistence* persistence;

public:
    DocumentsEditor(Document* document, Persistence* storage)
        : document(document), persistence(storage) {}

    // Adds plain text content to the document.
    void addText(string text) {
        document->addElement(make_shared<TextElement>(text));
    }

    // Adds an image block placeholder to the document.
    void addImage(string path) {
        document->addElement(make_shared<ImageElement>(path));
    }

    // Inserts a new line in the document.
    void addNewLine() {
        document->addElement(make_shared<NewLineElement>());
    }

    // Inserts a tab space into the document.
    void addTabSpace() {
        document->addElement(make_shared<TabSpaceElement>());
    }

    // Expose document history operations through the editor API.
    bool undo() {
        return document->undo();
    }

    bool redo() {
        return document->redo();
    }

    // Render on demand so the output always reflects the latest edit or undo.
    string renderDocuments() const {
        return document->render();
    }

    // Saves the current rendered content using the configured storage strategy.
    void saveDocuments() {
        persistence->save(renderDocuments());
    }
};

int main() {
    Document doc;
    FileStorage storage;
    DocumentsEditor editor(&doc, &storage);

    editor.addText("Google Docs Clone");
    editor.addNewLine();
    editor.addText("Project Status: Design Ready");
    editor.addNewLine();
    editor.addTabSpace();
    editor.addText("- Drafting document contents");
    editor.addNewLine();
    editor.addImage("images/logo.png");

    if (editor.undo()) {
        cout << "\nAfter undo:\n" << editor.renderDocuments() << endl;
    }
    if (editor.redo()) {
        cout << "\nAfter redo:\n" << editor.renderDocuments() << endl;
    }

    cout << "Rendered document:\n" << editor.renderDocuments() << endl;
    editor.saveDocuments();

    return 0;
}
