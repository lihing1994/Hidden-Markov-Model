//
//  HiddenMarkovModel.cpp
//  HiddenMarkovModel
//
//  Created by HINGLI on 10/30/20.
//

#include "HiddenMarkovModel.h"
HiddenMarkovModel::HiddenMarkovModel(std::vector<std::vector<float>>* aMatrix,std::vector<std::vector<float>> * bMatrix,std::vector<float> * piMatrix,int maxIters)
{
    this->_a_matrix = aMatrix;
    this->_b_matrix = bMatrix;
    this->_pi_matrix = piMatrix;
    this->_number_of_state = (int)this->_a_matrix->size();
    this->_is_alpha_pass_completed = false;
    this->_is_beta_pass_completed =false;
    this->_is_gammas_pass_completed = false;
    this->_number_of_observation = (int)this->_b_matrix->at(0).size();
    this->_max_iters = maxIters;
    // Inital the prob to negative infinity
    this->_old_log_prob = INT64_MIN;
    this->_number_of_iters = 1;
    
}
void HiddenMarkovModel::alpha_pass(std::vector<int> * observation_sequence)
{
    // i from 0 -> number of the state
    // t from 0 -> size of (observation_matrix).
    // Initialize the alpha matrix.
    if(!this->_is_alpha_pass_completed) {
        this->_initMatrix(&this->_alpha_matrix, (int)observation_sequence->size(), this->_number_of_state);
        this->_initMatrix(&this->_c_matrix, (int)observation_sequence->size());
    }
    this->_observation_sequence = observation_sequence;
    // Initialize the scaling matrix c.
    float temp_first = observation_sequence->at(0);
    this->_c_matrix.at(0) = 0;
    for(int i=0;i<this->_number_of_state;i++)
    {
        this->_alpha_matrix.at(0).at(i) = this->_pi_matrix->at(i) * this->_b_matrix->at(i).at(temp_first);
        this->_c_matrix.at(0) += this->_alpha_matrix.at(0).at(i);
    }
    //Scaling the alpha(0,i)
    this->_c_matrix.at(0) = 1/this->_c_matrix.at(0);
    for(int i =0;i<this->_number_of_state;i++) this->_alpha_matrix.at(0).at(i) *= _c_matrix.at(0);
    // compute alpha(t,i)
    for(int t = 1;t<observation_sequence->size();t++)
    {
        this->_c_matrix.at(t) = 0;
        for(int i=0;i<this->_number_of_state;i++)
        {
            float temp = 0.0f;
            for(int j =0;j<this->_number_of_state;j++)
            {
                temp += this->_alpha_matrix.at(t-1).at(j) * this->_a_matrix->at(j).at(i);
            }
            this->_alpha_matrix.at(t).at(i) = temp * this->_b_matrix->at(i).at(observation_sequence->at(t));
            this->_c_matrix.at(t) += this->_alpha_matrix.at(t).at(i);
        }
        //Scaling alpha(t,i)
        this->_c_matrix.at(t) = 1/this->_c_matrix.at(t);
        for(int i =0;i<this->_number_of_state;i++) this->_alpha_matrix.at(t).at(i) *= this->_c_matrix.at(t);
    }
    _is_alpha_pass_completed = true;
}
void HiddenMarkovModel::beta_pass(std::vector<int> * observation_sequence)
{
    if(!this->_is_alpha_pass_completed)
    {
        std::cout<<"alpha pass need to be completed first"<<std::endl;
        return;
    }
    // Initialize beta matrix.
    if(!this->_is_beta_pass_completed) this->_initMatrix(&_beta_matrix, (int)observation_sequence->size(), this->_number_of_state);
    this->_observation_sequence = observation_sequence;
    for(int i=0;i<this->_number_of_state;i++)
    {
        this->_beta_matrix.at((int)observation_sequence->size()-1).at(i) = this->_c_matrix.at((int)observation_sequence->size()-1);
    }
    for(int t=(int)observation_sequence->size()-2;t>=0;t--)
    {
        for(int i=0;i<_number_of_state;i++)
        {
            this->_beta_matrix.at(t).at(i) = 0;
            for(int j=0;j<_number_of_state;j++)
            {
                this->_beta_matrix.at(t).at(i) += this->_a_matrix->at(i).at(j) * this->_b_matrix->at(j).at(observation_sequence->at(t+1))
                        *this->_beta_matrix.at(t+1).at(j);
            }
            this->_beta_matrix.at(t).at(i) *= this->_c_matrix.at(t);
        }
    }
    this->_is_beta_pass_completed = true;
    
}
void HiddenMarkovModel::gammas_pass(std::vector<int>* observation_matrix)
{
    if(!this->_is_gammas_pass_completed)
    {
        this->_initMatrix(&this->_gammas_matrix, (int)observation_matrix->size(), _number_of_state);
        this->_initMatrix(&this->_di_gammas_matrix, (int)observation_matrix->size(), _number_of_state, _number_of_state);
    }
    for(int t=0;t<(int)observation_matrix->size()-1;t++)
    {
        float denom = 0;
        for(int i =0;i<_number_of_state;i++)
        {
            for(int j = 0;j<_number_of_state;j++)
            {
                denom += this->_alpha_matrix.at(t).at(i)*this->_a_matrix->at(i).at(j)*
                        this->_b_matrix->at(j).at(observation_matrix->at(t+1))*this->_beta_matrix.at(t+1).at(j);
            }
        }
        for(int i =0;i<_number_of_state;i++)
        {
            this->_gammas_matrix.at(t).at(i) = 0;
            for(int j=0;j<_number_of_state;j++)
            {
                this->_di_gammas_matrix.at(t).at(i).at(j) = this->_alpha_matrix.at(t).at(i) * this->_a_matrix->at(i).at(j)*
                                                            this->_b_matrix->at(j).at(observation_matrix->at(t+1)) *this->_beta_matrix.at(t+1).at(j);
                this->_di_gammas_matrix.at(t).at(i).at(j) /= denom;
                this->_gammas_matrix.at(t).at(i) += this->_di_gammas_matrix.at(t).at(i).at(j);
            }
        }
    }
    float denom =0;
    for(int i=0;i<_number_of_state;i++)
    {
        denom += this->_alpha_matrix.at((int)observation_matrix->size()-1).at(i);
    }
    for(int i=0;i<_number_of_state;i++)
    {
        this->_beta_matrix.at((int)observation_matrix->size()-1).at(i) = this->_alpha_matrix.at((int)observation_matrix->size()-1).at(i)/denom;
    }
    this->_is_gammas_pass_completed = true;
}

