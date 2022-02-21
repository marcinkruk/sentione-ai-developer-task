#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <polem-dev/CascadeLemmatizer.h>

#include "polem_enrich.h"

Polem_enrich::Polem_enrich()
    : cascade_lemmatizer{CascadeLemmatizer::assembleLemmatizer()}
{}

std::string Polem_enrich::enrich(std::string input) {

    auto j_object = input_to_json(input);
    if(j_object.empty())
        return "";

    auto j_docs = j_object.at("docs");
    for(auto doc : j_docs) {
        std::cout << "\n\n=== DOC\n";  
        auto j_labels = doc.at("labels");
        for(auto label : j_labels) {
            std::cout << "===== LABEL\n";
            if(label.at("serviceName") == "NER") {
                std::cout << "======= IS NER\n";
                std::cout << std::setw(4) << label << "\n";
            }
        }
    }

    return to_string(j_object);
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
