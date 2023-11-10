#ifndef __MATRIX_H
#define __MATRIX_H

/**
 * This is a C++ matrix library for TI DSP 67x seriese.
 * Most operations are mapped with operator functions 
 * (Ex. operator* means matrixes multiplication) by C++ specification.
 * Additionally, the library uses refrence counter technique to manage memory,
 * So in most cases, you don't care annoying new/delete operation.
 * Therefore I believe you can use the library intuitively :-)
 * 
 * Usage Ex)
 *  #include "matrix.h" 
 * 
 *  Matrix<double> some_matrix_op(){
 *    Matrix<double> m1(3, 3); m1 is a 3 by 3 zero matrix.
 *    {
 *      m1(0, 0) = 1.; // I hate Matlab, like C style indexes :-)
 *      m1(1, 1) = 2.;
 *      m1(2, 2) = 3.;
 *      m1(0, 1) = 4.;
 *    }
 *    Matrix<double> m2(Matrix::getI(3)); // m2 is a 3 by 3 unit matrix.
 *    return m1.transpose() * m2; // => {1, 0, 0; 4, 2, 0; 0, 0, 3}
 *  }
 *  
 * 
 * The library can handle some matrix operations far more quickly 
 * with "TMS320C67x DSP Library (dsp67x.lib)".
 * If you want to use this feature, please comment in some headers below 
 * like DSPF_... and set correctly the include / link configuaration
 * of your project.
 * 
 * If you have any questions / comments, 
 * please mail me < fenrir.naru atmark gmail dot com >.
 * 
 * Finally, credit is here:
 *  
 * This library is free and not be asuured to work correctly.
 * The credit holder of this library, M.Naruoka(fenrir), want that this library is used 
 * under the Attribution-NonCommercial-ShareAlike creative commons license.
 * you can confirm the limitation at http://creativecommons.org/licenses/by-nc-sa/.
 * 
 * === Japanese ===
 * これはTI DSP 67xシリーズ向けの行列ライブラリです。
 * 例えば行列積は*で表現されるなど、
 * 行列演算の多くはC++言語の特徴であるオペレータオーバーロードを使って
 * 実装されています。
 * またメモリの管理についてもリファレンスカウンターを使っているので、
 * 通常の使用ではメモリリークを気にする必要はありません。
 * 直感的に使えると信じています(笑)
 * 
 * またこのライブラリは"TMS320C67x DSP Library (dsp67x.lib)"を使って
 * 行列積などについてかなり高速化することが可能です。
 * 下のコメントアウトされたヘッダファイルのインクルード、ならびにdsp67x.libを
 * リンクすることでこのオプションは有効になります。
 * 
 * 何かありましたら、
 * fenrir.naru atmark gmail dot com までメールお願いいたします。
 * 
 * このライブラリは無償ですが、動作を保証するものではありません。
 * 著作者(M.Naruoka, ハンドルネームfenrir)はこのライブラリが
 * クリエィティブコモンズライセンス(表示 - 非営利 - 継承)の元で
 * 正しく利用されることを望みます。
 * ライセンスの詳細については、
 * http://creativecommons.org/licenses/by-nc-sa/deed.ja
 */

/*extern "C"{
#include <DSPF_dp_mat_mul.h>
#include <DSPF_sp_mat_mul.h>
#include <DSPF_dp_mat_trans.h>
#include <DSPF_sp_mat_trans.h>
#include <DSPF_dp_blk_move.h>
#include <DSPF_sp_blk_move.h>
}*/

#include <cstdio>
#include <cstring>
#include <cassert>
#include <new>

extern void canary_bird();

template<class FloatT>
class Array2D;

template <class FloatT>
class Array2D_Dense;

/**
 * Two-dimension array abstract class.
 * 
 * 2次元配列の抽象クラス
 * 
 */
template<class FloatT>
class Array2D{
  protected:
    unsigned int m_rows;
    unsigned int m_columns;
    
    typedef Array2D<FloatT> self_t;
    typedef Array2D_Dense<FloatT> dense_t;
  
  public:
    /**
     * シャローコピーをします。
     * 
     * @return (self_t *) 自分自身
     */
    virtual self_t *shallow_copy() const = 0;

    /**
     * Array2Dクラスのコンストラクタ。
     * 指定の行サイズ、指定の列サイズで仮想的な2次元配列を生成します。
     * 
     * @param rows 行数
     * @param columns 列数
     */
    Array2D(const unsigned int &rows, const unsigned int &columns)
        : m_rows(rows), m_columns(columns){
#ifdef _DEBUG
      canary_bird();
#endif
    }
    
    /**
     * Array2Dクラスのデストラクタ。
     * 
     */
    virtual ~Array2D(){
#ifdef _DEBUG
      canary_bird();
#endif
    }
    
    /**
     * 要素のゼロクリアをします。
     *
     */
    virtual void clear() = 0;
    
    /**
     * 2次元配列を複製(ディープコピー)します。
     * 
     * @return (self_t *) コピー
     */
    virtual self_t *copy() const = 0;

    /**
     * 中身が密な配列に変換します。
     *
     * @return (dense_t)
     */
    virtual dense_t dense() const = 0;
    
    /**
     * 行数を返します。
     * 
     * @return (int) 行数
     */
    unsigned int rows() const{return m_rows;}
    /**
     * 列数を返します。
     * 
     * @return (int) 列数
     */
    unsigned int columns() const{return m_columns;}
    
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    virtual FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column) = 0;
};

template <class FloatT>
class Array2D_BufferManager {
  protected:
    FloatT *m_buffer;
    int *ref; //参照カウンタ
    
    typedef Array2D_BufferManager<FloatT> self_t;
  
  public:
    /**
     * 管理しているバッファを返します。
     * 
     * @return (FloatT *) 単純配列
     */
    FloatT *buffer() const {return m_buffer;}
    
    /**
     * Array2D_BufferManagerクラスのコンストラクタ。
     * 管理対象とするバッファを登録します。
     * 
     * @param buffer 管理対象のバッファ
     */
    Array2D_BufferManager(FloatT *buffer) 
        : m_buffer(buffer), 
        ref(new int(0)) {
      assert(m_buffer && ref);
      (*ref)++;
    }
    
    /**
     * コピーコンストラクタ
     * 
     * @param orig コピー元
     */
    Array2D_BufferManager(const self_t &orig) 
        : m_buffer(orig.m_buffer), ref(orig.ref){
      if(ref){(*ref)++;}
    }
    
    /**
     * デストラクタ。
     */
    virtual ~Array2D_BufferManager(){
      if(ref && ((--(*ref)) <= 0)){
        delete [] m_buffer;
        delete ref;
      }
    }
    
    /**
     * 代入演算子。
     * 
     * @return (self_t) 自分自身
     */
    self_t &operator=(const self_t &array){
      if(this != &array){
        if(ref && ((--(*ref)) <= 0)){delete [] m_buffer; delete ref;}
        if(m_buffer = array.m_buffer){
          (*(ref = array.ref))++;
        }
      }
      return *this;
    }
};

/**
 * Two-dimension dense array class.
 * 
 * 中身が詰まった2次元配列
 * 
 */
