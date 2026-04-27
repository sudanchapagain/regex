/*
 * Thompson's construction algorithm, also called the McNaughton–Yamada–Thompson
 * algorithm is a method of transforming a regular expression into an
 * equivalent nondeterministic finite automaton (NFA).
 * 
 * This NFA can be used to match strings against the regular expression. This
 * algorithm is credited to Ken Thompson. Regular expressions and
 * nondeterministic finite automata are two representations of formal
 * languages. For instance, text processing utilities use regular expressions
 * to describe advanced search patterns, but NFAs are better suited for
 * execution on a computer. Hence, this algorithm is of practical interest,
 * since it can compile regular expressions into NFAs. From a theoretical
 * point of view, this algorithm is a part of the proof that they both accept
 * exactly the same languages, that is, the regular languages.
 * 
 * An NFA can be made deterministic by the powerset construction and then be
 * minimized to get an optimal automaton corresponding to the given regular
 * expression. However, an NFA may also be interpreted directly. To decide
 * whether two given regular expressions describe the same language, each can
 * be converted into an equivalent minimal deterministic finite automaton via
 * Thompson's construction, powerset construction, and DFA minimization. If,
 * and only if, the resulting automata agree up to renaming of states, the
 * regular expressions' languages agree.
 * 
 *                   - <https://en.wikipedia.org/wiki/Thompson's_construction>
*/


#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <fmt/base.h>

/*
 * A state in a nondeterministic finite automaton (NFA).
 *
 *
 * it has two kinds of edges:
 * epsilon_edges: transitions that consume no input. you can move along these
 *                without reading a character. these are what make the
 *                automaton nondeterministic and flexible.
 * edges: a mapping from a character to a list of possible next states. the
 *           list is important. from one state and one character, you may
 *           branch into multiple states.
 *
 *
 * accepting flag marks whether this state represents a successful end.
 * this means, when matching, if you land in any state with `accepting == true`
 * after consuming the whole string, the input matches.
 */
class Node {
public:
    std::vector<Node*> epsilon_edges;
    std::map<char, std::vector<Node*> > edges;
    bool accepting;

    Node(bool is_accepting = false) : accepting(is_accepting) {}

    void add_epsilon(Node* next) {
        epsilon_edges.push_back(next);
    }

    void add_edge(char c, Node* next) {
        edges[c].push_back(next);
    }
};

/*
 * NFA graph built out of Node objects which has
 * start, accept, storage
 *
 * Construction functions which is core of Thompson's construction
 * (`make_character(c)`) builds the simplest non-empty automaton. make epsilon
 * accepts empty strings. similarly, union, concat, kleene are all implemented
 * in the following manner. i think it's self explanatory.
 */
class Automaton {
public:
    Node* start;
    Node* accept;
    std::vector<std::unique_ptr<Node>> storage;

    Automaton() {
        std::unique_ptr<Node> s(new Node(false));
        std::unique_ptr<Node> a(new Node(true));

        start = s.get();
        accept = a.get();

        storage.push_back(std::move(s));
        storage.push_back(std::move(a));
    }

    void take_nodes_from(Automaton& other) {
        for (size_t i = 0; i < other.storage.size(); ++i) {
            storage.push_back(std::move(other.storage[i]));
        }
        other.storage.clear();
    }

    static Automaton make_character(char c) {
        Automaton a;
        a.start->add_edge(c, a.accept);
        return a;
    }

    static Automaton make_epsilon() {
        Automaton a;
        a.start->add_epsilon(a.accept);
        return a;
    }

    static Automaton make_union(Automaton& left, Automaton& right) {
        Automaton result;

        left.accept->accepting = false;
        right.accept->accepting = false;

        result.start->add_epsilon(left.start);
        result.start->add_epsilon(right.start);

        left.accept->add_epsilon(result.accept);
        right.accept->add_epsilon(result.accept);

        result.take_nodes_from(left);
        result.take_nodes_from(right);

        return result;
    }

    static Automaton make_concatenation(Automaton& left, Automaton& right) {
        Automaton result;

        left.accept->add_epsilon(right.start);
        left.accept->accepting = false;

        result.start = left.start;
        result.accept = right.accept;

        result.take_nodes_from(left);
        result.take_nodes_from(right);

        return result;
    }

