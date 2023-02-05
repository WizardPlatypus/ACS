template<typename T>
T nothing(const T& a, const T& b) {
    return 0;
}

template<typename T>
T add(const T& a, const T& b) {
    return a + b;
}

template<typename T>
T subtract(const T& a, const T& b) {
    return a - b;
}

template<typename T>
T multiply(const T& a, const T& b) {
    return a * b;
}

template<typename T>
T divide(const T& a, const T& b) {
    return a / b;
}

template<typename T>
T modulo(const T& a, const T& b) {
    return a % b;
}

// there is no standard implementation for operation % on operands of type const double, const float.
template<> double modulo(const double& a, const double& b) {
    return 0;
}
template<> float modulo(const float& a, const float& b) {
    return 0;
}