template <class FloatT>
class Array2D_Dense 
    : public Array2D<FloatT>, public Array2D_BufferManager<FloatT> {
  protected:
    
    typedef Array2D<FloatT> super_t;
    typedef Array2D<FloatT> root_t;
    typedef Array2D_Dense<FloatT> self_t;
    typedef Array2D_BufferManager<FloatT> buffer_manager_t;
  
  public:
    using buffer_manager_t::m_buffer;
    
    /**
     * Array2D_Denseクラスのコンストラクタ。
     * 指定の行サイズ、指定の列サイズで2次元配列を生成します。
     * また成分はすべてFloatT(0)で初期化されます。
     * 
     * @param rows 行数
     * @param columns 列数
     */
    Array2D_Dense(
        const unsigned int &rows, 
        const unsigned int &columns) 
        : super_t(rows, columns), 
        buffer_manager_t(new FloatT[rows * columns]) {
        
    }
    
    /**
     * Array2D_Denseクラスのコンストラクタ。
     * 指定の行サイズ、指定の列サイズで2次元配列を生成します。
     * また成分はserilaizedによって指定された値で生成されます
     *
     * @param rows 行数
     * @param columns 列数
     * @param serialized 成分
     */
    Array2D_Dense(
        const unsigned int &rows,
        const unsigned int &columns,
        const FloatT *serialized)
        : super_t(rows, columns),
        buffer_manager_t(new FloatT[rows * columns]) {
      memcpy(m_buffer, serialized, 
          sizeof(FloatT) * rows * columns);
    }
    
    /**
     * コピーコンストラクタ
     * 
     */
    Array2D_Dense(const self_t &orig) 
        : super_t(orig.m_rows, orig.m_columns),
        buffer_manager_t(orig) {
      
    }
    
    /**
     * デストラクタ。
     */
    ~Array2D_Dense(){}
    
    using super_t::rows;
    using super_t::columns;
    
    /**
     * 2次元配列を複製(ディープコピー)します。
     * 
     * @return (root_t) コピー
     */
    root_t *copy() const {
      self_t *array(new self_t(rows(), columns()));
      memcpy(array->buffer(), m_buffer, 
          sizeof(FloatT) * rows() * columns());
      return array;
    }
    
    /**
     * 中身が密な配列に変換します。
     *
     * @return (self_t)
     */
    self_t dense() const {return self_t(*this);}
    
    /**
     * シャローコピーをします。
     * 参照カウンタのインクリメントも同時に行います。
     * 
     * @return (Array2D_Dense *)自分自身
     */
    root_t *shallow_copy() const{return new self_t(*this);}
    
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      assert((row < rows()) && (column < columns()));
      return *(m_buffer + (row * columns()) + column);
    }
    
    /**
     * v素のゼロクリアをします。
     * 
     */
    void clear(){
      for(unsigned int i(0); 
          i < rows() * columns(); 
          i++){*(m_buffer + i) = FloatT(0);}
    }
    
    /**
     * 代入演算子。
     * 
     * @return (self_t) 自分自身
     */
    self_t &operator=(const self_t &another){
      buffer_manager_t::operator=(another);
      super_t::m_rows = another.m_rows;
      super_t::m_columns = another.m_columns;
      return *this;
    }
};

/**
 * Delegated two-dimension array abstract class.
 * 
 * 別の2次元配列に委譲を行う2次元配列クラス
 * 
 */
template<class FloatT>
class Array2D_Delegate : public Array2D<FloatT>{
  private:
    typedef Array2D<FloatT> super_t;
    typedef Array2D<FloatT> root_t;
    typedef Array2D_Dense<FloatT> dense_t;
    typedef Array2D_Delegate<FloatT> self_t;
    
    root_t *m_target;
    
  protected:
    /**
     * 委譲元を返します。
     * 
     */
    root_t &getTarget() const{return *m_target;}
  
  public:
    using super_t::rows;
    using super_t::columns;
    
    /**
     * 委譲先を返します。
     * 
     * @return (root_t) 委譲先
     */
    const root_t *getParent() const{return m_target;}
    
    /**
     * Array2D_Delegate(委譲2次元配列)クラスのコンストラクタ。
     * 
     * @param rows 行数
     * @param columns 列数
     * @param array 元の配列
     */
    Array2D_Delegate(const unsigned int &rows,
        const unsigned int &columns,
        const root_t &array)
        : super_t(rows, columns), m_target(array.shallow_copy()){}
    
    /**
     * コピーコンストラクタ
     * 
     */
    Array2D_Delegate(const Array2D_Delegate &array)
        : super_t(array.rows(), array.columns()), 
        m_target(array.m_target->shallow_copy()){}
        
    /**
     * デストラクタ。
     */
    ~Array2D_Delegate(){delete m_target;}
        
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      return m_target->operator()(row, column);
    }
    
    /**
     * 中身が密な配列に変換します。
     *
     * @return (dense_t)
     */
    dense_t dense() const {
      dense_t array(rows(), columns());
      for(unsigned int i(0); i < array.rows(); i++){
        for(unsigned int j(0); j < array.columns(); j++){
          array(i, j) 
            = (const_cast<Array2D_Delegate *>(this))->operator()(i, j);
        }
      }
      return array;
    }
    
    /**
     * 部分2次元配列を複製(ディープコピー)します。
     * 複製後は単純な2次元配列になります。
     * 
     * @return (Array2D<FloatT>) コピー
     */
    root_t *copy() const{
      return dense().shallow_copy();
    }
    
    /**
     * 要素のゼロクリアを行います。
     * 
     */
    void clear(){
      for(unsigned int i(0); i < this->rows(); i++){
        for(unsigned int j(0); j < this->columns(); j++){
          this->operator()(i, j) = FloatT(0);
        }
      } 
    }
};

/**
 * Transposed two-dimension array class.
 * 
 * 転置2次元配列をあらわすクラス
 * 
 */
template<class FloatT>
class Array2D_Transpose : public Array2D_Delegate<FloatT>{
  
  public:
    /**
     * Array2D_Transpose(部分2次元配列)クラスのコンスト宴Nタ
     * 
     * @param array 元の配列
     */
    Array2D_Transpose(const Array2D<FloatT> &array)
        : Array2D_Delegate<FloatT>(array.columns(), array.rows(), array){}
    
    /**
     * コピーコンストラクタ
     * 
     */
    Array2D_Transpose(const Array2D_Transpose &array)
        : Array2D_Delegate<FloatT>(array.rows(), array.columns(), array){}
    
    /**
     * シャローコピーをします。
     * 
     * @return (Array2D *)自分自身
     */
    Array2D<FloatT> *shallow_copy() const{return new Array2D_Transpose(*this);}
        
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      return Array2D_Delegate<FloatT>::operator()(column, row);
    }
    
    /**
     * 中身が密な配列に変換します。
     *
     * @return (Array2D_Dense<FloatT>)
     */
    Array2D_Dense<FloatT> dense() const {
      return Array2D_Delegate<FloatT>::dense();
    }
};


/**
 * Partial two-dimension array class.
 * 
 * 部分2次元配列をあらわすクラス
 * 
 */
template<class FloatT>
class Array2D_Partial : public Array2D_Delegate<FloatT>{
  
  public:
    /**
     * 中身が密な配列に変換します。
     *
     * @return (Array2D_Dense<FloatT>)
     */
    Array2D_Dense<FloatT> dense() const {
      return Array2D_Delegate<FloatT>::dense();
    }
    
    /**
     * 部分2次元配列を複製(ディープコピー)します。
     * 複製後は単純な2次元配列になります。
     * 
     * @return (Array2D<FloatT>) コピー
     */
    Array2D<FloatT> *copy() const{
      return Array2D_Delegate<FloatT>::copy();
    } 
  
  private:
    unsigned int m_RowOffset;
    unsigned int m_ColumnOffset;
    
  protected:
    /**
     * オフセット行を返します。
     * 
     * @return (int) 行数
     */
    unsigned int row_offset() const{return m_RowOffset;}
    /**
     * オフセット列を返します。
     * 
     * @return (int) 列数
     */
    unsigned int column_offset() const{return m_ColumnOffset;}
  
  public:
    /**
     * Array2D_Partial(部分2次元配列)クラスのコンストラクタ。
     * 
     * @param array 元の配列
     * @param offsetRow 部分2次元配列で(0,0)となる元の2次元配列のピボットの行インデックス
     * @param offsetColumn 同じく列インデックス
     * @param rows 行数
     * @param columns 列数
     */
    Array2D_Partial(const unsigned int &rows,
        const unsigned int &columns,
        const Array2D<FloatT> &array,
        const unsigned int &rowOffset,
        const unsigned int &columnOffset)
        : Array2D_Delegate<FloatT>(rows, columns, array),
        m_RowOffset(rowOffset), m_ColumnOffset(columnOffset){}
    
    /**
     * コピーコンストラクタ
     * 
     */
    Array2D_Partial(const Array2D_Partial &array)
        : Array2D_Delegate<FloatT>(array),
        m_RowOffset(array.row_offset()), m_ColumnOffset(array.column_offset()){}
    
