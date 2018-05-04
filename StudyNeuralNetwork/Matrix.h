// Matrix.h is placed in PUBLIC DOMAIN.
//

#if !defined( MATRIX_H )
#define MATRIX_H

#include <cstddef>
#include <cassert>
#include <vector>
#include <utility>


// �V���v���ȍs�񉉎Z�N���X
template< class NumberType >
class Matrix
{
	//
	//	�����o�֐�
	//
public:
	// �R���X�g���N�^
	explicit Matrix (const std::size_t rows, const std::size_t columns);
	// �R���X�g���N�^ (�P��̒l�ŏ�����)
	explicit Matrix (const std::size_t rows, const std::size_t columns, const NumberType initial_value);
	// �R���X�g���N�^ (���������X�g�ŏ�����)
	explicit Matrix (const std::size_t rows, const std::size_t columns, std::initializer_list< NumberType > initial_values);
	// �R���X�g���N�^ (�x�N�^�ŏ�����)
	explicit Matrix (const std::size_t rows, const std::size_t columns, const std::vector< NumberType > & initial_values);
	// �R�s�[�R���X�g���N�^
	Matrix (const Matrix &) = default;
	// ���[�u�R���X�g���N�^
	Matrix (Matrix && source);
	// �f�X�g���N�^
	~Matrix () = default;
	// ������Z�q
	Matrix & operator=(const Matrix &) = default;
	// ���[�u������Z�q
	Matrix & operator=(Matrix && source);

	// ���ׂĂ̗v�f���w��̒l�ɂ���
	Matrix & fill (const NumberType & value);

	// �s�����擾����
	std::size_t get_number_of_rows () const;
	// �񐔂��擾����
	std::size_t get_number_of_columns () const;
	// �v�f�Q�� (non-const)
	NumberType & operator()(const std::size_t row, const std::size_t column);
	// �v�f�Q�� (const)
	const NumberType & operator()(const std::size_t row, const std::size_t column) const;

	// ���Z
	Matrix operator+(const Matrix & rhs) const;
	// ���Z
	Matrix operator-(const Matrix & rhs) const;
	// ��Z
	Matrix operator*(const Matrix & rhs) const;
	// �A�_�}�[���� (�v�f���Ƃ̐�)
	Matrix element_product (const Matrix & rhs) const;

	// ���Z�������
	Matrix & operator+=(const Matrix & rhs);
	// ���Z�������
	Matrix & operator-=(const Matrix & rhs);
	// ��Z������� (�΃X�J���[)
	Matrix & operator*=(const NumberType & rhs);
	// ���Z������� (�΃X�J���[)
	Matrix & operator/=(const NumberType & rhs);
	// �C���v���[�X�ŃA�_�}�[���� (�v�f���Ƃ̐�)
	Matrix & element_product_inplace (const Matrix & rhs);

	// �]�u
	Matrix transpose () const;

	//
	//	�����o�ϐ�
	//
private:
	// �s��
	std::size_t rows_;
	// ��
	std::size_t columns_;
	// �ۑ��̈�
	std::vector< NumberType > buffer_;
};


// �R���X�g���N�^
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (rows * columns) {
	// �A�T�[�V����
	assert (rows > 0 && columns > 0);
}

// �R���X�g���N�^ (�P��̒l�ŏ�����)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, const NumberType initial_value)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (rows * columns, initial_value) {
	// �A�T�[�V����
	assert (rows > 0 && columns > 0);
}

// �R���X�g���N�^ (���������X�g�ŏ�����)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, std::initializer_list< NumberType > initial_values)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (initial_values) {
	// �A�T�[�V����
	assert (rows > 0 && columns > 0 && initial_values.size () == rows * columns);
}

// �R���X�g���N�^ (�x�N�^�ŏ�����)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, const std::vector< NumberType > & initial_values)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (initial_values) {
	// �A�T�[�V����
	assert (rows > 0 && columns > 0 && initial_values.size () == rows * columns);
}

