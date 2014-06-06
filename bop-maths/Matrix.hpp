#ifndef BOP_MATRIX_HPP
#define BOP_MATRIX_HPP
#ifndef BOP_MATRIX_DISCARD_BY
#define BOP_MATRIX_DISCARD_BY 0xFFFF
#endif
#include <initializer_list>
#include <string>
#include <sstream>
#include "Vector.hpp"

/*
	bop::maths::Matrix class file
*/

namespace bop {
	namespace maths{
		template<class T>
		class Matrix {
			protected:
				unsigned int width;
				unsigned int height;
			public:
				Vector<T>* data;
				
				//Constructors
				Matrix(unsigned int size, T fill = 0) {
					/*
						Square matrix constructor, sets all values as
						the given fill value.
					*/
					
					this->data = new Vector<T>[size];
					for (unsigned int i = 0; i < size; i++) {
						this->data[i] = Vector<T>(size, fill);
					}
					this->width = size;
					this->height = size;
				}
				
				Matrix(unsigned int width, unsigned int height, T fill = 0) {
					/*
						Non-square matrix constructor, sets all values as 
						the given fill value.
					*/
					this->data = new Vector<T>[height];
					for (unsigned int i = 0; i < height; i++) {
						this->data[i] = Vector<T>(width, fill);
					}
					this->height = height;
					this->width = width;
				}
				
				Matrix(std::initializer_list< std::initializer_list<T> > list) {
					/*
						Creates a matrix from a 2-dimensional initializer_list.
					*/
					this->height = list.size();
					this->data = new Vector<T>[this->height];
					unsigned int min_w = list.begin()->size();
					unsigned int i = 0;
					for (auto elem : list) {
						if (elem.size() < min_w) min_w = elem.size();
						this->data[i] = Vector<T>(elem);
						i++;
					}
					this->width = min_w;
				}
				
				Matrix(Matrix<T>& mat) {
					/*
						Copy constructor.
					*/
					this->width = mat.width;
					this->height = mat.height;
					this->data = new Vector<T>[this->height];
					for (unsigned int i = 0; i < this->width; i++) {
						this->data[i] = mat.data[i];
					}
				}
				
				Matrix(Matrix<T>&& mat) {
					/*
						Move constructor.
					*/
					this->width = mat.width;
					this->height = mat.height;
					this->data = mat.data;
					mat.data = NULL;
				}
				
				Matrix() {
					/*
						Empty constructor.
					*/
					this->data = NULL;
				}
				
				//Destructor
				~Matrix() {
					delete[] this->data;
					this->data = NULL;
				}
				
				//Operator overloads
				inline Vector<T>& operator[] (const unsigned int index) {
					return this->data[index];
				}
				
				Matrix<T>& operator= (const Matrix<T> &mat) {
					this->width = mat.width;
					this->height = mat.height;
					delete[] this->data;
					this->data = new Vector<T>[this->height];
					for (unsigned int i = 0; i < this->height; i++) {
						this->data[i] = mat.data[i];
					}
					return *this;
				}
				
				//Boolean logic overloads
				bool operator== (const Matrix<T>& mat) {
					if (this->width == mat.width && this->height == mat.height) {
						bool same = true;
						for (unsigned int i = 0; i < this->height && same; i++) {
							same = (this->data[i] == mat.data[i]);
						}
						return same;
					}
					else return false;
				}
				
				bool operator!= (const Matrix<T>& mat) {
					return !(this->operator==(mat));
				}
				
				//Arithmetic overloads
				Matrix<T>& operator*= (const T scalar) {
					/*
						Scalar multiplication member operator, multiplies all
						elements by the given scalar.
					*/
					for (unsigned int i = 0; i < this->height; i++) {
						this->data[i] *= scalar;
					}
					return *this;
				}
				
