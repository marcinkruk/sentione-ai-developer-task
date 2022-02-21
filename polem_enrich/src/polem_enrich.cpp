#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <polem-dev/CascadeLemmatizer.h>

#include "polem_enrich.h"
#include "polem_query.h"

Polem_enrich::Polem_enrich()
    : cascade_lemmatizer{CascadeLemmatizer::assembleLemmatizer()}
{}

std::string Polem_enrich::enrich(std::string input) {

    auto j_object = input_to_json(input);
    if(j_object.empty())
        return "";

    auto j = enrich_json(j_object);

    return to_string(j);
}

json Polem_enrich::input_to_json(std::string input) {

    if(!json::accept(input))
        return empty_json();

    json j_object = json::parse(input);

    // Given limited test cases input validation is simplified.
    if(!j_object.contains("docs"))
        return empty_json();

    return j_object;
}

inline json Polem_enrich::empty_json() {

    return "{}"_json;
}

json Polem_enrich::enrich_json(json input) {
    auto j_docs = input.at("docs");

    for(auto doc : j_docs) {
        auto j_labels = doc.at("labels");
        for(auto label : j_labels) {
            if(label.at("serviceName") == "NER") {
                Polem_query query{label.at("value")};

                for(auto lab : j_labels) {
                    if(lab.at("startToken") >= label.at("startToken")
                            && lab.at("startToken") <= label.at("endToken")) {

                        if(lab.at("fieldName") == "posTag") {
                            query.add_tag(lab.at("value"));
                        }

                        if(lab.at("fieldName") == "lemmas") {
                            query.add_lemma(lab.at("value")[0]);
                        }
                    }
                }
                std::cout << "************* " << query_lemmatizer(query) << "\n";
            }
        }
    }

    // Do nothing for development
    return input;
}

inline std::string Polem_enrich::query_lemmatizer(const Polem_query query) {
    return cascade_lemmatizer.lemmatizeS(
            query.get_orths(),
            query.get_lemmas(),
            query.get_tags(),
            false);
}