    static Automaton make_kleene_star(Automaton& base) {
        Automaton result;

        result.start->add_epsilon(base.start);
        result.start->add_epsilon(result.accept);

        base.accept->add_epsilon(base.start);
        base.accept->add_epsilon(result.accept);
        base.accept->accepting = false;

        result.take_nodes_from(base);

        return result;
    }
    /*
     * given a set of states, it expands it to include every state reachable
     * through only epsilon transitions. This is essential because the NFA can
     * move without consuming input. so need to normalize the current state
     * set before and after every character.
     */
    static std::set<Node*> epsilon_closure(const std::set<Node*>& input) {
        std::set<Node*> result = input;
        std::stack<Node*> st;

        for (std::set<Node*>::iterator it = input.begin(); it != input.end(); ++it) {
            st.push(*it);
        }

        while (!st.empty()) {
            Node* current = st.top();
            st.pop();

            for (size_t i = 0; i < current->epsilon_edges.size(); ++i) {
                Node* next = current->epsilon_edges[i];
                if (result.find(next) == result.end()) {
                    result.insert(next);
                    st.push(next);
                }
            }
        }

        return result;
    }

    // given a set of states, and a character, compute all possible next
    // states reachable by consuming that character.
    static std::set<Node*> move(const std::set<Node*>& input, char c) {
        std::set<Node*> result;

        for (std::set<Node*>::const_iterator it = input.begin(); it != input.end(); ++it) {
            Node* node = *it;

            std::map<char, std::vector<Node*> >::iterator found =
                node->edges.find(c);

            if (found == node->edges.end()) {
                continue;
            }

            const std::vector<Node*>& targets = found->second;
            for (size_t i = 0; i < targets.size(); ++i) {
                result.insert(targets[i]);
            }
        }

        return result;
    }
};

// regex string into an NFA via recursive descent -ish
class RegexParser {
public:
    RegexParser(const std::string& pattern): text(pattern), position(0) {}

    void compile() {
        machine = parse_expression();
    }

    bool matches(const std::string& input) {
        std::set<Node*> current;
        current.insert(machine.start);
        current = Automaton::epsilon_closure(current);

        for (size_t i = 0; i < input.size(); ++i) {
            current = Automaton::move(current, input[i]);
            current = Automaton::epsilon_closure(current);

            if (current.empty()) {
                return false;
            }
        }

        for (std::set<Node*>::iterator it = current.begin(); it != current.end(); ++it) {
            if ((*it)->accepting) {
                return true;
            }
        }

        return false;
    }

private:
    std::string text;
    size_t position;
    Automaton machine;

    Automaton parse_expression() {
        return parse_union();
    }

    Automaton parse_union() {
        Automaton left = parse_concatenation();

        while (position < text.size() && text[position] == '|') {
            ++position;
            Automaton right = parse_concatenation();
            left = Automaton::make_union(left, right);
        }

        return left;
    }

    Automaton parse_concatenation() {
        Automaton left = parse_star();

        while (position < text.size() &&
               text[position] != '|' &&
               text[position] != ')') {
            Automaton right = parse_star();
            left = Automaton::make_concatenation(left, right);
        }

        return left;
    }

    Automaton parse_star() {
        Automaton base = parse_atom();

        while (position < text.size() && text[position] == '*') {
            ++position;
            base = Automaton::make_kleene_star(base);
        }

        return base;
    }

    Automaton parse_atom() {
        if (position >= text.size()) {
            return Automaton::make_epsilon();
        }

        char c = text[position++];
        if (c == '(') {
            Automaton inside = parse_expression();

            if (position >= text.size() || text[position] != ')') {
                std::cerr << "error: missing ')'\n";
                std::exit(1);
            }

            ++position;
            return inside;
        }

        return Automaton::make_character(c);
    }
};

void print_help() {
    fmt::println("\nUSAGE:");
    fmt::println("\tregex <pattern> <string>");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_help();
        return EXIT_FAILURE;
    }

    std::string pattern = argv[1];
    std::string input   = argv[2];

    RegexParser parser(pattern);
    parser.compile();

    bool ok = parser.matches(input);

    if (ok) {
        fmt::println("match");
        return EXIT_SUCCESS;
    } else {
        fmt::println("no match");
        return EXIT_FAILURE;
    }
}