				Matrix<T>& operator*= (Matrix<T>& mat) {
					/*
						Matrix multiplication member operator.
					*/
					Vector<T>* temp = new Vector<T>[this->height];
					for (unsigned int i = 0; i < this->height; i++) {
						temp[i] = Vector<T>(mat.width);
					}
					for (unsigned int r = 0; r < this->height; r++) {
						for (unsigned int c = 0; c < mat.width; c++) {
							T product = 0;
							for (unsigned int i = 0; i < mat.width; i++) {
								product += (this->data[r][i] * mat.data[i][c]);
							}
							temp[r][c] = product;
							#ifdef BOP_MATRIX_MULTIPLY_DISCARD_TINY
							/*
								Hack to compensate for small values (e.g. at 10^-16) that
								are leftover from the silliness of representing numbers as
								a sequence of bits and the limits of precision.
							*/
							temp[r][c] += BOP_MATRIX_DISCARD_BY;
							temp[r][c] -= BOP_MATRIX_DISCARD_BY;
							#endif
						}
					}
					delete[] this->data;
					this->data = temp;
					this->width = mat.width;
					return *this;
				}
				
				Vector<T>& operator*= (Vector<T>& vec) {
					/*
						Seemingly counter-intuitive multiplication
						operation, although *= is being used with the
						matrix as the left operand what is being
						multiplied is the vector. The matrix is unaffected.
					*/
					if (vec.width == this->width) {
						T* temp_data = new T[this->height];
						for (unsigned int i = 0; i < this->height; i++) {
							T product = 0;
							for (unsigned int dot_index = 0; dot_index < vec.width; dot_index++) {
								product += (vec.data[dot_index] * this->data[i][dot_index]);
							}
							temp_data[i] = product;
							#ifdef BOP_MATRIX_MULTIPLY_DISCARD_TINY
							temp_data[i] += BOP_MATRIX_DISCARD_BY;
							temp_data[i] -= BOP_MATRIX_DISCARD_BY;
							#endif
						}
						delete[] vec.data;
						vec.data = temp_data;
						vec.width = this->height;
					}
					return vec;
				}
				
				Matrix<T>& operator/= (const T scalar) {
					/*
						Scalar division member operator, divides all elements
						by the given scalar.
					*/
					if (scalar != 0) {
						for (unsigned int i = 0; i < this->height; i++) {
							this->data[i] /= scalar;
						}
					}
					return *this;
				}
				
				Matrix<T>& operator+= (Matrix<T> &mat) {
					/*
						Addition member operator, adds all the elements of a
						given matrix to the matrix.
					*/	
					for (unsigned int i = 0; i < this->height; i++) {
						this->data[i] += mat[i];
					}
					return *this;
				}
				
				Matrix<T>& operator-= (Matrix<T> &mat) {
					/*
						Subtraction member operator, subtracts the value of
						each element in a given matrix from the matrix.
					*/
					for (unsigned int i = 0; i < this->height; i++) {
						this->data[i] -= mat[i];
					}
					return *this;
				}
				
				//Information functions
				inline unsigned int w() {
					/*
						Returns the width of the Matrix.
					*/
					return this->width;
				}
				
				inline unsigned int h() {
					/*
						Returns the height of the Matrix.
					*/
					return this->height;
				}
				
				inline bool square() {
					/*
						Tests if the Matrix is a square Matrix.
					*/
					return (this->width == this->height);
				}
				
				bool identity() {
					/*
						Tests if the matrix is an identity matrix.
					*/
					if (this->width == this->height) {
						bool identity = true;
						for (unsigned int y = 0; y < this->height && identity; y++) {
							for (unsigned int x = 0; x < this->width && identity; x++) {
								if (x == y) identity = (this->data[y][x] == 1);
								else identity = (this->data[y][x] == 0);
							}
						}
						return identity;
					}
					else return false;
				}
				
				std::string string(bool newlines = true) {
					/*
						Similar to the maths::Vector::string function, returns 
						a human-readable representation of the Matrix.
					*/
					std::ostringstream mat_str;
					for (unsigned int i = 0; i < this->height; i++) {
						mat_str << '[' << this->data[i].string(false) << ']';
						if (newlines) mat_str << '\n';
					}
					return mat_str.str();
				}
				
				//Matrix manipulation functions.
				
