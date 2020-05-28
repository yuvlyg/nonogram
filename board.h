#include<vector>

using namespace std;

class Board {
public:
	Board(vector<vector<int>> con_cols, vector<vector<int>> con_rows);
	void print();
	void printSolution();
	void setCellAndUpdate(size_t row, size_t col, int val);
	bool solve();
	bool solveBacktracking();

private:
	size_t numUnknowns();
	int chooseIndexToSolve(bool columns);
        void setCol(vector<int> col, size_t idx);
	bool hasCheckedAll();
	vector<int> getCol(size_t idx);
	void updateNCombinations(bool init);

	vector<vector <int>> m_matrix;
	size_t m_n_cols; 
	size_t m_n_rows;
	vector<int> m_combinations_cols;
	vector<int> m_combinations_rows;
	vector<bool> m_should_check_cols;
	vector<bool> m_should_check_rows;
	vector<vector<int>> m_con_cols;
	vector<vector<int>> m_con_rows;
};

