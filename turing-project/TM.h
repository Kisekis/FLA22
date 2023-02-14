#ifndef TMP_TM_H
#define TMP_TM_H
#include <set>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <regex>
#include <map>
typedef std::string State, SymbolList, DirectionList;
typedef char Symbol, Direction;
struct tapeLoc
{
    Symbol symbol;
    int index;
};
struct transition
{
    State oldState;
    State newState;
    SymbolList oldSymbols;
    SymbolList newSymbols;
    DirectionList directions;
};
enum class inputRegexType
{
    Q,
    S,
    G,
    q0,
    B,
    F,
    N,
    T
};

class TM
{
public:
    bool verbose;
    std::set<State> Q;                                // the finite set of states
    std::set<Symbol> S;                               // the finite set of input symbols
    std::set<Symbol> G;                               // the complete set of tape symbols
    State startState;                                 // the start state
    Symbol blankSymbol;                               // the blank symbol
    std::set<State> F;                                // the set of final states
    int tapeCount;                                    // the number of tapes
    std::vector<transition> transitions;              // transitions
    std::vector<std::deque<tapeLoc>> tapes;           // tapes
    std::vector<std::deque<tapeLoc>::iterator> heads; // heads
    State currentState;
    SymbolList input;

public:
    void run(const std::string &input);
    bool singleMove();
    void singleTapeMove(std::deque<tapeLoc>::iterator &it, Direction direction, int k, Symbol newSymbol);
    static bool symbolListMatch(const SymbolList &ts, const SymbolList &cur);
    void printFirstTape();
    void printVerboseTape(int i);
    void shrinkTape(int i);
    void checkInputValidity();
};
std::shared_ptr<TM> parseFile(const std::string &path, bool verbose);
void parseDefi(const std::string &line, const std::shared_ptr<TM> &tm, bool verbose);
void parseTrans(const std::string &line, const std::shared_ptr<TM> &tm, bool verbose);

#endif
