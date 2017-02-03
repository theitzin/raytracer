#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>
#include <cmath>

template <class T> class Vec3
{
    private:
        // A Vec3 simply has three properties called x, y and z
        T x, y, z;

    public:
        // ------------ Constructors ------------

        // Default constructor
        Vec3() { x = y = z = 0; };

        // Three parameter constructor
        Vec3(T xValue, T yValue, T zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        // ------------ Getters and setters ------------

        void set(const T &xValue, const T &yValue, const T &zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        T getX() const { return x; }
        T getY() const { return y; }
        T getZ() const { return z; }

        void setX(const T &xValue) { x = xValue; }
        void setY(const T &yValue) { y = yValue; }
        void setZ(const T &zValue) { z = zValue; }

        // ------------ Helper methods ------------

        // Method to reset a vector to zero
        void zero()
        {
            x = y = z = 0;
        }

        // Method to normalise a vector
        Vec3 normalise () const
        {
            // Calculate the magnitude of our vector
            T magnitude = sqrt((x * x) + (y * y) + (z * z));

            // As long as the magnitude isn't zero, divide each element by the magnitude
            // to get the normalised value between -1 and +1


            return Vec3(x / magnitude, y / magnitude, z / magnitude);
        }

        T length2() const
        {
            return (x * x) + (y * y) + (z * z);
        }

        T length() const
        {
            return sqrt(length2());
        }

        // Static method to calculate and return the scalar dot product of two vectors
        //
        // Note: The dot product of two vectors tell us things about the angle between
        // the vectors. That is, it tells us if they are pointing in the same direction
        // (i.e. are they parallel? If so, the dot product will be 1), or if they're
        // perpendicular (i.e. at 90 degrees to each other) the dot product will be 0,
        // or if they're pointing in opposite directions then the dot product will be -1.
        //
        // Usage example: double foo = Vec3<double>::dotProduct(vectorA, vectorB);
        static T dotProduct(const Vec3 &vec1, const Vec3 &vec2)
        {
            return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
        }

        // Non-static method to calculate and return the scalar dot product of this vector and another vector
        //
        // Usage example: double foo = vectorA.dotProduct(vectorB);
        T dotProduct(const Vec3 &vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        // Static method to calculate and return a vector which is the cross product of two vectors
        //
        // Note: The cross product is simply a vector which is perpendicular to the plane formed by
        // the first two vectors. Think of a desk like the one your laptop or keyboard is sitting on.
        // If you put one pencil pointing directly away from you, and then another pencil pointing to the
        // right so they form a "L" shape, the vector perpendicular to the plane made by these two pencils
        // points directly upwards.
        //
        // Whether the vector is perpendicularly pointing "up" or "down" depends on the "handedness" of the
        // coordinate system that you're using.
        //
        // Further reading: http://en.wikipedia.org/wiki/Cross_product
        //
        // Usage example: Vec3<double> crossVect = Vec3<double>::crossProduct(vectorA, vectorB);
        static Vec3 crossProduct(const Vec3 &vec1, const Vec3 &vec2)
        {
            return Vec3(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
        }

        // Easy adders
        void addX(T value) { x += value; }
        void addY(T value) { y += value; }
        void addZ(T value) { z += value; }

        // Method to return the distance between two vectors in 3D space
        //
        // Note: This is accurate, but not especially fast - depending on your needs you might
        // like to use the Manhattan Distance instead: http://en.wikipedia.org/wiki/Taxicab_geometry
        // There's a good discussion of it here: http://stackoverflow.com/questions/3693514/very-fast-3d-distance-check
        // The gist is, to find if we're within a given distance between two vectors you can use:
        //
        // bool within3DManhattanDistance(Vec3 c1, Vec3 c2, float distance)
        // {
        //      float dx = abs(c2.x - c1.x);
        //      if (dx > distance) return false; // too far in x direction
        //
        //      float dy = abs(c2.y - c1.y);
        //      if (dy > distance) return false; // too far in y direction
        //
        //      float dz = abs(c2.z - c1.z);
        //      if (dz > distance) return false; // too far in z direction
        //
        //      return true; // we're within the cube
        // }
        //
        // Or to just calculate the straight Manhattan distance you could use:
        //
        // float getManhattanDistance(Vec3 c1, Vec3 c2)
        // {
        //      float dx = abs(c2.x - c1.x);
        //      float dy = abs(c2.y - c1.y);
        //      float dz = abs(c2.z - c1.z);
        //      return dx+dy+dz;
        // }
        //
        static T getDistance(const Vec3 &v1, const Vec3 &v2)
        {
            T dx = v2.x - v1.x;
            T dy = v2.y - v1.y;
            T dz = v2.z - v1.z;

            return sqrt(dx * dx + dy * dy + dz * dz);
        }

        // Method to display the vector so you can easily check the values
        void display () const
        {
            std::cout << "X: " << x << "\t Y: " << y << "\t Z: " << z << std::endl;
        }

        // ------------ Overloaded operators ------------

        // Overloaded addition operator to add Vec3s together
        Vec3 operator+(const Vec3 &vector) const
        {
            return Vec3<T>(x + vector.x, y + vector.y, z + vector.z);
        }

        // Overloaded add and asssign operator to add Vec3s together
        void operator+=(const Vec3 &vector)
        {
            x += vector.x;
            y += vector.y;
            z += vector.z;
        }

        // Overloaded subtraction operator to subtract a Vec3 from another Vec3
        Vec3 operator-(const Vec3 &vector) const
        {
            return Vec3<T>(x - vector.x, y - vector.y, z - vector.z);
        }

        // Overloaded subtract and asssign operator to subtract a Vec3 from another Vec3
        void operator-=(const Vec3 &vector)
        {
            x -= vector.x;
            y -= vector.y;
            z -= vector.z;
        }

        // Overloaded multiplication operator to multiply two Vec3s together
        Vec3 operator*(const Vec3 &vector) const
        {
            return Vec3<T>(x * vector.x, y * vector.y, z * vector.z);
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Vec3 operator*(const T &value) const
        {
            return Vec3<T>(x * value, y * value, z * value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator*=(const T &value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Vec3 operator/(const T &value) const
        {
            return Vec3<T>(x / value, y / value, z / value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator/=(const T &value)
        {
            x /= value;
            y /= value;
            z /= value;
        }
};


template <class T> class Mat3
{
    private:
        Vec3<T> x, y, z;

    public:
        Mat3() {};
        Mat3(T xx, T yx, T zx, T xy, T yy, T zy, T xz, T yz, T zz)
        {
            x = Vec3<T>(xx, yx, zx);
            y = Vec3<T>(xy, yy, zy);
            z = Vec3<T>(xz, yz, zz);
        }
        Mat3(Vec3<T> xValue, Vec3<T> yValue, Vec3<T> zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        // ------------ Getters and setters ------------

        void set(const Vec3<T> &xValue, const Vec3<T> &yValue, const Vec3<T> &zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        Vec3<T> getX() const { return x; }
        Vec3<T> getY() const { return y; }
        Vec3<T> getZ() const { return z; }

        Vec3<T> getRowX() const { return Vec3<T>(x.getX(), y.getX(), z.getX()); }
        Vec3<T> getRowY() const { return Vec3<T>(x.getY(), y.getY(), z.getY()); }
        Vec3<T> getRowZ() const { return Vec3<T>(x.getZ(), y.getZ(), z.getZ()); }

        void setX(const Vec3<T> &xValue) { x = xValue; }
        void setY(const Vec3<T> &yValue) { y = yValue; }
        void setZ(const Vec3<T> &zValue) { z = zValue; }

        // ------------ Helper methods ------------

        void zero()
        {
            x.zero();
            y.zero();
            z.zero();
        }

        void addX(Vec3<T> value) { x += value; }
        void addY(Vec3<T> value) { y += value; }
        void addZ(Vec3<T> value) { z += value; }

        void display()
        {
            std::cout << x.getX() << "\t " << y.getX() << "\t " << z.getX() << std::endl;
            std::cout << x.getY() << "\t " << y.getY() << "\t " << z.getY() << std::endl;
            std::cout << x.getZ() << "\t " << y.getZ() << "\t " << z.getZ() << std::endl;
        }

        // ------------ Overloaded operators ------------

        Mat3 operator+(const Mat3 &mat) const
        {
            return Mat3<T>(x + mat.x, y + mat.y, z + mat.z);
        }

        void operator+=(const Mat3 &mat)
        {
            x += mat.x;
            y += mat.y;
            z += mat.z;
        }

        Mat3 operator-(const Mat3 &mat) const
        {
            return Mat3<T>(x - mat.x, y - mat.y, z - mat.z);
        }

        void operator-=(const Mat3 &mat)
        {
            x -= mat.x;
            y -= mat.y;
            z -= mat.z;
        }

        Mat3 operator*(const Mat3 &mat) const
        {
            Vec3<T> v1(getRowX().dotProduct(mat.getX()), getRowY().dotProduct(mat.getX()), getRowZ().dotProduct(mat.getX()));
            Vec3<T> v2(getRowX().dotProduct(mat.getY()), getRowY().dotProduct(mat.getY()), getRowZ().dotProduct(mat.getY()));
            Vec3<T> v3(getRowX().dotProduct(mat.getZ()), getRowY().dotProduct(mat.getZ()), getRowZ().dotProduct(mat.getZ()));
            return Mat3<T>(v1, v2, v3);
        }

        Vec3<T> operator*(const Vec3<T> & vec) const
        {
            return Vec3<T>(getRowX().dotProduct(vec), getRowY().dotProduct(vec), getRowZ().dotProduct(vec));
        }

        Mat3 operator*(const T &value) const
        {
            return Mat3<T>(x * value, y * value, z * value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator*=(const T &value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Mat3 operator/(const T &value) const
        {
            return Mat3<T>(x / value, y / value, z / value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator/=(const T &value)
        {
            x /= value;
            y /= value;
            z /= value;
        }

        Mat3 inverse()
        {
            T a = getX().getX();
            T b = getY().getX();
            T c = getZ().getX();
            T d = getX().getY();
            T e = getY().getY();
            T f = getZ().getY();
            T g = getX().getZ();
            T h = getY().getZ();
            T i = getZ().getZ();

            T det = a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
            Vec3<T> v1(e*i - f*h, f*g - d*i, d*h - e*g);
            Vec3<T> v2(c*h - b*i, a*i - c*g, b*g - a*h);
            Vec3<T> v3(b*f - c*e, c*d - a*f, a*e - b*d);

            return Mat3<T>(v1 * det, v2 * det, v3 * det);
        }

        Mat3 rotateHor(const T & val) 
        { 
            return operator*(Mat3<T>(1, 0, 0, 0, cos(val), sin(val), 0, -sin(val), cos(val)));
        }

        Mat3 rotateVer(const T & val)
        {
            return operator*(Mat3<T>(cos(val), 0, -sin(val), 0, 1, 0, sin(val), 0, cos(val)));
        }
};

template <class T> Mat3<T> Mat3Identity() {
    return Mat3<T>(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

#endif