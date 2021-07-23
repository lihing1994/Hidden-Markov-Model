//
//  Preprocessing.cpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 11/19/20.
//

#include "Preprocessing.h"
Preprocessing::Preprocessing(std::vector<int>sample_size,std::vector<std::string> family_name,int unique_optcode_threshold,int number_of_states)
{
    this->_each_family_sample_size = sample_size;
    this->_each_family_name = family_name;
    traning_sample_range.first = -99;
    traning_sample_range.second = -99;
    testing_sample_range.first = -99;
    testing_sample_range.second = -99;
    this->_unique_optcode_threshold = unique_optcode_threshold;
    this->_number_of_states = number_of_states;
}
void Preprocessing::pre_processing_certain_family()
{
    for(int i =0;i<this->_each_family_name.size();i++)
    {
        int family_size = this->_each_family_sample_size.at(i);
        std::string family_name = this->_each_family_name.at(i);
        std::vector<std::pair<std::string,int>> sort_map_zbot = this->read_certain_sample(family_name, family_size);
        this->output_traning_sample_replace_by_assign_char(family_name,sort_map_zbot,family_size,this->_unique_optcode_threshold);
        std::pair<int,int> return_range = this->random_select_traning_sample(i,1000);
        this->traning_sample_range.first = return_range.first;
        this->traning_sample_range.second = traning_sample_range.first + (return_range.second - return_range.first) * 0.9;
        this->testing_sample_range.first = traning_sample_range.second + 1;
        this->testing_sample_range.second = return_range.second;
        this->testing_range.push_back(testing_sample_range);
        this->output_obsevation_sequence(i, traning_sample_range);
    }
}
std::vector<std::pair<std::string,int>> Preprocessing::map_sort(std::map<std::string, int> m)
{
    std::map<std::string,int> return_map;
    std::vector<std::pair<std::string,int>> temp;
    for (auto & it : m) {
        temp.push_back(it);
    }
    sort(temp.begin(), temp.end(), [](std::pair<std::string,int> a, std::pair<std::string,int> b){
        return a.second > b.second;
    });
    return temp;
}
std::vector<std::pair<std::string,int>> Preprocessing::read_certain_sample(std::string family_name,int given_family_sample_size)
{
    std::map<std::string,int> map_zbot;
    for(int i=0;i<given_family_sample_size;i++)
    {
        std::fstream my_temp_file;
        my_temp_file.open(family_name+"/"+family_name+std::to_string(i+1)+".txt",std::ios::in);
        if(my_temp_file.is_open())
        {
            std::string temp_line;
            while(!my_temp_file.eof())
            {
                getline(my_temp_file,temp_line,'\n');
                //Skip the last empty line from the text file.
                if(temp_line.compare("")==0) continue;
                temp_line = temp_line.substr(0,temp_line.length()-1);
                // Add a assembly instruction to the map
                if(map_zbot.count(temp_line) == 0)
                {
                    map_zbot.insert(std::pair<std::string,int>(temp_line,1));
                }
                // If the assembly instruction is alreay in the map do following
                else
                {
                    map_zbot[temp_line] += 1;
                }
            }
        }
        my_temp_file.close();
    }
    // Sort the map
    return map_sort(map_zbot);
}
void Preprocessing::output_traning_sample_replace_by_assign_char(std::string family_name,std::vector<std::pair<std::string,int>> sort_map,int sample_size,int first_n)
{
    for(int i=0;i<sample_size;i++)
    {
        std::fstream my_file_read;
        std::fstream my_file_write;
        std::string temp_str;
        my_file_read.open(family_name+"/"+family_name+std::to_string(i+1)+".txt",std::ios::in);
        my_file_write.open(family_name+"_reduce"+"/"+family_name+"_reduce"+std::to_string(i+1)+".txt",std::ios::out|std::ios::trunc);
        if( my_file_read.is_open() && my_file_write.is_open())
        {
            while(!my_file_read.eof())
            {
                getline(my_file_read, temp_str, '\n');
                if(temp_str.compare("")==0) continue;
                temp_str = temp_str.substr(0,temp_str.length()-1);
                int to_write = associate_first_n_opcode(sort_map, temp_str, this->_unique_optcode_threshold);
                my_file_write<<to_write<<"\n";
            }
            my_file_write.close();
            my_file_read.close();
        }
        else
        {
            std::cout<<"Can not read the database file or can not creat database file"<<std::endl;
            std::cout<<family_name+"/"+family_name+std::to_string(i+1)+".txt"<<std::endl;
        }
    }
    std::cout<<"The process to ouput traning sample with reduce unqiue optcode for <"<<family_name<<"> family has completed"<<std::endl;
}
int Preprocessing::associate_first_n_opcode(std::vector<std::pair<std::string,int>> sort_map,std::string opcode,int first_n)
{
    if(first_n > sort_map.size()) return -9999;
    for(int i=0;i<first_n-1;i++)
    {
        if(sort_map.at(i).first==opcode)
        {
            return i;
        }
    }
    return first_n-1;
}
void Preprocessing::print_sort_map(std::vector<std::pair<std::string,int>> temp)
{
    for(int i=0;i<temp.size();i++)
    {
        std::cout<<temp.at(i).first<<": "<<temp.at(i).second<<std::endl;
    }
}
std::pair<int,int> Preprocessing::random_select_traning_sample(int family_index,int random_selected_size)
{
    if(random_selected_size > this->_each_family_sample_size.at(family_index))
    {
        std::cout<<"Error! The total number of traning sample size the number of sample you want to selecte"<<std::endl;
        std::pair<int,int> range;
        range.first = -99;
        range.second = -99;
        return range;
    }
    else
    {
        srand(time_t(NULL));
        int total_size = this->_each_family_sample_size.at(family_index);
        int off_set = std::rand() % (total_size + 1 - random_selected_size)+1;
        std::pair<int, int> range;
        range.first = off_set;
        range.second = random_selected_size + off_set;
        return range;
    }
}
std::pair<int, int> Preprocessing::get_traning_sample_range()
{
    return this->traning_sample_range;
}
std::pair<int,int> Preprocessing::get_testing_sample_range()
{
    return this->testing_sample_range;
}
void Preprocessing::output_obsevation_sequence(int family_index, std::pair<int, int> range)
{
    std::string family_name = this->_each_family_name.at(family_index);
    std::fstream output_file;
    std::fstream read_file;
    std::string temp_str;
    output_file.open(family_name+"_observation_sequence.txt",std::ios::out| std::ios::trunc);
    for(int i=range.first;i<=range.second;i++)
    {
        read_file.open(family_name+"_reduce"+"/"+family_name+"_reduce"+std::to_string(i)+".txt",std::ios::in);
        if(output_file.is_open()&&read_file.is_open())
        {
            while(!read_file.eof())
            {
                getline(read_file, temp_str, '\n');
                if(temp_str.compare("")==0) continue;
                output_file<<temp_str<<" ";
            }
            read_file.close();
        }
    }
    output_file.close();
    std::cout<<"The process to ouput the observation sequence for <"<<family_name<<"> family has completed."<<std::endl;
}
int Preprocessing::get_unique_optocode_threshold()
{
    return this->_unique_optcode_threshold;
}
std::vector<std::string> Preprocessing::get_observation_sequence_path()
{
    std::vector<std::string> observation_path_name;
    for(int i=0;i<this->_each_family_name.size();i++)
    {
        observation_path_name.push_back(_each_family_name.at(i)+"_observation_sequence.txt");
    }
    return observation_path_name;
}
std::vector<std::string> Preprocessing::generate_a_matrix(int hyperparameter_number_of_states)
{
    std::vector<std::string> to_return;
    return  to_return;
}
void Preprocessing::output_pi_matrix()
{
    std::fstream out_file;
    out_file.open("Pi_matrix.txt",std::ios::out|std::ios::trunc);
    float value_for_each_states = 0;
    float off_set = 0;
    if( this->_number_of_states % 2==0)
    {
        value_for_each_states = 1 / (float)_number_of_states;
        off_set = value_for_each_states / (float)_number_of_states;
        for(int i=0;i<this->_number_of_states;i++)
        {
            if(i % 2==0) out_file<<value_for_each_states + off_set <<" ";
            else out_file<<value_for_each_states - off_set<<" ";
        }
    }
    out_file.close();
}
void Preprocessing::output_a_matrix()
{
    std::fstream out_file;
    out_file.open("A_matrix.txt",std::ios::out|std::ios::trunc);
    float value_for_each_states = 0;
    float off_set = 0;
    for(int i=0;i<this->_number_of_states;i++)
    {
        if(this->_number_of_states % 2==0)
        {
            value_for_each_states = 1 / (float)_number_of_states;
            off_set = value_for_each_states / (float)_number_of_states;
            for(int j=0;j<this->_number_of_states;j++)
            {
                if(j % 2==0) out_file<<value_for_each_states + off_set <<" ";
                else out_file<<value_for_each_states - off_set<<" ";
            }
        }
        out_file<<"\n";
    }
}
void Preprocessing::output_b_matrix()
{
    std::fstream out_file;
    out_file.open("B_matrix.txt",std::ios::out|std::ios::trunc);
    float value_for_each_states = 0;
    float off_set = 0;
    float total =0;
    for(int i=0;i<this->_number_of_states;i++)
    {
        if(this->_unique_optcode_threshold % 2==0)
        {
            value_for_each_states = 1 / (float)_unique_optcode_threshold;
            off_set = value_for_each_states / (float)_unique_optcode_threshold;
            for(int j=0;j<this->_unique_optcode_threshold;j++)
            {
                if(j % 2==0){
                    out_file<<value_for_each_states + off_set <<" ";
                    total += value_for_each_states + off_set;
                }
                else {
                    out_file<<value_for_each_states - off_set<<" ";
                    total += value_for_each_states - off_set;
                }
            }
            out_file<<"\n";
        }
    }
}
std::vector<std::pair<int, int>> Preprocessing::get_testing_range()
{
    return this->testing_range;
}
