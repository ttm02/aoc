//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>
#include <memory>
#include <limits>

class Dir_entry{
public:
    Dir_entry(std::string name,std::shared_ptr<Dir_entry> parent,bool is_file);
    Dir_entry(std::string name,std::shared_ptr<Dir_entry> parent,bool is_file,size_t size);

    void add_entry(std::shared_ptr<Dir_entry> entry);
    std::shared_ptr<Dir_entry> get_subdir(std::string name);
    size_t get_size();
    std::shared_ptr<Dir_entry> get_parent();

    size_t get_sum_of_all_subdirs_with_max_size(size_t max_size);
    size_t get_minimum_dir_with_size_size(size_t max_size);


private:
    bool isFile;
    size_t size;
    std::string name;

    std::vector<std::shared_ptr<Dir_entry>> entries_fo_dir;
    std::shared_ptr<Dir_entry> parent;
};

std::shared_ptr<Dir_entry> Dir_entry::get_parent() {return this->parent;}

std::shared_ptr<Dir_entry> Dir_entry::get_subdir(std::string name) {
    auto entry = std::find_if(this->entries_fo_dir.begin(),this->entries_fo_dir.end(),
                              [name](std::shared_ptr<Dir_entry> entry){return entry->name==name;});
    assert(entry!=this->entries_fo_dir.end());
    return *entry;
}

size_t Dir_entry::get_minimum_dir_with_size_size(size_t max_size) {

   if (this->isFile)
       return std::numeric_limits<size_t>::max();
   else {
       auto my_size = this->get_size();
       if (my_size<max_size)
           my_size=std::numeric_limits<size_t>::max();
       return std::accumulate(this->entries_fo_dir.begin(),this->entries_fo_dir.end(),my_size,
                              [max_size](size_t accumulated,std::shared_ptr<Dir_entry> entry){
                                  return std::min(accumulated, entry->get_minimum_dir_with_size_size(max_size));
                              } );
   }
}

size_t Dir_entry::get_sum_of_all_subdirs_with_max_size(size_t max_size) {

    if (this->isFile)
        return 0;
    else {
        auto my_size = this->get_size();
        if (my_size>max_size)
            my_size=0;
        return std::accumulate(this->entries_fo_dir.begin(),this->entries_fo_dir.end(),my_size,
                               [max_size](size_t accumulated,std::shared_ptr<Dir_entry> entry){
                                   return accumulated + entry->get_sum_of_all_subdirs_with_max_size(max_size);
                               } );

    }
}

Dir_entry::Dir_entry(std::string name,std::shared_ptr<Dir_entry> parent,bool is_file) {
    assert(is_file==false);
    this->isFile=false;
    this->name=name;
    this->parent=parent;
    this->size=0;
}

Dir_entry::Dir_entry(std::string name,std::shared_ptr<Dir_entry> parent,bool is_file,size_t size) {
    assert(is_file==true);
    this->isFile=true;
    this->size=size;
    this->name=name;
    this->parent=parent;
}

void Dir_entry::add_entry(std::shared_ptr<Dir_entry> entry) {
    assert(this->isFile==false);
    this->entries_fo_dir.push_back(entry);
}

size_t Dir_entry::get_size() {
    if(isFile){
        return this->size;
    }
    else{
        return std::accumulate(this->entries_fo_dir.begin(),this->entries_fo_dir.end(),0,
                               [](size_t accumulated,std::shared_ptr<Dir_entry> entry){
            return accumulated + entry->get_size();
        } );
    }
}

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::shared_ptr<Dir_entry> root = std::make_shared<Dir_entry>("/",nullptr,false);
    std::shared_ptr<Dir_entry> current=root;

    for (auto line :input) {
        if (line[0]=='$'){
            // command
            std::cout << current->get_size() << "\n";
            if (line=="$ ls"){
                // nothing to do
            }
            else if (line=="$ cd ..")
                current = current->get_parent();
            else if (line=="$ cd /")
                current=root;
            else{
                auto name = line.substr(4,line.length());
                current = current->get_subdir(name);
            }

        }else
        {
            // output of ls
            if (line.rfind("dir", 0)!=std::string::npos){
                auto new_dir =std::make_shared<Dir_entry>(line.substr(3,line.length()),current,false);
                current->add_entry(new_dir);
            }else{
                auto delim_pos= line.find(" ");
                auto size = std::stol(line.substr(0,delim_pos));
                auto new_dir =std::make_shared<Dir_entry>(line.substr(delim_pos,line.length()),current,true,size);
                current->add_entry(new_dir);
            }
        }


    }

    std::cout << "Total used:" <<root->get_size() << "\n";
    std::cout << "To free :" << (root->get_size()+30000000)-70000000 <<"\n";
    std::cout << root->get_minimum_dir_with_size_size((root->get_size()+30000000)-70000000) << "\n";

    return 0;
}