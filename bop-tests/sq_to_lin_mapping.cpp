#include <iostream>
#include <utility>
#include <initializer_list>

/*
    Proof of concept for 1 dimentional array for matrix data representation.
*/

template<class T, T LM_zero = 0>
class LMatrix {
    protected:
        unsigned int width;
        unsigned int height;
        bool transpose;
        T* data;
        class IndexHolder {
            LMatrix<T>& mat;
            unsigned int temp_index;
            IndexHolder(LMatrix<T>& mat) {
                this->mat = mat;
            }
            T& operator[] (unsigned int index) {
                if (!this->mat.transposed) return this->mat.data[(this->mat.width * this->mat.temp_index) + index];
                else return this->mat.data[(this->mat.height * mat.temp_index) + index];
            }
        };
        IndexHolder map;
        inline unsigned int mapIndex(unsigned int row, unsigned int col) {
            return (row * this->width) + col;
        }
    public:
        
        LMatrix() {
            this->data = NULL;
            this->map(*this);
            this->transposed = false;
        }
        
        LMatrix(unsigned int w, unsigned int h, T fill = LM_zero) : LMatrix() {
            this->width = w;
            this->height = h;
            this->data = new T[w * h];
            for (unsigned int i = 0; i < (w * h); i++) {
                this->data[i] = fill;
            }
        }
        
        LMatrix(unsigned int size, T fill = LM_zero) : LMatrix(size, size, fill) {
            
        }
        
        LMatrix(std::initializer_list< std::initializer_list< T > > list) : LMatrix() {
            unsigned int height_count = 0;
            for (auto row : list) {
                unsigned int width_count = 0;
                for (auto col : row) {
                    this->data[(this->width * height_count) + width_count];
                    width_count++;
                }
                height_count++;
            }
        }
};

#define Matrix LMatrix<double>

int main() {
    
    return 0;
}
