#include<iostream>
#include<numeric>
#include <bits/stdc++.h> 

#include "board.h"
#include "helpers.cpp"

size_t getNCombinations(vector<int> con, size_t n){
	n = n - sum(con) + con.size();  // if there were con.size 1s
	n = n - con.size() + 1;  // if adjacent blocks were allowed
	return nChoosek(n, con.size());
}

Board::Board(vector<vector<int>> con_cols, vector<vector<int>> con_rows){
	m_n_cols = con_cols.size();
       	m_n_rows = con_rows.size();
	m_con_cols = con_cols;
	m_con_rows = con_rows;

	vector<int> tmp(m_n_cols, UNKNOWN);
	m_matrix = vector<vector<int>>(m_n_rows, tmp);

	m_combinations_cols = vector<int>(m_n_cols);
	m_combinations_rows = vector<int>(m_n_rows);
	m_should_check_cols = vector<bool>(m_n_cols, true);
	m_should_check_rows = vector<bool>(m_n_rows, true);

	for(size_t i=0; i<m_n_cols; i++){
		m_combinations_cols[i] = getNCombinations(con_cols[i], m_n_rows);
	}
	for(size_t i=0; i<m_n_rows; i++){
		m_combinations_rows[i] = getNCombinations(con_rows[i], m_n_cols);
	}
}

void Board::shouldCheckAll(){
	for(size_t i=0; i<m_n_cols; i++){
		m_should_check_cols[i] = true;
	}
	for(size_t i=0; i<m_n_rows; i++){
		m_should_check_rows[i] = true;
	}
}

bool Board::hasCheckedAll(){
	for(size_t i=0; i<m_n_cols; i++){
		if(m_should_check_cols[i]){
			return false;
		}
	}
	for(size_t i=0; i<m_n_rows; i++){
		if(m_should_check_rows[i]){
			return false;
		}
	}
	return true;
}

size_t maxLen(vector<vector<int>> vecs){
	size_t result = 0;
	for(size_t i = 0; i < vecs.size(); i++){
		if(vecs[i].size() > result){
			result = vecs[i].size();
		}
	}
	return result;
}

size_t numUnknownsVec(vector<int> vec){
	size_t num = 0;
	for(size_t j=0; j<vec.size(); j++){
		if(vec[j] == UNKNOWN){
			num++;
		}
	}
	return num;
}

size_t Board::numUnknowns(){
	size_t num = 0;
	for(size_t i=0; i<m_n_rows; i++){
		num += numUnknownsVec(m_matrix[i]);
	}
	return num;
}


void Board::print(){
	size_t rows_before_board = maxLen(m_con_cols);
	size_t cols_before_board = maxLen(m_con_rows);
	// print columns constrains
	for(size_t i=0; i<rows_before_board; i++){
		for(size_t j=0; j<cols_before_board; j++){
			std::cout << " ";
		}
		for(size_t j=0; j<m_n_cols; j++){
			if(i >= rows_before_board - m_con_cols[j].size()){
				std::cout << m_con_cols[j][i - rows_before_board + m_con_cols[j].size()];
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
	for(size_t i=0; i<m_n_rows; i++){
		for(size_t j=0; j < cols_before_board; j++){
		       if(j >= cols_before_board - m_con_rows[i].size()){
			       std::cout << m_con_rows[i][j - cols_before_board + m_con_rows[i].size()];
			} else {
				std::cout << " ";
			}
		}
		for(size_t j=0; j < m_n_cols; j++){
			if(m_matrix[i][j] == EMPTY){
				std::cout << ".";
			} else if(m_matrix[i][j] == FULL){
				std::cout << "X";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

void Board::setCol(vector<int> col, size_t idx){
	for(size_t i=0; i < m_n_rows; i++){
		m_matrix[i][idx] = col[i];
	}
}

vector<int> Board::getCol(size_t idx){
	vector<int> res(m_n_rows);
	for(size_t i=0; i<m_n_rows; i++){
		res[i] = m_matrix[i][idx];
	}
	return res;
}


size_t Board::chooseIndexToSolve(bool columns){
	vector<int> combs;
	vector<bool> should_check;
	if(columns){
		combs = m_combinations_cols;
		should_check = m_should_check_cols;
	} else {
		combs = m_combinations_rows;
		should_check = m_should_check_rows;
	}
	vector<int> vec(combs.size());
	int x=0;
	std::iota(vec.begin(), vec.end(), x++); //Initializing
	std::sort(vec.begin(), vec.end(), [&](int i,int j){return combs[i]<combs[j];} );
	vector<int> candidate_vec;
        for(size_t i=0; i < vec.size(); i++){
		size_t idx = vec[i];
		if(!should_check[idx]){
			continue;
		}
		if(columns){
			candidate_vec = getCol(idx);
		} else {
			candidate_vec = m_matrix[idx];
		}
		if(numUnknownsVec(candidate_vec) > 0){
		       return idx;
	        }
	}
	return 0;	
}
	

bool Board::solve(){
	while(numUnknowns() > 0){
		size_t idx = chooseIndexToSolve(false);
		std::cout << "solving row " << idx << std::endl;
		vector<int> row = getOptionsConsensus(m_con_rows[idx], m_n_cols, m_matrix[idx]);
		if(row == m_matrix[idx]){
			m_should_check_rows[idx] = false;
		} else {
			m_matrix[idx] = row;
			shouldCheckAll();
			print();
		}

		idx = chooseIndexToSolve(true);
		std::cout << "solving column " << idx << std::endl;
		vector<int> col = getOptionsConsensus(m_con_cols[idx], m_n_rows, getCol(idx));
		if(col == getCol(idx)){
			m_should_check_cols[idx] = false;
		} else {
			setCol(col, idx);
			shouldCheckAll();
			print();
		}
		if(hasCheckedAll()){
			return false;
		}
	}
	return true;
}


int main(int argc, char** argv){
	vector<vector<int>> con_cols = {{1}, {5}, {1, 1}, {3}, {1}};
	vector<vector<int>> con_rows = {{3}, {1, 1}, {3}, {1}, {2, 1}};
	size_t n = 16;
	vector<int> prefilled(16, UNKNOWN);
	prefilled[1] = FULL;
	Board b(con_cols, con_rows);
	//b.print();
	b.solve();
	//vector<int> con = {3,10};
	//printVecs(getOptions(con, 16, prefilled));
	//printVec(getOptionsConsensus(con, 16, prefilled));
	return 0;
}
