#include "TM.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
std::map<inputRegexType, std::pair<std::regex, bool>> inputRegexes = {
    {inputRegexType::Q, {std::regex("#Q\\s=\\s\\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}$"), false}},                                          // states
    {inputRegexType::S, {std::regex("#S\\s=\\s\\{([^\\s,;\\{\\}*_](,[^\\s,;\\{\\}*_])*)?\\}$"), false}},                                 // symbols
    {inputRegexType::G, {std::regex("#G\\s=\\s\\{[^\\s,;\\{\\}*](,[^\\s,;\\{\\}*])*\\}$"), false}},                                      // tape symbols
    {inputRegexType::q0, {std::regex("#q0\\s=\\s([a-zA-Z0-9_]+)$"), false}},                                                             // start state
    {inputRegexType::B, {std::regex("#B\\s=\\s_$"), false}},                                                                             // blank symbol
    {inputRegexType::F, {std::regex("#F\\s=\\s\\{([a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*)?\\}$"), false}},                                       // final states
    {inputRegexType::N, {std::regex("#N\\s=\\s(\\d+)$"), false}},                                                                        // number of tapes
    {inputRegexType::T, {std::regex("([a-zA-Z0-9_]+)\\s([^\\s,;\\{\\}]+)\\s([^\\s,;\\{\\}]+)\\s([lr*]+)\\s([a-zA-Z0-9_]+)$"), false}} // transitions
};

std::shared_ptr<TM> parseFile(const std::string &path, bool verbose)
{
    std::ifstream infile(path);
    std::string line;
    if (!infile)
    {
        std::cerr << "Cant open file" << std::endl;
        exit(1);
    }
    std::shared_ptr<TM> tm = std::make_shared<TM>();
    tm->verbose = verbose;
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue; // empty
        if (line[0] == ';')
            continue; // comment
        line = std::regex_replace(line, std::regex(" *$"), "");//remove tailing space
        
        if (line.find(';') != std::string::npos)
        { // comment in tail
            line = std::regex_replace(line, std::regex(" *;.*$"), "");
        }
        if (line[0] == '#')
        { // definition
            parseDefi(line, tm, verbose);
            continue;
        }
        if (std::regex_match(line, inputRegexes[inputRegexType::T].first))
        {
            parseTrans(line, tm, verbose);
            continue;
        }

        if (!verbose)
        {
            std::cerr << "syntax error" << std::endl;
        }
        else
        {
            std::cerr << "syntax error : (Wrong line)" << line << std::endl;
        }
        exit(1);
    }
    for (const auto &entry : inputRegexes)
    {
        if (!entry.second.second)
        {
            if (!verbose)
            {
                std::cerr << "syntax error" << std::endl;
            }
            else
            {
                std::cerr << "syntax error : "
                          << "(TM is lack of Definitions or Transitions)" << std::endl;
            }
            exit(1);
        }
    }
    return tm;
}

