#ifndef POLEM_ENRICH_H
#define POLEM_ENRICH_H

#include <string>
#include <nlohmann/json.hpp>
#include <polem-dev/CascadeLemmatizer.h>

using json = nlohmann::json;

class Polem_enrich {
    public:
        Polem_enrich();
        std::string enrich(std::string);
    private:
        json input_to_json(std::string);
        json empty_json();
        const CascadeLemmatizer cascade_lemmatizer;
};

#endif
