#include "PetriNet.h"
#include "tinyxml2.h"
#include <iostream>

using namespace tinyxml2;

// --- Helper Function to traverse nested pages ---
void parseElements(XMLElement* element, 
                   std::vector<std::string>& p_ids, 
                   std::vector<std::string>& t_ids,
                   std::vector<int>& init_marking,
                   std::unordered_map<std::string, int>& p_map,
                   std::unordered_map<std::string, int>& t_map,
                   std::vector<Arc>& raw_arcs) // We store arcs temp here
{
    XMLElement* child = element->FirstChildElement();
    while (child) {
        std::string tag = child->Name();
        const char* idAttr = child->Attribute("id");
        std::string id = idAttr ? idAttr : "";

        if (tag == "page") {
            // RECURSION: Dive into the page!
            parseElements(child, p_ids, t_ids, init_marking, p_map, t_map, raw_arcs);
        }
        else if (tag == "place" && !id.empty()) {
            p_map[id] = p_ids.size();
            p_ids.push_back(id);

            int tokens = 0;
            XMLElement* initMark = child->FirstChildElement("initialMarking");
            if (initMark) {
                XMLElement* text = initMark->FirstChildElement("text");
                if (text && text->GetText()) {
                    tokens = std::stoi(text->GetText());
                }
            }
            init_marking.push_back(tokens);
        }
        else if (tag == "transition" && !id.empty()) {
            t_map[id] = t_ids.size();
            t_ids.push_back(id);
        }
        else if (tag == "arc") {
            const char* src = child->Attribute("source");
            const char* tgt = child->Attribute("target");
            if (src && tgt) {
                Arc a;
                a.source_id = src;
                a.target_id = tgt;
                a.id = id;
                raw_arcs.push_back(a);
            }
        }
        
        child = child->NextSiblingElement();
    }
}

bool PetriNet::parsePNML(const std::string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Could not load file " << filename << std::endl;
        return false;
    }

    XMLElement* root = doc.FirstChildElement("pnml");
    if (!root) return false;
    XMLElement* net = root->FirstChildElement("net");
    if (!net) return false;

    // Temporary storage for arcs so we can process them AFTER all nodes are found
    std::vector<Arc> temp_arcs;

    // --- PASS 1: Recursive Parse ---
    parseElements(net, place_ids, transition_ids, initial_marking, place_map, transition_map, temp_arcs);

    // Resize matrices
    pre_matrix.assign(transition_ids.size(), std::vector<int>());
    post_matrix.assign(transition_ids.size(), std::vector<int>());

    // --- PASS 2: Process Arcs ---
    for (const auto& arc : temp_arcs) {
        if (place_map.count(arc.source_id) && transition_map.count(arc.target_id)) {
            // Place -> Transition
            int p_idx = place_map[arc.source_id];
            int t_idx = transition_map[arc.target_id];
            pre_matrix[t_idx].push_back(p_idx);
        }
        else if (transition_map.count(arc.source_id) && place_map.count(arc.target_id)) {
            // Transition -> Place
            int t_idx = transition_map[arc.source_id];
            int p_idx = place_map[arc.target_id];
            post_matrix[t_idx].push_back(p_idx);
        }
    }

    return verifyConsistency();
}

// ... Keep your verifyConsistency and getters the same ...
bool PetriNet::verifyConsistency() {
    if (place_ids.empty() && transition_ids.empty()) {
        std::cerr << "Error: No nodes found. (Did you handle <page> tags?)" << std::endl;
        return false;
    }
    std::cout << "Consistency Check Passed: " << place_ids.size() << " Places, " 
              << transition_ids.size() << " Transitions loaded." << std::endl;
    return true;
}

int PetriNet::getPlaceIndex(const std::string& id) const {
    if (place_map.find(id) != place_map.end()) return place_map.at(id);
    return -1;
}

int PetriNet::getTransitionIndex(const std::string& id) const {
    if (transition_map.find(id) != transition_map.end()) return transition_map.at(id);
    return -1;
}