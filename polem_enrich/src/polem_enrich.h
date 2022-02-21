#ifndef POLEM_ENRICH_H
#define POLEM_ENRICH_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <polem-dev/CascadeLemmatizer.h>

#include "polem_query.h"

using json = nlohmann::json;

class Polem_enrich {
    public:
        Polem_enrich();
        std::string enrich(std::string);
    private:
        json input_to_json(std::string);
        json empty_json();
        json enrich_json(json);
        Polem_query create_query(json, std::vector<json>);
        json create_label(json, std::string);
        std::string query_lemmatizer(const Polem_query);
        std::vector<json> create_polem_labels(std::vector<json>);

        CascadeLemmatizer cascade_lemmatizer;
};

#endif
