// Matrix.h is placed in PUBLIC DOMAIN.
//

#if !defined( MATRIX_H )
#define MATRIX_H

#include <cstddef>
#include <cassert>
#include <vector>
#include <utility>


// シンプルな行列演算クラス
template< class NumberType >
class Matrix
{
	//
	//	メンバ関数
	//
public:
	// コンストラクタ
	explicit Matrix (const std::size_t rows, const std::size_t columns);
	// コンストラクタ (単一の値で初期化)
	explicit Matrix (const std::size_t rows, const std::size_t columns, const NumberType initial_value);
	// コンストラクタ (初期化リストで初期化)
	explicit Matrix (const std::size_t rows, const std::size_t columns, std::initializer_list< NumberType > initial_values);
	// コンストラクタ (ベクタで初期化)
	explicit Matrix (const std::size_t rows, const std::size_t columns, const std::vector< NumberType > & initial_values);
	// コピーコンストラクタ
	Matrix (const Matrix &) = default;
	// ムーブコンストラクタ
	Matrix (Matrix && source);
	// デストラクタ
	~Matrix () = default;
	// 代入演算子
	Matrix & operator=(const Matrix &) = default;
	// ムーブ代入演算子
	Matrix & operator=(Matrix && source);

	// すべての要素を指定の値にする
	Matrix & fill (const NumberType & value);

	// 行数を取得する
	std::size_t get_number_of_rows () const;
	// 列数を取得する
	std::size_t get_number_of_columns () const;
	// 要素参照 (non-const)
	NumberType & operator()(const std::size_t row, const std::size_t column);
	// 要素参照 (const)
	const NumberType & operator()(const std::size_t row, const std::size_t column) const;

	// 加算
	Matrix operator+(const Matrix & rhs) const;
	// 減算
	Matrix operator-(const Matrix & rhs) const;
	// 乗算
	Matrix operator*(const Matrix & rhs) const;
	// アダマール積 (要素ごとの積)
	Matrix element_product (const Matrix & rhs) const;

	// 加算複合代入
	Matrix & operator+=(const Matrix & rhs);
	// 減算複合代入
	Matrix & operator-=(const Matrix & rhs);
	// 乗算複合代入 (対スカラー)
	Matrix & operator*=(const NumberType & rhs);
	// 除算複合代入 (対スカラー)
	Matrix & operator/=(const NumberType & rhs);
	// インプレースでアダマール積 (要素ごとの積)
	Matrix & element_product_inplace (const Matrix & rhs);

	// 転置
	Matrix transpose () const;

	//
	//	メンバ変数
	//
private:
	// 行数
	std::size_t rows_;
	// 列数
	std::size_t columns_;
	// 保存領域
	std::vector< NumberType > buffer_;
};


// コンストラクタ
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (rows * columns) {
	// アサーション
	assert (rows > 0 && columns > 0);
}

// コンストラクタ (単一の値で初期化)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, const NumberType initial_value)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (rows * columns, initial_value) {
	// アサーション
	assert (rows > 0 && columns > 0);
}

// コンストラクタ (初期化リストで初期化)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, std::initializer_list< NumberType > initial_values)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (initial_values) {
	// アサーション
	assert (rows > 0 && columns > 0 && initial_values.size () == rows * columns);
}

// コンストラクタ (ベクタで初期化)
template< class NumberType >
Matrix< NumberType >::Matrix (const std::size_t rows, const std::size_t columns, const std::vector< NumberType > & initial_values)
	: rows_ (rows)
	, columns_ (columns)
	, buffer_ (initial_values) {
	// アサーション
	assert (rows > 0 && columns > 0 && initial_values.size () == rows * columns);
}

// ムーブコンストラクタ
template< class NumberType >
Matrix< NumberType >::Matrix (Matrix && source)
	: rows_ (source.rows_)
	, columns_ (source.columns_)
	, buffer_ (std::move (source.buffer_)) {
	// source.buffer_ の内容が保証されなくなったので rows_ と columns_ も更新する
	source.rows_ = source.columns_ = 0;
}

// ムーブ代入演算子
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator=(Matrix && source) {
	rows_ = source.rows_;
	columns_ = source.columns_;
	buffer_ = std::move (source.buffer_);

	// source.buffer_ の内容が保証されなくなったので rows_ と columns_ も更新する
	source.rows_ = source.columns_ = 0;

	return *this;
}

//  すべての要素を指定の値にする
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::fill (const NumberType & value) {
	std::vector< NumberType > temp (rows_ * columns_, value);
	buffer_.swap (temp);
	return *this;

	// 一時的にメモリ消費量がほぼ倍になるので,
	// 要素数が非常に大きい場合は std::fill を使うほうがよいと思う.
	// by Atsushi OHTA (2016/7/2)
}

// 行数を取得する
template< class NumberType >
std::size_t Matrix< NumberType >::get_number_of_rows () const {
	return rows_;
}

// 列数を取得する
template< class NumberType >
std::size_t Matrix< NumberType >::get_number_of_columns () const {
	return columns_;
}

// 要素参照 (non-const)
template< class NumberType >
NumberType & Matrix< NumberType >::operator()(const std::size_t row, const std::size_t column) {
	// アサーション
	assert (row < rows_ && column < columns_);

	return buffer_[row * columns_ + column];
}

// 要素参照 (const)
template< class NumberType >
const NumberType & Matrix< NumberType >::operator()(const std::size_t row, const std::size_t column) const {
	// アサーション
	assert (row < rows_ && column < columns_);

	return buffer_[row * columns_ + column];
}

// 加算
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator+(const Matrix & rhs) const {
	Matrix result (*this);
	return result += rhs;
}

// 減算
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator-(const Matrix & rhs) const {
	Matrix result (*this);
	return result -= rhs;
}

// 乗算
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::operator*(const Matrix & rhs) const {
	// アサーション
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

// アダマール積 (要素ごとの積)
template< class NumberType >
Matrix< NumberType > Matrix< NumberType >::element_product (const Matrix & rhs) const {
	Matrix result (*this);
	return result.element_product_inplace (rhs);
}

// 加算複合代入
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator+=(const Matrix & rhs) {
	// アサーション
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem += *it++;
	}
	return *this;
}

// 減算複合代入
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator-=(const Matrix & rhs) {
	// アサーション
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem -= *it++;
	}
	return *this;
}

// 乗算複合代入 (対スカラー)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator*=(const NumberType & rhs) {
	for(auto & elem : buffer_)
	{
		elem *= rhs;
	}
	return *this;
}

// 除算複合代入 (対スカラー)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::operator/=(const NumberType & rhs) {
	for(auto & elem : buffer_)
	{
		elem /= rhs;
	}
	return *this;
}

// インプレースでアダマール積 (要素ごとの積)
template< class NumberType >
Matrix< NumberType > & Matrix< NumberType >::element_product_inplace (const Matrix & rhs) {
	// アサーション
	assert (rows_ == rhs.rows_ && columns_ == rhs.columns_);

	auto it = rhs.buffer_.cbegin ();
	for(auto & elem : buffer_)
	{
		elem *= *it++;
	}
	return *this;
}

// 転置
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