void HiddenMarkovModel::_initMatrix(std::vector<std::vector<std::vector<float>>> * ptr,int x,int y,int z)
{
    for(int i =0;i<x;i++)
    {
        std::vector<std::vector<float>> temp_two_D;
        for(int j =0;j<y;j++)
        {
            std::vector<float> temp_one_D;
            for(int k =0;k<z;k++)
            {
                temp_one_D.push_back(0.000001f);
            }
            temp_two_D.push_back(temp_one_D);
        }
        ptr->push_back(temp_two_D);
    }
}
void HiddenMarkovModel::_initMatrix(std::vector<std::vector<float> > *ptr, int row, int column)
{
    for(int i =0;i<row;i++)
    {
        std::vector<float> temp;
        for(int j =0;j<column;j++)
        {
            temp.push_back(0.000001f);
        }
        ptr->push_back(temp);
    }
}
void HiddenMarkovModel::_initMatrix(std::vector<float> * ptr,int column)
{
    for(int i=0;i<column;i++) ptr->push_back(0.000001f);
}
void HiddenMarkovModel::set_alpha_matrix(std::vector<std::vector<float>> alpha)
{
    this->_alpha_matrix = alpha;
}
void HiddenMarkovModel::set_beta_matrix(std::vector<std::vector<float>> beta)
{
    this->_beta_matrix = beta;
}
void HiddenMarkovModel::print_alpha_matrix()
{
    for(int i =0;i<this->_alpha_matrix.size();i++)
    {
        std::cout<<"| "<<this->_observation_sequence->at(i)<<" | ";
        for(int j =0;j<this->_alpha_matrix.at(i).size();j++)
        {
            std::cout<<this->_alpha_matrix.at(i).at(j)<<"   ";
        }
        std::cout<<std::endl;
    }
}
void HiddenMarkovModel::print_beta_matrix()
{
    for(int i =0;i<this->_beta_matrix.size();i++)
    {
        std::cout<<"| "<<this->_observation_sequence->at(i)<<" | ";
        for(int j =0;j<this->_beta_matrix.at(i).size();j++)
        {
            std::cout<<this->_beta_matrix.at(i).at(j)<<"   ";
        }
        std::cout<<std::endl;
    }
}
std::vector<std::vector <float>>* HiddenMarkovModel::get_alpha_matrix()
{
    return &this->_alpha_matrix;
}
std::vector<std::vector <float>>* HiddenMarkovModel::get_beta_matrix()
{
    return &this->_beta_matrix;
}
void HiddenMarkovModel::print_gammas_matrix()
{
    for(int i=0;i<this->_gammas_matrix.size();i++)
    {
        std::cout<<"| "<<this->_observation_sequence->at(i)<<" | ";
        for(int j=0;j<this->_gammas_matrix.at(i).size();j++)
        {
            std::cout<<this->_gammas_matrix.at(i).at(j)<<"   ";
        }
        std::cout<<std::endl;
    }
}
void HiddenMarkovModel::re_estimation(std::vector<int>* observation_matrix)
{
    // Re-restimate pi matrix
    for(int i=0;i<this->_number_of_state;i++)
    {
        this->_pi_matrix->at(i) = this->_gammas_matrix.at(0).at(i);
    }
    // Re-restimate A matrix
    for(int i=0;i<this->_number_of_state;i++)
    {
        for(int j=0;j<this->_number_of_state;j++)
        {
            double numer = 0;
            double denom = 0;
            for(int t =0;t<observation_matrix->size()-1;t++)
            {
                numer += this->_di_gammas_matrix.at(t).at(i).at(j);
                denom += this->_gammas_matrix.at(t).at(i);
            }
            this->_a_matrix->at(i).at(j) = numer/denom;
        }
    }
    for(int i = 0;i<this->_number_of_state;i++)
    {
        for(int j=0;j<this->_number_of_observation;j++)
        {
            float numer = 0;
            float denom = 0;
            for(int t =0;t<observation_matrix->size()-1;t++)
            {
                if(this->_observation_sequence->at(t) == j) numer += this->_gammas_matrix.at(t).at(i);
                else denom += this->_gammas_matrix.at(t).at(i);
            }
            this->_b_matrix->at(i).at(j) = numer/denom;
        }
    }
}
bool HiddenMarkovModel::earily_stopping(std::vector<int> *observation_sequence)
{
    double temp_pro = 0;
    for(int i =0;i<this->_c_matrix.size();i++)
    {
        temp_pro += log10(this->_c_matrix.at(i));
    }
    temp_pro *= -1;
    if(++this->_number_of_iters < this->_max_iters && temp_pro > this->_old_log_prob)
    {
        this->_old_log_prob = temp_pro;
        return false;
    }
    else
    {
        return true;
    }
}
void HiddenMarkovModel::training(std::vector<int> *observation_sequence)
{
    this->alpha_pass(observation_sequence);
    this->beta_pass(observation_sequence);
    this->gammas_pass(observation_sequence);
    this->re_estimation(observation_sequence);
    while(!this->earily_stopping(observation_sequence))
    {
        this->alpha_pass(observation_sequence);
        this->beta_pass(observation_sequence);
        this->gammas_pass(observation_sequence);
        this->re_estimation(observation_sequence);
    }
    std::cout<<"Log value after "<<this->_number_of_iters<<" iterations executed equals to :"<<this->_old_log_prob<<std::endl;
}
double HiddenMarkovModel::cal_log_value_for_training()
{
    double temp_pro = 0;
    for(int i =0;i<this->_c_matrix.size();i++)
    {
        temp_pro += log(this->_c_matrix.at(i));
    }
    return temp_pro *= -1;
}
double HiddenMarkovModel::get_current_prob()
{
    return this->_old_log_prob;
}