// ���[�u�R���X�g���N�^
template< class NumberType >
Matrix< NumberType >::Matrix (Matrix && source)
	: rows_ (source.rows_)
	, columns_ (source.columns_)
	, buffer_ (std::move (source.buffer_)) {
	// source.buffer_ �̓��e���ۏ؂���Ȃ��Ȃ����̂� rows_ �� columns_ ���X�V����
	source.rows_ = source.columns_ = 0;
}

// ���[�u������Z�q
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator=(Matrix && source) {
	rows_ = source.rows_;
	columns_ = source.columns_;
	buffer_ = std::move (source.buffer_);

	// source.buffer_ �̓��e���ۏ؂���Ȃ��Ȃ����̂� rows_ �� columns_ ���X�V����
	source.rows_ = source.columns_ = 0;

	return *this;
}

//  ���ׂĂ̗v�f���w��̒l�ɂ���
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::fill (const NumberType & value) {
	std::vector< NumberType > temp (rows_ * columns_, value);
	buffer_.swap (temp);
	return *this;

	// �ꎞ�I�Ƀ���������ʂ��قڔ{�ɂȂ�̂�,
	// �v�f�������ɑ傫���ꍇ�� std::fill ���g���ق����悢�Ǝv��.
	// by Atsushi OHTA (2016/7/2)
}

// �s�����擾����
template< class NumberType >
std::size_t Matrix< NumberType >::get_number_of_rows () const {
	return rows_;
}

// �񐔂��擾����
template< class NumberType >
std::size_t Matrix< NumberType >::get_number_of_columns () const {
	return columns_;
}

// �v�f�Q�� (non-const)
template< class NumberType >
NumberType & Matrix< NumberType >::operator()(const std::size_t row, const std::size_t column) {
	// �A�T�[�V����
	assert (row < rows_ && column < columns_);

	return buffer_[row * columns_ + column];
}

// �v�f�Q�� (const)
template< class NumberType >
const NumberType & Matrix< NumberType >::operator()(const std::size_t row, const std::size_t column) const {
	// �A�T�[�V����
	assert (row < rows_ && column < columns_);

	return buffer_[row * columns_ + column];
}

// ���Z
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator+(const Matrix & rhs) const {
	Matrix result (*this);
	return result += rhs;
}

// ���Z
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator-(const Matrix & rhs) const {
	Matrix result (*this);
	return result -= rhs;
}

// ��Z
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator*(const Matrix & rhs) const {
	// �A�T�[�V����
	assert (columns_ == rhs.rows_);

	Matrix result (rows_, rhs.columns_);
	for(auto i = 0; i < rows_; ++i)
	{
		for(auto j = 0; j < rhs.columns_; ++j)
		{
			NumberType temp = NumberType (0);
			for(auto k = 0; k < columns_; ++k)
			{
				temp += ( *this )( i, k ) * rhs (k, j);
			}
			result (i, j) = temp;
		}
	}
	return result;
}

// �A�_�}�[���� (�v�f���Ƃ̐�)
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::element_product (const Matrix & rhs) const {
	Matrix result (*this);
	return result.element_product_inplace (rhs);
}

// ���Z�������
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator+=(const Matrix & rhs) {
	// �A�T�[�V����
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem += *it++;
	}
	return *this;
}

// ���Z�������
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator-=(const Matrix & rhs) {
	// �A�T�[�V����
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem -= *it++;
	}
	return *this;
}

// ��Z������� (�΃X�J���[)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator*=(const NumberType & rhs) {
	for(auto & elem : buffer_)
	{
		elem *= rhs;
	}
	return *this;
}

// ���Z������� (�΃X�J���[)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator/=(const NumberType & rhs) {
	for(auto & elem : buffer_)
	{
		elem /= rhs;
	}
	return *this;
}

// �C���v���[�X�ŃA�_�}�[���� (�v�f���Ƃ̐�)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::element_product_inplace (const Matrix & rhs) {
	// �A�T�[�V����
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem *= *it++;
	}
	return *this;
}

// �]�u
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::transpose () const {
	Matrix result (columns_, rows_);
	for(auto i = 0; i < columns_; ++i)
	{
		for(auto j = 0; j < rows_; ++j)
		{
			result (i, j) = ( *this )( j, i );
		}
	}
	return result;
}


#endif	// !defined( MATRIX_H )