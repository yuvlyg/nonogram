#include<vector>

using namespace std;

class Board {
public:
	Board(vector<vector<int>> con_cols, vector<vector<int>> con_rows);
	void print();
	bool solve();
private:
	size_t numUnknowns();
	size_t chooseIndexToSolve(bool columns);
        void setCol(vector<int> col, size_t idx);
	void shouldCheckAll();
	bool hasCheckedAll();
	vector<int> getCol(size_t idx);

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