    /**
     * シャローコピーをします。
     * 
     * @return (Array2D *)自分自身
     */
    Array2D<FloatT> *shallow_copy() const{return new Array2D_Partial(*this);}
        
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      return Array2D_Delegate<FloatT>::operator()(
          row + row_offset(), column + column_offset());
    }
};

template <class FloatT>
class Matrix;

/**
 * @brief 委譲された行列
 *
 * 委譲された行列(部分行列、転置行列などの派生行列)の基底クラス
 * 元の行列クラスではライトウエイト実装(代入時、シャローコピーによる解決)であるため、
 * そのままでは派生クラスにおいて弊害が発生します。
 * 当該項目を解消するのが、このクラスの役割です。
 */
template <class T>
class DelegatedMatrix : public Matrix<T>{
  protected:
    typedef Matrix<T> super_t;
    typedef DelegatedMatrix<T> self_t;

    super_t &substitute(const super_t &matrix){
      if((this != &matrix) && (super_t::m_Storage)){
#define __MIN_MACRO(x, y) ((x) < (y) ? (x) : (y))
        for(unsigned int i(0); i < __MIN_MACRO(super_t::rows(), matrix.rows()); i++){
          for(unsigned int j(0); j < __MIN_MACRO(super_t::columns(), matrix.columns()); j++){
            (*this)(i, j) = (const_cast<super_t &>(matrix))(i, j);
          }
        }
#undef __MIN_MACRO
      }
      return *this;
    }

    DelegatedMatrix(const typename super_t::storage_t *storage)
        : super_t(storage){}
    virtual ~DelegatedMatrix(){}

    /**
     * 代入演算子。
     * 基底クラスのStorageを変更する操作と異なり、成分どおしの代入を行います。
     *
     * @param matrix 代入する行列
     */
    self_t &operator=(const super_t &matrix){
      return static_cast<self_t &>(self_t::substitute(matrix));
    }

    self_t &operator=(const self_t &matrix){
      return static_cast<self_t &>(self_t::substitute(matrix));
    }

  public:
    Matrix<T> original() const{
      return super_t::make_instance(
          static_cast<Array2D_Delegate<T> *>(Matrix<T>::m_Storage)
            ->getParent()->shallow_copy());
    }
};

/**
 * @brief 転置行列
 *
 * 転置行列をあらわすクラス。
 * クラス内クラスとして定義。
 * 転置2次元配列を表現したArray2D_Transposeと協力して転置行列を実現しています。
 *
 * @see Array2D_Transpose 転置2次元配列
 */
template <class T>
class TransposedMatrix : public DelegatedMatrix<T>{
  protected:
    typedef Matrix<T> root_t;
    typedef DelegatedMatrix<T> super_t;
    typedef TransposedMatrix<T> self_t;

  public:
    using root_t::rows;
    using root_t::columns;
    
    /**
     * TransposedMatrix(転置行列)クラスのコンストラクタ。
     *
     * @param matrix 元の行列
     */
    TransposedMatrix(const root_t &matrix)
        : super_t(new Array2D_Transpose<T>(*(matrix.storage()))){}

    /**
     * デストラクタ。
     */
    ~TransposedMatrix(){}

    /**
     * 転置行列を転置して元の行列に戻します。
     * 返却される行列はもとの行列とリンクしています。
     * もとの行列との切り離しを行うにはtranspose().copy()としてください。
     *
     * @return (root_t) 転置行列
     */
    inline root_t untranspose() const{
      return super_t::original();
    }

    self_t &operator=(const root_t &matrix){
      return static_cast<self_t &>(super_t::substitute(matrix));
    }
    
    /**
     * 行列を乗算します。(転置 * 非転置)
     *
     * @param matrix かける行列
     * @return (root_t) 結果
     */
    root_t operator*(const root_t &matrix) const{
      return super_t::operator*(matrix);
    }
    
    /**
     * 行列を乗算します。(転置 * 転置)
     *
     * @param matrix かける行列
     * @return (root_t) 結果
     * @throw MatrixException 行列の積算が成立しない場合(オペランド行列の列数が引数行列の行数と等しくない)
     */
    root_t operator*(const self_t &matrix) const{
      return operator*((const root_t &)matrix);
    }
};

/**
 * @brief 部分行列
 *
 * 部分行列をあらわすクラス
 * クラス内クラスとして定義。
 * 部分2次元配列を表現したArray2D_Partialと協力して部分行列を実現しています。
 *
 * @see Array2D_Partial 部分2次元配列
 */
template <class T>
class PartialMatrix : public DelegatedMatrix<T>{
  protected:
    typedef Matrix<T> root_t;
    typedef DelegatedMatrix<T> super_t;
    typedef PartialMatrix<T> self_t;

  public:
    /**
     * PartialMatrix(部分行列)クラスのコンストラクタ。
     *
     * @param matrix 元の行列
     * @param rows 行数
     * @param columns 列数
     * @param rowOffset 部分行列で(0,0)となる元の行列のピボットの行インデックス
     * @param columnOffset 同じく列インデックス
     */
    PartialMatrix(
        const root_t &matrix,
        const unsigned int &rows,
        const unsigned int &columns,
        const unsigned int &rowOffset,
        const unsigned int &columnOffset)
            : super_t(new Array2D_Partial<T>(
                rows, columns,
                *(matrix.storage()),
                rowOffset, columnOffset)){}

    /**
     * デストラクタ。
     */
    ~PartialMatrix(){}

    self_t &operator=(const root_t &matrix){
      return static_cast<self_t &>(super_t::substitute(matrix));
    }
};

/**
 * Matrix class.
 * 
 * 行列のクラス
 * 
 */
template <class FloatT = double>
class Matrix{
  public:
    typedef Matrix<FloatT> self_t;
    typedef Array2D<FloatT> storage_t;
      
  protected:
    storage_t *m_Storage;
    
    /**
     * Matrixクラスのコンストラクタ。
     * ストレージを指定して新しく行列を作ります。
     * 
     * @param array ストレージ
     */
    Matrix(const storage_t *storage) : m_Storage(const_cast<storage_t *>(storage)){}
    
    static self_t make_instance(const storage_t *storage){
      return self_t(storage);
    }
    
    /**
     * Matrixクラスを作成するヘルパ関数。
     * 指定の行数、指定の列数で行列を生成しますが、
     * 成分については初期化を行わないため不定です。
     *
     * @param rows 行数
     * @param columns 列数
     */
    static self_t naked(
        const unsigned int &rows,
        const unsigned int &columns){
      return Matrix(new Array2D_Dense<FloatT>(rows, columns));
    }
  
  public:
    /**
     * 内部的な保存形式を返します。
     * 
     * @return (const storage_t *) ストレージ
     */
    const storage_t *storage() const{return m_Storage;}

    /**
     * Matrixクラスのコンストラクタ。
     * 
     */
    Matrix() : m_Storage(NULL){}
    
    /**
     * Matrixクラスのコンストラクタ。
     * 指定の行数、指定の列数で行列を生成します。
     * また成分はすべてFloatT(0)で初期化されます。
     * 
     *     
     * @param rows 行数
     * @param columns 列数
     */
    Matrix(const unsigned int &rows, const unsigned int &columns)
        : m_Storage(new Array2D_Dense<FloatT>(rows, columns)){m_Storage->clear();}
    
    /**
     * Matrixクラスのコンストラクタ。
     * 指定の行数、指定の列数で行列を生成します。
     * また成分はseializedで復元されます。
     *
     * @param rows 行数
     * @param columns 列数
     * @param serialized 成分
     */
    Matrix(
        const unsigned int &rows, const unsigned int &columns,
        const FloatT *serialized)
        : m_Storage(new Array2D_Dense<FloatT>(rows, columns, serialized)){}
    
    /**
     * コピーコンストラクタ。
     * シャローコピーを生成します。
     * 
     * @param matrix コピー元
     */
    Matrix(const Matrix &matrix) : m_Storage(matrix.m_Storage->shallow_copy()){}
    /**
     * デストラクタ。
     */
    virtual ~Matrix(){delete m_Storage;}
    
