#ifndef POLEM_ENRICH_H
#define POLEM_ENRICH_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Polem_enrich {
    public:
        std::string enrich(std::string);
};

#endif