void parseDefi(const std::string &line, const std::shared_ptr<TM> &tm, bool verbose)
{
    bool matched = false;
    for (auto &entry : inputRegexes)
    {
        if (std::regex_match(line, entry.second.first))
        {
            if (entry.second.second)
            {
                if (!verbose)
                {
                    std::cerr << "syntax error" << std::endl;
                }
                else
                {
                    std::cerr << "syntax error : "
                              << "(Duplicated TM definition)" << line << std::endl;
                }
                exit(1);
            }
            matched = true;
            entry.second.second = true;
            switch (entry.first)
            {
            case inputRegexType::Q:
            {
                std::regex subStr("[a-zA-Z0-9_]+");
                std::sregex_iterator iter(line.begin(), line.end(), subStr);
                std::sregex_iterator end;
                int count = 0;
                while (iter != end)
                {
                    if (count >= 1)
                    {
                        tm->Q.insert((*iter)[0].str());
                    }
                    ++iter;
                    ++count;
                }
                break;
            }
            case inputRegexType::S:
            {
                std::regex subStr("[^\\s,;\\{\\}\\*_]");
                std::sregex_iterator iter(line.begin(), line.end(), subStr);
                std::sregex_iterator end;
                int count = 0;
                while (iter != end)
                {
                    if (count >= 3)
                    {
                        tm->S.insert((*iter)[0].str()[0]);
                    }
                    ++iter;
                    ++count;
                }
                break;
            }
            case inputRegexType::G:
            {
                std::regex subStr("[^\\s,;\\{\\}\\*]");
                std::sregex_iterator iter(line.begin(), line.end(), subStr);
                std::sregex_iterator end;
                int count = 0;
                while (iter != end)
                {
                    if (count >= 3)
                    {
                        tm->G.insert((*iter)[0].str()[0]);
                    }
                    ++iter;
                    ++count;
                }
                break;
            }
            case inputRegexType::q0:
            {
                std::smatch m;
                std::regex_search(line, m, entry.second.first);
                tm->startState = m[1].str();
                break;
            }
            case inputRegexType::B:
                tm->blankSymbol = '_';
                break;
            case inputRegexType::F:
            {
                std::regex subStr("[a-zA-Z0-9_]+");
                std::sregex_iterator iter(line.begin(), line.end(), subStr);
                std::sregex_iterator end;
                int count = 0;
                while (iter != end)
                {
                    if (count >= 1)
                    {
                        tm->F.insert((*iter)[0].str());
                    }
                    ++iter;
                    ++count;
                }
                break;
            }
            case inputRegexType::N:
            {
                std::smatch m;
                std::regex_search(line, m, entry.second.first);
                tm->tapeCount = std::stoi(m[1].str());
                break;
            }
            }
            break;
        }
    }
    if (!matched)
    {
        if (!verbose)
        {
            std::cerr << "syntax error" << std::endl;
        }
        else
        {
            std::cerr << "syntax error : "
                      << "(Wrong definition)" << line << std::endl;
        }
        exit(1);
    }
}

