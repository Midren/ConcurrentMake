#include <map>
#include "dependecies_parser.h"

using namespace boost::filesystem;
void file_dependencies(const path& file_path, const std::string& dep_dir){
    current_path(dep_dir);
    std::vector<std::string> strs;
    boost::split(strs, file_path.filename().string(), boost::is_any_of("."));
    boost::format format_obj("g++ -MM %1% -MF %2%/%3%.mf");
    std::string command = (format_obj % file_path % dep_dir % strs[0]).str();
    system(command.c_str());
}

void list_project_dependencies(const std::string& project_path){
    create_directory("../dep");
    current_path("../dep");
    path dep_dir = current_path();
    std::cout << dep_dir.string() << std::endl;
    if(is_directory(project_path)){
        for(auto& entry: boost::make_iterator_range(recursive_directory_iterator(project_path), {})){
            if (entry.path().extension() == ".cpp") {
                file_dependencies(entry.path(), dep_dir.string());
            }
        }
        return;
    }
    throw std::invalid_argument("No such project path!");
}

void print_graph(std::map<std::string, std::vector<std::string>>& graph) {
    std::ofstream output_file;
    output_file.open("dep_graph.dot");
    output_file << "digraph {" << std::endl;
    for(auto const& [key, val]: graph){
        for (auto const& dep: val){
            output_file << key << "->" << dep << ";" << std::endl;
        }
    }
    output_file << "}";
    output_file.close();
}

std::map<std::string, std::vector<std::string>> create_graph(path dep_path){
    std::map<std::string, std::vector<std::string>> vertexes;
    for(auto& entry: boost::make_iterator_range(directory_iterator(dep_path), {})){
        std::vector<std::string> dep = parse_dep_file(entry.path());
        path p = entry.path();
        p.replace_extension(".cpp");
        vertexes[p.stem().string()] = dep;
    }
    return vertexes;
}

std::vector<std::string> parse_dep_file(const path& dep_file){
    std::vector<std::string> dependency;
    std::ifstream f(dep_file.string());
    std::string line;
    getline(f, line);
    while(getline(f, line)){
        path name(line);
        dependency.push_back(name.stem().string());
    }
    return dependency;
}