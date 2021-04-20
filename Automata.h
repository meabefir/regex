#pragma once
#include "Node.h"
#include "Transition.h"

struct hash_pair
{
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

typedef std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> dfa_container;

class Automata
{
private:
	std::vector<Node*>* nodes;
    dfa_container dfa;
    int n;
    int nr_m;
    std::string nod_start;
    int nr_finale;
    std::vector<std::string> finale;

    std::string lambda;

    std::string regex;

public:
	Automata(std::vector<Node*>*);
	~Automata();

    bool isLambda(std::string);

	std::string getRegex();
};