  protected:
    /**
     * 代入演算子をサポートするための関数
     * 内部的にはシャローコピーを行っています。
     *
     * @return (self_t) 自分自身
     */
    virtual self_t &substitute(const self_t &matrix){
      if(this != &matrix){
        delete m_Storage;
        if(matrix.m_Storage){
          m_Storage = matrix.m_Storage->shallow_copy();
        }
      }
      return *this;
    }

  public:
    /**
     * 代入演算子。
     *
     * @return (self_t) 自分自身
     */
    self_t &operator=(const self_t &matrix){
      return substitute(matrix);
    }
    
    /**
     * 行列を複製(ディープコピー)します。
     * 
     * @return (self_t) コピー
     */
    self_t copy() const{
      return self_t(m_Storage->copy());
    }
    
    /**
     * 行数を返します。
     * 
     * @return (int) 行数
     */
    unsigned int rows() const{return m_Storage->rows();}
    /**
     * 列数を返します。
     * 
     * @return (int) 列数
     */
    unsigned int columns() const{return m_Storage->columns();}
    
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(const unsigned int &row, const unsigned int &column){
      return m_Storage->operator()(row, column);
    }
    /**
     * 指定した行列成分を返します。(Matlab風味)
     * 
     * @param row 行番号(開始番号は1～)
     * @param column 列番号(開始番号は1～)
     * @return (FloatT) 成分
     */
    FloatT &matlab(const int &row, const int &column){
      return (*this)(row - 1, column - 1);
    }
    
    /**
     * 要素をゼロクリアします
     * 
     */
    self_t clear(){
      m_Storage->clear();
      return *this;
    }
    
    /**
     * 指定のスカラー行列を生成します。
     *
     * @param size 指定の行数(列数)
     * @param scalar 値
     */
    static self_t getScalar(const unsigned int &size, const FloatT &scalar){
      self_t result(size, size);
      for(unsigned int i = 0; i < size; i++){result(i, i) = scalar;}
      return result;
    }
    
    /**
     * 指定の単位行列を生成します。
     * 
     * @parma size 指定行数(列数)
     */
    static self_t getI(const int &size){
      return getScalar(size, FloatT(1));
    }
    
    typedef TransposedMatrix<FloatT> transposed_t;
    
    /**
     * 行列を転置します。
     * 転置された行列はもとの行列とリンクしています。
     * もとの行列との切り離しを行うにはtranspose().copy()としてください。
     * 
     * @return (transposed_t) 転置行列
     */
    transposed_t transpose() const{
      return transposed_t(*this);
    }
    
    typedef PartialMatrix<FloatT> partial_t;
    
    /**
     * 指定した部分行列を返します。
     *
     * @param rowSize 行サイズ
     * @param columnSize 列サイズ
     * @param rowOffset 開始行インデックス
     * @param columnOffset 開始列インデックス
     * @return (PartialMatrix<T>) 部分行列
     */
    partial_t partial(
        const unsigned int &rowSize,
        const unsigned int &columnSize,
        const unsigned int &rowOffset,
        const unsigned int &columnOffset) const {
      assert((rowSize + rowOffset > rows()) 
          || (columnSize + columnOffset > columns()));
      return partial_t(*this, rowSize, columnSize, rowOffset, columnOffset);
    }
    
    /**
     * 指定した行の行ベクトルを返します。
     * 
     * @param row 行インデックス
     * @return (self_t) 行ベクトル
     */
    partial_t rowVector(const unsigned int &row) const{
      assert(row < rows());
      return partial_t(*this, 1, columns(), row, 0);
    }
    /**
     * 指定した列の列ベクトルを返します。
     * 
     * @param column 列インデックス
     * @return (partial_t) 列ベクトル
     */
    partial_t columnVector(const unsigned int &column) const{
      assert(column < columns());
      return partial_t(*this, rows(), 1, 0, column);
    }
    
    /**
     * 行を入れ替えます。破壊的メソッドです。
     * 
     * @param row1 行インデックス1
     * @param row2 行インデックス2
     * @return (self_t) 自分自身
     */
    self_t &exchangeRows(const unsigned int &row1, const unsigned int &row2){
      assert(row1 < rows() && row2 < rows());
      FloatT temp;
      for(unsigned int j = 0; j < columns(); j++){
        temp = (*this)(row1, j);
        (*this)(row1, j) = (*this)(row2, j);
        (*this)(row2, j) = temp;
      }
      return *this;
    }
    /**
     * 列を入れ替えます。破壊的メソッドです。
     * 
     * @param column1 列インデックス1
     * @param column2 列インデックス2
     * @return (self_t) 自分自身
     */
    self_t &exchangeColumns(const unsigned int &column1, const unsigned int &column2){
      assert(column1 < columns() && column2 < columns());
      FloatT temp;
      for(unsigned int i = 0; i < rows(); i++){
        temp = (*this)(i, column1);
        (*this)(i, column1) = (*this)(i, column2);
        (*this)(i, column2) = temp;
      }
      return *this;
    }
    
    /**
     * 正方行列かどうか調べます。
     * 
     * @return (bool) 正方行列である場合true、それ以外の場合false
     */
    bool isSquare() const{return rows() == columns();}
    
    /**
     * 対角行列かどうか調べます
     *
     * @return (bool) 対角行列である場合true、それ以外の場合false
     */
    bool isDiagonal() const{
      if(isSquare()){
        for(unsigned int i = 0; i < rows(); i++){
          for(unsigned int j = i + 1; j < columns(); j++){
            if((const_cast<Matrix &>(*this)(i, j) != FloatT(0))
                || (const_cast<Matrix &>(*this)(j, i) != FloatT(0))){
              return false;
            }
          }
        }
        return true;
      }else{return false;}
    }
    
    /**
     * 対称行列かどうか調べます。
     * 
     * @return (bool) 対称行列である場合true、それ以外の場合false
     */
    bool isSymmetric() const{
      if(isSquare()){
        for(unsigned int i = 0; i < rows(); i++){
          for(unsigned int j = i + 1; j < columns(); j++){
            if(const_cast<Matrix &>(*this)(i, j) != const_cast<Matrix &>(*this)(j, i)){
              return false;
            }
          } 
        }
        return true;
      }else{return false;}
    }
    
    /**
     * 行列の大きさが異なるか調べる
     *
     * @param matrix 比較対象
     * @return (bool) 異なっている場合true
     */
    bool isDifferentSize(const self_t &matrix) const{
      return (rows() != matrix.rows()) || (columns() != matrix.columns());
    }
    
    /**
     * 行列のトレースを返します。
     *
     * @param do_check 正方行列かを調べる、デフォルトtrue
     * @return (T) トレース
     */
    FloatT trace(bool do_check = true) const {
      assert(do_check && !isSquare());
      FloatT tr(0);
      for(unsigned i(0); i < rows(); i++){
        tr += (*const_cast<self_t *>(this))(i, i);
      }
      return tr;
    }
    
    /**
     * 行列の成分全てを指定倍します。破壊的メソッドです。
     * 
     * @param scalar 倍数
     * @return (self_t) 自分自身
     */
    self_t &operator*=(const FloatT &scalar){
      for(unsigned int i = 0; i < rows(); i++){
        for(unsigned int j = 0; j < columns(); j++){
          (*this)(i, j) *= scalar;
        }
      }
      return *this;
    }
    /**
     * 行列の成分全てを指定倍します。
     * 
     * @param scalar 倍数
     * @return (self_t) 結果
     */
    self_t operator*(const FloatT &scalar) const{return (copy() *= scalar);}
    /**
     * 行列の成分全てを指定倍します。
     * 
     * @param scalar 倍数
     * @return (self_t) 結果
     */
    friend self_t operator*(const FloatT &scalar, const self_t &matrix){return matrix * scalar;}
    /**
     * 行列の成分全てを除算します。破壊的メソッドです。
     * 
     * @param scalar 倍数
     * @return (self_t) 自分自身
     */
    self_t &operator/=(const FloatT &scalar){return (*this) *= (1 / scalar);}
    /**
     * 行列の成分全てを除算します。
     * 
     * @param scalar 倍数
     * @return (self_t) 結果
     */
    self_t operator/(const FloatT &scalar) const{return (copy() /= scalar);}
    /**
     * 行列の成分全てを除算します。
     * 
     * @param scalar 倍数
     * @return (self_t) 結果
     */
    friend self_t operator/(const FloatT &scalar, const self_t &matrix){return matrix / scalar;}
    
