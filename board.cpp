#include<iostream>
#include<numeric>
#include <bits/stdc++.h> 
#include <unistd.h>

#include "board.h"
#include "helpers.cpp"

size_t getNCombinations(vector<int> con, size_t n){
	n = n - sum(con) + con.size();  // if there were con.size 1s
	n = n - con.size() + 1;  // if adjacent blocks were allowed
	return nChoosek_with_limit(n, con.size());
}

size_t getNCombinations_heuristic(vector<int> con, size_t n, size_t n_unknowns){
	n = min(n, sum(con) + con.size() + n_unknowns);
	return getNCombinations(con, n);
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

	updateNCombinations(true);
	printVec(m_combinations_cols);
	printVec(m_combinations_rows);
}

void Board::updateNCombinations(bool init){
	size_t n;
	bool any_updated = false;
	for(size_t i=0; i<m_n_cols; i++){
		n = getNCombinations_heuristic(m_con_cols[i], m_n_rows, numUnknownsVec(getCol(i)));
		if(init || (n < m_combinations_cols[i])){
			m_combinations_cols[i] = n;
			any_updated = true;
		}
	}
	for(size_t i=0; i<m_n_rows; i++){
		n = getNCombinations_heuristic(m_con_rows[i], m_n_cols, numUnknownsVec(m_matrix[i]));
		if(init || (n < m_combinations_rows[i])){
			cout << "updating n combinations, row " << i << ": " << m_combinations_rows[i] << " to " << n << endl;
			m_combinations_rows[i] = n;
			any_updated = true;
		}
	}
	cout << "update n combinations, did update? " << any_updated << endl;
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


int Board::chooseIndexToSolve(bool columns){
	vector<int> combs;
	vector<bool> * should_check;
	if(columns){
		combs = m_combinations_cols;
		should_check = &m_should_check_cols;
	} else {
		combs = m_combinations_rows;
		should_check = &m_should_check_rows;
	}
	//cout << "should check: ";
	//printVec(*should_check);
	vector<int> vec(combs.size());
	int x=0;
	std::iota(vec.begin(), vec.end(), x++); //Initializing
	std::sort(vec.begin(), vec.end(), [&](int i,int j){return combs[i]<combs[j];} );
	vector<int> candidate_vec;
        for(size_t i=0; i < vec.size(); i++){
		size_t idx = vec[i];
		//cout << "idx: " << idx << ", combs: " << combs[idx] << ", should check: " << (*should_check)[idx] << endl;
		if(!(*should_check)[idx]){
			continue;
		}
		if(columns){
			candidate_vec = getCol(idx);
		} else {
			candidate_vec = m_matrix[idx];
		}
		//cout << "num unknowns: " << numUnknownsVec(candidate_vec) << endl;
		if(numUnknownsVec(candidate_vec) > 0){
			//cout << "found an index: " << idx << endl; 
			return idx;
	        } else {
			(*should_check)[idx] = false;
		}
	}
	cout << "didn't find a index to solve. columns:" << columns << endl;
	return -1;
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

void Board::setCellAndUpdate(size_t row_idx, size_t col_idx, int val){
	if(m_matrix[row_idx][col_idx] != UNKNOWN){
		cout << "warning, setting a known value" << endl;
		throw;
	}
	m_matrix[row_idx][col_idx] = val;
	m_should_check_rows[row_idx] = true;
	m_should_check_cols[col_idx] = true;
}

bool Board::solveBacktracking(){
	cout << "solveBacktracking" << endl;
	print();
	size_t i, j;
	int n_options;
	// check validity TODO remove because will take forever
	for(i=0; i < m_n_rows; i++){
		getOptionsConsensus(m_con_rows[i], m_n_cols, m_matrix[i], &n_options);
		if(n_options == 0){
			return false;
		}
	}
	for(i=0; i < m_n_cols; i++){
		getOptionsConsensus(m_con_cols[i], m_n_rows, getCol(i), &n_options);
		if(n_options == 0){
			return false;
		}
	}

	bool found_unknown = false;
	// find first unknown
	for(i=0; i < m_n_rows; i++){
		m_matrix[i] = getOptionsConsensus(m_con_rows[i], m_n_cols, m_matrix[i], &n_options);
		for(j=0; j<m_n_cols; j++){
			if(m_matrix[i][j] == UNKNOWN){
				found_unknown = true;
				break;
			}
		}
		if(found_unknown){
			break;
		}
	}
	if(!found_unknown){
		print();
		return true;
	}
	
	vector<int> val = {FULL, EMPTY};
	size_t idx;
	for(idx=0; idx<2; idx++){
		std::cout << "calling recursion " << val[idx] << endl;
		Board b = *this;
		b.setCellAndUpdate(i, j, val[idx]);
		if(b.solveBacktracking()){
			return true;
		}
	}
	return false;
}

bool Board::solve(){
	while(numUnknowns() > 0){
		updateNCombinations(false);
		cout << "choose index columns" << endl;
		int col_idx = chooseIndexToSolve(true);
		cout << "choose index rows" << endl;
		int row_idx = chooseIndexToSolve(false);
		bool solve_using_rows = true;
		if((col_idx == -1) && (row_idx == -1)){
			cout << "didn't find an index to solve, returning false" << endl;
			return false;
		} else if(row_idx == -1) {
			solve_using_rows = false;
		} else if((col_idx != -1)  && (m_combinations_cols[col_idx] < m_combinations_rows[row_idx])){
			solve_using_rows = false;
		}
		if (solve_using_rows){
			size_t idx = row_idx;
			std::cout << "solving row " << idx << " # combinations: " << m_combinations_rows[idx] << std::endl;
			std::cout << numUnknowns() << std::endl;
			
			if(m_combinations_rows[idx] > 1000000){
				// find index to enumerate
				size_t j;
				for(j=0; j<m_n_cols; j++){
					if(m_matrix[idx][j] == UNKNOWN){
						break;
					}
				}
				vector<int> val = {FULL, EMPTY};
				for(size_t val_idx=0; val_idx<2; val_idx++){
					std::cout << "calling recursion " << idx << ", " << j << ": " << val[val_idx] << endl;
					Board b = *this;
					b.setCellAndUpdate(idx, j, val[val_idx]);
					getOptionsConsensus(m_con_rows[idx], m_n_cols, b.m_matrix[idx], &b.m_combinations_rows[idx]);
					if(m_combinations_rows[idx] == 0){
						std::cout << "recursion: assiged val illegal, next val" << endl;
						continue;
					}
					b.print();
					if(b.solve()){
						return true;
					}
				}
				std::cout << "recursion: two children failed, returning false" << endl;
				return false;
			}
			vector<int> row = getOptionsConsensus(m_con_rows[idx], m_n_cols, m_matrix[idx], &m_combinations_rows[idx]);
			if(m_combinations_rows[idx] == 0){
				return false;
			}
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
		} else {
			size_t idx = col_idx;
			std::cout << "solving col " << idx << " # combinations: " << m_combinations_cols[idx] << std::endl;
			vector<int> current_col = getCol(idx);
			vector<int> col = getOptionsConsensus(m_con_cols[idx], m_n_rows, current_col, &m_combinations_cols[idx]);
			if(m_combinations_cols[idx] == 0){
				return false;
			}
			if(col == current_col){
				m_should_check_cols[idx] = false;
			} else {
				setCol(col, idx);
				for(size_t i=0; i < col.size(); i++){
					if(col[i] != current_col[i]){
						m_should_check_rows[i] = true;
					}
				}
				print();
			}
		}
		if(hasCheckedAll()){
			print();
			cout << "no indices that should_check, return false" << endl;
			return false;
		}
	}
	printSolution();
	return true;
}


int main(int argc, char** argv){
	if(argc != 3){
		cout << "Usage: " << argv[0] << " cols.txt rows.txt" << std::endl;
		return -1;
	}

	Board b = boardFromTextFiles(argv[1], argv[2]);
	bool res = b.solve();

	if(res){
		return 0;
	}
	return -1;
}
