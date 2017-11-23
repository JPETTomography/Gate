/**
 *  @copyright Copyright 2018 The J-PET Gate Authors. All rights reserved.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  @file Variable.hh
 */
#ifndef VARIABLE_HH
#define VARIABLE_HH
#include "TTree.h"
#include <string>
#include <memory>
#include <stdexcept>
#include<iostream>

/**@About: This class is representation of variable from GateGlobalActor and the basic concept used in GlobalActorReader.
 * Class take care of extracing data from branch and take care of extracting valid data.
 * @Author: Mateusz Bała
 * @Email: bala.mateusz@gmail.com
*/
template <class T>
class Variable
{
public:
    Variable() {upValue.reset();}
    ~Variable() {}

    /** This function return value of variable, but if variable was not initialized (what mean variable does not exist) this function throw exception.
    */
    T GetValue()
    {
        if(upValue.get() != nullptr)
            return *upValue.get();
        throw std::logic_error("Calling variable not initialized in tree.");
    }
    /** Use this method to attach object of this class to branch.
     * This function try attach to branch - if this is imposble (what means that branch does not exist) this function return false.
     * @param: tree - tree from root file
     * @param: branch_name - name of branch conatining variable
     * @return: return false when branch does not exist, otherwise return true
    */
    bool TryAttachToBranch(TTree** tree, std::string branch_name)
    {
        if((*tree)->FindBranch(branch_name.c_str()) == nullptr)
            return false;

        upValue.reset(new T());
        pValue = upValue.get();
        (*tree)->SetBranchAddress(branch_name.c_str(), &pValue);
        return true;
    }

private:
    std::unique_ptr<T> upValue;
    T* pValue;
};

#endif // VARIABLE_HH