				inline void swapRows(unsigned int index_a, unsigned int index_b) {
					/*
						Row swap function, for use in such algorithms as Gauss-Jordan
						elimination.
					*/
					if (index_a != index_b) {
						this->data[index_a].swap(this->data[index_b]);
					}
				}
				
				void swap(Matrix<T>& mat) {
					/*
						Matrix object swap function.
					*/
					Vector<T>* temp_arr = mat.data;
					unsigned int temp_w = mat.width;
					unsigned int temp_h = mat.height;
					mat.data = this->data;
					mat.width = this->width;
					mat.height = this->height;
					this->data = temp_arr;
					this->width = temp_w;
					this->height = temp_h;
				}
				
				void impose(Matrix<T>& mat, unsigned int row_off = 0, unsigned int col_off = 0) {
					for (unsigned int i = 0; i < mat.height && i + row_off < this->height; i++) {
						this->data[i + row_off].impose(mat[i], col_off);
					}
				}
		};
		
		//External arithmetic overloads
		
		template<class T>
		Matrix<T> operator* (Matrix<T> &mat, const T scalar) {
			Matrix<T> mat_p(mat);
			mat_p *= scalar;
			return mat_p;
		}
		
		template<class T>
		Matrix<T> operator* (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_p(mat1);
			mat_p *= mat2;
			return mat_p;
		}
		
		template<class T>
		Vector<T> operator* (Matrix<T>& mat, Vector<T>& vec) {
			Vector<T> vec_p(vec);
			mat *= vec_p;
			return vec_p;
		}
		
		template<class T>
		Matrix<T> operator/ (Matrix<T> &mat, const T scalar) {
			Matrix<T> mat_p(mat);
			mat_p /= scalar;
			return mat_p;
		}
		
		template<class T>
		Matrix<T> operator+ (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_sum(mat1);
			mat_sum += mat2;
			return mat_sum;
		}
		
		template<class T>
		Matrix<T> operator- (Matrix<T> &mat1, Matrix<T>& mat2) {
			Matrix<T> mat_sum(mat1);
			mat_sum -= mat2;
			return mat_sum;
		}
		
		//Matrix related functions
		template<class T>
		T det(Matrix<T>& mat, Vector<bool>& allowed_cols, unsigned int& size) {
			/*
				Recursive matrix determinant function.
			*/
			if (size == 2) {
				//the  base case, finding the 2 by 2 matrix determinant
				int c1 = -1, c2 = -1;
				for (unsigned int i = 0; i < mat.w(); i++) {
					if (allowed_cols[i]) {
						if (c1 < 0) {
							c1 = i;
						}
						else if (c2 < 0) {
							c2 = i;
						}
						else {
							break;
						}
					}
				}
				return (mat[mat.h() - 2][c1] * mat[mat.h() - 1][c2]) - (mat[mat.h() - 2][c2] * mat[mat.h() - 1][c1]);
			}
			else {
				size--;
				T product = 0;
				T multi = 1;
				for (unsigned int i = 0; i < mat.w(); i++) {
					if (!allowed_cols[i]) continue;
					else {
						allowed_cols[i] = false;
						product += (multi * (mat[mat.h() - size][i] * det(mat, allowed_cols, size)));
						multi *= -1;
						allowed_cols[i] = true;
						
					}
				}
				size++;
				return product;
			}
		}
		
		template<class T>
		T det(Matrix<T>& mat) {
			/*
				Matrix determinant init function.
			*/
			if (!mat.square()) return 0;
			else {
				if (mat.w() == 2) {
					
					//shortcut to determinant of a 2 by 2 matrix
					return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
				}
				else {
					Vector<bool> allowed_cols(mat.w(), true);
					T product = 0;
					T multi = 1;
					unsigned int size = mat.h() - 1;
					for (unsigned int i = 0; i < mat.w(); i++) {
						allowed_cols[i] = false;
						product += (multi * (mat[0][i] * det(mat, allowed_cols, size)));
						multi *= -1;
						allowed_cols[i] = true;
					}
					return product;
				}
			}
		}
		
