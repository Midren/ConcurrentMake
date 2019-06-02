//
// Created by YuraYeliseev on 2019-05-19.
//

#ifndef CONCURRENT_MAKE_DEPENDECIES_PARSER_H
#define CONCURRENT_MAKE_DEPENDECIES_PARSER_H

#include <iostream>
#include "boost/format.hpp"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

void print_graph(std::map<std::string, std::vector<std::string>> &graph);

void file_dependencies(const path &file_path, const std::string &dep_dir);

void list_project_dependencies(const std::string &project_path);

std::map<std::string, std::vector<std::string>> create_graph(path dep_path);

std::vector<std::string> parse_dep_file(const path &dep_file);

#endif //CONCURRENT_MAKE_DEPENDECIES_PARSER_H
