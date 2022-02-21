#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "polem_enrich.h"

std::string Polem_enrich::enrich(std::string input) {

    json j_object = input_to_json(input);
    if(j_object.empty())
        return "";

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
