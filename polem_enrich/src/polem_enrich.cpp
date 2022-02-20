#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "polem_enrich.h"

std::string Polem_enrich::enrich(std::string input) {

    json j = json::parse(input);

    return to_string(j.at("docs"));
}