void parseTrans(const std::string &line, const std::shared_ptr<TM> &tm, bool verbose)
{
    std::smatch m;
    transition ts;
    std::regex_search(line, m, inputRegexes[inputRegexType::T].first);

    ts.oldState = m[1].str();
    ts.oldSymbols = m[2].str();
    ts.newSymbols = m[3].str();
    ts.directions = m[4].str();
    ts.newState = m[5].str();
    tm->transitions.push_back(ts);
    inputRegexes[inputRegexType::T].second = true;
}
void TM::shrinkTape(int i)
{
    auto &tape = this->tapes[i];
    auto it = this->heads[i];
    while (true)
    {
        if (tape.begin() != it && tape.begin()->symbol == '_')
        {
            tape.pop_front();
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        if (tape.end() - 1 != it && (tape.end() - 1)->symbol == '_')
        {
            tape.pop_back();
        }
        else
        {
            break;
        }
    }
}
void TM::printVerboseTape(int i)
{
    using std::cout;
    using std::endl;
    using std::string;
    string index;
    string tape;
    string head;
    shrinkTape(i);
    for (auto it = this->tapes[i].begin(); it != this->tapes[i].end(); it++)
    {
        index += ' ' + std::to_string(abs(it->index));
        string tapeAppend(index.size() - tape.size(), ' ');
        tapeAppend[1] = it->symbol;
        tape += tapeAppend;
        string headAppend(index.size() - head.size(), ' ');
        if (it == this->heads[i])
        {
            headAppend[1] = '^';
        }
        head += headAppend;
    }
    if (i / 10 == 0)
    {
        cout << "Index" << i << " :" << index << endl;
        cout << "Tape" << i << "  :" << tape << endl;
        cout << "Head" << i << "  :" << head << endl;
    }
    else
    {
        cout << "Index" << i << ":" << index << endl;
        cout << "Tape" << i << " :" << tape << endl;
        cout << "Head" << i << " :" << head << endl;
    }
}

bool TM::singleMove()
{
    static int step = 0;
    if (verbose)
    {
        using std::cout;
        using std::endl;
        cout << "Step   : " << step << endl;
        cout << "State  : " << this->currentState << endl;
        for (int i = 0; i < tapeCount; i++)
        {
            printVerboseTape(i);
        }
        cout << "---------------------------------------------" << endl;
    }
    SymbolList now;
    for (const auto &it : this->heads)
    {
        now += (*it).symbol;
    }
    for (auto &ts : this->transitions)
    {
        if (symbolListMatch(ts.oldSymbols, now) && ts.oldState == this->currentState)
        {
            for (int i = 0; i < this->tapeCount; i++)
            {
                singleTapeMove(this->heads[i], ts.directions[i], i, ts.newSymbols[i]);
            }
            this->currentState = ts.newState;
            step++;
            return true;
        }
    }
    step++;
    return false;
}
void TM::run(const std::string &input)
{
    this->input = input;
    checkInputValidity();
    int n = input.size();
    for (int i = 0; i < this->tapeCount; i++)
    {
        if (i == 0)
        {
            std::deque<tapeLoc> first;
            if (n == 0)
            {
                first.push_back({'_', 0});
            }
            else
            {
                for (int j = 0; j < n; j++)
                {
                    first.push_back({input[j], j});
                }
            }
            this->tapes.push_back(first);
            this->heads.push_back(this->tapes[i].begin());
            continue;
        }
        std::deque<tapeLoc> blankTape;
        if (n == 0)
        {
            blankTape.push_back({'_', 0});
        }
        else
        {
            for (int j = 0; j < n; j++)
            {
                blankTape.push_back({'_', j});
            }
        }
        this->tapes.push_back(blankTape);
        this->heads.push_back(this->tapes[i].begin());
    }
    this->currentState = this->startState;
    if (verbose)
    {
        std::cout << "Input: " << input << std::endl;
        std::cout << "==================== RUN ====================" << std::endl;
    }
    while (TM::singleMove())
    {
    }
    if (verbose)
    {
        std::cout << "Result: ";
        printFirstTape();
        std::cout << "==================== END ====================" << std::endl;
    }
    else
    {
        printFirstTape();
    }
}
bool TM::symbolListMatch(const SymbolList &ts, const SymbolList &cur)
{
    int m = ts.size();
    int n = cur.size();
    if (m != n)
    {
        std::cerr << "Wrong size" << std::endl;
    }
    for (int i = 0; i < n; i++)
    {
        if (!(ts[i] == cur[i] || (ts[i] == '*' && cur[i] != '_')))
        {
            return false;
        }
    }
    return true;
}
void TM::singleTapeMove(std::deque<tapeLoc>::iterator &it, Direction direction, int k, Symbol newSymbol)
{
    switch (direction)
    {
    case '*':
        if (newSymbol != '*')
        {
            (*it).symbol = newSymbol;
        }
        return;
    case 'r':
        if (it == this->tapes[k].end() - 1)
        {
            this->tapes[k].push_back({'_', (*it).index + 1});
        }
        if (newSymbol != '*')
        {
            (*it).symbol = newSymbol;
        }
        it++;
        return;
    case 'l':
        if (it == this->tapes[k].begin())
        {
            this->tapes[k].push_front({'_', (*it).index - 1});
        }
        if (newSymbol != '*')
        {
            (*it).symbol = newSymbol;
        }
        it--;
        return;
    }
}
void TM::printFirstTape()
{
    std::string output;
    for (const auto &x : tapes[0])
    {
        output += x.symbol;
    }
    output = std::regex_replace(output, std::regex("^_+"), "");
    output = std::regex_replace(output, std::regex("_+$"), "");
    std::cout << output << std::endl;
}

void TM::checkInputValidity()
{
    std::string mark = "       ";
    for (const auto &c : this->input)
    {
        if (this->S.find(c) == this->S.end())
        {
            if (!verbose)
            {
                std::cerr << "illegal input" << std::endl;
            }
            else
            {
                std::cerr << "Input: " << input << std::endl;
                std::cerr << "==================== ERR ====================" << std::endl;
                std::cerr << "error: '" << c << "' was not declared in the set of input symbols" << std::endl;
                std::cerr << "Input: " << input << std::endl;
                std::cerr << mark << '^' << std::endl;
                std::cerr << "==================== END ====================" << std::endl;
            }
            exit(1);
        }
        mark += ' ';
    }
}
