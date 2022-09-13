#pragma once

struct Vector2D {
    float x;
    float y;

    Vector2D();
    Vector2D(float x, float y);
    
    ~Vector2D() = default;

    void Add(const Vector2D& v);                 // v1.Add(v2)
    void Sub(const Vector2D& v);                 // v1.Sub(v2)
    void Scale(const float n);               // v1.Scale(n)
    Vector2D Rotate(const float angle) const;    // v1.Rotate(angle)

    float Magnitude() const;                 // v1.Magnitude()
    float MagnitudeSquared() const;          // v1.MagnitudeSquared()

    Vector2D& Normalize();                       // v1.Normalize()
    Vector2D UnitVector() const;                 // v1.UnitVector()
    Vector2D Normal() const;                     // n = v1.Normal()

    float Dot(const Vector2D& v) const;          // v1.Dot(v2)
    float Cross(const Vector2D& v) const;        // v1.Cross(v2)

    Vector2D& operator = (const Vector2D& v);        // v1 = v2
    bool operator == (const Vector2D& v) const;  // v1 == v2
    bool operator != (const Vector2D& v) const;  // v1 != v2
    
    Vector2D operator + (const Vector2D& v) const;   // v1 + v2
    Vector2D operator - (const Vector2D& v) const;   // v1 - v2
    Vector2D operator * (const float n) const;   // v1 * n
    Vector2D operator / (const float n) const;   // v1 / n
    Vector2D operator - ();                      // -v1

    Vector2D& operator += (const Vector2D& v);       // v1 += v2
    Vector2D& operator -= (const Vector2D& v);       // v1 -= v2
    Vector2D& operator *= (const float n);       // v1 *= n
    Vector2D& operator /= (const float n);       // v1 /= n
};