    /**
     * 行列を成分ごとに加算します。破壊的メソッドです。
     * 
     * @param matrix 加える行列
     * @return (self_t) 自分自身
     */
    self_t &operator+=(const self_t &matrix){
      assert(rows() == matrix.rows() && columns() == matrix.columns());
      for(unsigned int i = 0; i < rows(); i++){
        for(unsigned int j = 0; j < columns(); j++){
          (*this)(i, j) += const_cast<self_t &>(matrix)(i, j);
        }
      }
      return *this;
    }

    /**
     * 行列を成分ごとに加算します。
     * 
     * @param matrix 加える行列
     * @return (self_t) 結果
     */
    self_t operator+(const self_t &matrix) const{return (copy() += matrix);}
    
    /**
     * 行列を成分ごとに減算します。
     * 
     * @param matrix 引く行列
     * @return (self_t) 自分自身
     */
    self_t &operator-=(const self_t &matrix){
      assert(rows() == matrix.rows() && columns() == matrix.columns());
      for(unsigned int i = 0; i < rows(); i++){
        for(unsigned int j = 0; j < columns(); j++){
          (*this)(i, j) -= const_cast<self_t &>(matrix)(i, j);
        }
      }
      return *this;
    }
    
    /**
     * 行列を成分ごとに減算します。
     * 
     * @param matrix 引く行列
     * @return (self_t) 結果
     */
    self_t operator-(const self_t &matrix) const{return (copy() -= matrix);}
    
    /**
     * 行列を乗算します。
     * 
     * @param matrix ゥける行列
     * @return (self_t) 結果
     */
    self_t operator*(const self_t &matrix) const{
      assert(columns() == matrix.rows());
      self_t result(self_t::naked(rows(), matrix.columns())); 
      for(unsigned int i = 0; i < result.rows(); i++){
        for(unsigned int j = 0; j < result.columns(); j++){
          for(unsigned int k = 0; k < columns(); k++){
            result(i, j) += (*const_cast<self_t *>(this))(i, k) * (*const_cast<self_t *>(&matrix))(k, j);
          }
        }
      }
      return result;
    }
    
    /**
     * 行列を乗算します。(転置行列バージョン)
     *
     * @param matrix かける行列
     * @return (self_t) 結果
     */
    self_t operator*(const transposed_t &matrix) const{
      return operator*((const self_t &)matrix);
    }
    
    /**
     * 行列を乗算します。破壊的メソッドです。
     *
     * @param matrix かける行列
     * @return (self_t) 自分自身
     */
    template <class RhsMatrix>
    self_t &operator*=(const RhsMatrix &matrix){
      return (*this = (*this * matrix));
    }
    
    /**
     * 単項演算子-。
     * 効果は matrix * -1と同じです。
     * 
     * @return (self_t) -matrix
     */
    self_t operator-() const{return (copy() *= -1);}
    
    /**
     * 補行列(余因子行列)を求めます。
     * 
     * @param row 行インデックス
     * @param column 列インデックス
     * @return (self_t) 補行列
     */
    self_t coMatrix(const unsigned int &row, const unsigned int &column) const{
      assert(row < rows() && column < columns());
      self_t result(rows() - 1, columns() - 1);
      for(int i = 0; i < rows() - 1; i++){
        for(int j = 0; j < columns() - 1; j++){
          result(i, j) = (*const_cast<self_t *>(this))((i < row ? i : i + 1), (j < column ? j : j + 1));
        }  
      }
      return result;
    }
    
    /**
     * 行列式を計算します。
     * 
     * @return (FloatT) 結果
     */
    FloatT determinant(bool do_check = false) const{
      assert((!do_check) || isSquare());
      if(rows() == 1){
        return (*const_cast<self_t *>(this))(0, 0);
      }else{
        FloatT sum(0);
        for(int i = 0; i < rows(); i++){
          if((*const_cast<self_t *>(this))(i, 0) != 0){
            sum += (*const_cast<self_t *>(this))(i, 0) * (coMatrix(i, 0).determinant()) * (i % 2 == 0 ? 1 : -1);
          }
        }
        return sum;
      }
    }
    
    /**
     * LU分解をします。
     * (0, 0)～(n-1, n-1):  L行列
     * (0, n)～(n-1, 2n-1): U行列
     * 
     * @return (self_t) LU分解
     */
    self_t decomposeLU(bool do_check = false) const{
      assert((!do_check) || isSquare());
      unsigned int size(rows());
      self_t LU(self_t::naked(size, size * 2));
#define L(i, j) LU(i, j)
#define U(i, j) LU(i, j + size)
      for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
          if(i >= j){
            L(i, j) = (*const_cast<Matrix *>(this))(i, j);
            for(int k = 0; k < j; k++){
              L(i, j) -= (L(i, k) * U(k, j));
            }
          }else{
            U(i, j) = (*const_cast<Matrix *>(this))(i, j);
            for(int k = 0; k < i; k++){
              U(i, j) -= (L(i, k) * U(k, j));
            }
            U(i, j) /= L(i, i);
          }
        }
        U(i, i) = 1;
      }
#undef L
#undef U
      return LU;
    }
     
    /**
     * UD分解をします。
     * (0, 0)～(n-1,n-1):  U行列
     * (0, n)～(n-1,2n-1): D行列
     * 
     * @return (self_t) UD分解
     */
    self_t decomposeUD(bool do_check = false) const{
      assert((!do_check) || isSymmetric());
      unsigned int size(rows());
      self_t P(copy());
      self_t UD(size, size * 2);
#define U(i, j) UD(i, j)
#define D(i, j) UD(i, j + size)
      for(int i = size - 1; i >= 0; i--){
        D(i, i) = P(i, i);
        U(i, i) = FloatT(1);      
        for(int j = 0; j < i; j++){
          U(j, i) = P(j, i) / D(i, i);
          for(int k = 0; k <= j; k++){
            P(k, j) -= U(k, i) * D(i, i) * U(j, i);
          }
        }
      }
#undef U
#undef D
      return UD;
    }
    
    /**
     * 逆行列を求めます。
     * 
     * @return (self_t) 逆行列
     */
    self_t inverse(bool do_check = false) const{
      assert((!do_check) || isSquare());
      
      unsigned int size(rows());
      
      //ガウス消去法
      self_t left(copy());
      self_t right(self_t::getI(size));
      for(unsigned int i = 0; i < size; i++){
        if(left(i, i) == FloatT(0)){ //(i, i)が存在するように並べ替え
          for(unsigned int j = i+1; j <= size; j++){
            assert(j != size);
            if(left(j, i) != FloatT(0)){
              left.exchangeRows(j, i);
              right.exchangeRows(j, i);
              break;
            }
          }
        }
        if(left(i, i) != FloatT(1)){
          for(unsigned int j = 0; j < size; j++){right(i, j) /= left(i, i);}
          for(unsigned int j = i+1; j < size; j++){left(i, j) /= left(i, i);}
          left(i, i) = FloatT(1);
        }
        for(unsigned int k = 0; k < size; k++){
          if(k == i){continue;}
          if(left(k, i) != FloatT(0)){
            for(unsigned int j = 0; j < size; j++){right(k, j) -= right(i, j) * left(k, i);}
            for(unsigned int j = i+1; j < size; j++){left(k, j) -= left(i, j) * left(k, i);}
            left(k, i) = FloatT(0);
          }
        }
      }
      //cout << "L:" << left << endl;
      //cout << "R:" << right << endl;
      
      return right;
      
      //LU分解
      /*
      */
    }
    /**
     * 逆行列をかけます。破壊的メソッドです。
     * 
     * @param matrix 行列
     * @return (self_t) 自分自身
     */
    self_t &operator/=(const self_t &matrix){return (*this) *= matrix.inverse();}
    /**
     * 逆行列をかけます。
     * 
     * @param matrix 行列
     * @return (self_t) 結果
     */
    self_t operator/(const self_t &matrix) const{return (copy() /= matrix);}
    
    /**
     * ピボットを指定して、加算します。
     * 破壊的です。
     * 
     * @param row 行インデックス
     * @param column 列インデックス
     * @param matrix 足す行列
     */
    self_t pivotMerge(const int &row, const int &column, const self_t &matrix){
      for(int i = 0; i < matrix.rows(); i++){
        if(row + i < 0){continue;}
        else if(row + i >= rows()){break;}
        for(int j = 0; j < matrix.columns(); j++){
          if(column + j < 0){continue;}
          else if(column + j >= columns()){break;}
          (*this)(row + i, column + j) += (*const_cast<self_t *>(&matrix))(i, j);
        }
      }
      return *this;
    }
    
    /**
     * ピボットを指定して、加算します。
     * 
     * @param row 行インデックス
     * @param column 列インデックス
     * @param matrix 足す行列
     */
    self_t pivotAdd(const int &row, const int &column, const self_t &matrix) const{
      return copy().pivotMerge(row, column, matrix);
    }
    
    /**
     * 行列を見やすい形で出力します。
     * 
     */
    void inspect(char *buffer, int buffer_size) const{
      using std::snprintf;
      if(m_Storage){
        int printed;
        printed = snprintf(buffer, buffer_size, "{");
        buffer += printed;
        buffer_size -= printed;
        for(unsigned int i = 0; i < rows(); i++){
          printed = snprintf(buffer, buffer_size, (i == 0 ? "\n{" : ",\n{"));
          buffer += printed;
          buffer_size -= printed;
          for(unsigned int j = 0; j < columns(); j++){
            /*printed = printf((j == 0 ? "%f" : ",%f"), 
                               const_cast<self_t *>(this)->operator()(i, j));*/
            printed = snprintf(buffer, buffer_size, (j == 0 ? "%f" : ",%f"), 
                               const_cast<self_t *>(this)->operator()(i, j));
            buffer += printed;
            buffer_size -= printed;
          }
          printed = snprintf(buffer, buffer_size, "}");
          buffer += printed;
          buffer_size -= printed;
        }
        snprintf(buffer, buffer_size, "\n}");
      }
    }
};

