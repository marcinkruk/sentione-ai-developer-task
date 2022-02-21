#include <iostream>
#include <string>
#include <vector>
#include <iterator>

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

    for(unsigned int i = 0; i < j_docs.size(); ++i) {
        auto polem_labels = create_json_with_polem_labels(j_docs[i].at("labels"));
        j_docs[i]["labels"] = polem_labels;
    }

    input["docs"] = j_docs;
    return input;
}

std::vector<json> Polem_enrich::create_json_with_polem_labels(std::vector<json> j_labels) {
    std::vector<json> polem_labels;

    for(auto label : j_labels) {
        if(label.at("serviceName") == "NER") {
            auto query = create_query(label, j_labels);
            auto query_result = query_lemmatizer(query);
            auto label_to_add = create_label(label, query_result);
            polem_labels.push_back(label_to_add);
        }
    }

    j_labels.insert(
        j_labels.end(),
        std::make_move_iterator(polem_labels.begin()),
        std::make_move_iterator(polem_labels.end()));

    return j_labels;
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
    /*
     * THIS IS A PROPER CALL, YET RETURNS WRONG RESULT, WTF???
     */

    auto result = cascade_lemmatizer.lemmatizeS(
            query.get_orths(),
            query.get_lemmas(),
            query.get_tags(),
            false);
    /*
    std::cout << " Orths: " << query.get_orths() << "\n";
    std::cout << "Lemmas: " << query.get_lemmas() << "\n";
    std::cout << "  Tags: " << query.get_tags() << "\n";
    std::cout << "Result: " << result << "\n\n";
    */
    return result;
}

json Polem_enrich::create_label(json j_label, std::string value) {
    std::string s_value = "{\"value\": \"" + value + "\"}";
    json j_value = json::parse(s_value);
    json j_polem = R"(
            {
                "fieldName": "polem",
                "name": "polem",
                "serviceName": "polem"
            }
        )"_json;

    j_label.update(j_value);
    j_label.update(j_polem);

    return j_label;
}
