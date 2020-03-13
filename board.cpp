#include<iostream>
#include<numeric>
#include <bits/stdc++.h> 

#include "board.h"
#include "helpers.cpp"

size_t getNCombinations(vector<int> con, size_t n){
	n = n - sum(con) + con.size();  // if there were con.size 1s
	n = n - con.size() + 1;  // if adjacent blocks were allowed
	return nChoosek_with_limit(n, con.size());
}

Board::Board(vector<vector<int>> con_cols, vector<vector<int>> con_rows){
	m_n_cols = con_cols.size();
       	m_n_rows = con_rows.size();
	m_con_cols = con_cols;
	m_con_rows = con_rows;
        cout << m_n_cols << ", " << m_n_rows << std::endl;
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
	printVec(m_combinations_cols);
	printVec(m_combinations_rows);
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

void Board::printSolution(){
	for(size_t i=0; i < m_n_rows; i++){
		for(size_t j=0; j < m_n_cols; j++){
			if(m_matrix[i][j] == EMPTY){
				std::cout << " ";
			} else if(m_matrix[i][j] == FULL){
				std::cout << "X";
			} else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
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

vector<vector<int>> vectorFromTextFile(char * file_path){
	
	std::ifstream infile(file_path);
	std::string line;
	vector<vector<int>> result;
	vector<int> tmp;
	while (std::getline(infile, line)) {
		int i = stoi(line);
		if(i != -1){
			tmp.push_back(i);
		} else {	
			result.push_back(tmp);
			tmp = vector<int>();
		}
	}
	infile.close();
	//printVecs(result);
	return result;
}

Board boardFromTextFiles(char * cols_path, char * rows_path){
	vector<vector<int>> con_cols = vectorFromTextFile(cols_path);
	vector<vector<int>> con_rows = vectorFromTextFile(rows_path);
	Board b(con_cols, con_rows);
	return b;
}

bool Board::solve(){
	while(numUnknowns() > 0){
		size_t idx = chooseIndexToSolve(false);
		std::cout << "solving row " << idx << ". " << m_combinations_rows[idx] << std::endl;
		vector<int> row = getOptionsConsensus(m_con_rows[idx], m_n_cols, m_matrix[idx]);
		if(row == m_matrix[idx]){
			m_should_check_rows[idx] = false;
		} else {
			for(size_t i=0; i < row.size(); i++){
				if(row[i] != m_matrix[idx][i]){
					m_should_check_cols[i] = true;
				}
			}
			m_matrix[idx] = row;
		}

		idx = chooseIndexToSolve(true);
		std::cout << "solving col" << idx << ". " << m_combinations_cols[idx] << std::endl;
		vector<int> current_col = getCol(idx);
		vector<int> col = getOptionsConsensus(m_con_cols[idx], m_n_rows, current_col);
		if(col == current_col){
			m_should_check_cols[idx] = false;
		} else {
			setCol(col, idx);
			for(size_t i=0; i < col.size(); i++){
				if(col[i] != current_col[i]){
					m_should_check_rows[i] = true;
				}
			}
			printSolution();
		}
		if(hasCheckedAll()){
			print();
			return false;
		}
	}
	printSolution();
	return true;
}


int main(int argc, char** argv){
	std::cout << SIZE_MAX;
	if(argc != 3){
		cout << "Usage: " << argv[0] << " cols.txt rows.txt" << std::endl;
		return -1;
	}
	Board b = boardFromTextFiles(argv[1], argv[2]);
	b.print();
	b.solve();
	return 0;
}