		template<class T>
		Matrix<T> identityMatrix(unsigned int size) {
			/*
				Returns an Identity Matrix.
			*/
			Matrix<T> unit_m(size);
			for (unsigned int i = 0; i < size; i++) {
				unit_m[i][i] = 1;
			}
			return unit_m;
		}
		
		
		template<class T>
		bool invertable(Matrix<T> &mat) {
			return (mat.square() && det(mat) != 0);
		}
		
		template<class T>
		Matrix<T> inverseMatrix(Matrix<T> mat) {
			/*
				Matrix inverse by Gauss-Jordan method ([A|I] -> [I|A']).
			*/
			T deter = det(mat);
			if (deter != 0) {
				if (mat.h() == 2) {
					/*
						2 by 2 matrix shortcut.
					*/
					T temp = mat[0][0];
					mat[0][0] = mat[1][1];
					mat[1][1] = temp;
					mat[0][1] *= -1;
					mat[1][0] *= -1;
					mat /= deter;
					return mat;
				}
				else {
					#ifndef BOP_MATRIX_INVERSE_BY_COFACTOR
					Matrix<T> inverse = identityMatrix<T>(mat.h());
					/*
						non-2x2 matrix inverse solution
					*/
					for (unsigned int col = 0; col < mat.w(); col++) {
						unsigned int row_index;
						/*
							Find the first row whose pivot index is the same as the
							identity column (col) that needs representation. This is
							the Pivot row.
						*/
						for (row_index = col; row_index < mat.h() && static_cast<unsigned int>(mat[row_index].pivot()) != col; row_index++);
						/*
							Simultaneously swap the rows in the given matrix and to-be
							inverse matrix so that the Pivot row is at the index where
							it's pivot index matches the row index.
						*/
						mat.swapRows(col, row_index);
						inverse.swapRows(col, row_index);
						/*
							Divide the row at the Pivot row's index in the to-be inverse
							by the value at the Pivot row's pivot value. Then, divide the
							Pivot row by it's pivot value, making the pivot value in the
							Pivot row 1.
						*/
						inverse[col] /= mat[col][col];
						mat[col] /= mat[col][col];
						for (unsigned int i = col + 1; i < mat.h(); i++) {
							if (i == col || static_cast<unsigned int>(mat[i].pivot()) != col) continue;
							else {
								/*
									For each row in the matrix with a pivot index identical to
									the Pivot row's index, subtract the Pivot row multiplied by
									their pivot's value from the row, leaving their pivot value
									as 0. Mirror these actions on the to-be inverse Matrix.
								*/
								T temp = mat[i][col];
								
								mat[col] *= temp;
								inverse[col] *= temp;
								mat[i] -= mat[col];
								inverse[i] -= inverse[col];
								mat[col] /= temp;
								inverse[col] /= temp;
							}
						}
					}
					for (unsigned int col = mat.h() - 1; col > 0; col--) {
						for (unsigned int sub_row = 0; sub_row < col; sub_row++) {
							T temp = mat[sub_row][col];
							/*
								Now the matrix is in row echelon form, reduce it to an 
								identity matrix.
							*/
							if (temp != 0) {
								mat[col] *= temp;
								inverse[col] *= temp;
								mat[sub_row] -= mat[col];
								inverse[sub_row] -= inverse[col];
								mat[col] /= temp;
								inverse[col] /= temp;
							}
						}
					}
					return inverse;
					#else
					//todo: cofactor matrix solution 
					#endif
				}
			}
			else {
				return identityMatrix<T>(mat.h());
			}
		}
		
		template<class T>
		Matrix<T> transposeMatrix(Matrix<T> &mat) {
			/*
				Creates & returns a transposed version of the given matrix.
			*/
			Matrix<T> trans(mat.h(), mat.w(), 0);
			for (unsigned int y = 0; y < mat.h(); y++) {
				for (unsigned int x = 0; x < mat.w(); x++) {
					trans[x][y] = mat[y][x];
				}
			}
			return trans;
		}
	}
}

#endif