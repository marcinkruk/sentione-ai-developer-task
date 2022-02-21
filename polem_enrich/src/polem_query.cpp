
#include <string>
#include <vector>
#include <boost/algorithm/string/join.hpp>

#include "polem_query.h"

Polem_query::Polem_query(std::string orths)
    : orths{orths}
{}

void Polem_query::add_lemma(std::string lemma) {
    lemmas.push_back(std::move(lemma));
}

void Polem_query::add_tag(std::string tag) {
    tags.push_back(std::move(tag));
}

std::string Polem_query::get_orths() const {
    return orths;
}

std::string Polem_query::get_lemmas() const {
    return boost::algorithm::join(lemmas, " ");
}

std::string Polem_query::get_tags() const {
    return boost::algorithm::join(tags, " ");
}
