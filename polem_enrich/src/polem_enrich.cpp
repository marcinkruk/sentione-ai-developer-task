#include <iostream>
#include <string>
#include <vector>

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
                auto query = create_query(label, j_labels);
                auto query_result = query_lemmatizer(query);
                auto label_to_add = create_label(label, query_result);
            }
        }
    }

    // Do nothing for development
    return input;
}

Polem_query Polem_enrich::create_query(json ner_json, std::vector<json> j_labels) {
    Polem_query query{ner_json.at("value")};

    for(auto lab : j_labels) {
        if(lab.at("startToken") >= ner_json.at("startToken")
                && lab.at("startToken") <= ner_json.at("endToken")) {

            if(lab.at("fieldName") == "posTag") {
                query.add_tag(lab.at("value"));
            }

            if(lab.at("fieldName") == "lemmas") {
                query.add_lemma(lab.at("value")[0]);
            }
        }
    }

    return query;
}

inline std::string Polem_enrich::query_lemmatizer(const Polem_query query) {
    return cascade_lemmatizer.lemmatizeS(
            query.get_orths(),
            query.get_lemmas(),
            query.get_tags(),
            false);
}

json Polem_enrich::create_label(json j_label, std::string value) {

    std::cout << std::setw(4) << j_label << "\n\n";

    json j_polem = R"(
            {
                "fieldName": "polem",
                "name": "polem",
                "serviceName": "polem"
            }
        )"_json;

    j_label.update(j_polem);

    std::cout << std::setw(4) << j_label << value << "\n";

    return j_label;
}
