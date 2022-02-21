#ifndef POLEM_QUERY_H
#define POLEM_QUERY_H

#include <string>
#include <vector>

class Polem_query {

    public:
        Polem_query() = delete;
        Polem_query(std::string);

        void add_lemma(std::string);
        void add_tag(std::string);

        std::string get_orths() const;
        std::string get_lemmas() const;
        std::string get_tags() const;

    private:
        const std::string orths;
        std::vector<std::string> lemmas;
        std::vector<std::string> tags;

};

#endif
