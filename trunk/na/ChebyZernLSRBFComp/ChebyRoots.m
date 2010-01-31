function p = ChebyRoots(N, a, b)
%Return the roots of the N-th fisrt kind chebyshev polynomial
%return p: roots, i.e. the interpolating points in a b
t = (1:2:(2*N-1)) * pi / (2*N);

p = 0.5*(b-a)*cos(t) + 0.5*(b+a);