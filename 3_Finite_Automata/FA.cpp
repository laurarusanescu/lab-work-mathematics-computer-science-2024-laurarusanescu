#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class FiniteAutomaton {
private:
    set<string> states;
    set<char> alphabet;
    map<pair<string, char>, string> transitions;  // Each transition is now a direct state change
    set<string> finalStates;
    string startState;

public:
    void loadFA(const string &filePath) {
        ifstream file(filePath);
        if (!file) {
            cerr << "Error opening file!" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            parseStates(line, states);
            break;
        }

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            parseAlphabet(line, alphabet);
            break;
        }

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            startState = line;
            break;
        }

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            parseStates(line, finalStates);
            break;
        }

        while (getline(file, line)) {
            if (!line.empty() && line[0] != '#') {
                parseTransition(line);
            }
        }
        file.close();
    }

    void displayRules() const {
        // Display states
        cout << "States: ";
        for (const auto &state : states) {
            cout << state << " ";
        }
        cout << endl;

        // Display alphabet
        cout << "Alphabet: ";
        for (const auto &symbol : alphabet) {
            cout << symbol << " ";
        }
        cout << endl;

        // Display start state
        cout << "Start State: " << startState << endl;

        // Display final states
        cout << "Final States: ";
        for (const auto &state : finalStates) {
            cout << state << " ";
        }
        cout << endl;

        // Display transitions
        cout << "Transitions: " << endl;
        for (const auto &transition : transitions) {
            cout << transition.first.first << " --" << transition.first.second
                 << "--> " << transition.second << endl;
        }
        cout << endl;
    }
    // Verify a given string is a valid lexical token based on the finite automaton
    bool verifyLexicalToken(const string &inputString) {
        string currentState = startState;

        for (char symbol : inputString) {
            if (alphabet.find(symbol) == alphabet.end()) {
                cout << "Symbol " << symbol << " not in alphabet!" << endl;
                return false;
            }

            pair<string, char> key = make_pair(currentState, symbol);
            if (transitions.find(key) != transitions.end()) {
                currentState = transitions[key];
            } else {
                cout << "No transition from state " << currentState << " with symbol " << symbol << endl;
                return false;
            }
        }
        return finalStates.find(currentState) != finalStates.end();
    }

private:
    void parseStates(const string &line, set<string> &stateSet) {
        stringstream ss(line);
        string state;
        while (ss >> state) {
            stateSet.insert(state);
        }
    }

    void parseAlphabet(const string &line, set<char> &alphabetSet) {
        stringstream ss(line);
        char symbol;
        while (ss >> symbol) {
            alphabetSet.insert(symbol);
        }
    }

    void parseTransition(const string &line) {
        stringstream ss(line);
        string fromState, toState;
        char symbol;
        ss >> fromState >> symbol >> toState;
        transitions[make_pair(fromState, symbol)] = toState;
    }
};

int main() {
    FiniteAutomaton fa;
    fa.loadFA("FA.in");

    fa.displayRules();

    // Token classification (optional, uncomment when adding back verifyLexicalToken)
    string token;
    cout << "Enter a token to classify: ";
    cin >> token;

    // Uncomment the following lines when verifyLexicalToken is implemented:
    if (fa.verifyLexicalToken(token)) {
        cout << "'" << token << "' is a valid lexical token." << endl;
    } else {
        cout << "'" << token << "' is not a valid lexical token." << endl;
    }

    return 0;
}
