#include "Automata.h"
#include "Node.h"

Automata::Automata(std::vector<Node*>* nodes):
    nodes(nodes), lambda("e")
{
    n = this->nodes->size();
    for (auto& np : *this->nodes)
    {
        std::string from_id, to_id, symbol;
        for (auto& trans : *np->getTransitions())
        {
            from_id = trans->getNode1()->getId();
            to_id = trans->getNode2()->getId();
            symbol = trans->getText();

            std::replace(symbol.begin(), symbol.end(), ',', '+');

            if (dfa.find({ from_id, to_id }) == dfa.end())
            {
                dfa[{from_id, to_id}] = symbol;
            }
            else
            {
                dfa[{from_id, to_id}] += "+" + symbol;
            }
        }
    }

    // parantezam tranzitiile
    for (auto& per : dfa)
    {
        per.second = "(" + per.second + ")";
    }

    // set the start node
    nod_start = "";
    for (Node* node : *this->nodes)
    {
        bool is_starting = node->isStartNode();
        if (is_starting)
            nod_start = node->getId();
    }
    if (nod_start == "")
        return;

    // set nr of finale and add them in vector
    for (Node* node : *this->nodes)
    {
        if (node->isFinalNodeFunc())
        {
            nr_finale++;
            finale.push_back(node->getId());
        }
    }
    if (nr_finale == 0)
        return;
    /// verifica daca in nodul de start intra noduri
    /// daca da, fa nod auxiliar care merge cu lambda in fostul nod de start

    for (auto& per : dfa)
    {
        std::string to = per.first.second;

        if (to == nod_start)
        {
            dfa[{"-1", nod_start}] = "e";
            nod_start = "-1";
            n++;
            break;
        }
    }

    /// daca sunt mai multe noduri finale
    if (finale.size() > 1)
    {
        for (std::string fin : finale)
        {
            dfa[{fin, "-2"}] = "e";
        }
        finale.clear();
        finale.push_back("-2");
        n++;
    }

    /// daca este doar un nod final dar acesta merge si in alte stari
    else if (finale.size() == 1)
    {
        for (auto& per : dfa)
        {
            std::string from = per.first.first;

            if (from == finale[0])
            {
                dfa[{from, "-2"}] = "e";
                finale.clear();
                finale.push_back("-2");
                n++;
                break;
            }
        }
    }

    while (n > 2)
    {
        /// pick node to remove
        std::string node_to_remove;

        for (auto& per : dfa)
        {
            std::string from = per.first.first;
            if (from != nod_start && from != finale[0])
            {
                node_to_remove = from;
                break;
            }
        }

        // verifica daca nodul de eliminat are muchie care duce tot in el
        std::string loop = "";
        for (auto& per : dfa)
        {
            if (per.first.first == per.first.second && per.first.first == node_to_remove)
            {
                loop = per.second;
                break;
            }
        }

        // pt fiecare nod care intra in nodul de eliminat
        for (auto& per : dfa)
        {
            std::string goes_to = per.first.second;
            std::string _from = per.first.first;
            if (goes_to == node_to_remove && _from != node_to_remove)
            {
                // pt fiecare nod in care ajunge prin intermediul nodului de eliminat
                for (auto& per2 : dfa)
                {
                    std::string from = per2.first.first;
                    std::string _to = per2.first.second;
                    if (from == node_to_remove && _to != node_to_remove)
                    {
                        // aici avem 2 tranzitii care trebuie conectate de nodul de eliminat

                        std::string from_origin = per.first.first;
                        std::string to_origin = per2.first.second;

                        // verificam daca exista deja in dfa muchia
                        // si daca exista o concatenam la raspuns
                        std::string trans_here_already = "";
                        if (dfa.find({ from_origin, to_origin }) != dfa.end())
                            trans_here_already = dfa[{from_origin, to_origin}];

                        std::string trans_to_add = "";
                        // adauga tranzitia care duce la nodul de eliminat
                        if (!isLambda(per.second))
                            trans_to_add += per.second;

                        // adauga loopul
                        if (loop != "" && !isLambda(loop))
                        {
                            if (trans_to_add == "")
                                trans_to_add = loop + "*";
                            else
                                trans_to_add += "x" + loop + "*";
                        }
                        
                        // adauga tranzitia care pleaca de la nodul de eliminat
                        if (!isLambda(per2.second) && per2.second != "")
                            if (trans_to_add == "")
                                trans_to_add = per2.second;
                            else
                                trans_to_add += "x" + per2.second;

                        if (trans_to_add != "" && !isLambda(trans_to_add))
                            trans_to_add = "(" + trans_to_add + ")";

                        // daca exista deja o tranzitie intre aceleasi noduri concateneaza
                        if (trans_here_already != "" && !isLambda(trans_here_already))
                        {
                            if (trans_to_add == "")
                                trans_to_add = trans_here_already;
                            else
                            {
                                if (trans_to_add == "" || isLambda(trans_to_add))
                                   trans_to_add = trans_here_already;
                                else
                                   trans_to_add = "(" + trans_to_add + "+" + trans_here_already + ")";
                            }
                        }

                        /// add or update the new transition
                        dfa[{from_origin, to_origin}] = trans_to_add;

                    }
                }
            }
        }
        // remove all transitions that contain node_to_remove
        for (auto it = dfa.cbegin(); it != dfa.cend();)
        {
            if ((*it).first.first == node_to_remove || (*it).first.second == node_to_remove)
            {
                it = dfa.erase(it);
            }
            else
            {
                ++it;
            }
        }
        n--;
    }

    regex = dfa[{nod_start, finale[0]}];
}

Automata::~Automata()
{
}

bool Automata::isLambda(std::string s)
{
    if (s == lambda)
        return true;
    if (s == "(" + lambda + ")")
        return true;
    return false;
}

std::string Automata::getRegex()
{
    return regex;
}
