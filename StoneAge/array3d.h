#ifndef ARRAY3D_H__
#define ARRAY3D_H__

template <class T>
class Array3d
{
public:
	Array3d(int x, int y, int z);
	~Array3d();
	Array3d(const Array3d& array);

	void Set(int x, int y, int z, T value);
	T& Get(int x, int y, int z) const;

	void Reset(T value);

	T* GetTValue();
	void SetBlocksFromBin(T* value);

private:
	int To1dIndex(int x, int y, int z) const;

private:
	int m_x, m_y, m_z;
	T* m_value;
};

template <class T>
Array3d<T>::Array3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
	m_value = new T[m_x * m_y * m_z];
}

template <class T>
Array3d<T>::~Array3d()
{
	delete[] m_value;
}

template <class T>
Array3d<T>::Array3d(const Array3d& array) : m_x(array.m_x), m_y(array.m_y), m_z(array.m_z)
{
	m_value = new T[m_x * m_y * m_z];

	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_value[i] = array.m_value[i]; // ==> méthode Reset ?
}

template <class T>
void Array3d<T>::Set(int x, int y, int z, T value)
{
	m_value[To1dIndex(x, y, z)] = value;
}

template <class T>
T& Array3d<T>::Get(int x, int y, int z) const
{
	return m_value[To1dIndex(x, y, z)];
}

template <class T>
void Array3d<T>::Reset(T value)
{
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_value[i] = value;
}

template <class T>
int Array3d<T>::To1dIndex(int x, int y, int z) const
{
	return x + (z * m_x) + (y * m_z * m_x);
}

template <class T>
T* Array3d<T>::GetTValue() {
	return m_value;
}

template <class T>
void Array3d<T>::SetBlocksFromBin(T* value) {
	m_value = value;
}
#endif
