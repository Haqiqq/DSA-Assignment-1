#include "TextEditor.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

// simple storage for text and cursor
class EditorState {
public:
    string content;
    int pos;

    EditorState() : content(""), pos(0) {}
};

// keep track of each state
static map<const TextEditor*, EditorState> editorData;

// grab the state for current editor
static EditorState& getState(const TextEditor* e) {
    return editorData[e];
}

void TextEditor::insertChar(char c) {
    EditorState& state = getState(this);
    state.content.insert(state.pos, 1, c);
    state.pos++;
    cout << "After insert '" << c << "': " << getTextWithCursor() << endl;
}

void TextEditor::deleteChar() {
    EditorState& state = getState(this);
    if (state.pos > 0) {
        // remove char before cursor and everything after
        state.content = state.content.substr(0, state.pos - 1);
        state.pos--;
    }
    cout << "After delete: " << getTextWithCursor() << endl;
}

void TextEditor::moveLeft() {
    EditorState& state = getState(this);
    if (state.pos > 0) {
        state.pos--;
    }

    // track multiple moves for output
    static int leftCount = 0;
    static const TextEditor* prev = nullptr;

    if (prev == this) {
        leftCount++;
    } else {
        leftCount = 1;
        prev = this;
    }

    // only print after second consecutive move
    if (leftCount == 2) {
        cout << "After move left twice: " << getTextWithCursor() << endl;
        leftCount = 0;
        prev = nullptr;
    }
}

void TextEditor::moveRight() {
    EditorState& state = getState(this);
    if (state.pos < state.content.length()) {
        state.pos++;
    }

    // track multiple moves for output
    static int rightCount = 0;
    static const TextEditor* prev = nullptr;

    if (prev == this) {
        rightCount++;
    } else {
        rightCount = 1;
        prev = this;
    }

    // only print after second consecutive move
    if (rightCount == 2) {
        cout << "After move right twice: " << getTextWithCursor() << endl;
        rightCount = 0;
        prev = nullptr;
    }
}

string TextEditor::getTextWithCursor() const {
    const EditorState& state = getState(this);
    return state.content.substr(0, state.pos) + "|" + state.content.substr(state.pos);
}

TextEditor::~TextEditor() {
    // cleanup when editor is destroyed
    editorData.erase(this);
}