#if defined(DSPF_DP_MAT_MUL_H_) || defined(DSPF_SP_MAT_MUL_ASM_H_)
/**
 * アライメントされた密2次元配列をあらわすクラス
 * 
 */
template<class FloatT>
class Array2D_DenseAligned 
    : public Array2D<FloatT>, public Array2D_BufferManager<FloatT> {
  protected:
    unsigned int m_buffer_rows;
    unsigned int m_buffer_columns;
  
  protected:
    typedef Array2D<FloatT> super_t;
    typedef Array2D<FloatT> root_t;
    typedef Array2D_DenseAligned<FloatT> self_t;
    typedef Array2D_BufferManager<FloatT> buffer_manager_t;
    
  public:
    using buffer_manager_t::m_buffer;
    
    unsigned int buffer_rows() const {
      return m_buffer_rows;
    }
    unsigned int buffer_columns() const {
      return m_buffer_columns;
    }
    
    static unsigned int aligned_rows(
        const unsigned int &_rows,
        const unsigned int &_columns){
      return ((_rows == 1) || (_columns == 1) || (_rows % 2 == 0))
          ? _rows
          : (_rows + 1);
    }
    
    static unsigned int aligned_columns(
        const unsigned int &_rows,
        const unsigned int &_columns){
      return ((_rows == 1) || (_columns == 1) || (_columns % 2 == 0))
          ? _columns
          : (_columns + 1);
    }
    
  protected:
    /**
     * Array2D_DenseAligned(アライメントされた2次元配列)クラスのコンストラクタ。
     * 
     * @param _rows 行数
     * @param _columns 列数
     */
    Array2D_DenseAligned(
        const unsigned int &_rows,
        const unsigned int &_columns)
        : super_t(_rows, _columns),
        buffer_manager_t(
            new FloatT[aligned_rows(_rows, _columns) * aligned_columns(_rows, _columns)]),
        m_buffer_rows(aligned_rows(_rows, _columns)),
        m_buffer_columns(aligned_columns(_rows, _columns)){
          
    }
    
    /**
     * Array2D_DenseAligned(アライメントされた2次元配列)クラスのコンストラクタ。
     * 成分はseializedで復元されます。
     * 
     * @param _rows 行数
     * @param _columns 列数
     * @param serialized 成分
     */
    Array2D_DenseAligned(
        const unsigned int &_rows,
        const unsigned int &_columns,
        const FloatT *serialized)
        : super_t(_rows, _columns),
        buffer_manager_t(
            new FloatT[aligned_rows(_rows, _columns) * aligned_columns(_rows, _columns)]),
        m_buffer_rows(aligned_rows(_rows, _columns)),
        m_buffer_columns(aligned_columns(_rows, _columns)){
      
      FloatT *src(const_cast<FloatT *>(serialized));
      FloatT *dist(m_buffer);
      for(unsigned int i = 0; i < _rows; i++){
        memcpy(dist, src, sizeof(FloatT) * _columns);
        src += _columns;
        dist += m_buffer_columns;
      }
    }
  
  public:
    /**
     * コピーコンストラクタ
     * 
     */
    Array2D_DenseAligned(const self_t &orig)
        : super_t(orig),
        buffer_manager_t(orig),
        m_buffer_rows(orig.m_buffer_rows),
        m_buffer_columns(orig.m_buffer_columns){}
    
    /**
     * デストラクタ。
     */
    ~Array2D_DenseAligned(){}
    
    using super_t::rows;
    using super_t::columns;
    
  protected:
    /**
     * 複製(ディープコピー)のヘルパ
     * 
     * @param dist コピー先
     * @return (root_t) コピー
     */
    root_t *copy_helper(self_t *dist) const {
      memcpy(dist->buffer(), m_buffer, 
          sizeof(FloatT) * m_buffer_rows * m_buffer_columns);
      return dist;
    }
    
  public:
    /**
     * 指定した行列成分を返します。
     * 
     * @param row 行インデックス(開始番号は0～)
     * @param column 列インデックス(開始番号は0～)
     * @return (FloatT) 成分
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      assert((row < rows()) && (column < columns()));
      return *(m_buffer + (row * m_buffer_columns) + column);
    }
    
    /**
     * 要素のゼロクリアをします。
     * 
     */
    void clear(){
      for(unsigned int i(0); 
          i < m_buffer_rows * m_buffer_columns; 
          i++){*(m_buffer + i) = FloatT(0);}
    }
    
    /**
     * 代入演算子。
     * 
     * @return (self_t) 自分自身
     */
    self_t &operator=(const self_t &another){
      buffer_manager_t::operator=(another);
      m_buffer_rows = another.m_buffer_rows;
      m_buffer_columns = another.m_buffer_columns;
      return *this;
    }
};

#if defined(DSPF_DP_BLK_MOVE_H_) || defined(DSPF_SP_BLK_MOVE_ASM_H_)

/*
 * 初期化
 */
#define MAKE_SPECIALIZED(type, prefix) \
template<> \
Array2D_DenseAligned<type >::Array2D_DenseAligned( \
    const unsigned int &_rows, const unsigned int &_columns, \
    const type *serialized) \
    : super_t(_rows, _columns), \
    buffer_manager_t( \
        new type[aligned_rows(_rows, _columns) * aligned_columns(_rows, _columns)]), \
    m_buffer_rows(aligned_rows(_rows, _columns)), \
    m_buffer_columns(aligned_columns(_rows, _columns)){ \
  \
  type *src(const_cast<type *>(serialized)); \
  type *dist(m_buffer); \
  if((_rows == 1) || (_columns == 1)){ \
    unsigned int move_size(_rows * _columns); \
    if(move_size % 2 == 1){ \
      move_size--; \
      dist[move_size] = src[move_size]; \
    } \
    DSPF_ ## prefix ## _blk_move(src, dist, move_size); \
  }else if(_columns % 2 == 0){ \
    DSPF_ ## prefix ## _blk_move(src, dist, _rows * _columns); \
  }else{ \
    unsigned int column_step(_columns - 1); \
    for(unsigned int i = 0; i < _rows; i++){ \
      DSPF_ ## prefix ## _blk_move(src, dist, column_step); \
      src += column_step; \
      dist[column_step] = *(src++); \
      dist += m_buffer_columns; \
    } \
  } \
} \
template <> \
Array2D<type > *Array2D_DenseAligned<type >::copy_helper( \
    Array2D_DenseAligned<type > *dist) const { \
  unsigned int move_size(m_buffer_rows * m_buffer_columns); \
  if(move_size % 2 == 1){ \
    move_size--; \
    (dist->m_buffer)[move_size] = m_buffer[move_size]; \
  } \
  DSPF_ ## prefix ## _blk_move(m_buffer, dist->m_buffer, move_size); \
  return dist; \
}

