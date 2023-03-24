#ifndef ARRAY2D_H__
#define ARRAY2D_H__

template <class T>
class Array2d
{
public:
	Array2d(int x, int y);
	~Array2d();
	Array2d(const Array2d& array);

	void Set(int x, int y, T value);
	T& Get(int x, int y) const;
	int GetSizeX();
	int GetSizeY();
	void Reset(T value);

private:
	int To1dIndex(int x, int y) const;

private:
	int m_x, m_y;
	T* m_value;
};

template <class T>
Array2d<T>::Array2d(int x, int y) : m_x(x), m_y(y)
{
	m_value = new T[m_x * m_y];
}

template <class T>
Array2d<T>::~Array2d()
{
	delete[] m_value;
}

template <class T>
Array2d<T>::Array2d(const Array2d& array) : m_x(array.m_x), m_y(array.m_y)
{
	m_value = new T[m_x * m_y];
	for (int i = 0; i < m_x * m_y; ++i)
		m_value[i] = array.m_value[i];
}

template <class T>
void Array2d<T>::Set(int x, int y, T value)
{
	m_value[To1dIndex(x, y)] = value;
}

template <class T>
T& Array2d<T>::Get(int x, int y) const
{
	return m_value[To1dIndex(x, y)];
}

template <class T>
void Array2d<T>::Reset(T value)
{
	for (int i = 0; i < m_x * m_y; ++i)
		m_value[i] = value;
}

template <class T>
int Array2d<T>::To1dIndex(int x, int y) const
{
	return x + (y * m_x); //À revoir, je ne suis pas sur si ça fonctionne comme il le faut
}

template <class T>
int Array2d<T>::GetSizeX()
{
	return m_x;
}

template <class T>
int Array2d<T>::GetSizeY()
{
	return m_y;
}
#endif
