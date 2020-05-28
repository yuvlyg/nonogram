#include<iostream>

#define EMPTY (0)
#define FULL (1)
#define UNKNOWN (2)

void printVec(vector<int> vec){
	std::cout << "printVec: ";
	for(size_t i=0 ; i < vec.size() ; i++){
		std::cout << vec[i]  << " ";
	}
	std::cout << std::endl;
}

void printVec(vector<bool> vec){
	std::cout << "printVec: ";
	for(size_t i=0 ; i < vec.size() ; i++){
		std::cout << vec[i]  << " ";
	}
	std::cout << std::endl;
}

void printVecs(vector<vector<int>> vec){
	for(size_t i=0; i < vec.size(); i++){
		printVec(vec[i]);
	}
	std::cout << std::endl;
}

size_t min(size_t a, size_t b){
	if(a < b){
		return a;
	}
	return b;
}

bool doesMatch(vector<int> option, vector<int> prefilled){
	for(size_t j=0; j < option.size(); j++){
		if((prefilled[j] != UNKNOWN) & (prefilled[j] != option[j])){
			return false;
		}
	}
	return true;
}

int sum(vector<int> vec){
	int s = 0;
	for(size_t i=0; i < vec.size(); i++){
		s += vec[i];
	}
	return s;
}

size_t nChoosek_with_limit(size_t n, size_t k) {
	if (k > n) return 0;
	if (k * 2 > n) k = n-k;
	if (k == 0) return 1;

        size_t result = n;
	for(size_t i = 2; i <= k; ++i ) {
		result *= (n-i+1);
		result /= i;
		if(result > (2 << 28)){
			return (2 << 28);
		}
	}
	return result;
}

vector<vector<int>> getOptions(vector<int> con, size_t n, vector<int> prefilled){
	//std::cout << "getOptions " << con.size() << "\t" << n ;
	if(prefilled.size() != n){
		std::cout << "prefilled.size() != n" << std::endl;
	}
	vector<vector<int>> results;
	//printVec(con);
	//std::cout << std::endl;

	if(con.size() == 0){
		vector<int> v(n, EMPTY);  // empty row
		if(doesMatch(v, prefilled)){
			results.push_back(v);
		}
		return results;
	}
	int sum_cons = sum(con);
	if(sum_cons + con.size() - 1 > n) {
		return results;  // no options
	}
	if(con.size() == 1){
		for(size_t i=0; i < n - con[0] + 1; i++){
			vector<int> v(n, EMPTY);
			for(size_t j=0; j < con[0] ; j++){
				v[i + j] = FULL;
			}
			if(doesMatch(v, prefilled)){
				results.push_back(v);
			}
		}
		return results;
	}
	for(size_t i=0; i <= n - sum_cons - con.size() + 1; i++){
		//std::cout << " trying " << i << " " << std::endl;
		vector<int> r(i + con[0] + 1, EMPTY);
		for(size_t j = 0; j < con[0]; j++){
			r[i + j] = FULL;
		}
		vector<int> prefilled_prefix(prefilled.begin(), prefilled.begin() + r.size());
		if(!doesMatch(r, prefilled_prefix)){
			continue;
		}
		vector<int> prefilled_suffix(prefilled.begin() + r.size(), prefilled.end());
		vector<int> con_but_first;
		vector<vector<int>> tmp_res;
		con_but_first = con;
		con_but_first.erase(con_but_first.begin());
		//std::cout << "calling recursion" << std::endl;
		tmp_res = getOptions(con_but_first, n - i - con[0] - 1, prefilled_suffix);
		for(size_t j=0; j < tmp_res.size(); j++){
			tmp_res[j].insert(tmp_res[j].begin(), r.begin(), r.end());
			results.push_back(tmp_res[j]);
		}
	}
	return results;
}

vector<int> getOptionsConsensus(vector<int> con, size_t n, vector<int> prefilled, int * o_n_options){
	vector<vector<int>> options = getOptions(con, n, prefilled);
	*o_n_options = options.size();
	if(options.size() == 0){
		vector<int> result(n, UNKNOWN);
		return result;
	}
	vector<int> result(options[0]);
	for(size_t i=0; i<options.size(); i++){
		for(size_t j=0; j<n; j++){
			if(options[i][j] != result[j]){
				result[j] = UNKNOWN;
			}
		}
	}
	return result;
}

