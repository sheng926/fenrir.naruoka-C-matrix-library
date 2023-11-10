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
 * �����TI DSP 67x�V���[�Y�����̍s�񃉃C�u�����ł��B
 * �Ⴆ�΍s��ς�*�ŕ\�������ȂǁA
 * �s�񉉎Z�̑�����C++����̓����ł���I�y���[�^�I�[�o�[���[�h���g����
 * ��������Ă��܂��B
 * �܂��������̊Ǘ��ɂ��Ă����t�@�����X�J�E���^�[���g���Ă���̂ŁA
 * �ʏ�̎g�p�ł̓��������[�N���C�ɂ���K�v�͂���܂���B
 * �����I�Ɏg����ƐM���Ă��܂�(��)
 * 
 * �܂����̃��C�u������"TMS320C67x DSP Library (dsp67x.lib)"���g����
 * �s��ςȂǂɂ��Ă��Ȃ荂�������邱�Ƃ��\�ł��B
 * ���̃R�����g�A�E�g���ꂽ�w�b�_�t�@�C���̃C���N���[�h�A�Ȃ�т�dsp67x.lib��
 * �����N���邱�Ƃł��̃I�v�V�����͗L���ɂȂ�܂��B
 * 
 * ��������܂�����A
 * fenrir.naru atmark gmail dot com �܂Ń��[�����肢�������܂��B
 * 
 * ���̃��C�u�����͖����ł����A�����ۏ؂�����̂ł͂���܂���B
 * �����(M.Naruoka, �n���h���l�[��fenrir)�͂��̃��C�u������
 * �N���G�B�e�B�u�R�����Y���C�Z���X(�\�� - ��c�� - �p��)�̌���
 * ���������p����邱�Ƃ�]�݂܂��B
 * ���C�Z���X�̏ڍׂɂ��ẮA
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
 * 2�����z��̒��ۃN���X
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
     * �V�����[�R�s�[�����܂��B
     * 
     * @return (self_t *) �������g
     */
    virtual self_t *shallow_copy() const = 0;

    /**
     * Array2D�N���X�̃R���X�g���N�^�B
     * �w��̍s�T�C�Y�A�w��̗�T�C�Y�ŉ��z�I��2�����z��𐶐����܂��B
     * 
     * @param rows �s��
     * @param columns ��
     */
    Array2D(const unsigned int &rows, const unsigned int &columns)
        : m_rows(rows), m_columns(columns){
#ifdef _DEBUG
      canary_bird();
#endif
    }
    
    /**
     * Array2D�N���X�̃f�X�g���N�^�B
     * 
     */
    virtual ~Array2D(){
#ifdef _DEBUG
      canary_bird();
#endif
    }
    
    /**
     * �v�f�̃[���N���A�����܂��B
     *
     */
    virtual void clear() = 0;
    
    /**
     * 2�����z��𕡐�(�f�B�[�v�R�s�[)���܂��B
     * 
     * @return (self_t *) �R�s�[
     */
    virtual self_t *copy() const = 0;

    /**
     * ���g�����Ȕz��ɕϊ����܂��B
     *
     * @return (dense_t)
     */
    virtual dense_t dense() const = 0;
    
    /**
     * �s����Ԃ��܂��B
     * 
     * @return (int) �s��
     */
    unsigned int rows() const{return m_rows;}
    /**
     * �񐔂�Ԃ��܂��B
     * 
     * @return (int) ��
     */
    unsigned int columns() const{return m_columns;}
    
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    virtual FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column) = 0;
};

template <class FloatT>
class Array2D_BufferManager {
  protected:
    FloatT *m_buffer;
    int *ref; //�Q�ƃJ�E���^
    
    typedef Array2D_BufferManager<FloatT> self_t;
  
  public:
    /**
     * �Ǘ����Ă���o�b�t�@��Ԃ��܂��B
     * 
     * @return (FloatT *) �P���z��
     */
    FloatT *buffer() const {return m_buffer;}
    
    /**
     * Array2D_BufferManager�N���X�̃R���X�g���N�^�B
     * �Ǘ��ΏۂƂ���o�b�t�@��o�^���܂��B
     * 
     * @param buffer �Ǘ��Ώۂ̃o�b�t�@
     */
    Array2D_BufferManager(FloatT *buffer) 
        : m_buffer(buffer), 
        ref(new int(0)) {
      assert(m_buffer && ref);
      (*ref)++;
    }
    
    /**
     * �R�s�[�R���X�g���N�^
     * 
     * @param orig �R�s�[��
     */
    Array2D_BufferManager(const self_t &orig) 
        : m_buffer(orig.m_buffer), ref(orig.ref){
      if(ref){(*ref)++;}
    }
    
    /**
     * �f�X�g���N�^�B
     */
    virtual ~Array2D_BufferManager(){
      if(ref && ((--(*ref)) <= 0)){
        delete [] m_buffer;
        delete ref;
      }
    }
    
    /**
     * ������Z�q�B
     * 
     * @return (self_t) �������g
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
 * ���g���l�܂���2�����z��
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
     * Array2D_Dense�N���X�̃R���X�g���N�^�B
     * �w��̍s�T�C�Y�A�w��̗�T�C�Y��2�����z��𐶐����܂��B
     * �܂������͂��ׂ�FloatT(0)�ŏ���������܂��B
     * 
     * @param rows �s��
     * @param columns ��
     */
    Array2D_Dense(
        const unsigned int &rows, 
        const unsigned int &columns) 
        : super_t(rows, columns), 
        buffer_manager_t(new FloatT[rows * columns]) {
        
    }
    
    /**
     * Array2D_Dense�N���X�̃R���X�g���N�^�B
     * �w��̍s�T�C�Y�A�w��̗�T�C�Y��2�����z��𐶐����܂��B
     * �܂�������serilaized�ɂ���Ďw�肳�ꂽ�l�Ő�������܂�
     *
     * @param rows �s��
     * @param columns ��
     * @param serialized ����
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
     * �R�s�[�R���X�g���N�^
     * 
     */
    Array2D_Dense(const self_t &orig) 
        : super_t(orig.m_rows, orig.m_columns),
        buffer_manager_t(orig) {
      
    }
    
    /**
     * �f�X�g���N�^�B
     */
    ~Array2D_Dense(){}
    
    using super_t::rows;
    using super_t::columns;
    
    /**
     * 2�����z��𕡐�(�f�B�[�v�R�s�[)���܂��B
     * 
     * @return (root_t) �R�s�[
     */
    root_t *copy() const {
      self_t *array(new self_t(rows(), columns()));
      memcpy(array->buffer(), m_buffer, 
          sizeof(FloatT) * rows() * columns());
      return array;
    }
    
    /**
     * ���g�����Ȕz��ɕϊ����܂��B
     *
     * @return (self_t)
     */
    self_t dense() const {return self_t(*this);}
    
    /**
     * �V�����[�R�s�[�����܂��B
     * �Q�ƃJ�E���^�̃C���N�������g�������ɍs���܂��B
     * 
     * @return (Array2D_Dense *)�������g
     */
    root_t *shallow_copy() const{return new self_t(*this);}
    
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      assert((row < rows()) && (column < columns()));
      return *(m_buffer + (row * columns()) + column);
    }
    
    /**
     * v�f�̃[���N���A�����܂��B
     * 
     */
    void clear(){
      for(unsigned int i(0); 
          i < rows() * columns(); 
          i++){*(m_buffer + i) = FloatT(0);}
    }
    
    /**
     * ������Z�q�B
     * 
     * @return (self_t) �������g
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
 * �ʂ�2�����z��ɈϏ����s��2�����z��N���X
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
     * �Ϗ�����Ԃ��܂��B
     * 
     */
    root_t &getTarget() const{return *m_target;}
  
  public:
    using super_t::rows;
    using super_t::columns;
    
    /**
     * �Ϗ����Ԃ��܂��B
     * 
     * @return (root_t) �Ϗ���
     */
    const root_t *getParent() const{return m_target;}
    
    /**
     * Array2D_Delegate(�Ϗ�2�����z��)�N���X�̃R���X�g���N�^�B
     * 
     * @param rows �s��
     * @param columns ��
     * @param array ���̔z��
     */
    Array2D_Delegate(const unsigned int &rows,
        const unsigned int &columns,
        const root_t &array)
        : super_t(rows, columns), m_target(array.shallow_copy()){}
    
    /**
     * �R�s�[�R���X�g���N�^
     * 
     */
    Array2D_Delegate(const Array2D_Delegate &array)
        : super_t(array.rows(), array.columns()), 
        m_target(array.m_target->shallow_copy()){}
        
    /**
     * �f�X�g���N�^�B
     */
    ~Array2D_Delegate(){delete m_target;}
        
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      return m_target->operator()(row, column);
    }
    
    /**
     * ���g�����Ȕz��ɕϊ����܂��B
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
     * ����2�����z��𕡐�(�f�B�[�v�R�s�[)���܂��B
     * ������͒P����2�����z��ɂȂ�܂��B
     * 
     * @return (Array2D<FloatT>) �R�s�[
     */
    root_t *copy() const{
      return dense().shallow_copy();
    }
    
    /**
     * �v�f�̃[���N���A���s���܂��B
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
 * �]�u2�����z�������킷�N���X
 * 
 */
template<class FloatT>
class Array2D_Transpose : public Array2D_Delegate<FloatT>{
  
  public:
    /**
     * Array2D_Transpose(����2�����z��)�N���X�̃R���X�g��N�^
     * 
     * @param array ���̔z��
     */
    Array2D_Transpose(const Array2D<FloatT> &array)
        : Array2D_Delegate<FloatT>(array.columns(), array.rows(), array){}
    
    /**
     * �R�s�[�R���X�g���N�^
     * 
     */
    Array2D_Transpose(const Array2D_Transpose &array)
        : Array2D_Delegate<FloatT>(array.rows(), array.columns(), array){}
    
    /**
     * �V�����[�R�s�[�����܂��B
     * 
     * @return (Array2D *)�������g
     */
    Array2D<FloatT> *shallow_copy() const{return new Array2D_Transpose(*this);}
        
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      return Array2D_Delegate<FloatT>::operator()(column, row);
    }
    
    /**
     * ���g�����Ȕz��ɕϊ����܂��B
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
 * ����2�����z�������킷�N���X
 * 
 */
template<class FloatT>
class Array2D_Partial : public Array2D_Delegate<FloatT>{
  
  public:
    /**
     * ���g�����Ȕz��ɕϊ����܂��B
     *
     * @return (Array2D_Dense<FloatT>)
     */
    Array2D_Dense<FloatT> dense() const {
      return Array2D_Delegate<FloatT>::dense();
    }
    
    /**
     * ����2�����z��𕡐�(�f�B�[�v�R�s�[)���܂��B
     * ������͒P����2�����z��ɂȂ�܂��B
     * 
     * @return (Array2D<FloatT>) �R�s�[
     */
    Array2D<FloatT> *copy() const{
      return Array2D_Delegate<FloatT>::copy();
    } 
  
  private:
    unsigned int m_RowOffset;
    unsigned int m_ColumnOffset;
    
  protected:
    /**
     * �I�t�Z�b�g�s��Ԃ��܂��B
     * 
     * @return (int) �s��
     */
    unsigned int row_offset() const{return m_RowOffset;}
    /**
     * �I�t�Z�b�g���Ԃ��܂��B
     * 
     * @return (int) ��
     */
    unsigned int column_offset() const{return m_ColumnOffset;}
  
  public:
    /**
     * Array2D_Partial(����2�����z��)�N���X�̃R���X�g���N�^�B
     * 
     * @param array ���̔z��
     * @param offsetRow ����2�����z���(0,0)�ƂȂ錳��2�����z��̃s�{�b�g�̍s�C���f�b�N�X
     * @param offsetColumn ��������C���f�b�N�X
     * @param rows �s��
     * @param columns ��
     */
    Array2D_Partial(const unsigned int &rows,
        const unsigned int &columns,
        const Array2D<FloatT> &array,
        const unsigned int &rowOffset,
        const unsigned int &columnOffset)
        : Array2D_Delegate<FloatT>(rows, columns, array),
        m_RowOffset(rowOffset), m_ColumnOffset(columnOffset){}
    
    /**
     * �R�s�[�R���X�g���N�^
     * 
     */
    Array2D_Partial(const Array2D_Partial &array)
        : Array2D_Delegate<FloatT>(array),
        m_RowOffset(array.row_offset()), m_ColumnOffset(array.column_offset()){}
    
    /**
     * �V�����[�R�s�[�����܂��B
     * 
     * @return (Array2D *)�������g
     */
    Array2D<FloatT> *shallow_copy() const{return new Array2D_Partial(*this);}
        
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
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
 * @brief �Ϗ����ꂽ�s��
 *
 * �Ϗ����ꂽ�s��(�����s��A�]�u�s��Ȃǂ̔h���s��)�̊��N���X
 * ���̍s��N���X�ł̓��C�g�E�G�C�g����(������A�V�����[�R�s�[�ɂ�����)�ł��邽�߁A
 * ���̂܂܂ł͔h���N���X�ɂ����ĕ��Q���������܂��B
 * ���Y���ڂ���������̂��A���̃N���X�̖����ł��B
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
     * ������Z�q�B
     * ���N���X��Storage��ύX���鑀��ƈقȂ�A�����ǂ����̑�����s���܂��B
     *
     * @param matrix �������s��
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
 * @brief �]�u�s��
 *
 * �]�u�s�������킷�N���X�B
 * �N���X���N���X�Ƃ��Ē�`�B
 * �]�u2�����z���\������Array2D_Transpose�Ƌ��͂��ē]�u�s����������Ă��܂��B
 *
 * @see Array2D_Transpose �]�u2�����z��
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
     * TransposedMatrix(�]�u�s��)�N���X�̃R���X�g���N�^�B
     *
     * @param matrix ���̍s��
     */
    TransposedMatrix(const root_t &matrix)
        : super_t(new Array2D_Transpose<T>(*(matrix.storage()))){}

    /**
     * �f�X�g���N�^�B
     */
    ~TransposedMatrix(){}

    /**
     * �]�u�s���]�u���Č��̍s��ɖ߂��܂��B
     * �ԋp�����s��͂��Ƃ̍s��ƃ����N���Ă��܂��B
     * ���Ƃ̍s��Ƃ̐؂藣�����s���ɂ�transpose().copy()�Ƃ��Ă��������B
     *
     * @return (root_t) �]�u�s��
     */
    inline root_t untranspose() const{
      return super_t::original();
    }

    self_t &operator=(const root_t &matrix){
      return static_cast<self_t &>(super_t::substitute(matrix));
    }
    
    /**
     * �s�����Z���܂��B(�]�u * ��]�u)
     *
     * @param matrix ������s��
     * @return (root_t) ����
     */
    root_t operator*(const root_t &matrix) const{
      return super_t::operator*(matrix);
    }
    
    /**
     * �s�����Z���܂��B(�]�u * �]�u)
     *
     * @param matrix ������s��
     * @return (root_t) ����
     * @throw MatrixException �s��̐ώZ���������Ȃ��ꍇ(�I�y�����h�s��̗񐔂������s��̍s���Ɠ������Ȃ�)
     */
    root_t operator*(const self_t &matrix) const{
      return operator*((const root_t &)matrix);
    }
};

/**
 * @brief �����s��
 *
 * �����s�������킷�N���X
 * �N���X���N���X�Ƃ��Ē�`�B
 * ����2�����z���\������Array2D_Partial�Ƌ��͂��ĕ����s����������Ă��܂��B
 *
 * @see Array2D_Partial ����2�����z��
 */
template <class T>
class PartialMatrix : public DelegatedMatrix<T>{
  protected:
    typedef Matrix<T> root_t;
    typedef DelegatedMatrix<T> super_t;
    typedef PartialMatrix<T> self_t;

  public:
    /**
     * PartialMatrix(�����s��)�N���X�̃R���X�g���N�^�B
     *
     * @param matrix ���̍s��
     * @param rows �s��
     * @param columns ��
     * @param rowOffset �����s���(0,0)�ƂȂ錳�̍s��̃s�{�b�g�̍s�C���f�b�N�X
     * @param columnOffset ��������C���f�b�N�X
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
     * �f�X�g���N�^�B
     */
    ~PartialMatrix(){}

    self_t &operator=(const root_t &matrix){
      return static_cast<self_t &>(super_t::substitute(matrix));
    }
};

/**
 * Matrix class.
 * 
 * �s��̃N���X
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
     * Matrix�N���X�̃R���X�g���N�^�B
     * �X�g���[�W���w�肵�ĐV�����s������܂��B
     * 
     * @param array �X�g���[�W
     */
    Matrix(const storage_t *storage) : m_Storage(const_cast<storage_t *>(storage)){}
    
    static self_t make_instance(const storage_t *storage){
      return self_t(storage);
    }
    
    /**
     * Matrix�N���X���쐬����w���p�֐��B
     * �w��̍s���A�w��̗񐔂ōs��𐶐����܂����A
     * �����ɂ��Ă͏��������s��Ȃ����ߕs��ł��B
     *
     * @param rows �s��
     * @param columns ��
     */
    static self_t naked(
        const unsigned int &rows,
        const unsigned int &columns){
      return Matrix(new Array2D_Dense<FloatT>(rows, columns));
    }
  
  public:
    /**
     * �����I�ȕۑ��`����Ԃ��܂��B
     * 
     * @return (const storage_t *) �X�g���[�W
     */
    const storage_t *storage() const{return m_Storage;}

    /**
     * Matrix�N���X�̃R���X�g���N�^�B
     * 
     */
    Matrix() : m_Storage(NULL){}
    
    /**
     * Matrix�N���X�̃R���X�g���N�^�B
     * �w��̍s���A�w��̗񐔂ōs��𐶐����܂��B
     * �܂������͂��ׂ�FloatT(0)�ŏ���������܂��B
     * 
     *     
     * @param rows �s��
     * @param columns ��
     */
    Matrix(const unsigned int &rows, const unsigned int &columns)
        : m_Storage(new Array2D_Dense<FloatT>(rows, columns)){m_Storage->clear();}
    
    /**
     * Matrix�N���X�̃R���X�g���N�^�B
     * �w��̍s���A�w��̗񐔂ōs��𐶐����܂��B
     * �܂�������seialized�ŕ�������܂��B
     *
     * @param rows �s��
     * @param columns ��
     * @param serialized ����
     */
    Matrix(
        const unsigned int &rows, const unsigned int &columns,
        const FloatT *serialized)
        : m_Storage(new Array2D_Dense<FloatT>(rows, columns, serialized)){}
    
    /**
     * �R�s�[�R���X�g���N�^�B
     * �V�����[�R�s�[�𐶐����܂��B
     * 
     * @param matrix �R�s�[��
     */
    Matrix(const Matrix &matrix) : m_Storage(matrix.m_Storage->shallow_copy()){}
    /**
     * �f�X�g���N�^�B
     */
    virtual ~Matrix(){delete m_Storage;}
    
  protected:
    /**
     * ������Z�q���T�|�[�g���邽�߂̊֐�
     * �����I�ɂ̓V�����[�R�s�[���s���Ă��܂��B
     *
     * @return (self_t) �������g
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
     * ������Z�q�B
     *
     * @return (self_t) �������g
     */
    self_t &operator=(const self_t &matrix){
      return substitute(matrix);
    }
    
    /**
     * �s��𕡐�(�f�B�[�v�R�s�[)���܂��B
     * 
     * @return (self_t) �R�s�[
     */
    self_t copy() const{
      return self_t(m_Storage->copy());
    }
    
    /**
     * �s����Ԃ��܂��B
     * 
     * @return (int) �s��
     */
    unsigned int rows() const{return m_Storage->rows();}
    /**
     * �񐔂�Ԃ��܂��B
     * 
     * @return (int) ��
     */
    unsigned int columns() const{return m_Storage->columns();}
    
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    inline FloatT &operator()(const unsigned int &row, const unsigned int &column){
      return m_Storage->operator()(row, column);
    }
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B(Matlab����)
     * 
     * @param row �s�ԍ�(�J�n�ԍ���1�`)
     * @param column ��ԍ�(�J�n�ԍ���1�`)
     * @return (FloatT) ����
     */
    FloatT &matlab(const int &row, const int &column){
      return (*this)(row - 1, column - 1);
    }
    
    /**
     * �v�f���[���N���A���܂�
     * 
     */
    self_t clear(){
      m_Storage->clear();
      return *this;
    }
    
    /**
     * �w��̃X�J���[�s��𐶐����܂��B
     *
     * @param size �w��̍s��(��)
     * @param scalar �l
     */
    static self_t getScalar(const unsigned int &size, const FloatT &scalar){
      self_t result(size, size);
      for(unsigned int i = 0; i < size; i++){result(i, i) = scalar;}
      return result;
    }
    
    /**
     * �w��̒P�ʍs��𐶐����܂��B
     * 
     * @parma size �w��s��(��)
     */
    static self_t getI(const int &size){
      return getScalar(size, FloatT(1));
    }
    
    typedef TransposedMatrix<FloatT> transposed_t;
    
    /**
     * �s���]�u���܂��B
     * �]�u���ꂽ�s��͂��Ƃ̍s��ƃ����N���Ă��܂��B
     * ���Ƃ̍s��Ƃ̐؂藣�����s���ɂ�transpose().copy()�Ƃ��Ă��������B
     * 
     * @return (transposed_t) �]�u�s��
     */
    transposed_t transpose() const{
      return transposed_t(*this);
    }
    
    typedef PartialMatrix<FloatT> partial_t;
    
    /**
     * �w�肵�������s���Ԃ��܂��B
     *
     * @param rowSize �s�T�C�Y
     * @param columnSize ��T�C�Y
     * @param rowOffset �J�n�s�C���f�b�N�X
     * @param columnOffset �J�n��C���f�b�N�X
     * @return (PartialMatrix<T>) �����s��
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
     * �w�肵���s�̍s�x�N�g����Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X
     * @return (self_t) �s�x�N�g��
     */
    partial_t rowVector(const unsigned int &row) const{
      assert(row < rows());
      return partial_t(*this, 1, columns(), row, 0);
    }
    /**
     * �w�肵����̗�x�N�g����Ԃ��܂��B
     * 
     * @param column ��C���f�b�N�X
     * @return (partial_t) ��x�N�g��
     */
    partial_t columnVector(const unsigned int &column) const{
      assert(column < columns());
      return partial_t(*this, rows(), 1, 0, column);
    }
    
    /**
     * �s�����ւ��܂��B�j��I���\�b�h�ł��B
     * 
     * @param row1 �s�C���f�b�N�X1
     * @param row2 �s�C���f�b�N�X2
     * @return (self_t) �������g
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
     * ������ւ��܂��B�j��I���\�b�h�ł��B
     * 
     * @param column1 ��C���f�b�N�X1
     * @param column2 ��C���f�b�N�X2
     * @return (self_t) �������g
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
     * �����s�񂩂ǂ������ׂ܂��B
     * 
     * @return (bool) �����s��ł���ꍇtrue�A����ȊO�̏ꍇfalse
     */
    bool isSquare() const{return rows() == columns();}
    
    /**
     * �Ίp�s�񂩂ǂ������ׂ܂�
     *
     * @return (bool) �Ίp�s��ł���ꍇtrue�A����ȊO�̏ꍇfalse
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
     * �Ώ̍s�񂩂ǂ������ׂ܂��B
     * 
     * @return (bool) �Ώ̍s��ł���ꍇtrue�A����ȊO�̏ꍇfalse
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
     * �s��̑傫�����قȂ邩���ׂ�
     *
     * @param matrix ��r�Ώ�
     * @return (bool) �قȂ��Ă���ꍇtrue
     */
    bool isDifferentSize(const self_t &matrix) const{
      return (rows() != matrix.rows()) || (columns() != matrix.columns());
    }
    
    /**
     * �s��̃g���[�X��Ԃ��܂��B
     *
     * @param do_check �����s�񂩂𒲂ׂ�A�f�t�H���gtrue
     * @return (T) �g���[�X
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
     * �s��̐����S�Ă��w��{���܂��B�j��I���\�b�h�ł��B
     * 
     * @param scalar �{��
     * @return (self_t) �������g
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
     * �s��̐����S�Ă��w��{���܂��B
     * 
     * @param scalar �{��
     * @return (self_t) ����
     */
    self_t operator*(const FloatT &scalar) const{return (copy() *= scalar);}
    /**
     * �s��̐����S�Ă��w��{���܂��B
     * 
     * @param scalar �{��
     * @return (self_t) ����
     */
    friend self_t operator*(const FloatT &scalar, const self_t &matrix){return matrix * scalar;}
    /**
     * �s��̐����S�Ă����Z���܂��B�j��I���\�b�h�ł��B
     * 
     * @param scalar �{��
     * @return (self_t) �������g
     */
    self_t &operator/=(const FloatT &scalar){return (*this) *= (1 / scalar);}
    /**
     * �s��̐����S�Ă����Z���܂��B
     * 
     * @param scalar �{��
     * @return (self_t) ����
     */
    self_t operator/(const FloatT &scalar) const{return (copy() /= scalar);}
    /**
     * �s��̐����S�Ă����Z���܂��B
     * 
     * @param scalar �{��
     * @return (self_t) ����
     */
    friend self_t operator/(const FloatT &scalar, const self_t &matrix){return matrix / scalar;}
    
    /**
     * �s��𐬕����Ƃɉ��Z���܂��B�j��I���\�b�h�ł��B
     * 
     * @param matrix ������s��
     * @return (self_t) �������g
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
     * �s��𐬕����Ƃɉ��Z���܂��B
     * 
     * @param matrix ������s��
     * @return (self_t) ����
     */
    self_t operator+(const self_t &matrix) const{return (copy() += matrix);}
    
    /**
     * �s��𐬕����ƂɌ��Z���܂��B
     * 
     * @param matrix �����s��
     * @return (self_t) �������g
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
     * �s��𐬕����ƂɌ��Z���܂��B
     * 
     * @param matrix �����s��
     * @return (self_t) ����
     */
    self_t operator-(const self_t &matrix) const{return (copy() -= matrix);}
    
    /**
     * �s�����Z���܂��B
     * 
     * @param matrix �D����s��
     * @return (self_t) ����
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
     * �s�����Z���܂��B(�]�u�s��o�[�W����)
     *
     * @param matrix ������s��
     * @return (self_t) ����
     */
    self_t operator*(const transposed_t &matrix) const{
      return operator*((const self_t &)matrix);
    }
    
    /**
     * �s�����Z���܂��B�j��I���\�b�h�ł��B
     *
     * @param matrix ������s��
     * @return (self_t) �������g
     */
    template <class RhsMatrix>
    self_t &operator*=(const RhsMatrix &matrix){
      return (*this = (*this * matrix));
    }
    
    /**
     * �P�����Z�q-�B
     * ���ʂ� matrix * -1�Ɠ����ł��B
     * 
     * @return (self_t) -matrix
     */
    self_t operator-() const{return (copy() *= -1);}
    
    /**
     * ��s��(�]���q�s��)�����߂܂��B
     * 
     * @param row �s�C���f�b�N�X
     * @param column ��C���f�b�N�X
     * @return (self_t) ��s��
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
     * �s�񎮂��v�Z���܂��B
     * 
     * @return (FloatT) ����
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
     * LU���������܂��B
     * (0, 0)�`(n-1, n-1):  L�s��
     * (0, n)�`(n-1, 2n-1): U�s��
     * 
     * @return (self_t) LU����
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
     * UD���������܂��B
     * (0, 0)�`(n-1,n-1):  U�s��
     * (0, n)�`(n-1,2n-1): D�s��
     * 
     * @return (self_t) UD����
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
     * �t�s������߂܂��B
     * 
     * @return (self_t) �t�s��
     */
    self_t inverse(bool do_check = false) const{
      assert((!do_check) || isSquare());
      
      unsigned int size(rows());
      
      //�K�E�X�����@
      self_t left(copy());
      self_t right(self_t::getI(size));
      for(unsigned int i = 0; i < size; i++){
        if(left(i, i) == FloatT(0)){ //(i, i)�����݂���悤�ɕ��בւ�
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
      
      //LU����
      /*
      */
    }
    /**
     * �t�s��������܂��B�j��I���\�b�h�ł��B
     * 
     * @param matrix �s��
     * @return (self_t) �������g
     */
    self_t &operator/=(const self_t &matrix){return (*this) *= matrix.inverse();}
    /**
     * �t�s��������܂��B
     * 
     * @param matrix �s��
     * @return (self_t) ����
     */
    self_t operator/(const self_t &matrix) const{return (copy() /= matrix);}
    
    /**
     * �s�{�b�g���w�肵�āA���Z���܂��B
     * �j��I�ł��B
     * 
     * @param row �s�C���f�b�N�X
     * @param column ��C���f�b�N�X
     * @param matrix �����s��
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
     * �s�{�b�g���w�肵�āA���Z���܂��B
     * 
     * @param row �s�C���f�b�N�X
     * @param column ��C���f�b�N�X
     * @param matrix �����s��
     */
    self_t pivotAdd(const int &row, const int &column, const self_t &matrix) const{
      return copy().pivotMerge(row, column, matrix);
    }
    
    /**
     * �s������₷���`�ŏo�͂��܂��B
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
 * �A���C�����g���ꂽ��2�����z�������킷�N���X
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
     * Array2D_DenseAligned(�A���C�����g���ꂽ2�����z��)�N���X�̃R���X�g���N�^�B
     * 
     * @param _rows �s��
     * @param _columns ��
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
     * Array2D_DenseAligned(�A���C�����g���ꂽ2�����z��)�N���X�̃R���X�g���N�^�B
     * ������seialized�ŕ�������܂��B
     * 
     * @param _rows �s��
     * @param _columns ��
     * @param serialized ����
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
     * �R�s�[�R���X�g���N�^
     * 
     */
    Array2D_DenseAligned(const self_t &orig)
        : super_t(orig),
        buffer_manager_t(orig),
        m_buffer_rows(orig.m_buffer_rows),
        m_buffer_columns(orig.m_buffer_columns){}
    
    /**
     * �f�X�g���N�^�B
     */
    ~Array2D_DenseAligned(){}
    
    using super_t::rows;
    using super_t::columns;
    
  protected:
    /**
     * ����(�f�B�[�v�R�s�[)�̃w���p
     * 
     * @param dist �R�s�[��
     * @return (root_t) �R�s�[
     */
    root_t *copy_helper(self_t *dist) const {
      memcpy(dist->buffer(), m_buffer, 
          sizeof(FloatT) * m_buffer_rows * m_buffer_columns);
      return dist;
    }
    
  public:
    /**
     * �w�肵���s�񐬕���Ԃ��܂��B
     * 
     * @param row �s�C���f�b�N�X(�J�n�ԍ���0�`)
     * @param column ��C���f�b�N�X(�J�n�ԍ���0�`)
     * @return (FloatT) ����
     */
    inline FloatT &operator()(
        const unsigned int &row, 
        const unsigned int &column){
      assert((row < rows()) && (column < columns()));
      return *(m_buffer + (row * m_buffer_columns) + column);
    }
    
    /**
     * �v�f�̃[���N���A�����܂��B
     * 
     */
    void clear(){
      for(unsigned int i(0); 
          i < m_buffer_rows * m_buffer_columns; 
          i++){*(m_buffer + i) = FloatT(0);}
    }
    
    /**
     * ������Z�q�B
     * 
     * @return (self_t) �������g
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
 * ������
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
 * �A���C�����g���ꂽ��2�����z�������킷�N���X
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
     * ����(�f�B�[�v�R�s�[)���܂��B
     * 
     * @return (root_t) �R�s�[
     */ \
    root_t *copy() const { \
      return copy_helper(new self_t(rows(), columns())); \
    } \
    \
    /**
     * ���g�����Ȕz��ɕϊ����܂��B
     *
     * @return (Array2D_Dense<FloatT>)
     */ \
    self_t dense() const {return self_t(*this);} \
    \
    /**
     * �V�����[�R�s�[�����܂��B
     * 
     * @return (Array2D *)�������g
     */ \
    root_t *shallow_copy() const{return new self_t(*this);} \
    \
    /**
     * ������Z�q�B
     * 
     * @return (self_t) �������g
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
 * �֐��p�̓��ꉻ�̐��`
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
 * �s��̐����S�Ă��w��揜(unrolled version)
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
 * �s��̐������Ƃɉ����Z(unrolled version)
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
  int indx_c, indy_c; // ������ւ̈ړ�
  int indx_r, indy_r; // �s�����ւ̈ړ�
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
  int indx_c, indy_c; // ������ւ̈ړ�
  int indx_r, indy_r; // �s�����ւ̈ړ�
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
  // ���[�v�W�J�o�[�W����
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
 * �s��̏�Z(��]�u * ��]�u)
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
 * �s��̏�Z(��]�u * �]�u, �]�u * ��]�u, �]�u * �]�u)
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
 * �]�u
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