template<>
Array2D_DenseAligned<double>::Array2D_DenseAligned(
    const unsigned int &_rows, const unsigned int &_columns,
    const double *serialized)
    : super_t(_rows, _columns),
    buffer_manager_t(
        new double[aligned_rows(_rows, _columns) * aligned_columns(_rows, _columns)]),
    m_buffer_rows(aligned_rows(_rows, _columns)),
    m_buffer_columns(aligned_columns(_rows, _columns)){
  
  double *src(const_cast<double *>(serialized));
  double *dist(m_buffer);
  if((_rows == 1) || (_columns == 1)){
    unsigned int move_size(_rows * _columns);
    if(move_size % 2 == 1){
      move_size--;
      dist[move_size] = src[move_size];
    }
    DSPF_dp_blk_move(src, dist, move_size);
  }else if(_columns % 2 == 0){
    DSPF_dp_blk_move(src, dist, _rows * _columns);
  }else{
    unsigned int column_step(_columns - 1);
    for(unsigned int i = 0; i < _rows; i++){
      DSPF_dp_blk_move(src, dist, column_step);
      src += column_step;
      dist[column_step] = *(src++);
      dist += m_buffer_columns;
    }
  }
}
template <>
Array2D<double> *Array2D_DenseAligned<double>::copy_helper(
    Array2D_DenseAligned<double> *dist) const {
  unsigned int move_size(m_buffer_rows * m_buffer_columns);
  if(move_size % 2 == 1){
    move_size--;
    (dist->m_buffer)[move_size] = m_buffer[move_size];
  }
  DSPF_dp_blk_move(m_buffer, dist->m_buffer, move_size);
  return dist;
}
    
#if defined(DSPF_DP_BLK_MOVE_H_)
//MAKE_SPECIALIZED(double, dp)
#endif
#if defined(DSPF_SP_BLK_MOVE_ASM_H_)
MAKE_SPECIALIZED(float, sp)
#endif

#undef MAKE_SPECIALIZED

#endif

/**
 * アライメントされた密2次元配列をあらわすクラス
 * 
 */
#define MAKE_SPECIALIZED(type) \
template<> \
class Array2D_Dense<type > \
    : public Array2D_DenseAligned<type > { \
  protected: \
    typedef Array2D_DenseAligned<type > super_t; \
    typedef Array2D<type > root_t; \
    typedef Array2D_Dense<type > self_t; \
    \
  public: \
    Array2D_Dense( \
        const unsigned int &_rows, \
        const unsigned int &_columns) \
        : super_t(_rows, _columns){} \
    \
    Array2D_Dense( \
        const unsigned int &_rows, \
        const unsigned int &_columns, \
        const type *serialized) \
        : super_t(_rows, _columns, serialized){} \
    \
    Array2D_Dense(const self_t &orig) \
        : super_t(orig) {} \
    \
    ~Array2D_Dense(){} \
    \
    /**
     * 複製(ディープコピー)します。
     * 
     * @return (root_t) コピー
     */ \
    root_t *copy() const { \
      return copy_helper(new self_t(rows(), columns())); \
    } \
    \
    /**
     * 中身が密な配列に変換します。
     *
     * @return (Array2D_Dense<FloatT>)
     */ \
    self_t dense() const {return self_t(*this);} \
    \
    /**
     * シャローコピーをします。
     * 
     * @return (Array2D *)自分自身
     */ \
    root_t *shallow_copy() const{return new self_t(*this);} \
    \
    /**
     * 代入演算子。
     * 
     * @return (self_t) 自分自身
     */ \
    self_t &operator=(const self_t &another){ \
      super_t::operator=(another); \
      return *this; \
    } \
};

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED(float)
#endif

#undef MAKE_SPECIALIZED

/*
 * 関数用の特殊化の雛形
 */
#if 0
#define MAKE_SPECIALIZED(type) \
template <> \
Matrix<type > Matrix<type >::func() const{ \
  Matrix<type > result(Matrix<type >::naked(rows(), columns())); \
    Array2D_Dense<type > x(storage()->dense()); \
    Array2D_Dense<type > r(result.storage()->dense()); \
    \
    return result; \
}

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED(float)
#endif

#undef MAKE_SPECIALIZED
#endif

/*
 * 行列の成分全てを指定乗除(unrolled version)
 */
#define MAKE_SPECIALIZED(type) \
template <> \
Matrix<type > Matrix<type >::operator*(const type &scalar) const{ \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  type *x_buf(x.buffer()); \
  type *r_buf(r->buffer()); \
  unsigned int elements(x.buffer_rows() * x.buffer_columns()); \
  for(unsigned int i(elements / 2); i > 0; i--){ \
    *(r_buf++) = *(x_buf++) * scalar; \
    *(r_buf++) = *(x_buf++) * scalar; \
  } \
  if(elements % 2 != 0){*r_buf = *x_buf * scalar;} \
  return Matrix<type >(r); \
} \
template<> \
Matrix<type > Matrix<type >::operator/(const type &scalar) const{ \
  return (*this) * (1. / scalar); \
}

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED(float)
#endif

#undef MAKE_SPECIALIZED

/*
 * 行列の成分ごとに加減算(unrolled version)
 */
#define MAKE_SPECIALIZED(type) \
template <> \
Matrix<type > Matrix<type >::operator+(const Matrix<type > &matrix) const{ \
  assert((rows() == matrix.rows()) && (columns() == matrix.columns())); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  Array2D_Dense<type > y(matrix.storage()->dense()); \
  type *x_buf(x.buffer()); \
  type *y_buf(y.buffer()); \
  type *r_buf(r->buffer()); \
  unsigned int elements(x.buffer_rows() * x.buffer_columns()); \
  for(unsigned int i(elements / 2); i > 0; i--){ \
    *(r_buf++) = *(x_buf++) + *(y_buf++); \
    *(r_buf++) = *(x_buf++) + *(y_buf++); \
  } \
  if(elements % 2 != 0){*r_buf = (*x_buf) + (*y_buf);} \
  return Matrix<type >(r); \
} \
template<> \
Matrix<type > Matrix<type >::operator-(const Matrix<type > &matrix) const{ \
  assert((rows() == matrix.rows()) && (columns() == matrix.columns())); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  Array2D_Dense<type > y(matrix.storage()->dense()); \
  type *x_buf(x.buffer()); \
  type *y_buf(y.buffer()); \
  type *r_buf(r->buffer()); \
  unsigned int elements(x.buffer_rows() * x.buffer_columns()); \
  for(unsigned int i(elements / 2); i > 0; i--){ \
    *(r_buf++) = *(x_buf++) - *(y_buf++); \
    *(r_buf++) = *(x_buf++) - *(y_buf++); \
  } \
  if(elements % 2 != 0){*r_buf = (*x_buf) - (*y_buf);} \
  return Matrix<type >(r); \
}

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED(float)
#endif

#undef MAKE_SPECIALIZED

