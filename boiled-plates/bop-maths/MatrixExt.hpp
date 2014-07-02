#ifndef BOP_MATRIX_EXT_HPP
#define BOP_MATRIX_EXT_HPP
#include <cmath>
#include <utility>
#include "Matrix.hpp"

/*
	bop::maths::Matrix External functions.
*/

namespace bop {
	namespace maths{
		//Matrix related functions
		template<class T>
		inline T det(Matrix<T>& mat, Vector<bool>& allowed_cols, unsigned int& size) {
			/*
				Recursive matrix determinant function.
			*/
			if (size == 2) {
				//the  base case, finding the 2 by 2 matrix determinant
				int c1 = -1, c2 = -1;
				for (unsigned int i = 0; i < mat.width(); i++) {
					if (allowed_cols[i]) {
						if (c1 < 0) {
							c1 = i;
						}
						else if (c2 < 0) {
							c2 = i;
							break;
						}
					}
				}
				return (mat[mat.height() - 2][c1] * mat[mat.height() - 1][c2]) - (mat[mat.height() - 2][c2] * mat[mat.height() - 1][c1]);
			}
			else {
				size--;
				T product = 0;
				T multi = 1;
				for (unsigned int i = 0; i < mat.width(); i++) {
					if (!allowed_cols[i]) continue;
					else {
						allowed_cols[i] = false;
						product += (multi * (mat[mat.height() - size][i] * det(mat, allowed_cols, size)));
						multi *= -1;
						allowed_cols[i] = true;
						
					}
				}
				size++;
				return product;
			}
		}
		
		template<class T>
		inline T det(Matrix<T>& mat) {
			/*
				Matrix determinant init function.
			*/
			if (!mat.square()) return 0;
			else {
				if (mat.width() == 2) {
					
					//shortcut to determinant of a 2 by 2 matrix
					return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
				}
				else {
					Vector<bool> allowed_cols(mat.width(), true);
					T product = 0;
					T multi = 1;
					unsigned int size = mat.height() - 1;
					for (unsigned int i = 0; i < mat.width(); i++) {
						allowed_cols[i] = false;
						product += (multi * (mat[0][i] * det(mat, allowed_cols, size)));
						multi *= -1;
						allowed_cols[i] = true;
					}
					return product;
				}
			}
		}
		
		template<class T = BOP_MATHS_DEFAULT_TYPE>
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
		
		template<class T = BOP_MATHS_DEFAULT_TYPE>
		Matrix<T> identityMatrix(unsigned int width, unsigned int height) {
			/*
				Returns an Identity Matrix.
			*/
			Matrix<T> unit_m(width, height);
			for (unsigned int i = 0; i < width && i < height; i++) {
				unit_m[i][i] = 1;
			}
			return unit_m;
		}
		
		template <class T = BOP_MATHS_DEFAULT_TYPE>
		Matrix<T> identityMatrix(Matrix<T>& mat) {
			return identityMatrix<T>(mat.width(), mat.height());
		}
		
		template<class T>
		bool invertable(Matrix<T>& mat) {
			return (det(mat) != 0);
		}
		
		template<class T>
		Matrix<T> inverseMatrix(Matrix<T> mat) {
			/*
				Matrix inverse by Gauss-Jordan method ([A|I] -> [I|A']).
			*/
			T deter = det(mat);
			if (deter != 0) {
				if (mat.height() == 2) {
					/*
						2 by 2 matrix shortcut.
					*/
					std::swap(mat[1][1], mat[0][0]);
					mat[0][1] *= -1;
					mat[1][0] *= -1;
					mat /= deter;
					return mat;
				}
				else {
					#ifndef BOP_MATRIX_INVERSE_BY_COFACTOR
					Matrix<T> inverse = identityMatrix<T>(mat.height());
					/*
						non-2x2 matrix inverse solution
					*/
					for (unsigned int col = 0; col < mat.width(); col++) {
						unsigned int row_index;
						/*
							Find the first row whose pivot index is the same as the
							identity column (col) that needs representation. This is
							the Pivot row.
						*/
						for (row_index = col; row_index < mat.height() && mat[row_index].pivot() != col; row_index++);
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
						T div_temp = mat[col][col];
						for (unsigned int div_col = 0; div_col < mat.width(); div_col++) {
							inverse[col][div_col] /= div_temp;
							mat[col][div_col] /= div_temp;
						}
						
						for (unsigned int i = col + 1; i < mat.height(); i++) {
							if (i == col || mat[i].pivot() != col) continue;
							else {
								/*
									For each row in the matrix with a pivot index identical to
									the Pivot row's index, subtract the Pivot row multiplied by
									their pivot's value from the row, leaving their pivot value
									as 0. Mirror these actions on the to-be inverse Matrix.
								*/
								T temp = mat[i][col];
								for (unsigned int elem = 0; elem < mat.width(); elem++) {
									mat[i][elem] -= mat[col][elem] * temp;
									inverse[i][elem] -= inverse[col][elem] * temp;
								}
							}
						}
					}
					for (unsigned int col = mat.height() - 1; col > 0; col--) {
						for (unsigned int sub_row = 0; sub_row < col; sub_row++) {
							T temp = mat[sub_row][col];
							/*
								Now the matrix is in row echelon form, reduce it to an 
								identity matrix.
							*/
							for (unsigned int elem = 0; elem < mat.width(); elem++) {
								mat[sub_row][elem] -= mat[col][elem] * temp;
								inverse[sub_row][elem] -= inverse[col][elem] * temp;
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
				return identityMatrix<T>(mat.height());
			}
		}
		
		template<class T>
		Matrix<T> transposeMatrix(Matrix<T> &mat) {
			/*
				Creates & returns a transposed version of the given matrix.
			*/
			Matrix<T> trans(mat.height(), mat.width(), 0);
			for (unsigned int y = 0; y < mat.height(); y++) {
				for (unsigned int x = 0; x < mat.width(); x++) {
					trans[x][y] = mat[y][x];
				}
			}
			return trans;
		}
		
		template<class T = BOP_MATHS_DEFAULT_TYPE>
		Matrix<T> rotationMatrix(double angle, unsigned int size = 2, bool clockwise = false, bool rads = false) {
			/*
				Generates a rotation matrix for the angle and size given.
			*/
			
			if (!rads) {
				if (clockwise) angle -= 180;
				angle = ((pi()/180.0) * angle); //Convert the degrees to radiens
			}
			if (size == 2) {
				Matrix<T> rotmat = {{static_cast<T>(cos(angle)), static_cast<T>(-sin(angle))},
									{static_cast<T>(sin(angle)), static_cast<T>(cos(angle))}};
				return rotmat;
			}
			else {
				//todo
				return identityMatrix<T>(size);
			}
		}
	}
}

#endif