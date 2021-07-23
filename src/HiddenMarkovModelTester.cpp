//
//  HiddenMarkovModelTester.cpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 11/1/20.
//

#include "HiddenMarkovModelTester.h"
HiddenMarkovModelTester::HiddenMarkovModelTester(std::string a_matrix_path, std::string b_matrix_path, std::string pi_matrix_path, std::string observation_sequence_path)
{
    this->_a_matrix_path = a_matrix_path;
    this->_b_matrix_path = b_matrix_path;
    this->_pi_matrix_path = pi_matrix_path;
    this->_observation_sequence_path = observation_sequence_path;
    this->_a_matrix = this->_get_float_matrix_from_txt_file(a_matrix_path);
    this->_b_matrix = this->_get_float_matrix_from_txt_file(b_matrix_path);
    this->_pi_matrix = this->_get_float_one_by_one_matrix_from_txt_file(pi_matrix_path);
    this->_observation_sequence = this->_get_int_one_by_one_matrix_from_txt_file(observation_sequence_path);
    this->_hmm = new HiddenMarkovModel(&this->_a_matrix,&this->_b_matrix,&this->_pi_matrix);
}
HiddenMarkovModelTester:: ~HiddenMarkovModelTester()
{
    delete this->_hmm;
}

std::vector<std::vector<float>> HiddenMarkovModelTester::_get_float_matrix_from_txt_file(std::string path)
{
    std::vector<std::vector<float>> to_return;
    std::ifstream temp_input_matrix(path,std::ios::in);
    if(!temp_input_matrix.is_open())
    {
        std::cout<<"Unable to open file: "<<path<<std::endl;
        temp_input_matrix.close();
        return to_return;
    }
    else
    {
        int row_count = 0;
        std::string line;
        while(std::getline(temp_input_matrix,line))
        {
            std::vector<float> temp_row;
            to_return.push_back(temp_row);
            std::istringstream iss(line);
            std::string temp_split_str;
            while(iss>>temp_split_str)
            {
                to_return.at(row_count).push_back(std::stof(temp_split_str));
            }
            row_count++;
        }
        temp_input_matrix.close();
        return to_return;
    }
}
std::vector<float> HiddenMarkovModelTester::_get_float_one_by_one_matrix_from_txt_file(std::string path)
{
    std::vector<float> to_return;
    std::ifstream temp_input_matrix(path,std::ios::in);
    if(!temp_input_matrix.is_open())
    {
        std::cout<<"Unable to open file: "<<path<<std::endl;
        temp_input_matrix.close();
        return to_return;
    }
    else
    {
        std::string line;
        std::getline(temp_input_matrix,line);
        std::istringstream iss(line);
        std::string temp_split_str;
        while(iss>>temp_split_str)
        {
            to_return.push_back(std::stof(temp_split_str));
        }
        temp_input_matrix.close();
        return to_return;
    }
}
std::vector<int> HiddenMarkovModelTester::_get_int_one_by_one_matrix_from_txt_file(std::string path)
{
    std::vector<int> to_return;
    std::ifstream temp_input_matrix(path,std::ios::in);
    if(!temp_input_matrix.is_open())
    {
        std::cout<<"No File Open"<<std::endl;
        temp_input_matrix.close();
        return to_return;
    }
    else
    {
        std::string line;
        std::getline(temp_input_matrix,line);
        std::istringstream iss(line);
        std::string temp_split_str;
        while(iss>>temp_split_str)
        {
            to_return.push_back(std::stoi(temp_split_str));
        }
        temp_input_matrix.close();
        return to_return;
    }
}
void HiddenMarkovModelTester::print_matrix(std::vector<std::vector<float>> temp_matrix)
{
    for(int i=0;i<temp_matrix.size();i++)
    {
        for(int j = 0;j<temp_matrix.at(i).size();j++)
        {
            std::cout<<temp_matrix.at(i).at(j)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"########################"<<std::endl;
}
void HiddenMarkovModelTester::print_matrix(std::vector<float> temp_matrix)
{
    for(int i=0;i<temp_matrix.size();i++)
    {
        std::cout<<temp_matrix.at(i)<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"########################"<<std::endl;
}
void HiddenMarkovModelTester::print_matrix(std::vector<int> temp_matrix)
{
    for(int i=0;i<temp_matrix.size();i++)
    {
        std::cout<<temp_matrix.at(i)<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"########################"<<std::endl;
}
void HiddenMarkovModelTester::alpha_pass_tester()
{
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!Alpha Pass!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    this->_hmm->alpha_pass(&this->_observation_sequence);
    this->_hmm->print_alpha_matrix();
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
}
void HiddenMarkovModelTester::beta_pass_tester()
{
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!Beta Pass!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    this->_hmm->beta_pass(&this->_observation_sequence);
    //this->_hmm->print_beta_matrix();
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
}
void HiddenMarkovModelTester::gammas_pass_tester()
{
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!Gammas Pass!!!!!!!!!!!!!!!!!!"<<std::endl;
    this->_hmm->gammas_pass(&this->_observation_sequence);
    //this->_hmm->print_gammas_matrix();
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
}
void HiddenMarkovModelTester::output_alpha_matrix()
{
    char * output_path_ptr;
    char path[PATH_MAX + 1];
    output_path_ptr = getcwd( path, PATH_MAX + 1 );
    std::vector<std::vector<float>> * temp_alpha = this->_hmm->get_alpha_matrix();
    std::fstream out_file("alpha_matrix_output.txt",std::ios::out);
    if(out_file.is_open())
    {
        for(int i = 0;i<temp_alpha->size();i++)
        {
            out_file<<"| "<<this->_observation_sequence.at(i)<<" | ";
            for(int j =0;j<temp_alpha->at(i).size();j++)
            {
                out_file<<temp_alpha->at(i).at(j)<<"   ";
            }
            out_file<<"\n";
        }
    }
    out_file.close();
    if(output_path_ptr!=nullptr)
    {
        printf("########################################################################################################\n");
        printf("The output alpha matrix file has been saved at %s/alpha_matrix_output.txt.\n",output_path_ptr);
        printf("########################################################################################################\n\n");
    }
}
void HiddenMarkovModelTester::output_beta_matrix()
{
    char * output_path_ptr;
    char path[PATH_MAX + 1];
    output_path_ptr = getcwd( path, PATH_MAX + 1 );
    std::vector<std::vector<float>> * temp_beta = this->_hmm->get_beta_matrix();
    std::fstream out_file("beta_matrix_output.txt",std::ios::out);
    if(out_file.is_open())
    {
        for(int i = 0;i<temp_beta->size();i++)
        {
            out_file<<"| "<<this->_observation_sequence.at(i)<<" | ";
            for(int j =0;j<temp_beta->at(i).size();j++)
            {
                out_file<<temp_beta->at(i).at(j)<<"   ";
            }
            out_file<<"\n";
        }
    }
    out_file.close();
    if(output_path_ptr!=nullptr)
    {
        printf("########################################################################################################\n");
        printf("The output beta matrix file has been saved at %s/beta_matrix_output.txt.\n",output_path_ptr);
        printf("########################################################################################################\n\n");
    }
    
}
void HiddenMarkovModelTester::get_a_matrix_size()
{
    for(int i=0;i<this->_a_matrix.size();i++)
    {
        std::cout<<(int)this->_a_matrix.at(i).size()<<std::endl;
    }
}
void HiddenMarkovModelTester::get_b_matrix_size()
{
    for(int i=0;i<this->_b_matrix.size();i++)
    {
        std::cout<<(int)this->_b_matrix.at(i).size()<<std::endl;
    }
}
void HiddenMarkovModelTester::get_pi_matri_size()
{
    std::cout<<this->_pi_matrix.size()<<std::endl;
}
void HiddenMarkovModelTester::re_estimation()
{
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!Re-estimation!!!!!!!!!!!!!!!!!"<<std::endl;
    this->_hmm->re_estimation(&this->_observation_sequence);
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
}
void HiddenMarkovModelTester::traning()
{
    this->_hmm->training(&this->_observation_sequence);
    this->output_pi_matrix();
    this->output_a_matrix();
    this->output_b_matrix();
}
void HiddenMarkovModelTester::output_a_matrix()
{
    char * output_path_ptr;
    char path[PATH_MAX + 1];
    output_path_ptr = getcwd( path, PATH_MAX + 1 );
    std::fstream out_file(this->_a_matrix_path,std::ios::out|std::ios::trunc);
    if(out_file.is_open())
    {
        for(int i = 0;i<_a_matrix.size();i++)
        {
            for(int j =0;j<_a_matrix.at(i).size();j++)
            {
                out_file<<_a_matrix.at(i).at(j)<<" ";
            }
            out_file<<"\n";
        }
    }
    out_file.close();
    if(output_path_ptr!=nullptr)
    {
        printf("########################################################################################################\n");
        printf("The output A-matrix file has been saved at %s/A_matrix.txt.\n",output_path_ptr);
        printf("########################################################################################################\n\n");
    }
}
void HiddenMarkovModelTester::output_b_matrix()
{
    char * output_path_ptr;
    char path[PATH_MAX + 1];
    output_path_ptr = getcwd( path, PATH_MAX + 1 );
    std::fstream out_file(this->_b_matrix_path,std::ios::out|std::ios::trunc);
    if(out_file.is_open())
    {
        for(int i = 0;i<_b_matrix.size();i++)
        {
            for(int j =0;j<_b_matrix.at(i).size();j++)
            {
                out_file<<_b_matrix.at(i).at(j)<<" ";
            }
            out_file<<"\n";
        }
    }
    out_file.close();
    if(output_path_ptr!=nullptr)
    {
        printf("########################################################################################################\n");
        printf("The output B-matrix file has been saved at %s/B_matrix.txt.\n",output_path_ptr);
        printf("########################################################################################################\n\n");
    }
}
void HiddenMarkovModelTester::output_pi_matrix()
{
    char * output_path_ptr;
    char path[PATH_MAX + 1];
    output_path_ptr = getcwd( path, PATH_MAX + 1 );
    std::fstream out_file(this->_pi_matrix_path,std::ios::out|std::ios::trunc);
    if(out_file.is_open())
    {
        for(int i =0;i<this->_pi_matrix.size();i++)
        {
            out_file<<_pi_matrix.at(i)<<" ";
        }
    }
    out_file.close();
    if(output_path_ptr!=nullptr)
    {
        printf("########################################################################################################\n");
        printf("The output Pi-matrix file has been saved at %s/Pi_matrix.txt.\n",output_path_ptr);
        printf("########################################################################################################\n\n");
    }
}
void HiddenMarkovModelTester::testing(std::string family_name,int index_begin, int index_end)
{
    std::vector<int> temp_tester;
    std::fstream read_file;
    for(int i=index_begin; i<index_end;i++)
    {
        read_file.open(family_name+"_reduce/"+family_name+"_reduce"+std::to_string(i)+".txt",std::ios::in);
        if(read_file.is_open())
        {
            std::string temp_str;
            while(!read_file.eof())
            {
                getline(read_file, temp_str, '\n');
                if(temp_str.compare("")==0) continue;
                temp_tester.push_back(stoi(temp_str));
            }
            HiddenMarkovModel * hmm_for_test = new HiddenMarkovModel(&this->_a_matrix, &this->_b_matrix, &this->_pi_matrix);
            hmm_for_test->alpha_pass(&temp_tester);
            std::cout<<"Probably for <"<<family_name<<i<<".txt> "<<hmm_for_test->cal_log_value_for_training()<<std::endl;
            delete hmm_for_test;
            temp_tester.clear();
            read_file.close();
        }
    }
}