template <class FloatT>
void mat_mul(FloatT *x, const int r1, const int c1, 
    FloatT *y, const int c2, 
    FloatT *r,
    bool x_trans = false, bool y_trans = false){
  int indx_c, indy_c; // 列方向への移動
  int indx_r, indy_r; // 行方向への移動
  if(x_trans){
    indx_c = r1;
    indx_r = 1;
  }else{
    indx_c = 1;
    indx_r = c1;
  }
  if(y_trans){
    indy_c = c1;
    indy_r = 1;
  }else{
    indy_c = 1;
    indy_r = c2;
  }
  int indx, indy, indr(0);
  for(unsigned int i(0); i < r1; i++){
    for(unsigned int j(0); j < c2; j++){
      indx = i * indx_r;
      indy = j * indy_c;
      r[indr] = FloatT(0);
      for(unsigned int k(0); k < c1; k++){
        r[indr] += x[indx] * y[indy];
        indx += indx_c;
        indy += indy_r;
      }
      indr++;
    }
  }
}

template <class FloatT>
void mat_mul_unrolled(FloatT *x, const int r1, const int c1, 
    FloatT *y, const int c2, 
    FloatT *r,
    bool x_trans = false, bool y_trans = false){
  if((r1 == 1) || (c1 == 1) || (c2 == 1)){
    return mat_mul(x, r1, c1, y, c2, r, x_trans, y_trans);
  }
  int indx_c, indy_c; // 列方向への移動
  int indx_r, indy_r; // 行方向への移動
  if(x_trans){
    indx_c = r1;
    indx_r = 1;
  }else{
    indx_c = 1;
    indx_r = c1;
  }
  if(y_trans){
    indy_c = c1;
    indy_r = 1;
  }else{
    indy_c = 1;
    indy_r = c2;
  }
  int indx, indy, indr(0);
  // ループ展開バージョン
  for(int i(0); i < r1; i += 2){
    for(int j(0); j < c2; j += 2){
      indx = i * indx_r;
      indy = j * indy_c;
      FloatT sum00(0), sum01(0), sum10(0), sum11(0);
      for(int k(c1); k > 0; k -= 2){
        sum00 += x[indx]                   * y[indy];
        sum01 += x[indx]                   * y[indy + indy_c];
        sum00 += x[indx + indx_c]          * y[indy + indy_r];
        sum01 += x[indx + indx_c]          * y[indy + indy_c + indy_r];
        sum10 += x[indx + indx_r]          * y[indy];
        sum11 += x[indx + indx_r]          * y[indy + indy_c];
        sum10 += x[indx + indx_c + indx_r] * y[indy + indy_r];
        sum11 += x[indx + indx_c + indx_r] * y[indy + indy_c + indy_r];
        indx += (indx_c * 2);
        indy += (indy_r * 2);
      }
      r[indr] = sum00;
      r[indr + 1] = sum01;
      r[indr + c2] = sum10;
      r[indr + c2 + 1] = sum11;
      indr += 2;
    }
    indr += c2;
  }
}

/*
 * 行列の乗算(非転置 * 非転置)
 * 
 */
#define MAKE_SPECIALIZED1(type, prefix) \
template<> \
Matrix<type > Matrix<type >::operator*(const Matrix<type > &matrix) const{ \
  assert(columns() == matrix.rows()); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), matrix.columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  Array2D_Dense<type > y(matrix.storage()->dense()); \
  \
  if((rows() == 1) || (columns() == 1) || (matrix.columns() == 1)){ \
    mat_mul(x.buffer(), x.buffer_rows(), x.buffer_columns(), \
        y.buffer(), y.buffer_columns(), \
        r->buffer()); \
  }else{ \
    /*LOG_printf(&trace, "%d, parent: %d", rows(), x.rows());
    LOG_printf(&trace, "%d, parent: %d", columns(), x.columns());
    LOG_printf(&trace, "%d, parent: %d", matrix.columns(), y.columns());
    if((rows() > x.rows()) || (columns() > x.columns()) || (matrix.columns() > y.columns())){
      LOG_printf(&trace, "error!!");
    }*/ \
    \
    DSPF_ ## prefix ## _mat_mul(x.buffer(), x.buffer_rows(), x.buffer_columns(), \
        y.buffer(), y.buffer_columns(), \
        r->buffer()); \
  } \
  return Matrix<type >(r); \
}
#define MAKE_SPECIALIZED2(type) \
template<> \
Matrix<type > Matrix<type >::operator*( \
    const Matrix<type > &matrix) const { \
  assert(this->columns() == matrix.rows()); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), matrix.columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  Array2D_Dense<type > y(matrix.storage()->dense()); \
  \
  mat_mul_unrolled(x.buffer(), x.buffer_rows(), x.buffer_columns(), \
      y.buffer(), y.buffer_columns(), \
      r->buffer(), \
      false, false); \
  \
  return Matrix<type >(r); \
}

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED1(double, dp)
//MAKE_SPECIALIZED2(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED1(float, sp)
//MAKE_SPECIALIZED2(float)
#endif

#undef MAKE_SPECIALIZED1
#undef MAKE_SPECIALIZED2

/*
 * 行列の乗算(非転置 * 転置, 転置 * 非転置, 転置 * 転置)
 */
#define MAKE_SPECIALIZED(type) \
template<> \
Matrix<type > Matrix<type >::operator*( \
    const TransposedMatrix<type > &matrix) const { \
  assert(this->columns() == matrix.rows()); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), matrix.columns())); \
  Array2D_Dense<type > x(storage()->dense()); \
  Array2D_Dense<type > y(matrix.untranspose().storage()->dense()); \
  \
  mat_mul_unrolled(x.buffer(), x.buffer_rows(), x.buffer_columns(), \
      y.buffer(), y.buffer_rows(), \
      r->buffer(), \
      false, true); \
  \
  return Matrix<type >(r); \
} \
template<> \
Matrix<type > TransposedMatrix<type >::operator*( \
    const Matrix<type > &matrix) const { \
  assert(columns() == matrix.rows()); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), matrix.columns())); \
  Array2D_Dense<type > x(untranspose().storage()->dense()); \
  Array2D_Dense<type > y(matrix.storage()->dense()); \
  \
  mat_mul_unrolled(x.buffer(), x.buffer_columns(), x.buffer_rows(), \
      y.buffer(), y.buffer_columns(), \
      r->buffer(), \
      true, false); \
  \
  return Matrix<type >::make_instance(r); \
} \
template<> \
Matrix<type > TransposedMatrix<type >::operator*( \
    const TransposedMatrix<type > &matrix) const { \
  assert(columns() == matrix.rows()); \
  Array2D_Dense<type > *r( \
      new Array2D_Dense<type >(rows(), matrix.columns())); \
  Array2D_Dense<type > x(untranspose().storage()->dense()); \
  Array2D_Dense<type > y(matrix.untranspose().storage()->dense()); \
  \
  mat_mul_unrolled(x.buffer(), x.buffer_columns(), x.buffer_rows(), \
      y.buffer(), y.buffer_rows(), \
      r->buffer(), \
      true, true); \
  \
  return Matrix<type >::make_instance(r); \
}

#if defined(DSPF_DP_MAT_MUL_H_)
MAKE_SPECIALIZED(double)
#endif
#if defined(DSPF_SP_MAT_MUL_ASM_H_)
MAKE_SPECIALIZED(float)
#endif

#undef MAKE_SPECIALIZED

#endif


#if defined(DSPF_DP_MAT_TRANS_H_) || defined(DSPF_SP_MAT_MUL_ASM_H_)

/*
 * 転置
 */
#define MAKE_SPECIALIZED(type, prefix) \
template<> \
Array2D_Dense<type > Array2D_Transpose<type >::dense() const { \
  Array2D_Dense<type > before_transposed( \
      Array2D_Delegate<type >::getTarget().dense()); \
  Array2D_Dense<type > transposed( \
      before_transposed.columns(), \
      before_transposed.rows()); \
  DSPF_ ## prefix ## _mat_trans(before_transposed.buffer(), \
      before_transposed.buffer_rows(), before_transposed.buffer_columns(), \
      transposed.buffer()); \
  return transposed; \
}

#if defined(DSPF_DP_MAT_TRANS_H_)
MAKE_SPECIALIZED(double, dp)
#endif
#if defined(DSPF_SP_MAT_TRANS_ASM_H_)
MAKE_SPECIALIZED(float, sp)
#endif

#undef MAKE_SPECIALIZED

#endif

#endif /* __MATRIX_H */
