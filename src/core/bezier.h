/*
 * Written by Lukas Rennhofer @2025
 * Engine inspired by Dominique Roduit
 * Spe* @2025 - MIT Licese
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

int fact(int n) {
    int res = 1;

    for (int i = 1; i <= n; i++)
        res *= i;

    return res;
}

int binomial(int n, int k) {
    return fact(n) / (fact(k) * fact(n - k));
}

template<class T>
T bezier(std::vector<T> controls, float t) {
    T point = T(0.0);

    for (int k = 0, n = controls.size(); k < n; k++)
        point += controls[k] * (float)binomial(n-1, k) * (float)pow(1.0 - t, n - 1 - k) * (float)pow(t, k);

    return point;
}

template<class T>
T evaluate_piecewise_bezier(std::vector<T> bezier_points, float t) {
    int nb_segments = (bezier_points.size()-1)/3;

    int selected_segment = floor(t*nb_segments);
    if (selected_segment == nb_segments)
        return bezier_points.back();

    int start = selected_segment*3;
    std::vector<T> to_eval = std::vector<T>();
    for (int i = 0; i < 4; ++i) {
        to_eval.push_back(bezier_points.at(start+i));
    }
    float new_t = t*nb_segments - floor(t*nb_segments);

    return bezier(to_eval, new_t);
}

template<class T>
std::vector<T> generate_piecewise_bezier(std::vector<T> vertices, int c=2) {
    assert(0 <= c && c <= 2);
    assert(vertices.size() >= 4);

    assert((vertices.size() - 4) % (3 - c) == 0);

    int to_generate = c;

    std::vector<T> result = std::vector<T>();
    result.insert(result.begin(), vertices.begin(), vertices.begin() + 4);

    int last_prev_vertex = 3;
    for (int i=last_prev_vertex+1; i<vertices.size(); ++i) {
        if (to_generate > 0) {
            T generated = 2.0f*result.at(last_prev_vertex) - result.at(last_prev_vertex-1);
            result.push_back(generated);
        } else {
            result.push_back(vertices.at(i));
            ++i;
        }

        if (to_generate > 1) {
            T tmp = 2.0f*result.at(last_prev_vertex-1) - result.at(last_prev_vertex-2);

            result.push_back(2.0f*result.at(last_prev_vertex+1) - tmp);
        } else {
            result.push_back(vertices.at(i));
            ++i;
        }

        result.push_back(vertices.at(i));

        last_prev_vertex += 3;
    }

    return result;
}
