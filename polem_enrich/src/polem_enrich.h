#ifndef POLEM_ENRICH_H
#define POLEM_ENRICH_H

#include <string>
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
        std::string query_lemmatizer(const Polem_query);

        CascadeLemmatizer cascade_lemmatizer;
};

#endif
